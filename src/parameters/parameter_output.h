

#ifndef VDOT_PARAMETER_OUTPUT_H
#define VDOT_PARAMETER_OUTPUT_H

#include <godot_cpp/classes/resource.hpp>

#include "parameter.h"

class OutputParameter : public Parameter {
    GDCLASS( OutputParameter, Parameter )
  protected:
    static void _bind_methods();

  public:
    static constexpr const char *SIGNAL_VALUE_CHANGED = "value_changed";

    float parameter_value = 0.0f;

    [[nodiscard]] float get_parameter_value() const;
    void set_parameter_value( float value );
};

namespace godot {

#define MAKE_TYPED_ARRAY( m_type, m_variant_type )                                                 \
    template <> class TypedArray<m_type> : public Array {                                          \
      public:                                                                                      \
        _FORCE_INLINE_ void operator=( const Array &p_array ) {                                    \
            ERR_FAIL_COND_MSG( !is_same_typed( p_array ),                                          \
                               "Cannot assign an array with a different element type." );          \
            Array::operator=( p_array );                                                           \
        }                                                                                          \
        _FORCE_INLINE_ TypedArray( std::initializer_list<Variant> p_init ) :                       \
            Array( Array( p_init ), m_variant_type, StringName(), Variant() ) {                    \
        }                                                                                          \
        _FORCE_INLINE_ TypedArray( const Variant &p_variant ) : TypedArray( Array( p_variant ) ) { \
        }                                                                                          \
        _FORCE_INLINE_ TypedArray( const Array &p_array ) {                                        \
            set_typed( m_variant_type, StringName(), Variant() );                                  \
            if ( is_same_typed( p_array ) ) {                                                      \
                Array::operator=( p_array );                                                       \
            } else {                                                                               \
                assign( p_array );                                                                 \
            }                                                                                      \
        }                                                                                          \
        _FORCE_INLINE_ TypedArray() {                                                              \
            set_typed( m_variant_type, StringName(), Variant() );                                  \
        }                                                                                          \
    };

#define MAKE_TYPED_ARRAY_INFO( m_type, m_variant_type )                                            \
    template <> struct GetTypeInfo<TypedArray<m_type>> {                                           \
        static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;     \
        static constexpr GDExtensionClassMethodArgumentMetadata METADATA =                         \
            GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;                                             \
        static inline PropertyInfo get_class_info() {                                              \
            return make_property_info(                                                             \
                Variant::Type::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE,                                \
                Variant::get_type_name( m_variant_type ).utf8().get_data() );                      \
        }                                                                                          \
    };                                                                                             \
    template <> struct GetTypeInfo<const TypedArray<m_type> &> {                                   \
        static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;     \
        static constexpr GDExtensionClassMethodArgumentMetadata METADATA =                         \
            GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;                                             \
        static inline PropertyInfo get_class_info() {                                              \
            return make_property_info(                                                             \
                Variant::Type::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE,                                \
                Variant::get_type_name( m_variant_type ).utf8().get_data() );                      \
        }                                                                                          \
    };

    MAKE_TYPED_ARRAY( OutputParameter, Variant::OBJECT )
    MAKE_TYPED_ARRAY_INFO( OutputParameter, Variant::OBJECT )

#undef MAKE_TYPED_ARRAY_INFO
#undef MAKE_TYPED_ARRAY

}

#endif // VDOT_PARAMETER_OUTPUT_H
