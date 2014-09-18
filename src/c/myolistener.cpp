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

    if (eventHandle == 0) {
      return;
    }

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

  // Connections
  void onConnect(myo::Myo* myo, uint64_t timestamp)
  {
    myo_ids[myo] = this->GUID();

    this->addEventData(myo, timestamp, "connect", EventData());
  }
  void onPair(myo::Myo* myo, uint64_t timestamp)
  {
    this->addEventData(myo, timestamp, "pair", EventData());
  }
  void onArmRecognized(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection direction)
  {
    this->addEventData(myo, timestamp, "arm", EventData());
  }

  // Disconnects
  void onDisconnect(myo::Myo* myo, uint64_t timestamp)
  {
    this->addEventData(myo, timestamp, "disconnect", EventData());
  }
  void onUnpair(myo::Myo* myo, uint64_t timestamp)
  {
    this->addEventData(myo, timestamp, "unpair", EventData());
  }
  void onArmlost(myo::Myo* myo, uint64_t timestamp)
  {
    this->addEventData(myo, timestamp, "armlost", EventData());
  }

  // Orientaion
  void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
  {
    const int count = 4;
    float values[count] = { quat.x(), quat.y(), quat.z(), quat.w() };
    this->addEventData(myo, timestamp, "orientation", VectorEventData(values, count));
  }
  void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& vec)
  {
    const int count = 3;
    float values[count] = { values[0] = vec.x(), values[1] = vec.y(), values[2] = vec.z() };
    this->addEventData(myo, timestamp, "gyroscope", VectorEventData(values, count));
  }
  void onAccelerationData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& vec)
  {
    const int count = 3;
    float values[count] = { values[0] = vec.x(), values[1] = vec.y(), values[2] = vec.z() };
    this->addEventData(myo, timestamp, "acceleration", VectorEventData(values, count));
  }


  // Pose
  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
  {
    this->addEventData(myo, timestamp, "pose", StringEventData(pose.toString()));
  }

  // Rssi
  void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi)
  {
    this->addEventData(myo, timestamp, "rssi", NumberEventData(rssi));
  }
};


#endif