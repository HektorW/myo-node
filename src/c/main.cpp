// Includes
#include <iostream>
#include <stdexcept>
#include <stdio.h>

#include <node.h>
#include <v8.h>
#include <uv.h>

#include <myo.hpp>

#include <string>
#include <map>

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

// exports
Handle<Value> addListener(const Arguments* args);
Handle<Value> start(const Arguments* args);
Handle<Value> stop(const Arguments* args);



/////////////////
// # Variables //
/////////////////
myo::Hub* hub;
map<string, EventHandle> eventHandles;
bool thread_running = false;





void threadLoop(uv_work_t* thread)
{
  /*try {
    myo::Myo* myo = hub->waitForAnyMyo(10000);

    if (!myo) throw std::runtime_error("Unable to find any myo device.");
    if (!myo->isTrained()) throw std::runtime_error("Myo device has not been trained. Do you need to run the trainer?");

    MyoListener listener(eventHandles);
    hub->addListener(&listener);

    while(thread_running)
    {
      hub->run(33);
    }
  }
  catch(const std::exception& e)
  {
    fprintf(stderr, "Error, exception caught: %s\n", e.what());
  }*/
}
void closeThread(uv_work_t* thread)
{
  delete hub;
}

void postToMainThread(uv_async_t* handle, int status) {
  // EventHandle* temp = getHandleFromAsync(handle);

  // if (!temp) return;

  // set data

  // Local<Value> args[1] = { String::NewSymbol(temp->name.c_str()) };
  // temp->callback->Call(Context::GetCurrent()->Global(), 1, args);
}



////////////////////////
// Exported functions //
////////////////////////
// addListener
Handle<Value> addListener(const Arguments& args)
{
  // add error checking
  /*std::string type = std::string(*v8::String::AsciiValue(args[0]->ToString()));
  Persistent<Function> callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

  EventHandle handle;
  switch (type) {
    case "connect":
    case "disconnect":
      handle = EventHandle();
      break;
    case "orientation":
      handle = VectorEventHandle(type, 4);
      break;
    case "gyroscope":
      handle = VectorEventHandle(type, 3);
      break;
  }

  handle.callback = callback;
  handle.type = type;

  eventHandles.add(type, handle);*/

  return Undefined();
}

Handle<Value> start(const Arguments& args)
{
  if (!thread_running)
  {
    string appId = string(*v8::String::AsciiValue(args[0]->ToString()));
    // hub = new myo::Hub(appId);
    hub = new myo::Hub();

    uv_work_t thread;

    for (map<string, EventHandle>::iterator iter = eventHandles.begin(); iter != eventHandles.end(); ++iter) {
      EventHandle handle = iter->second;

      uv_rwlock_init(&handle.lock);
      uv_async_init(uv_default_loop(), &handle.async_handle, postToMainThread);
    }

    uv_queue_work(uv_default_loop(), &thread, threadLoop, (uv_after_work_cb)closeThread);

    thread_running = true; 
  }

  return Undefined();
}

Handle<Value> stop(const Arguments& args)
{
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
