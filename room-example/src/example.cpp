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

void signalHandler(int signum){
  client-> stop_auto_ping();
  client->room_terminating();
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
  client = new Maestro::Client(api_url, 5);
  client->initialize(scheduler, id);
  std::string addr = client->get_address();
  std::cout << "address " << addr << std::endl;
  client->room_ready();
}

int main() {
  initialize();

  std::thread t1 = client->start_auto_ping();
  t1.join();

  client->room_terminated(); 
  return EXIT_SUCCESS;
}
