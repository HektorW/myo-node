// Includes
#include <node.h>
#include <v8.h>

#include <uv.h>

#include <string>
#include <map>

#include "datahandle.cpp"

using namespace v8;


struct NodeHandle {
  Persistent<Function> callback;
  uv_async_t asyncHandle;
  std::string name;
};




bool setup = false;
uv_work_t thread;
NodeHandle fooCB;
NodeHandle barCB;
// std::map<std::string, NodeHandle>


void startThread(uv_work_t* thread)
{
  int i = 2;
  while(i--) {
    printf("loop start\n");
    uv_async_send(&fooCB.asyncHandle);
    uv_async_send(&barCB.asyncHandle);
  }
  printf("thread work is done\n");
}
void closeThread(uv_work_t* thread) {
  uv_close((uv_handle_t*) &fooCB.asyncHandle, 0);
  uv_close((uv_handle_t*) &barCB.asyncHandle, 0);
}

void AsyncUpdate(uv_async_t* handle, int status) {
  NodeHandle* temp;

  if (handle == &fooCB.asyncHandle) {
    temp = &fooCB;
  }
  if (handle == &barCB.asyncHandle) {
    temp = &barCB;
  }

  Local<Value> args[1] = { String::NewSymbol(temp->name.c_str()) };
  temp->callback->Call(Context::GetCurrent()->Global(), 1, args);
}

void setupThread()
{
  if (setup) return;

  for (DataHandle iter = handles.first(); iter != handles.last(); iter = handles.next()) {
    uv_async_init(uv_default_loop(), &iter.asyncHandle, AsyncUpdate);
  }

  uv_async_init(uv_default_loop(), &fooCB.asyncHandle, AsyncUpdate);
  uv_async_init(uv_default_loop(), &barCB.asyncHandle, AsyncUpdate);
  uv_queue_work(uv_default_loop(), &thread, startThread, (uv_after_work_cb)closeThread);

  setup = true;
}




Handle<Value> foo(const Arguments& args)
{
  fooCB.callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  fooCB.name = "foo";
  setupThread();
  return Undefined();
}
Handle<Value> bar(const Arguments& args)
{
  barCB.callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  barCB.name = "bar";
  setupThread();
  return Undefined();
}

void init(v8::Handle<v8::Object> exports)
{
  exports->Set(String::NewSymbol("foo"), FunctionTemplate::New(foo)->GetFunction());
  exports->Set(String::NewSymbol("bar"), FunctionTemplate::New(bar)->GetFunction());
}


Handle<Value> start(const Arguments& args)
{
  startThread();
}
Handle<Value> stop(const Arguments& args)
{
  this.running = false;
}


std::map<std::string, DataHandle> handles;

Handle<Value> addListener(const Arguments& args)
{
  std::string type = std::string(*v8::String::AsciiValue(args[0]->ToString()));
  Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

  DataHandle handle;
  switch (type) {
    case "connect":
    case "disconnect":
      handle = DataHandle();
      break;
    case "orientation":
      handle = VectorDataHandle(type, 4);
      break;
    case "gyroscope":
      handle = VectorDataHandle(type, 3);
      break;
  }

  handle.callback = callback;
  handle.type = type;

  handles.add(type, handle);

  return Undefined();
}



// Exports
NODE_MODULE(myo, init)
