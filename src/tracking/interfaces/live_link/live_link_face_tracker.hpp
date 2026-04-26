#ifndef VDOT_LIVE_LINK_FACE_TRACKER_H
#define VDOT_LIVE_LINK_FACE_TRACKER_H

#include "tracking/interfaces/live_link/live_link_server.hpp"
#include "tracking/tracker_face.hpp"

/**
 * Implements a LiveLink-based face tracker.
 */
class LiveLinkFaceTracker : public FaceTracker {
  GDCLASS(LiveLinkFaceTracker, FaceTracker)

  godot::Ref<LiveLinkClient> _client = nullptr;

protected:
  static void _bind_methods();

public:
  LiveLinkFaceTracker() = default;
  explicit LiveLinkFaceTracker(godot::Ref<LiveLinkClient> const &p_client);
  ~LiveLinkFaceTracker() override;

  static godot::StringName _tracker_name(
    godot::Ref<LiveLinkClient> const &client
  );
};

#endif // VDOT_LIVE_LINK_FACE_TRACKER_H
