#ifndef _client_h
#define _client_h

#include <iostream>
#include <string>

#define ROOM_READY "room-ready"
#define MATCH_ENDED "match-ended"
#define MATCH_STARTED "match-started"
 
namespace Maestro 
{
  class Client
  {
    private:
      std::string maestro_api_url;
      int ping_interval;
      std::string room_id;
      std::string room_namespace;
    public:
      Client();
      Client(std::string maestro_api_url);
      Client(std::string maestro_api_url, int ping_interval);
      std::string get_address();
      int get_ping_interval();
      bool initialize();
      bool match_ended();
      bool match_started();
      void ping();
      bool room_ready();
      void set_maestro_api_url(std::string maestro_api_url);
      void set_ping_interval(int ping_interval);
      bool update_status(std::string status);
  };
};
 
#endif
