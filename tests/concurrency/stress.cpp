// Drives a single Maestro::Client from several threads at once, the same way
// the Unity plugin does: an autoping thread calling ping() while the game
// thread reports status, running matches and player events.
//
// Usage: stress [seconds]   (MAESTRO_API_URL defaults to http://127.0.0.1:5000)
// Exits 0 if the process survives the whole run and no call stays blocked
// behind a hung request.

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include "../../cpplib/include/client.h"

int main(int argc, char** argv) {
  const char* env_url = std::getenv("MAESTRO_API_URL");
  std::string url = env_url ? env_url : "http://127.0.0.1:5000";
  int seconds = argc > 1 ? std::atoi(argv[1]) : 30;

  Maestro::Client client(url, 1);
  client.initialize("stress-scheduler", "stress-room");

  std::atomic<bool> stop(false);
  std::atomic<long> calls(0);
  std::vector<std::thread> threads;

  // Autoping thread (Maestro.cs AutopingFunction).
  threads.emplace_back([&] {
    while (!stop.load()) {
      client.ping("{\"source\":\"autoping\"}");
      calls++;
    }
  });

  // Game thread: room lifecycle and running matches.
  threads.emplace_back([&] {
    for (int i = 0; !stop.load(); i++) {
      client.set_running_matches(i % 5);
      if (i % 2 == 0) {
        client.room_ready("{\"source\":\"game\"}");
      } else {
        client.room_occupied("{\"source\":\"game\"}");
      }
      calls++;
    }
  });

  // Game thread: player events.
  threads.emplace_back([&] {
    while (!stop.load()) {
      client.player_join("{\"playerId\":\"p1\"}");
      client.player_left("{\"playerId\":\"p1\"}");
      calls += 2;
    }
  });

  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  stop.store(true);
  auto stopped_at = std::chrono::steady_clock::now();
  for (auto& t : threads) {
    t.join();
  }
  auto drain = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::steady_clock::now() - stopped_at).count();

  std::cerr << "survived " << seconds << "s, " << calls.load() << " calls, "
            << drain << "s to finish in-flight calls\n";

  // At most four calls can be queued on the connection when we stop (one per
  // thread, two for the player thread), each capped by the request timeout.
  // A hung request that isn't cut off keeps everyone waiting much longer.
  if (drain > 4 * REQUEST_TIMEOUT_SECONDS + 5) {
    std::cerr << "calls stayed blocked behind a hung request\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
