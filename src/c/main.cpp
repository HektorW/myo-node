// Includes
#include <iostream>
#include <stdexcept>
#include <stdio.h>

#include <node.h>
#include <v8.h>
#include <uv.h>

#include <myo/myo.hpp>

#include <string>
#include <map>
#include <queue>

#include "eventhandle.cpp"
#include "myoListener.cpp"

using namespace v8;
using namespace std;





// # Defintions
// thread
void threadLoop(uv_work_t*);
void closeThread(uv_work_t*);
void postToMainThread(uv_async_t*, int);

// utils
EventHandle* getHandleFromAsync(uv_async_t* handle);
string getStringArgument(const Arguments& args, int index);

// exports
Handle<Value> addListener(const Arguments& args);
Handle<Value> start(const Arguments& args);
Handle<Value> stop(const Arguments& args);



/////////////////
// # Variables //
/////////////////
myo::Hub* hub;
map<string, EventHandle> eventHandles;
bool thread_running = false;
uv_work_t thread;





void threadLoop(uv_work_t* thread)
{
  try {
    myo::Myo* myo = hub->waitForMyo(10000);

    if (!myo) throw std::runtime_error("Unable to find any myo device.");

    printf("myo connected, starting listening loop\n");

    MyoListener listener(&eventHandles);
    hub->addListener(&listener);

    printf("starting loop\n");
    while(thread_running)
    {
      hub->runOnce(33);
    }
  }
  catch(const std::exception& e)
  {
    fprintf(stderr, "Error, exception caught: %s\n", e.what());
  }
}
void closeThread(uv_work_t* thread)
{
  printf("closing thread\n");
  delete hub;
}

void postToMainThread(uv_async_t* handle, int status) {
  EventHandle* eventHandle = getHandleFromAsync(handle);

  if (!eventHandle) return;

  while (!eventHandle->data.empty())
  {
    EventData data = eventHandle->data.front();
    eventHandle->data.pop();


    Local<Object> argumentObject = Object::New();

    argumentObject->Set(String::NewSymbol("myoId"), String::NewSymbol(data.myo_id.c_str()));
    argumentObject->Set(String::NewSymbol("timestamp"), Number::New(data.timestamp));

    Handle<Value> extra = data.getData();

    if (extra != Undefined())
    {
      argumentObject->Set(String::NewSymbol(eventHandle->type.c_str()), extra);
    }


    Local<Value> args[1] = { argumentObject };
    eventHandle->callback->Call(Context::GetCurrent()->Global(), 1, args);
  }
}


// Utils
EventHandle* getHandleFromAsync(uv_async_t* handle)
{
  for(map<string, EventHandle>::iterator iter = eventHandles.begin(); iter != eventHandles.end(); ++iter)
  {
    if (&iter->second.async_handle == handle)
      return &iter->second;
  }
  return 0;
}
string getStringArgument(const Arguments& args, int index)
{
  if (args.Length() < index || !args[index]->IsString())
  {
    // ThrowException(Exception::TypeError(String::New(string("Argument nr ") + to_string(index) + string(" is not a string"))));
    ThrowException(Exception::TypeError(String::New("invalid arguments")));
  }
  return string(*v8::String::AsciiValue(args[0]->ToString()));;
}



////////////////////////
// Exported functions //
////////////////////////
// addListener
Handle<Value> addListener(const Arguments& args)
{
  printf("c++, exports, addListener\n");


  // add error checking
  string type = getStringArgument(args, 0);
  Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

  EventHandle handle(type, callback);
  eventHandles[type] = handle;

  return Undefined();
}

Handle<Value> start(const Arguments& args)
{
  printf("c++, exports, start\n");
  if (!thread_running)
  {
    try 
    {
      string appId = string(*v8::String::AsciiValue(args[0]->ToString()));
      hub = new myo::Hub(appId);

      for (map<string, EventHandle>::iterator iter = eventHandles.begin(); iter != eventHandles.end(); ++iter) {
        EventHandle handle = iter->second;

        uv_rwlock_init(&handle.lock);
        uv_async_init(uv_default_loop(), &handle.async_handle, postToMainThread);
      }

      uv_queue_work(uv_default_loop(), &thread, threadLoop, (uv_after_work_cb)closeThread);

      thread_running = true; 
    }
    catch(const std::exception& e)
    {
      fprintf(stderr, "Error, exception caught: %s\n", e.what());
    }
  }

  return Undefined();
}

Handle<Value> stop(const Arguments& args)
{
  printf("c++, exports, stop\n");

  thread_running = false;
  return Undefined();
}





////////////////////
// Expose exports //
////////////////////
void setupExports(v8::Handle<v8::Object> exports)
{
  exports->Set(String::NewSymbol("addListener"), FunctionTemplate::New(addListener)->GetFunction());
  exports->Set(String::NewSymbol("start"), FunctionTemplate::New(start)->GetFunction());
  exports->Set(String::NewSymbol("stop"), FunctionTemplate::New(stop)->GetFunction());
}

// setup module 
NODE_MODULE(myo, setupExports)
