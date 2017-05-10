#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <sys/signal.h>
#include "../../cpplib/include/client.h"

Maestro::Client *client;
std::atomic<int> stop(0);
std::condition_variable stop_threads;
std::mutex m;

void ping_loop(){
  while (1) {
    std::unique_lock<std::mutex> l(m);
    client->ping();
    std::cout << "ping sent!" << std::endl;
    if(stop_threads.wait_for(l, std::chrono::seconds(client->get_ping_interval())) ==
        std::cv_status::no_timeout) break;
  }
}

void signalHandler(int signum){
  client->room_terminating();
  stop.store(1);
}

void initialize(){
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
  char const * api_url = std::getenv("MAESTRO_API_URL");
  if (!api_url || strlen(api_url) == 0) {
		api_url = "http://localhost:5000";
  }
  char const * scheduler = std::getenv("MAESTRO_SCHEDULER_NAME");
  char const * id = std::getenv("MAESTRO_ROOM_ID");
  if (!scheduler || !id || strlen(scheduler) == 0 || strlen(id) == 0) {
		scheduler = "test";
		id = "test";
  }
  client = new Maestro::Client(api_url);
  client->initialize(scheduler, id);
  std::string addr = client->get_address();
  std::cout << "address " << addr << std::endl;
  client->room_ready();
}

int main() {
  initialize();
  std::thread t1(ping_loop);

  while (!stop.load()){
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  stop_threads.notify_all();
  t1.join();

  client->room_terminated(); 
  return EXIT_SUCCESS;
}