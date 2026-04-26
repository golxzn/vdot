#include "tracking/interfaces/live_link/live_link_face_tracker.hpp"

void LiveLinkFaceTracker::_bind_methods() {}

LiveLinkFaceTracker::LiveLinkFaceTracker(
  godot::Ref<LiveLinkClient> const &p_client
) {
  _client             = p_client;

  tracker_type        = TrackingServer::TRACKER_FACE;
  tracker_name        = _tracker_name(_client);
  tracker_description = godot::vformat(
    "LiveLink face tracker %s (%s).", _client->_name, _client->_id
  );
}

LiveLinkFaceTracker::~LiveLinkFaceTracker() = default;

godot::StringName LiveLinkFaceTracker::_tracker_name(
  godot::Ref<LiveLinkClient> const &client
) {
  return client->_name;
}
