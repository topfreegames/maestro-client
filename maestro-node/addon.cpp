#include <nan.h>
#include <functional>
#include <iostream>

#include "client.h"

using namespace Nan;
using namespace v8;
using namespace std;

class MaestroWorker : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init) {
      v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
      tpl->SetClassName(Nan::New("MaestroWorker").ToLocalChecked());
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      Nan::SetPrototypeMethod(tpl, "initialize", Initialize);
      Nan::SetPrototypeMethod(tpl, "updateStatus", UpdateStatus);
      Nan::SetPrototypeMethod(tpl, "roomOccupied", RoomOccupied);
      Nan::SetPrototypeMethod(tpl, "roomTerminated", RoomTerminated);
      Nan::SetPrototypeMethod(tpl, "roomTerminating", RoomTerminating);
      Nan::SetPrototypeMethod(tpl, "roomReady", RoomReady);
      Nan::SetPrototypeMethod(tpl, "playerJoin", PlayerJoin);
      Nan::SetPrototypeMethod(tpl, "playerLeft", PlayerLeft);
      Nan::SetPrototypeMethod(tpl, "startAutoPing", StartAutoPing);
      Nan::SetPrototypeMethod(tpl, "stopAutoPing", StopAutoPing);
      Nan::SetPrototypeMethod(tpl, "getAddress", GetAddress);
      constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
      Nan::Set(target, Nan::New("MaestroWorker").ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
    }

  private:
    explicit MaestroWorker(string maestro_api_url) {
      this->client = new Maestro::Client(maestro_api_url);
    }
    explicit MaestroWorker(string maestro_api_url, int ping_interval) {
      this->client = new Maestro::Client(maestro_api_url, ping_interval);
    }
    ~MaestroWorker() {}

    static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      if (info.Length() == 0) {
        Nan::ThrowTypeError("inform maestro api url");
        return;
      } else if (!info[0]->IsString()) {
        Nan::ThrowTypeError("maestro api url must be string");
        return;
      } else if (info.Length() > 1 && !info[1]->IsNumber()) {
        Nan::ThrowTypeError("ping interval must be an integer");
        return;
      }

      std::string maestro_api_url {*Nan::Utf8String(info[0])};
      MaestroWorker* obj;
      if (info.Length() == 1) {
        obj = new MaestroWorker(maestro_api_url);
      } else {
        int ping_interval = info[1]->IntegerValue();
        obj = new MaestroWorker(maestro_api_url, ping_interval);
      }
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(Initialize) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    bool success;
    if (info.Length() == 0) {
      success = obj->client->initialize();
    } else if (info.Length() == 2) {
      std::string scheduler {*Nan::Utf8String(info[0])};
      std::string room_id {*Nan::Utf8String(info[1])};
      success = obj->client->initialize(scheduler, room_id);
    } else {
      Nan::ThrowTypeError("wrong number of arguments: use no arguments or two arguments(scheduler name and room id)");
      return;
    }
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(UpdateStatus) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string status {*Nan::Utf8String(info[0])};
    std::string metadata {*Nan::Utf8String(info[1])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->update_status(status, metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(RoomOccupied) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->room_occupied(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(RoomTerminated) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->room_terminated(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(RoomTerminating) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->room_terminating(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(RoomReady) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->room_ready(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(PlayerJoin) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->player_join(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(PlayerLeft) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::string metadata {*Nan::Utf8String(info[0])};
    metadata = metadata == "undefined" ? "" : metadata;
    bool success = obj->client->player_left(metadata);
    info.GetReturnValue().Set(Nan::New<Boolean>(success));
  }

  static NAN_METHOD(StartAutoPing) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    std::thread th = obj->client->start_auto_ping();
    th.detach();
  }

  static NAN_METHOD(StopAutoPing) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    obj->client->stop_auto_ping();
  }

  static NAN_METHOD(GetAddress) {
    MaestroWorker* obj = Nan::ObjectWrap::Unwrap<MaestroWorker>(info.Holder());
    string address = obj->client->get_address();
    info.GetReturnValue().Set(Nan::New(address).ToLocalChecked());
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> maestro;
    return maestro;
  }

  Maestro::Client *client;
};

NODE_MODULE(maestro, MaestroWorker::Init)
