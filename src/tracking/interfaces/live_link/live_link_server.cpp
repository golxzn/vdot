#include "tracking/interfaces/live_link/live_link_server.hpp"

#include <ranges>
#include <string>

#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/classes/stream_peer_buffer.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "utility/bytes-advance.hpp"

/*
METAHUMAN:
Bytes received: 584
data:
0100240031334146323233332D463035302D344530342D383937442D413041354136324637353642F87C3F00A533E43E3C00000001000000FB00A013401440165017704DC04BD00B200E000000000000000000000000981DB8090001000040011001D000D0000000000000000000000000000000000000000000005BD05B000000000016141C0000000000000000000000000000000000000000000000000000D0130000FFFFFFFF00005C0B64095A0C00000000B0005001000000007408900B000000000E0E00000000000090144004A0001001A00CA000C003E00300000000B00610028001A0020000000000000000502B4038E029A0249011E010F0059007801300141001E0000C0C0A0A100F0C0CF0024002501340123008A0086008D007C007A00850004000C000A000300060016000C004B00420024000A000000000000000000000000000E60339040000620000000000E41D0000A008880AD00443040000000000000000000000000000000000000000000000000000000000000000C209B80A4C03AA020000000000000000C0090000500000007C09D80A00000000800D3A0A000000004A07FC01000000000000000000000000F00FCA0700000000A8060000000060370035000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000821F8D40A1766F4005F801C1885121C1706D20C2197E4B40
*/

void LiveLinkARKitPacket::dump() const {
  std::array constexpr static symbols{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };

  std::array<char, sizeof(ARKitBlendShapes) * 2 + 1> str_buf;
  str_buf.back() = '\0';

  auto constexpr static copy_string{
    [](auto &buf, auto const &array, size_t const len) {
      size_t const length{ std::min(std::size(buf) - 1, len) };
      std::copy_n(std::begin(array), length, std::begin(buf));
      buf[length] = '\0';
      return std::data(buf);
    }
  };


  using utl = godot::UtilityFunctions;

  utl::print(
    "============================== ARKitPacket =============================="
  );

  utl::print("version          : ", version);
  utl::print("uuid_length      : ", uuid_length);
  utl::print("uuid             : ", copy_string(str_buf, uuid, uuid_length));
  utl::print("name_length      : ", name_length);
  utl::print("name             : ", copy_string(str_buf, uuid, name_length));
  utl::print("timestamp        : ", timestamp);
  utl::print("magic_2          : ", magic_2);
  utl::print("blend_shape_count: ", blend_shape_count);

  auto bytes{ blend_shapes.binary() };
  for (size_t i{}; i < std::size(bytes); ++i) {
    uint8_t const byte{ bytes[i] };
    str_buf[i * 2 + 0] = symbols[(byte >> 4)];
    str_buf[i * 2 + 1] = symbols[byte & 0xF];
  }
  utl::print("blend_shapes     : ", std::data(str_buf));
}

auto LiveLinkARKitPacket::from_bytes(godot::PackedByteArray const &bytes)
  -> LiveLinkARKitPacket {
  // {
  //   std::array constexpr static symbols{ '0', '1', '2', '3', '4', '5',
  //                                        '6', '7', '8', '9', 'A', 'B',
  //                                        'C', 'D', 'E', 'F' };
  //   int64_t const bytes_count{ std::size(bytes) };
  //   godot::UtilityFunctions::print(
  //     "=============================== DUMP ==============================="
  //   );
  //   godot::UtilityFunctions::print("Bytes received: ", bytes_count);
  //   std::string dump_str;
  //   dump_str.resize(bytes_count * 2 + 1);
  //   dump_str.back() = '\0';
  //   for (int64_t i{}; i < bytes_count; ++i) {
  //     uint8_t const byte{ bytes[i] };
  //     dump_str[i * 2 + 0] = symbols[(byte >> 4)];
  //     dump_str[i * 2 + 1] = symbols[byte & 0xF];
  //   }
  //   godot::UtilityFunctions::print("data: ", std::data(dump_str));
  // }

  size_t const bytes_count{ static_cast<size_t>(std::size(bytes)) };
  if (bytes_count < MAX_NAME_LENGTH * sizeof(float)) {
    godot::UtilityFunctions::print(
      "BYTES COUNT LESS THAN ", MAX_NAME_LENGTH * sizeof(float)
    );
    return {};
  }

  uint8_t const *cur{ bytes.ptr() };
  uint8_t const *end{ std::next(bytes.ptr(), std::size(bytes)) };

  LiveLinkARKitPacket packet;

  using namespace vdot;

  packet.version     = util::advance<uint8_t>(cur);
  packet.uuid_length = util::advance<uint32_t>(cur);
  if (cur + packet.uuid_length > end) {
    godot::UtilityFunctions::print("INVALID UUID LENGTH ", packet.uuid_length);
    packet.dump();
    return {};
  }
  util::advance_to(packet.uuid, cur, packet.uuid_length);

  packet.name_length = util::advance<uint32_t>(cur);
  if (cur + packet.name_length > end) {
    godot::UtilityFunctions::print("INVALID NAME LENGTH ", packet.name_length);
    godot::UtilityFunctions::print(
      "cur offset: ", std::distance(bytes.ptr(), cur)
    );
    packet.dump();
    return {};
  }
  util::advance_to(packet.name, cur, packet.name_length);

  packet.timestamp         = util::advance<uint64_t>(cur);
  packet.magic_2           = util::advance<uint64_t, std::endian::native>(cur);
  packet.blend_shape_count = util::advance<uint8_t>(cur);

  size_t const blend_shapes_count{
    static_cast<size_t>(packet.blend_shape_count)
  };
  if (blend_shapes_count != BLEND_SHAPE_COUNT) {
    godot::UtilityFunctions::print(
      "INVALID BLEND_SHAPE_COUNT ", blend_shapes_count
    );
    packet.dump();
    return packet;
  }

  util::advance_to(packet.blend_shapes.array(), cur);

  return packet;
}

float LiveLinkClientData::get_blend_shape(ARKit::BlendShape blendShape) {
  // +1 because the LiveLink data uses 0 as a blend shape count index.
  // TODO: double check the +1 reasoning
  return _data.array()[blendShape];
}

void LiveLinkClientData::_bind_methods() {
  using namespace godot;

  static constexpr auto make_property_info{ [](auto const name) {
    return PropertyInfo{
      Variant::FLOAT, name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY,
    };
  } };

  using self        = LiveLinkClientData;
  using method_type = decltype(&LiveLinkClientData::_get_eye_blink_left);
  auto constexpr mt{
    std::make_tuple<char const *, char const *, method_type>
  };

  // clang-format off
  std::array static const methods{
    mt("eye_blink_left",         "_get_eye_blink_left",         &self::_get_eye_blink_left        ),
    mt("eye_look_down_left",     "_get_eye_look_down_left",     &self::_get_eye_look_down_left    ),
    mt("eye_look_in_left",       "_get_eye_look_in_left",       &self::_get_eye_look_in_left      ),
    mt("eye_look_out_left",      "_get_eye_look_out_left",      &self::_get_eye_look_out_left     ),
    mt("eye_look_up_left",       "_get_eye_look_up_left",       &self::_get_eye_look_up_left      ),
    mt("eye_squint_left",        "_get_eye_squint_left",        &self::_get_eye_squint_left       ),
    mt("eye_wide_left",          "_get_eye_wide_left",          &self::_get_eye_wide_left         ),
    mt("eye_blink_right",        "_get_eye_blink_right",        &self::_get_eye_blink_right       ),
    mt("eye_look_down_right",    "_get_eye_look_down_right",    &self::_get_eye_look_down_right   ),
    mt("eye_look_in_right",      "_get_eye_look_in_right",      &self::_get_eye_look_in_right     ),
    mt("eye_look_out_right",     "_get_eye_look_out_right",     &self::_get_eye_look_out_right    ),
    mt("eye_look_up_right",      "_get_eye_look_up_right",      &self::_get_eye_look_up_right     ),
    mt("eye_squint_right",       "_get_eye_squint_right",       &self::_get_eye_squint_right      ),
    mt("eye_wide_right",         "_get_eye_wide_right",         &self::_get_eye_wide_right        ),
    mt("jaw_forward",            "_get_jaw_forward",            &self::_get_jaw_forward           ),
    mt("jaw_right",              "_get_jaw_right",              &self::_get_jaw_right             ),
    mt("jaw_left",               "_get_jaw_left",               &self::_get_jaw_left              ),
    mt("jaw_open",               "_get_jaw_open",               &self::_get_jaw_open              ),
    mt("mouth_close",            "_get_mouth_close",            &self::_get_mouth_close           ),
    mt("mouth_funnel",           "_get_mouth_funnel",           &self::_get_mouth_funnel          ),
    mt("mouth_pucker",           "_get_mouth_pucker",           &self::_get_mouth_pucker          ),
    mt("mouth_right",            "_get_mouth_right",            &self::_get_mouth_right           ),
    mt("mouth_left",             "_get_mouth_left",             &self::_get_mouth_left            ),
    mt("mouth_smile_left",       "_get_mouth_smile_left",       &self::_get_mouth_smile_left      ),
    mt("mouth_smile_right",      "_get_mouth_smile_right",      &self::_get_mouth_smile_right     ),
    mt("mouth_frown_left",       "_get_mouth_frown_left",       &self::_get_mouth_frown_left      ),
    mt("mouth_frown_right",      "_get_mouth_frown_right",      &self::_get_mouth_frown_right     ),
    mt("mouth_dimple_left",      "_get_mouth_dimple_left",      &self::_get_mouth_dimple_left     ),
    mt("mouth_dimple_right",     "_get_mouth_dimple_right",     &self::_get_mouth_dimple_right    ),
    mt("mouth_stretch_left",     "_get_mouth_stretch_left",     &self::_get_mouth_stretch_left    ),
    mt("mouth_stretch_right",    "_get_mouth_stretch_right",    &self::_get_mouth_stretch_right   ),
    mt("mouth_roll_lower",       "_get_mouth_roll_lower",       &self::_get_mouth_roll_lower      ),
    mt("mouth_roll_upper",       "_get_mouth_roll_upper",       &self::_get_mouth_roll_upper      ),
    mt("mouth_shrug_lower",      "_get_mouth_shrug_lower",      &self::_get_mouth_shrug_lower     ),
    mt("mouth_shrug_upper",      "_get_mouth_shrug_upper",      &self::_get_mouth_shrug_upper     ),
    mt("mouth_press_left",       "_get_mouth_press_left",       &self::_get_mouth_press_left      ),
    mt("mouth_press_right",      "_get_mouth_press_right",      &self::_get_mouth_press_right     ),
    mt("mouth_lower_down_left",  "_get_mouth_lower_down_left",  &self::_get_mouth_lower_down_left ),
    mt("mouth_lower_down_right", "_get_mouth_lower_down_right", &self::_get_mouth_lower_down_right),
    mt("mouth_upper_up_left",    "_get_mouth_upper_up_left",    &self::_get_mouth_upper_up_left   ),
    mt("mouth_upper_up_right",   "_get_mouth_upper_up_right",   &self::_get_mouth_upper_up_right  ),
    mt("brow_down_left",         "_get_brow_down_left",         &self::_get_brow_down_left        ),
    mt("brow_down_right",        "_get_brow_down_right",        &self::_get_brow_down_right       ),
    mt("brow_inner_up",          "_get_brow_inner_up",          &self::_get_brow_inner_up         ),
    mt("brow_outer_up_left",     "_get_brow_outer_up_left",     &self::_get_brow_outer_up_left    ),
    mt("brow_outer_up_right",    "_get_brow_outer_up_right",    &self::_get_brow_outer_up_right   ),
    mt("cheek_puff",             "_get_cheek_puff",             &self::_get_cheek_puff            ),
    mt("cheek_squint_left",      "_get_cheek_squint_left",      &self::_get_cheek_squint_left     ),
    mt("cheek_squint_right",     "_get_cheek_squint_right",     &self::_get_cheek_squint_right    ),
    mt("nose_sneer_left",        "_get_nose_sneer_left",        &self::_get_nose_sneer_left       ),
    mt("nose_sneer_right",       "_get_nose_sneer_right",       &self::_get_nose_sneer_right      ),
    mt("tongue_out",             "_get_tongue_out",             &self::_get_tongue_out            ),
    mt("head_yaw",               "_get_head_yaw",               &self::_get_head_yaw              ),
    mt("head_pitch",             "_get_head_pitch",             &self::_get_head_pitch            ),
    mt("head_roll",              "_get_head_roll",              &self::_get_head_roll             ),
    mt("left_eye_yaw",           "_get_left_eye_yaw",           &self::_get_left_eye_yaw          ),
    mt("left_eye_pitch",         "_get_left_eye_pitch",         &self::_get_left_eye_pitch        ),
    mt("left_eye_roll",          "_get_left_eye_roll",          &self::_get_left_eye_roll         ),
    mt("right_eye_yaw",          "_get_right_eye_yaw",          &self::_get_right_eye_yaw         ),
    mt("right_eye_pitch",        "_get_right_eye_pitch",        &self::_get_right_eye_pitch       ),
    mt("right_eye_roll",         "_get_right_eye_roll",         &self::_get_right_eye_roll        ),
  };
  // clang-format on

  for (auto const &[name, id, ptr] : methods) {
    ClassDB::bind_method(D_METHOD(id), ptr);
    ADD_PROPERTY(make_property_info(name), "", id);
  }

  BIND_ENUM_CONSTANT(ARKit::EyeBlinkLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeLookDownLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeLookInLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeLookOutLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeLookUpLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeSquintLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeWideLeft)
  BIND_ENUM_CONSTANT(ARKit::EyeBlinkRight)
  BIND_ENUM_CONSTANT(ARKit::EyeLookDownRight)
  BIND_ENUM_CONSTANT(ARKit::EyeLookInRight)
  BIND_ENUM_CONSTANT(ARKit::EyeLookOutRight)
  BIND_ENUM_CONSTANT(ARKit::EyeLookUpRight)
  BIND_ENUM_CONSTANT(ARKit::EyeSquintRight)
  BIND_ENUM_CONSTANT(ARKit::EyeWideRight)
  BIND_ENUM_CONSTANT(ARKit::JawForward)
  BIND_ENUM_CONSTANT(ARKit::JawRight)
  BIND_ENUM_CONSTANT(ARKit::JawLeft)
  BIND_ENUM_CONSTANT(ARKit::JawOpen)
  BIND_ENUM_CONSTANT(ARKit::MouthClose)
  BIND_ENUM_CONSTANT(ARKit::MouthFunnel)
  BIND_ENUM_CONSTANT(ARKit::MouthPucker)
  BIND_ENUM_CONSTANT(ARKit::MouthRight)
  BIND_ENUM_CONSTANT(ARKit::MouthLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthSmileLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthSmileRight)
  BIND_ENUM_CONSTANT(ARKit::MouthFrownLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthFrownRight)
  BIND_ENUM_CONSTANT(ARKit::MouthDimpleLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthDimpleRight)
  BIND_ENUM_CONSTANT(ARKit::MouthStretchLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthStretchRight)
  BIND_ENUM_CONSTANT(ARKit::MouthRollLower)
  BIND_ENUM_CONSTANT(ARKit::MouthRollUpper)
  BIND_ENUM_CONSTANT(ARKit::MouthShrugLower)
  BIND_ENUM_CONSTANT(ARKit::MouthShrugUpper)
  BIND_ENUM_CONSTANT(ARKit::MouthPressLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthPressRight)
  BIND_ENUM_CONSTANT(ARKit::MouthLowerDownLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthLowerDownRight)
  BIND_ENUM_CONSTANT(ARKit::MouthUpperUpLeft)
  BIND_ENUM_CONSTANT(ARKit::MouthUpperUpRight)
  BIND_ENUM_CONSTANT(ARKit::BrowDownLeft)
  BIND_ENUM_CONSTANT(ARKit::BrowDownRight)
  BIND_ENUM_CONSTANT(ARKit::BrowInnerUp)
  BIND_ENUM_CONSTANT(ARKit::BrowOuterUpLeft)
  BIND_ENUM_CONSTANT(ARKit::BrowOuterUpRight)
  BIND_ENUM_CONSTANT(ARKit::CheekPuff)
  BIND_ENUM_CONSTANT(ARKit::CheekSquintLeft)
  BIND_ENUM_CONSTANT(ARKit::CheekSquintRight)
  BIND_ENUM_CONSTANT(ARKit::NoseSneerLeft)
  BIND_ENUM_CONSTANT(ARKit::NoseSneerRight)
  BIND_ENUM_CONSTANT(ARKit::TongueOut)
}

LiveLinkServer::~LiveLinkServer() {
  if (!_disable_polling && _thread->is_started()) {
    // Thread must be disposed (or "joined"), for portability.
    _thread->wait_to_finish();
  }
}

auto LiveLinkServer::listen() -> godot::Error {
  if (auto const err{ _server->listen(_port) }; err != godot::OK) {
    return err;
  }

  _running = true;
  if (!_disable_polling) {
    _thread->start(godot::Callable(this, "_thread_poll"));
  }

  godot::UtilityFunctions::print("Started LiveLink server on: ", _port);

  return godot::OK;
}

auto LiveLinkServer::stop() -> godot::Error {
  _server_mutex->lock();

  _running = false;
  _server->stop();

  _server_mutex->unlock();

  for (auto const &[_, client] : _clients) {
    godot::UtilityFunctions::print(
      "Existing LiveLinkFace peer disconnected for shutdown: %s (%s)",
      client->_name,
      client->_id
    );
    emit_signal("client_disconnected", client);
  }

  _clients.clear();
  _unidentified_clients.clear();


  godot::UtilityFunctions::print("Stopped LiveLink server on: ", _port);

  return godot::OK;
}

auto LiveLinkServer::poll() -> godot::Error {
  if (auto const err{ fetch_clients() }; err != godot::OK) { return err; }
  return poll_packages();
}

auto LiveLinkServer::fetch_clients() -> godot::Error {
  if (auto const err{ _server->poll() }; err != godot::OK) { return err; }

  if (_server->is_connection_available()) {
    if (auto peer{ _server->take_connection() }; peer.is_valid()) {
      _unidentified_clients.push_back(peer);
    }
  }

  std::vector<int> resolved_peers{};
  resolved_peers.reserve(std::size(_unidentified_clients));
  for (int peer_idx{}; peer_idx < std::size(_unidentified_clients);
       ++peer_idx) {
    auto peer{ _unidentified_clients.get(peer_idx) };

    auto const packet_count{ peer->get_available_packet_count() };
    if (packet_count <= 0) { break; }

    // Pop all but the latest packet.
    for (int i = 0; i < packet_count - 1; ++i) {
      std::ignore = peer->get_packet();
    }

    auto const packet{ LiveLinkARKitPacket::from_bytes(peer->get_packet()) };

    godot::String device_id{ std::data(packet.uuid) };
    resolved_peers.push_back(peer_idx);

    if (_clients.has(device_id)) {
      godot::Ref<LiveLinkClient> &client{ _clients.get(device_id) };

      // If there is already a connection registered for this device,
      // remove the connection and replace it with the new one.
      client->_connection->close();
      client->_connection = peer;

      continue;
    }

    godot::Ref<LiveLinkClient> client{ memnew(LiveLinkClient()) };
    client->_id         = std::move(device_id);
    client->_name       = std::data(packet.name);
    client->_last_seen  = packet.timestamp;
    client->_connection = peer;

    if (packet.blend_shape_count == BLEND_SHAPE_COUNT) {
      client->_values->_timecode = packet.timestamp;
      client->_values->_data     = packet.blend_shapes;
    }

    _clients[client->_id] = client;

    godot::UtilityFunctions::print(
      "New LiveLinkFace peer connected: %s (%s)", client->_name, client->_id
    );

    emit_signal("client_connected", client);
  }

  // Clear unidentified peers that are now known.
  if (!_unidentified_clients.is_empty()) {
    // for (auto const idx : resolved_peers | std::views::reverse) {
    for (auto const idx : std::views::reverse(resolved_peers)) {
      _unidentified_clients.erase(_unidentified_clients.get(idx));
    }
  }

  return godot::OK;
}

auto LiveLinkServer::poll_packages() -> godot::Error {
  // Now poll for packets from clients with already established connections
  for (auto const &[_, client] : _clients) {
    auto const packets_available{
      client->_connection->get_available_packet_count()
    };

    if (packets_available == 0) { continue; }

    // Discard all but the most recent packet
    for (int i = 0; i < packets_available - 1; i++) {
      client->_connection->get_packet(); // discard
    }

    auto const packet{
      LiveLinkARKitPacket::from_bytes(client->_connection->get_packet())
    };

    client->_last_seen = packet.timestamp;
    if (packet.blend_shape_count == BLEND_SHAPE_COUNT) {
      // TODO: should we be using recounted and creating a new instance
      // instead?
      client->_values->_timecode = packet.timestamp;
      client->_values->_data     = packet.blend_shapes;

      emit_signal("client_updated", client);
    }
  }

  return godot::OK;
}

void LiveLinkServer::set_port(uint16_t port) {
  _port = port;

  stop();
  listen();
}

void LiveLinkServer::_thread_poll() {
  while (_running.load(std::memory_order_relaxed)) {
    if (_server_mutex->try_lock()) {
      poll();
      _server_mutex->unlock();
    }
  }
}

void LiveLinkServer::_bind_methods() {
  using namespace godot;

  ADD_SIGNAL(MethodInfo(
    "client_connected",
    PropertyInfo(
      Variant::OBJECT,
      "client",
      PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
      LiveLinkClient::get_class_static()
    )
  ));
  ADD_SIGNAL(MethodInfo(
    "client_disconnected",
    PropertyInfo(
      Variant::OBJECT,
      "client",
      PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
      LiveLinkClient::get_class_static()
    )
  ));
  ADD_SIGNAL(MethodInfo(
    "client_updated",
    PropertyInfo(
      Variant::OBJECT,
      "client",
      PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
      LiveLinkClient::get_class_static()
    )
  ));

  ClassDB::bind_method(D_METHOD("get_port"), &LiveLinkServer::get_port);
  ClassDB::bind_method(
    D_METHOD("set_port", "port"), &LiveLinkServer::set_port
  );
  ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");

  ClassDB::bind_method(D_METHOD("listen"), &LiveLinkServer::listen);
  ClassDB::bind_method(D_METHOD("stop"), &LiveLinkServer::stop);
  ClassDB::bind_method(D_METHOD("poll"), &LiveLinkServer::poll);
  ClassDB::bind_method(
    D_METHOD("fetch_clients"), &LiveLinkServer::fetch_clients
  );
  ClassDB::bind_method(
    D_METHOD("poll_packages"), &LiveLinkServer::poll_packages
  );

  ClassDB::bind_method(
    D_METHOD("_thread_poll"), &LiveLinkServer::_thread_poll
  );
}
