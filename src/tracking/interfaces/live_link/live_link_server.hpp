#ifndef VDOT_LIVE_LINK_SERVER_H
#define VDOT_LIVE_LINK_SERVER_H

#include <cstdint>
#include <span>

#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packet_peer_udp.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/udp_server.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include "tracking/standards/arkit_blend_shapes.hpp"

inline constexpr size_t BLEND_SHAPE_COUNT{ 61 };
inline constexpr size_t MAX_NAME_LENGTH{ 64 };
inline constexpr size_t ARKIT_PACKET_VERSION{ 0x06 };

struct ARKitBlendShapes {
  using array_type = std::array<float, BLEND_SHAPE_COUNT>;

  // ARKit Blendshapes

  float eye_blink_left{};
  float eye_look_down_left{};
  float eye_look_in_left{};
  float eye_look_out_left{};
  float eye_look_up_left{};
  float eye_squint_left{};
  float eye_wide_left{};

  float eye_blink_right{};
  float eye_look_down_right{};
  float eye_look_in_right{};
  float eye_look_out_right{};
  float eye_look_up_right{};
  float eye_squint_right{};
  float eye_wide_right{};

  float jaw_forward{};
  float jaw_right{};
  float jaw_left{};
  float jaw_open{};

  float mouth_close{};
  float mouth_funnel{};
  float mouth_pucker{};
  float mouth_right{};
  float mouth_left{};

  float mouth_smile_left{};
  float mouth_smile_right{};

  float mouth_frown_left{};
  float mouth_frown_right{};

  float mouth_dimple_left{};
  float mouth_dimple_right{};

  float mouth_stretch_left{};
  float mouth_stretch_right{};

  float mouth_roll_lower{};
  float mouth_roll_upper{};

  float mouth_shrug_lower{};
  float mouth_shrug_upper{};

  float mouth_press_left{};
  float mouth_press_right{};

  float mouth_lower_down_left{};
  float mouth_lower_down_right{};

  float mouth_upper_up_left{};
  float mouth_upper_up_right{};

  float brow_down_left{};
  float brow_down_right{};

  float brow_inner_up{};

  float brow_outer_up_left{};
  float brow_outer_up_right{};

  float cheek_puff{};

  float cheek_squint_left{};
  float cheek_squint_right{};

  float nose_sneer_left{};
  float nose_sneer_right{};

  float tongue_out{};

  // LiveLink specific
  float head_yaw{};
  float head_pitch{};
  float head_roll{};

  float left_eye_yaw{};
  float left_eye_pitch{};
  float left_eye_roll{};

  float right_eye_yaw{};
  float right_eye_pitch{};
  float right_eye_roll{};

  inline auto operator=(array_type const &array) noexcept
    -> ARKitBlendShapes & {
    reinterpret_cast<array_type &>(*this) = array;
    return *this;
  }

  inline auto operator[](this auto &&self, size_t const index) noexcept {
    return self.array()[index];
  }

  inline auto array() const noexcept -> array_type const & {
    return reinterpret_cast<array_type const &>(*this);
  }

  inline auto array() noexcept -> array_type & {
    return reinterpret_cast<array_type &>(*this);
  }

  inline auto binary() const noexcept -> std::span<uint8_t const> {
    return std::span<uint8_t const>{
      reinterpret_cast<uint8_t const *>(this),
      sizeof(ARKitBlendShapes),
    };
  }

  inline auto binary() noexcept -> std::span<uint8_t> {
    return std::span<uint8_t>{
      reinterpret_cast<uint8_t *>(this),
      sizeof(ARKitBlendShapes),
    };
  }
};

struct LiveLinkARKitPacket {
  using uuid_array = std::array<char, 36>;
  using name_array = std::array<char, MAX_NAME_LENGTH>;

  uint8_t          version;
  uint32_t         uuid_length;
  uuid_array       uuid;
  uint32_t         name_length;
  name_array       name;
  uint64_t         timestamp;
  uint64_t         magic_2;
  uint8_t          blend_shape_count;
  ARKitBlendShapes blend_shapes;

  void dump() const;

  static auto from_bytes(godot::PackedByteArray const &bytes)
    -> LiveLinkARKitPacket;
};

/**
 * Holds and decodes packet data received from a Live Link client.
 */
class LiveLinkClientData : public godot::RefCounted {
  GDCLASS(LiveLinkClientData, godot::RefCounted)

public:
  uint64_t         _timecode;
  ARKitBlendShapes _data;

  LiveLinkClientData()           = default;
  ~LiveLinkClientData() override = default;

  float get_blend_shape(ARKit::BlendShape blendShape);

  [[nodiscard]] float _get_eye_blink_left() const {
    return _data.eye_blink_left;
  }

  [[nodiscard]] float _get_eye_look_down_left() const {
    return _data.eye_look_down_left;
  }

  [[nodiscard]] float _get_eye_look_in_left() const {
    return _data.eye_look_in_left;
  }

  [[nodiscard]] float _get_eye_look_out_left() const {
    return _data.eye_look_out_left;
  }

  [[nodiscard]] float _get_eye_look_up_left() const {
    return _data.eye_look_up_left;
  }

  [[nodiscard]] float _get_eye_squint_left() const {
    return _data.eye_squint_left;
  }

  [[nodiscard]] float _get_eye_wide_left() const {
    return _data.eye_wide_left;
  }

  [[nodiscard]] float _get_eye_blink_right() const {
    return _data.eye_blink_right;
  }

  [[nodiscard]] float _get_eye_look_down_right() const {
    return _data.eye_look_down_right;
  }

  [[nodiscard]] float _get_eye_look_in_right() const {
    return _data.eye_look_in_right;
  }

  [[nodiscard]] float _get_eye_look_out_right() const {
    return _data.eye_look_out_right;
  }

  [[nodiscard]] float _get_eye_look_up_right() const {
    return _data.eye_look_up_right;
  }

  [[nodiscard]] float _get_eye_squint_right() const {
    return _data.eye_squint_right;
  }

  [[nodiscard]] float _get_eye_wide_right() const {
    return _data.eye_wide_right;
  }

  [[nodiscard]] float _get_jaw_forward() const { return _data.jaw_forward; }

  [[nodiscard]] float _get_jaw_right() const { return _data.jaw_right; }

  [[nodiscard]] float _get_jaw_left() const { return _data.jaw_left; }

  [[nodiscard]] float _get_jaw_open() const { return _data.jaw_open; }

  [[nodiscard]] float _get_mouth_close() const { return _data.mouth_close; }

  [[nodiscard]] float _get_mouth_funnel() const { return _data.mouth_funnel; }

  [[nodiscard]] float _get_mouth_pucker() const { return _data.mouth_pucker; }

  [[nodiscard]] float _get_mouth_right() const { return _data.mouth_right; }

  [[nodiscard]] float _get_mouth_left() const { return _data.mouth_left; }

  [[nodiscard]] float _get_mouth_smile_left() const {
    return _data.mouth_smile_left;
  }

  [[nodiscard]] float _get_mouth_smile_right() const {
    return _data.mouth_smile_right;
  }

  [[nodiscard]] float _get_mouth_frown_left() const {
    return _data.mouth_frown_left;
  }

  [[nodiscard]] float _get_mouth_frown_right() const {
    return _data.mouth_frown_right;
  }

  [[nodiscard]] float _get_mouth_dimple_left() const {
    return _data.mouth_dimple_left;
  }

  [[nodiscard]] float _get_mouth_dimple_right() const {
    return _data.mouth_dimple_right;
  }

  [[nodiscard]] float _get_mouth_stretch_left() const {
    return _data.mouth_stretch_left;
  }

  [[nodiscard]] float _get_mouth_stretch_right() const {
    return _data.mouth_stretch_right;
  }

  [[nodiscard]] float _get_mouth_roll_lower() const {
    return _data.mouth_roll_lower;
  }

  [[nodiscard]] float _get_mouth_roll_upper() const {
    return _data.mouth_roll_upper;
  }

  [[nodiscard]] float _get_mouth_shrug_lower() const {
    return _data.mouth_shrug_lower;
  }

  [[nodiscard]] float _get_mouth_shrug_upper() const {
    return _data.mouth_shrug_upper;
  }

  [[nodiscard]] float _get_mouth_press_left() const {
    return _data.mouth_press_left;
  }

  [[nodiscard]] float _get_mouth_press_right() const {
    return _data.mouth_press_right;
  }

  [[nodiscard]] float _get_mouth_lower_down_left() const {
    return _data.mouth_lower_down_left;
  }

  [[nodiscard]] float _get_mouth_lower_down_right() const {
    return _data.mouth_lower_down_right;
  }

  [[nodiscard]] float _get_mouth_upper_up_left() const {
    return _data.mouth_upper_up_left;
  }

  [[nodiscard]] float _get_mouth_upper_up_right() const {
    return _data.mouth_upper_up_right;
  }

  [[nodiscard]] float _get_brow_down_left() const {
    return _data.brow_down_left;
  }

  [[nodiscard]] float _get_brow_down_right() const {
    return _data.brow_down_right;
  }

  [[nodiscard]] float _get_brow_inner_up() const {
    return _data.brow_inner_up;
  }

  [[nodiscard]] float _get_brow_outer_up_left() const {
    return _data.brow_outer_up_left;
  }

  [[nodiscard]] float _get_brow_outer_up_right() const {
    return _data.brow_outer_up_right;
  }

  [[nodiscard]] float _get_cheek_puff() const { return _data.cheek_puff; }

  [[nodiscard]] float _get_cheek_squint_left() const {
    return _data.cheek_squint_left;
  }

  [[nodiscard]] float _get_cheek_squint_right() const {
    return _data.cheek_squint_right;
  }

  [[nodiscard]] float _get_nose_sneer_left() const {
    return _data.nose_sneer_left;
  }

  [[nodiscard]] float _get_nose_sneer_right() const {
    return _data.nose_sneer_right;
  }

  [[nodiscard]] float _get_tongue_out() const { return _data.tongue_out; }

  [[nodiscard]] float _get_head_yaw() const { return _data.head_yaw; }

  [[nodiscard]] float _get_head_pitch() const { return _data.head_pitch; }

  [[nodiscard]] float _get_head_roll() const { return _data.head_roll; }

  [[nodiscard]] float _get_left_eye_yaw() const { return _data.left_eye_yaw; }

  [[nodiscard]] float _get_left_eye_pitch() const {
    return _data.left_eye_pitch;
  }

  [[nodiscard]] float _get_left_eye_roll() const {
    return _data.left_eye_roll;
  }

  [[nodiscard]] float _get_right_eye_yaw() const {
    return _data.right_eye_yaw;
  }

  [[nodiscard]] float _get_right_eye_pitch() const {
    return _data.right_eye_pitch;
  }

  [[nodiscard]] float _get_right_eye_roll() const {
    return _data.right_eye_roll;
  }

protected:
  static void _bind_methods();
};

/**
 * Represents a client device connected to the Live Link server.
 *
 * Contains information about the connected client, and the client's current
 * blend-shape values.
 */
class LiveLinkClient : public godot::RefCounted {
  GDCLASS(LiveLinkClient, godot::RefCounted)
public:
  LiveLinkClient()           = default;
  ~LiveLinkClient() override = default;

  godot::String _id;
  godot::String _name;

  uint64_t                         _last_seen;
  godot::Ref<godot::PacketPeerUDP> _connection;
  godot::Ref<LiveLinkClientData>   _values{ memnew(LiveLinkClientData) };

protected:
  static void _bind_methods() {}
};

/**
 * Serves a Live Link server on the specified port that the Live Link Face app
 * can connect to.
 */
class LiveLinkServer : public godot::RefCounted {
  GDCLASS(LiveLinkServer, godot::RefCounted)
public:
  bool _disable_polling = false;

  LiveLinkServer()      = default;
  ~LiveLinkServer() override;

  auto listen() -> godot::Error;
  auto stop() -> godot::Error;

  auto poll() -> godot::Error;
  auto fetch_clients() -> godot::Error;
  auto poll_packages() -> godot::Error;

  [[nodiscard]] uint16_t get_port() const noexcept { return _port; }

  void set_port(uint16_t port);

  void _thread_poll();

protected:
  static void _bind_methods();

private:
  uint16_t _port{ 11111 };

  godot::Ref<godot::Thread>    _thread{ memnew(godot::Thread) };
  godot::Ref<godot::UDPServer> _server{ memnew(godot::UDPServer) };
  godot::Ref<godot::Mutex>     _server_mutex{ memnew(godot::Mutex) };
  std::atomic<bool>            _running{ true };

  godot::List<godot::Ref<godot::PacketPeerUDP>> _unidentified_clients{};
  godot::HashMap<godot::String, godot::Ref<LiveLinkClient>> _clients{};
};

#endif // VDOT_LIVE_LINK_SERVER_H
