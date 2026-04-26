#ifndef VDOT_MODEL_LOADER_H
#define VDOT_MODEL_LOADER_H

#include <godot_cpp/classes/object.hpp>

#include "models/model_format.hpp"

/**
 * Singleton for loading models.
 */
class ModelLoader : public godot::Object {
  GDCLASS(ModelLoader, godot::Object)
protected:
  static constexpr char const
    *BUNDLE_EXTRACT_BASE_PATH = "user://temp/bundles/models";

  static ModelLoader *singleton;

  godot::TypedArray<ModelFormat> formats;

  static void _bind_methods();

public:
  static ModelLoader *get_singleton() { return singleton; }

  ModelLoader();

  godot::TypedArray<ModelFormat> get_model_formats() const;
  void add_model_format(godot::Ref<ModelFormat> const &p_format);

  [[nodiscard]] godot::Ref<ModelFormat> find_format_by_path(
    godot::String const &p_path
  ) const;
  [[nodiscard]] Model *load_from_path(godot::String const &p_path) const;

  [[nodiscard]] godot::Ref<ModelFormat> find_format(
    godot::String const &p_format
  ) const;
  [[nodiscard]] godot::Ref<ModelBundle> load_bundle(
    godot::String const &p_path
  ) const;
  [[nodiscard]] Model *create_from_bundle(
    godot::Ref<ModelBundle> const &p_bundle
  ) const;
};

#endif // VDOT_MODEL_LOADER_H
