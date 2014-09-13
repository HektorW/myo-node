#ifndef MYO_LISTENER
#define MYO_LISTENER

#include <myo.hpp>
#include <datahandle.cpp>
#include <map>
#include <string>


class MyoListener : public myo::DeviceListener
{
private:
  std::map<std::string, DataHandle>* p_dataHandles

public:
  MyoListener(std::map<std::string, DataHandle>* dataHandles)
    : p_dataHandles(dataHandles)
  {}

  void onConnect(myo::Myo* myo, uint64_t timestamp)
  {
    // this->p_dataHandles->
  }

  void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
  {
  }

  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
  {
  }
};


#endif