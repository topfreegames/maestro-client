#ifndef _client_h
#define _client_h

#include <iostream>
#include <string>

#define ROOM_READY "room-ready"
#define ROOM_OCCUPIED "room-ready"
#define ROOM_TERMINATED "terminated"
#define ROOM_TERMINATING "terminating"
 
namespace Maestro 
{
  class Client
  {
    private:
      std::string maestro_api_url;
      int ping_interval;
      std::string room_id;
      std::string room_scheduler;
      std::string status;
    public:
      Client();
      Client(std::string maestro_api_url);
      Client(std::string maestro_api_url, int ping_interval);
      std::string get_address();
      int get_ping_interval();
      std::string get_status();
      bool initialize();
      bool initialize(std::string scheduler, std::string id);
      bool ping();
      void set_maestro_api_url(std::string maestro_api_url);
      void set_ping_interval(int ping_interval);
      bool room_occupied();
      bool room_ready();
      bool room_terminated();
      bool room_terminating();
      bool update_status(std::string status);
  };
};
 
#endif
