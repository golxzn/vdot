#ifndef VDOT_COLLABORATION_SESSION_H
#define VDOT_COLLABORATION_SESSION_H

#include <godot_cpp/classes/ref_counted.hpp>

#include "avatar/avatar.hpp"
#include "collaboration_peer.hpp"

namespace godot {

#define MAKE_TYPED_ARRAY(m_type, m_variant_type)                         \
  template<>                                                             \
  class TypedArray<m_type> : public Array {                              \
  public:                                                                \
    _FORCE_INLINE_ void operator=(const Array &p_array) {                \
      ERR_FAIL_COND_MSG(                                                 \
        !is_same_typed(p_array),                                         \
        "Cannot assign an array with a different element type."          \
      );                                                                 \
      Array::operator=(p_array);                                         \
    }                                                                    \
    _FORCE_INLINE_ TypedArray(std::initializer_list<Variant> p_init)     \
      : Array(Array(p_init), m_variant_type, StringName(), Variant()) {} \
    _FORCE_INLINE_ TypedArray(const Variant &p_variant)                  \
      : TypedArray(Array(p_variant)) {}                                  \
    _FORCE_INLINE_ TypedArray(const Array &p_array) {                    \
      set_typed(m_variant_type, StringName(), Variant());                \
      if (is_same_typed(p_array)) {                                      \
        Array::operator=(p_array);                                       \
      } else {                                                           \
        assign(p_array);                                                 \
      }                                                                  \
    }                                                                    \
    _FORCE_INLINE_ TypedArray() {                                        \
      set_typed(m_variant_type, StringName(), Variant());                \
    }                                                                    \
  };

#define MAKE_TYPED_ARRAY_INFO(m_type, m_variant_type)            \
  template<>                                                     \
  struct GetTypeInfo<TypedArray<m_type>> {                       \
    static constexpr GDExtensionVariantType                      \
      VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;             \
    static constexpr GDExtensionClassMethodArgumentMetadata      \
      METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;      \
    static inline PropertyInfo get_class_info() {                \
      return make_property_info(                                 \
        Variant::Type::ARRAY,                                    \
        "",                                                      \
        PROPERTY_HINT_ARRAY_TYPE,                                \
        Variant::get_type_name(m_variant_type).utf8().get_data() \
      );                                                         \
    }                                                            \
  };                                                             \
  template<>                                                     \
  struct GetTypeInfo<const TypedArray<m_type> &> {               \
    static constexpr GDExtensionVariantType                      \
      VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;             \
    static constexpr GDExtensionClassMethodArgumentMetadata      \
      METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;      \
    static inline PropertyInfo get_class_info() {                \
      return make_property_info(                                 \
        Variant::Type::ARRAY,                                    \
        "",                                                      \
        PROPERTY_HINT_ARRAY_TYPE,                                \
        Variant::get_type_name(m_variant_type).utf8().get_data() \
      );                                                         \
    }                                                            \
  };

MAKE_TYPED_ARRAY(Ref<CollaborationPeer>, Variant::OBJECT)
MAKE_TYPED_ARRAY_INFO(Ref<CollaborationPeer>, Variant::OBJECT)

MAKE_TYPED_ARRAY(Avatar, Variant::OBJECT)
MAKE_TYPED_ARRAY_INFO(Avatar, Variant::OBJECT)

#undef MAKE_TYPED_ARRAY_INFO
#undef MAKE_TYPED_ARRAY

} // namespace godot

class CollaborationSession : public godot::Node {
  GDCLASS(CollaborationSession, godot::Node)
protected:
  godot::Ref<CollaborationPeer>                      local_peer;
  godot::HashMap<int, godot::Ref<CollaborationPeer>> remote_peers;

  // Avatars local to this device that are being shared to the session.
  godot::TypedArray<Avatar> local_avatars;

  static void _bind_methods();

public:
  CollaborationSession();

  void _ready() override;

  godot::Error start_server(); // start a session server
  godot::Error connect_to_server(godot::String const &p_address, int p_port);

  void add_local_avatar(Avatar *p_avatar);
  void remove_local_avatar(Avatar *p_avatar);

  void _client_connected_to_server();
  void _client_connection_failed();
  void _client_server_disconnected();

  void _peer_connected(int id);
  void _peer_disconnected(int id);
};

#endif // VDOT_COLLABORATION_SESSION_H
