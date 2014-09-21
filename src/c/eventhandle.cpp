#ifndef DATA_HANDLE
#define DATA_HANDLE

#include <node.h>
#include <v8.h>
#include <uv.h>

#include <string>
#include <queue>

using namespace v8;
using namespace std;



class EventData
{
public:
  string myo_id;
  uint64_t timestamp;

  EventData() {}

  virtual Handle<Value> getData()
  {
    return Undefined();
  }
};


class VectorEventData : public EventData
{
public:
  float* values;
  int count;

  VectorEventData(float* values, int count) {
    this->values = values;
    this->count = count;
  }
  ~VectorEventData()
  {
    // if (values != 0)
      // delete[] values;
    // values = 0;
  }

  Handle<Value> getData()
  {
    printf("VectorEventData.getData\n");
    Handle<Object> obj = Object::New();

    const string names[4] = {"x", "y", "z", "w"};
    for (int i = 0; i < this->count; ++i) {
      obj->Set(String::NewSymbol(names[i].c_str()), Number::New(this->values[i]));
    }

    return obj;
  }
};

class StringEventData : public EventData
{
public:
  string value;

  StringEventData(string value) : value(value) {}

  Handle<Value> getData()
  {
    return String::NewSymbol(this->value.c_str());
  }
};


class NumberEventData: public EventData
{
public:
  int value;

  NumberEventData(int value) : value(value) {}

  Handle<Value> getData()
  {
    return Number::New(this->value);
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
  EventHandle(string type, Persistent<Function> callback) : type(type), callback(callback) {}

  ~EventHandle() {}
};








#endif