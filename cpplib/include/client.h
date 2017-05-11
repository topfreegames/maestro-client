#ifndef _client_h
#define _client_h

#include <iostream>
#include <string>

#define ROOM_READY "ready"
#define ROOM_OCCUPIED "occupied"
#define ROOM_TERMINATED "terminated"
#define ROOM_TERMINATING "terminating"

namespace Maestro 
{
  class __attribute__ ((visibility ("default"))) Client
  {
    private:
      std::string maestro_api_url;
      std::string room_id;
      std::string room_scheduler;
      std::string status;
    public:
      Client();
      Client(std::string maestro_api_url);
      std::string get_address();
      std::string get_status();
      bool initialize();
      bool initialize(std::string scheduler, std::string id);
      bool ping();
      void set_maestro_api_url(std::string maestro_api_url);
      bool room_occupied();
      bool room_ready();
      bool room_terminated();
      bool room_terminating();
      bool update_status(std::string status);
  };
};

#endif
