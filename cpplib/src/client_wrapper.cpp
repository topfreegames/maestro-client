#include "../include/client.h"
#include <boost/format.hpp>
#include <cstring>

using namespace Maestro;

extern "C" {

  void (*debugLog)(const char*) = NULL;

  Client* internal_create_client(const char * maestro_api_url){
    Client* client = new Client(maestro_api_url);
    return client;
  }

  char* make_string_copy (const char* str) {
    if (str == NULL) return NULL;
    char* res = (char *)malloc(strlen(str) + 1);
    strcpy(res, str);
    return res;
  }

  const char * internal_get_address(Client *obj) {
    const char * res = obj->get_address().c_str();
    char * strCopy = make_string_copy(res);
    return strCopy;
  }

  bool internal_initialize(Client *obj){
    return obj->initialize();
  }

  bool internal_initialize_with_scheduler_and_id(Client *obj, const char * scheduler, const char * id) {
    return obj->initialize(scheduler, id);
  }

  bool internal_ping(Client* obj){
    return obj->ping();
  }

  bool internal_room_ready(Client* obj) {
    return obj->room_ready();
  }

  bool internal_room_occupied(Client* obj) {
    return obj->room_occupied();
  }

  bool internal_room_terminated(Client* obj) {
    return obj->room_terminated();
  }

  bool internal_room_terminating(Client* obj) {
    return obj->room_terminating();
  }

  void set_debug(void(*d)(const char*)){
    debugLog = d;
  }
}
