// Includes
#include <node.h>
#include <v8.h>

#include <uv.h>

#include <string>

using namespace v8;



struct cbStruct {
  Persistent<Function> cb;
  uv_async_t asyncHandle;
  std::string name;

  void AsyncUpdate(uv_async_t* handle, int status) {
    Local<Value> arguments[1] = { String::NewSymbol(this->name.c_str()) };
    this->cb->Call(Context::GetCurrent()->Global(), 1, arguments);
  }
};




bool setup = false;
uv_work_t thread;
cbStruct fooCB;
cbStruct barCB;


void startThread(uv_work_t* thread)
{
  int i = 2;
  while(i--) {

  }
}
void closeThread(uv_work_t* thread) {}

void Sync(uv_async_t* handle, int status) {}

void setupThread()
{
  if (setup) return;

  uv_async_init(uv_default_loop(), &fooCB.asyncHandle, Sync);
  // uv_async_init(uv_default_loop(), &barCB.asyncHandle, barCB.AsyncUpdate);

  uv_queue_work(uv_default_loop(), &thread, startThread, (uv_after_work_cb)closeThread);

  setup = true;
}




Handle<Value> foo(const Arguments& args)
{
  fooCB.cb = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  fooCB.name = "foo";
  setupThread();
  return Undefined();
}
Handle<Value> bar(const Arguments& args)
{
  barCB.cb = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  barCB.name = "bar";
  setupThread();
  return Undefined();
}

void init(v8::Handle<v8::Object> exports)
{
  exports->Set(String::NewSymbol("foo"), FunctionTemplate::New(foo)->GetFunction());
  exports->Set(String::NewSymbol("bar"), FunctionTemplate::New(bar)->GetFunction());
}

// Exports
NODE_MODULE(myo, init)
