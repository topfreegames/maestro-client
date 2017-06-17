#include "../include/client.h"
#include <boost/format.hpp>
#include <cstring>
#include <csignal>

using namespace Maestro;

extern "C" {

  void (*debugLog)(const char*) = NULL;
  void (*onTerminating)() = NULL;

  Client* internal_create_client(const char * maestro_api_url){
    Client* client = new Client(maestro_api_url);
    return client;
  }

  Client* internal_create_client_with_ping_interval(const char * maestro_api_url, int ping_interval){
    Client* client = new Client(maestro_api_url, ping_interval);
    return client;
  }

  char* make_string_copy (const char* str) {
    if (str == NULL) return NULL;
    char* res = (char *)malloc(strlen(str) + 1);
    strcpy(res, str);
    return res;
  }

  void signal_handler(int signumber) {
    if (onTerminating != NULL) {
      onTerminating();
    } 
    exit(signumber);
  }

  const char * internal_get_address(Client *obj) {
    return make_string_copy(obj->get_address().c_str());
  }

  bool internal_initialize(Client *obj){
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
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

  void internal_set_ping_interval(Client* obj, int ping_interval){
    obj->set_ping_interval(ping_interval);
  }

  void internal_start_auto_ping(Client* obj) {
    obj->start_auto_ping();
  }

  void internal_stop_auto_ping(Client* obj){
    obj->stop_auto_ping();
  }

  int internal_get_ping_interval(Client* obj){
    return obj->get_ping_interval();
  }

  void set_debug(void(*d)(const char*)){
    debugLog = d;
  }

  void set_on_terminating(void(*d)()){
    onTerminating = d;
  }

}
