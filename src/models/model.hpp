#ifndef VDOT_MODEL_H
#define VDOT_MODEL_H

#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>

#include "model_bundle.hpp"
#include "model_parameter.hpp"

/**
 * Model provides the base class for VTuber avatar/model.
 *
 * Models are rendered in viewports to make various means of projecting
 *  and rendering them in different contexts them easier.
 */
class Model : public godot::SubViewport {
  GDCLASS(Model, godot::SubViewport)

  void _on_property_updated(godot::StringName const &p_name, float p_value);

protected:
  godot::TypedArray<ModelParameter> parameters;

  static void _bind_methods();

  void _add_model_parameter(godot::Ref<ModelParameter> const &p_parameter);

public:
  Model() = default;

  [[nodiscard]] godot::TypedArray<ModelParameter> get_model_parameters() const;

  bool _set(godot::StringName const &p_name, godot::Variant const &p_value);
  bool _get(godot::StringName const &p_name, godot::Variant &r_ret) const;
  bool _property_can_revert(godot::StringName const &p_name) const;
  bool _property_get_revert(
    godot::StringName const &p_name,
    godot::Variant          &r_property
  ) const;
  void _get_property_list(godot::List<godot::PropertyInfo> *p_list);

  /**
   * Returns a model bundle for the currently loaded model assets.
   *
   * Primarily intended for use with networked collaboration sessions.
   *
   * @return the packed model bundle.
   */
  virtual godot::Ref<ModelBundle> pack_bundle() { return {}; }
};

#endif // VDOT_MODEL_H
