#ifndef DATA_HANDLE
#define DATA_HANDLE

#include <node.h>
#include <v8.h>
#include <uv.h>

#include <string>
#include <queue>

using namespace v8;
using namespace std;


// class VectorEventData : public EventData;
// class PoseEventData : public EventData;



class EventData {
public:
  string myo_id;
  uint64_t timestamp;

  EventData() {}
  EventData(string myo_id, uint64_t timestamp)
    : myo_id(myo_id), timestamp(timestamp)
  {}

  Handle<Value> getData()
  {
    return Undefined();
  }
};


class VectorEventData : public EventData {
public:
  float* values;
  int count;
  string name;

  VectorEventData(string name, float* values, int count) {
    this->name = name;
    this->values = values;
    this->count = count;
  }
  ~VectorEventData()
  {
    if (values) delete[] values;
    values = 0;
  }

  Handle<Value> getData()
  {
    Local<Object> obj = Object::New();

    const string names[4] = {"x", "y", "z", "w"};
    for (int i = 0; i < this->count; ++i) {
      obj->Set(String::NewSymbol(names[i].c_str()), Number::New(this->values[i]));
    }

    return obj;
  }
};

class PoseEventData : public EventData {
public:
  string pose;
  string name;

  PoseEventData() : name("pose")
  {}

  Handle<Value> getData()
  {
    return String::NewSymbol(this->pose.c_str());
  }
};





class EventHandle
{
public:
  Persistent<Function> callback;
  uv_async_t async_handle;
  uv_rwlock_t lock;
  string type;

  queue<EventData> data;

  EventHandle() {}
  ~EventHandle() {}
};








#endif