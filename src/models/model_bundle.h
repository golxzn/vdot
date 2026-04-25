
#ifndef VDOT_MODEL_BUNDLE_H
#define VDOT_MODEL_BUNDLE_H

#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource_format_saver.hpp>

#include <godot_cpp/templates/hash_map.hpp>

/**
 * A data transport resource that encapsulates the required
 *  files and/or resources for rendering a model.
 */
class ModelBundle : public godot::Resource {
    GDCLASS( ModelBundle, godot::Resource )
  public:
    struct BundleFile {
        godot::PackedByteArray data;
    };

  protected:
    // should be set in the constructor by implementations.
    // used to determine the model format to load with.
    godot::StringName format_type = "unknown";

    godot::HashMap<godot::String, BundleFile> files;

    static void _bind_methods();

    void set_format_type( const godot::StringName &p_format );

  public:
    [[nodiscard]] godot::StringName get_format_type() const;

    godot::Error add_directory( const godot::String &p_path, const godot::String &p_basepath,
                                bool p_recursive = true );
    godot::Error add_file( const godot::String &p_path, const godot::String &p_basepath );

    void create_file( const godot::String &p_path );
    void set_file_data( const godot::String &p_path, const godot::PackedByteArray &p_data );
    godot::PackedByteArray get_file_data( const godot::String &p_path ) const;

    godot::Vector<godot::String> get_file_list() const;

    bool _set( const godot::StringName &p_name, const godot::Variant &p_property );
    bool _get( const godot::StringName &p_name, godot::Variant &r_property ) const;
    void _get_property_list( godot::List<godot::PropertyInfo> *p_list ) const;

    godot::String compute_hash() const;

    godot::Error unpack_bundle( const godot::String &p_path );
    godot::Error pack_bundle( const godot::String &p_path );
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

    MAKE_TYPED_ARRAY( Ref<ModelBundle>, Variant::OBJECT )
    MAKE_TYPED_ARRAY_INFO( Ref<ModelBundle>, Variant::OBJECT )

#undef MAKE_TYPED_ARRAY_INFO
#undef MAKE_TYPED_ARRAY

};

#endif // VDOT_MODEL_BUNDLE_H
