#ifndef VDOT_FACE_TRACKER_PANEL_H
#define VDOT_FACE_TRACKER_PANEL_H

#include <godot_cpp/classes/control.hpp>

#include "tracking/tracker_face.hpp"

class FaceTrackerPanel : public godot::Control {
  GDCLASS(FaceTrackerPanel, godot::Control)

  godot::Ref<FaceTracker> _tracker;

protected:
  static void _bind_methods();

public:
  FaceTrackerPanel();
  ~FaceTrackerPanel();

  void set_tracker(godot::Ref<FaceTracker> const &p_tracker);
};

#endif // VDOT_FACE_TRACKER_PANEL_H
