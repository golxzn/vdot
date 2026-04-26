#ifndef VDOT_LIVE_LINK_PANEL_H
#define VDOT_LIVE_LINK_PANEL_H

#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "live_link_server.hpp"

class LiveLinkPanel : public godot::Control {
  GDCLASS(LiveLinkPanel, godot::Control)

  godot::MeshInstance3D *_mesh_instance;
  godot::Button         *_start_button;

  godot::Ref<LiveLinkServer> _server;

  bool _server_active = false;

protected:
  static void _bind_methods();

public:
  LiveLinkPanel();
  ~LiveLinkPanel() override;

  void _process(double delta) override;

  static godot::StringName blend_shape_name(int i);

  void start_server();
  void stop_server();

  void update_mesh(godot::Ref<LiveLinkClientData> const data);

  void _on_start_button_pressed();

  void _on_server_client_connected(godot::Ref<LiveLinkClient> const &client);
  void _on_server_client_disconnected(
    godot::Ref<LiveLinkClient> const &client
  );
  void _on_server_client_updated(godot::Ref<LiveLinkClient> const &client);
};

#endif // VDOT_LIVE_LINK_PANEL_H
