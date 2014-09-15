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

  int _guid;

  std::string GUID()
  {
    return std::string("myo" + _guid++);
  }

  EventHandle* getEventHandle(std::string type)
  {
    std::map<std::string, EventHandle>::iterator iter = bindings->find(type);
    if (iter == bindings->end())
      return 0;
    return &iter->second;    
  }

  void addEventData(myo::Myo* myo, uint64_t timestamp, std::string type, EventData eventData)
  {
    EventHandle* eventHandle = this->getEventHandle(type);
    std::string myo_id = this->myo_ids[myo];

    eventData.myo_id = myo_id;
    eventData.timestamp = timestamp;

    uv_rwlock_wrlock(&eventHandle->lock);
    eventHandle->data.push(eventData);
    uv_rwlock_wrunlock(&eventHandle->lock);

    uv_async_send(&eventHandle->async_handle);
  }

public:
  MyoListener(std::map<std::string, EventHandle>* dataHandles) : bindings(dataHandles), _guid(1)
  {}

  void onConnect(myo::Myo* myo, uint64_t timestamp)
  {
    myo_ids[myo] = this->GUID();

    this->addEventData(myo, timestamp, "connect", EventData());
  }

  void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
  {
    const int count = 4;
    float* values = new float[count];
    values[0] = quat.x();
    values[1] = quat.y();
    values[2] = quat.z();
    values[3] = quat.w();

    this->addEventData(myo, timestamp, "orientaion", VectorEventData(values, count));
  }

  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
  {
    this->addEventData(myo, timestamp, "pose", PoseEventData(pose.toString()));
  }
};


#endif