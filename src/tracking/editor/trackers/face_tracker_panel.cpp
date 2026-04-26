#include "face_tracker_panel.hpp"

void FaceTrackerPanel::_bind_methods() {}

FaceTrackerPanel::FaceTrackerPanel() {}

FaceTrackerPanel::~FaceTrackerPanel() {}

void FaceTrackerPanel::set_tracker(godot::Ref<FaceTracker> const &p_tracker) {
  _tracker = p_tracker;
}
