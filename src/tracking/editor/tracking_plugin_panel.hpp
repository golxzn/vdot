#ifndef VDOT_TRACKING_EDITOR_PANEL_H
#define VDOT_TRACKING_EDITOR_PANEL_H

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "tracking/tracking_server.hpp"

class TrackingPluginPanel : public godot::Control {
  GDCLASS(TrackingPluginPanel, godot::Control)

  godot::ItemList *_interface_list       = nullptr;
  godot::ItemList *_tracker_list         = nullptr;

  godot::Control *_main_container        = nullptr;
  godot::Control *_main_container_item   = nullptr;

  TrackingInterface *_selected_interface = nullptr;

protected:
  static void _bind_methods();

public:
  TrackingPluginPanel();
  ~TrackingPluginPanel() override;

  void _enter_tree() override;

  void _process(double delta) override;

  void _update_interface_list();
  void _update_tracker_list();

  void _on_interface_added(godot::StringName const &p_interface_name);
  void _on_interface_removed(godot::StringName const &p_interface_name);

  void _on_tracker_added(
    godot::StringName const    &p_tracker_name,
    TrackingServer::TrackerType p_tracker_type
  );
  void _on_tracker_removed(
    godot::StringName const    &p_tracker_name,
    TrackingServer::TrackerType p_tracker_type
  );

  void _on_interface_selected(int p_index);
  void _on_tracker_selected(int p_index);
};

#endif // VDOT_TRACKING_EDITOR_PANEL_H
