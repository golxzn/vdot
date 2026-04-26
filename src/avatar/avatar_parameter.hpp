#ifndef VDOT_AVATAR_PARAMETER_H
#define VDOT_AVATAR_PARAMETER_H

#include <godot_cpp/classes/resource.hpp>

#include "parameters/parameter_input.hpp"
#include "parameters/parameter_output.hpp"

/**
 * Defines a n input->output parameter mapping associated with the avatar.
 */
class AvatarParameter : public godot::Resource {
  GDCLASS(AvatarParameter, godot::Resource)
protected:
  godot::StringName parameter_id; // unique for the model

  godot::String parameter_name;        // user-friendly name
  godot::String parameter_description; // user-friendly description

  // NOTE: use 0-1 range by default as that's fairly standard.
  godot::Vector2 input_range  = godot::Vector2(0.0f, 1.0f);
  godot::Vector2 output_range = godot::Vector2(0.0f, 1.0f);

  // references to the input parameter to pull values from,
  // and the output parameter to push the derived values to.
  //
  // NOTE: We don't store a direct reference to the input parameter, as the
  // parameter
  //  source may be changed or invalidated at any point (i.e., face tracker
  //  disconnecting).
  godot::StringName           input_parameter;
  godot::Ref<OutputParameter> output_parameter;

  static void _bind_methods();

public:
  [[nodiscard]] godot::StringName get_parameter_id() const;
  void set_parameter_id(godot::StringName const &p_id);

  [[nodiscard]] godot::String get_parameter_name() const;
  void                        set_parameter_name(godot::String const &p_name);

  [[nodiscard]] godot::String get_parameter_description() const;
  void set_parameter_description(godot::String const &p_description);

  godot::Vector2 get_input_range() const;
  void           set_input_range(godot::Vector2 const &p_range);

  godot::Vector2 get_output_range() const;
  void           set_output_range(godot::Vector2 const &p_range);

  godot::StringName get_input_parameter() const;
  void set_input_parameter(godot::StringName const &p_input_parameter);

  godot::Ref<OutputParameter> get_output_parameter() const;
  void                        set_output_parameter(
    godot::Ref<OutputParameter> const &p_output_parameter
  );

  float calculate_value(float p_input, double delta) const;

  void _on_input_changed(float p_input_value);
};

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

MAKE_TYPED_ARRAY(AvatarParameter, Variant::OBJECT)
MAKE_TYPED_ARRAY_INFO(AvatarParameter, Variant::OBJECT)

#undef MAKE_TYPED_ARRAY_INFO
#undef MAKE_TYPED_ARRAY

}; // namespace godot

#endif // VDOT_AVATAR_PARAMETER_H
