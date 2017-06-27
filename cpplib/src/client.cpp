#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <restclient-cpp/restclient.h>
#include "../include/utils.h"
#include <boost/format.hpp>
#include "../include/client.h"
#include "../include/json.hpp"

using namespace Maestro;
using json = nlohmann::json;

Client::Client() {
  this->status = "creating";
  this->ping_interval = 30;
}

Client::Client(std::string maestro_api_url) {
  this->status = "creating";
  this->ping_interval = 30;
  this->maestro_api_url = maestro_api_url;
}

Client::Client(std::string maestro_api_url, int ping_interval) {
  this->status = "creating";
  this->maestro_api_url = maestro_api_url;
  this->ping_interval = ping_interval;
}

std::string Client::get_address(){
  try {
    std::string address_url = (boost::format("%s/scheduler/%s/rooms/%s/address")
        % this->maestro_api_url % this->room_scheduler % this->room_id).str();
    RestClient::Response r = RestClient::get(address_url);
    if (r.code == 200) {
      return r.body;
    } else {
      return "";
    }
  } catch (std::invalid_argument e) {
    std::cout << "failed to send ping to the api e." << e.what() << "\n";
  }
  return "";
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

int Client::get_ping_interval() {
  return this->ping_interval;
}

bool Client::initialize(std::string scheduler, std::string id){
  this->room_scheduler = scheduler;
  this->room_id = id;
  // TODO check for connectivity with maestro api
  return true;
}

bool Client::room_occupied() {
  return this->update_status(ROOM_OCCUPIED);
}

bool Client::ping() {
  //TODO what happens if the api is offline?
  try {
    std::string put_url = (boost::format("%s/scheduler/%s/rooms/%s/ping")
        % this->maestro_api_url % this->room_scheduler % this->room_id).str();
    long timestamp = unix_timestamp();
    std::string putBody = (boost::format("{\"timestamp\": %ld, \"status\": \"%s\"}")
        % timestamp % this->status).str();
    RestClient::Response r = RestClient::put(put_url, "application/json", putBody);
    auto res = json::parse(r.body); 
    return res.at("success");
  } catch (std::invalid_argument e) {
    std::cout << "failed to send ping to the api e." << e.what() << "\n";
  }
  return false;
}

void Client::ping_loop(){
  while (!this->stop_ping.load()) {
    this->ping();
    std::this_thread::sleep_for(std::chrono::seconds(this->ping_interval));
  }
}

bool Client::room_terminated() {
  return this->update_status(ROOM_TERMINATED);
}

bool Client::room_terminating() {
  return this->update_status(ROOM_TERMINATING);
}

bool Client::room_ready() {
  return this->update_status(ROOM_READY);
}

void Client::set_maestro_api_url(std::string maestro_api_url) {
  this->maestro_api_url = maestro_api_url;
}

void Client::set_ping_interval(int interval) {
  this->ping_interval = interval;
}

std::thread Client::start_auto_ping(){
  this->stop_ping.store(0);
  return std::thread(&Client::ping_loop, this);
}

void Client::stop_auto_ping(){
  this->stop_ping.store(1);
}

bool Client::update_status(std::string status) {
  try {
    this->status = status;
    std::string put_url = (boost::format("%s/scheduler/%s/rooms/%s/status")
        % this->maestro_api_url % this->room_scheduler % this->room_id).str();
    long timestamp = unix_timestamp();
    RestClient::Response r = RestClient::put(put_url, "application/json",
        (boost::format("{\"timestamp\":%ld, \"status\":\"%s\"}") % timestamp % this->status).str());
    auto res = json::parse(r.body);
    return res.at("success");
  } catch (std::invalid_argument e) {
    std::cout << "failed to send ping to the api e. " << e.what() << "\n";
  }
  return false;
}
