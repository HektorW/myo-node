#ifdef NODE_BINDINGS
#define NODE_BINDINGS

#include <node.h>
#include <v8.h>

class NodeBindings
{
public:
  Persistent<Function> onConnectCallback;
  Persistent<Function> onPoseCallback;

  NodeBindings()
    : onConnectCallback(0), onPoseCallback(0)
  {
  }
};


#endif NODE_BINDINGS