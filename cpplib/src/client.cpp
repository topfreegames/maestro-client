#include <iostream>
#include <string>
#include <chrono>
#include <restclient-cpp/restclient.h>
#include "../include/utils.h"
#include <boost/format.hpp>
#include "../include/client.h"
#include "../include/json.hpp"

using namespace Maestro;
using json = nlohmann::json;

Client::Client() {
  this->ping_interval = 30;
}

Client::Client(std::string maestro_api_url) {
  this->maestro_api_url = maestro_api_url;
  this->ping_interval = 30;
}

Client::Client(std::string maestro_api_url, int ping_interval) {
  this->maestro_api_url = maestro_api_url;
  this->ping_interval = ping_interval;
}

std::string Client::get_address(){
  std::string address_url = (boost::format("%s/scheduler/%s/rooms/%s/address")
      % this->maestro_api_url % this->room_scheduler % this->room_id).str();
  RestClient::Response r = RestClient::get(address_url);
	auto res = json::parse(r.body); 
	if (res.at("success")) {
		return (boost::format("%s:%d") % res.at("host").get<std::string>() % res.at("port")).str();
	} else {
		return "";
	}
}

int Client::get_ping_interval(){
  return this-> ping_interval;
}

bool Client::initialize(){
  char * scheduler = std::getenv("MAESTRO_SCHEDULER_NAME");
  char * id = std::getenv("MAESTRO_ROOM_ID");
  if (!scheduler || !id || strlen(scheduler) == 0 || strlen(id) == 0) {
    return false;
  }
  this->room_scheduler = std::string(scheduler);
  this->room_id = std::string(id);
  // TODO check for connectivity with maestro api
  return true;
}

bool Client::initialize(std::string scheduler, std::string id){
  this->room_scheduler = scheduler;
  this->room_id = id;
  // TODO check for connectivity with maestro api
  return true;
}

bool Client::match_ended() {
  return this->update_status(MATCH_ENDED);
}

bool Client::match_started() {
  return this->update_status(MATCH_STARTED);
}

bool Client::ping() {
  //TODO what happens if the api is offline?
  std::string put_url = (boost::format("%s/scheduler/%s/rooms/%s/ping") 
      % this->maestro_api_url % this->room_scheduler % this->room_id).str();
  RestClient::Response r = RestClient::put(put_url, "application/json", "{\"timestamp\": }");
	auto res = json::parse(r.body); 
	return res.at("success");
}

bool Client::room_ready() {
  return this->update_status(ROOM_READY);
}

void Client::set_maestro_api_url(std::string maestro_api_url) {
  this->maestro_api_url = maestro_api_url;
}

void Client::set_ping_interval(int ping_interval) {
  this->ping_interval = ping_interval;
}

bool Client::update_status(std::string status) {
  std::string put_url = (boost::format("%s/scheduler/%s/rooms/%s/status") 
      % this->maestro_api_url % this->room_scheduler % this->room_id).str();
  long timestamp = unix_timestamp();
  RestClient::Response r = RestClient::put(put_url, "application/json", 
      (boost::format("{\"timestamp\":%ld, \"status\":\"%s\"}") % timestamp % status).str());
	auto res = json::parse(r.body); 
	return res.at("success");
}

bool Client::terminated() {
  return this->update_status(TERMINATED);
}

bool Client::terminating() {
  return this->update_status(TERMINATING);
}
