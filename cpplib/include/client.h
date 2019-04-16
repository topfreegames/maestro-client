#ifndef _client_h
#define _client_h

#include <iostream>
#include <string>
#include <atomic>
#include <thread>

#define ROOM_READY "ready"
#define ROOM_OCCUPIED "occupied"
#define ROOM_TERMINATED "terminated"
#define ROOM_TERMINATING "terminating"
#define PLAYER_JOIN "playerJoin"
#define PLAYER_LEFT "playerLeft"

namespace Maestro
{
  class __attribute__ ((visibility ("default"))) Client
  {
    private:
      std::string maestro_api_url;
      int ping_interval;
      void ping_loop();
      std::string room_id;
      std::string room_scheduler;
      std::string status;
      std::atomic<int> stop_ping;
      std::string send_event(std::string route_template, std::string event, std::string metadata);
    public:
      Client();
      Client(std::string maestro_api_url);
      Client(std::string maestro_api_url, int ping_interval);
      std::string get_address();
      std::string get_status();
      int get_ping_interval();
      bool initialize();
      bool initialize(std::string scheduler, std::string id);
      bool ping(std::string metadata);
      void set_maestro_api_url(std::string maestro_api_url);
      bool room_occupied(std::string metadata);
      bool room_ready(std::string metadata);
      bool room_terminated(std::string metadata);
      bool room_terminating(std::string metadata);
      std::string player_join(std::string metadata);
      std::string player_left(std::string metadata);
      std::string room_event(std::string event, std::string metadata);
      void set_ping_interval(int interval);
      std::thread start_auto_ping();
      void stop_auto_ping();
      bool update_status(std::string status, std::string metadata);
      std::string player_event(std::string event, std::string metadata);
  };
};

#endif
