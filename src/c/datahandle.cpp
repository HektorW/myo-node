#ifndef DATA_HANDLE
#define DATA_HANDLE

#include <node.h>
#include <v8.h>
#include <uv.h>
#include <string>

using namespace v8;


class DataHandle {
public:
  Persistent<Function>* callback;
  uv_async_t asyncHandle;
  std::string type;
  uint64_t timestamp;

  DataHandle()
    : callback(NULL)
  {}

  Handle<Value> getData()
  {
    return Undefined();
  }
};


class VectorDataHandle : public DataHandle {
public:
  float* values;
  int count;
  std::string name;

  VectorDataHandle(std::string name, int count) {
    this->name = name;
    this->count = count;
    this->values = new float[count];
  }
  ~VectorDataHandle()
  {
    if (values) delete values;
    values = 0;
  }

  Handle<Value> getData()
  {
    Local<Object> obj = Object::New();

    const std::string names[4] = {"x", "y", "z", "w"};
    for (int i = 0; i < this->count; ++i) {
      obj->Set(String::NewSymbol(names[i]), Number::New(this->values[i]));
    }

    return obj;
  }
};

class PoseDataHandle : public DataHandle {
public:
  std::string pose;
  std::string name;

  PoseDataHandle() : name("pose")
  {}

  Handle<Value> getData()
  {
    return String::NewSymbol(this->pose);
  }
};


#endif