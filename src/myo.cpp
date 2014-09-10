#include <node.h>
#include <v8.h>

#include <uv.h>

#include <myo.hpp>

#include <iostream>
#include <stdexcept>
#include <stdio.h>

#include <string>

using namespace v8;


/**
 * Questions
 * 
 * runOnce or run(for a set time)
 * report new values to js on each event or stack events and fire at once
 *
 * preferably use runOnce and fire events at each callback
 */

/**
 * TODO
 *
 * - might need to generate unique ids for each myo and couple them with myo instances memory adress
 */



void DoAsync(uv_work_t*);
void ProgressUpdate(uv_async_t*, int);
void AfterAsync(uv_work_t*);


// Global vars
Persistent<Function> persist;
uv_async_t async;
uv_rwlock_t lock;



// structs
struct async_req {
  uv_work_t req;
  Persistent<Function> callback;
};
struct myo_data {
  float orientation_x, orientation_y, orientation_z, orientation_w;
  float accelerometer_x, accelerometer_y, accelerometer_z;
  float gyroscope_x, gyroscope_y, gyroscope_z;
  myo::Pose pose;
} data;





// myo listener
class DataCollector : public myo::DeviceListener
{
public:
  DataCollector() {}

  void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
  {
    uv_rwlock_wrlock(&lock);
    data.orientation_x = quat.x();
    data.orientation_y = quat.y();
    data.orientation_z = quat.z();
    data.orientation_w = quat.w();
    uv_rwlock_wrunlock(&lock);
  }

  void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acc)
  {
    uv_rwlock_wrlock(&lock);
    data.accelerometer_x = acc.x();
    data.accelerometer_y = acc.y();
    data.accelerometer_z = acc.z();
    uv_rwlock_wrunlock(&lock);
  }

  void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
  {
    uv_rwlock_wrlock(&lock);
    data.gyroscope_x = gyro.x();
    data.gyroscope_y = gyro.y();
    data.gyroscope_z = gyro.z();
    uv_rwlock_wrunlock(&lock);
  }

  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
  {
    uv_rwlock_wrlock(&lock);
    data.pose = pose;
    uv_rwlock_wrunlock(&lock);
  }
};



void DoAsync(uv_work_t* r)
{
  try {
    myo::Hub hub;

    fprintf(stderr, "Attempting to find a Myo...\n");
    myo::Myo* myo = hub.waitForAnyMyo(10000);

    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    // fprintf(stderr, "Connected to %s.\n", myo->macAddressAsString());
    std::cout << "Connected to " << myo->macAddressAsString() << std::endl;

    if (!myo->isTrained()) {
      throw std::runtime_error("Myo has not been trained. Do you need to run the trainer?");
    }

    DataCollector collector;
    hub.addListener(&collector);

    while (true)
    {
      hub.run(33);

      uv_async_send(&async);
    }
  } catch(const std::exception& e) {
    fprintf(stderr, "Error, exception caught: %s\n", e.what());
  }
}

void ProgressUpdate(uv_async_t* handle, int status)
{
  Local<Object> obj = Object::New();
  Local<Object> orientation = Object::New();
  Local<Object> accelerometer = Object::New();
  Local<Object> gyroscope = Object::New();

  uv_rwlock_wrlock(&lock);

  orientation->Set(String::NewSymbol("x"), Number::New(data.orientation_x));
  orientation->Set(String::NewSymbol("y"), Number::New(data.orientation_y));
  orientation->Set(String::NewSymbol("z"), Number::New(data.orientation_z));
  orientation->Set(String::NewSymbol("w"), Number::New(data.orientation_w));

  accelerometer->Set(String::NewSymbol("x"), Number::New(data.accelerometer_x));
  accelerometer->Set(String::NewSymbol("y"), Number::New(data.accelerometer_y));
  accelerometer->Set(String::NewSymbol("z"), Number::New(data.accelerometer_z));

  gyroscope->Set(String::NewSymbol("x"), Number::New(data.gyroscope_x));
  gyroscope->Set(String::NewSymbol("y"), Number::New(data.gyroscope_y));
  gyroscope->Set(String::NewSymbol("z"), Number::New(data.gyroscope_z));

  obj->Set(String::NewSymbol("pose"), String::NewSymbol(data.pose.toString().c_str()));

  uv_rwlock_wrunlock(&lock);

  obj->Set(String::NewSymbol("orientation"), orientation);
  obj->Set(String::NewSymbol("accelerometer"), accelerometer);
  obj->Set(String::NewSymbol("gyroscope"), gyroscope);

  const unsigned argc = 1;
  Local<Value> argv[argc] = { obj };

  persist->Call(Context::GetCurrent()->Global(), argc, argv);
}

void AfterAsync(uv_work_t* r)
{ 
  fprintf(stderr, "Thread closed\n");

  uv_close((uv_handle_t*) &async, 0);
}







// Exports
v8::Handle<v8::Value> AddListener(const v8::Arguments& args)
{
  Handle<Function> ftpl = Handle<Function>::Cast(args[0]);

  async_req* req = new async_req;
  persist = Persistent<Function>::New(ftpl);

  // setup our thread lock
  uv_rwlock_init(&lock);

  // setup our async handler, injects into MainEvent loop
  uv_async_init(uv_default_loop(), &async, ProgressUpdate);
  // setup working thread
  uv_queue_work(uv_default_loop(),
                &req->req,
                DoAsync,
                (uv_after_work_cb)AfterAsync);

  return Undefined();
}


// setup module
void init(v8::Handle<v8::Object> exports)
{
  exports->Set(v8::String::NewSymbol("addListener"), v8::FunctionTemplate::New(AddListener)->GetFunction());
}

NODE_MODULE(myo, init)
