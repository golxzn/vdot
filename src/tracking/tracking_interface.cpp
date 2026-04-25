#include "tracking_interface.h"

#include "utility/method-hash.hpp"

void TrackingInterface::_bind_methods() {
  using namespace vdot;
  BIND_VIRTUAL_METHOD(
    TrackingInterface, get_name, make_hash(&TrackingInterface::get_name)
  );

  BIND_VIRTUAL_METHOD(
    TrackingInterface,
    is_initialized,
    make_hash(&TrackingInterface::is_initialized)
  );
  BIND_VIRTUAL_METHOD(
    TrackingInterface, initialize, make_hash(&TrackingInterface::initialize)
  );
  BIND_VIRTUAL_METHOD(
    TrackingInterface,
    uninitialize,
    make_hash(&TrackingInterface::uninitialize)
  );

  BIND_VIRTUAL_METHOD(
    TrackingInterface, process, make_hash(&TrackingInterface::process)
  );
}

TrackingInterface::TrackingInterface()  = default;

TrackingInterface::~TrackingInterface() = default;
