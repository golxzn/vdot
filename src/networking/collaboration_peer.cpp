#include "networking/collaboration_peer.hpp"

void CollaborationPeer::_bind_methods() {}

CollaborationPeer::CollaborationPeer() {}

CollaborationPeer::CollaborationPeer(
  godot::Ref<godot::ENetMultiplayerPeer> const &p_peer
)
  : enet_peer(p_peer) {}

godot::Ref<godot::ENetMultiplayerPeer> CollaborationPeer::
  get_enet_peer() const {
  return enet_peer;
}
