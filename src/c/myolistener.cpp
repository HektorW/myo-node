#ifndef MYO_LISTENER
#define MYO_LISTENER

#include <myo/myo.hpp>
#include <uv.h>

#include <map>
#include <string>

#include "eventhandle.cpp"

class MyoListener : public myo::DeviceListener
{
private:
  std::map<std::string, EventHandle>* bindings;
  std::map<myo::Myo*, std::string> myo_ids;

  int _guid = 1;

  string GUID()
  {
    return string("myo" + _guid++);
  }

public:
  MyoListener(std::map<std::string, EventHandle>* dataHandles) : bindings(dataHandles)
  {}

  void onConnect(myo::Myo* myo, uint64_t timestamp)
  {
    myo_ids[myo] = this->GUID();


    std::map<std::string, EventHandle>::iterator iter = bindings->find("connect");
    if (iter == bindings->end()) return;

    EventHandle binding = iter->second;

    string myo_id = myo_ids[myo];

    uv_rwlock_wrlock(&binding.lock);

    binding.data.push(EventData(myo_id, timestamp));

    uv_rwlock_wrunlock(&binding.lock);

    uv_async_send(&binding.async_handle);
  }

  void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
  {

  }

  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
  {
  }
};


#endif