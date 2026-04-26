#ifndef VDOT_AVATAR_BUNDLE_H
#define VDOT_AVATAR_BUNDLE_H

#include <godot_cpp/classes/resource.hpp>

#include "models/model_bundle.hpp"

/**
 * An avatar bundle encapsulates an avatar's config,
 *  model(s), etc. for storage or network transmission.
 */
class AvatarBundle : public godot::Resource {
  GDCLASS(AvatarBundle, godot::Resource)
protected:
  godot::String name = "unnamed";

  godot::TypedArray<godot::Ref<ModelBundle>> models;

  static void _bind_methods();

public:
  AvatarBundle();
  ~AvatarBundle() override;

  void                        set_avatar_name(godot::String const &p_name);
  [[nodiscard]] godot::String get_avatar_name() const;

  void set_models(godot::TypedArray<ModelBundle> const &p_models);
  [[nodiscard]] godot::TypedArray<ModelBundle> get_models() const;
  void add_model(godot::Ref<ModelBundle> const &p_model);
};

#endif // VDOT_AVATAR_BUNDLE_H
