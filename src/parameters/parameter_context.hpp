#ifndef VDOT_PARAMETER_CONTEXT_H
#define VDOT_PARAMETER_CONTEXT_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>

#include "parameter.hpp"
#include "parameter_input.hpp"

/**
 * Groups a collection of related parameters, typically
 *  associated with something else that provides them.
 */
class ParameterContext : public godot::RefCounted {
  GDCLASS(ParameterContext, godot::RefCounted)
protected:
  static void _bind_methods();

public:
  static constexpr char const *SIGNAL_PARAMETER_ADDED   = "parameter_added";
  static constexpr char const *SIGNAL_PARAMETER_REMOVED = "parameter_removed";

  godot::String context_id;
  godot::String context_label;
  godot::String context_description;

  godot::HashMap<godot::StringName, godot::Ref<InputParameter>>
    context_parameters;

  [[nodiscard]] godot::String get_context_id() const;
  [[nodiscard]] godot::String get_context_label() const;
  [[nodiscard]] godot::String get_context_description() const;

  [[nodiscard]] godot::HashMap<godot::StringName, godot::Ref<InputParameter>>
       get_context_parameters() const;
  void set_context_parameters(
    godot::HashMap<godot::StringName, godot::Ref<InputParameter>> const
      &p_parameters
  );

  void add_input_parameter(godot::Ref<InputParameter> const &p_parameter);
  void remove_input_parameter(godot::StringName const &p_parameter_name);

  [[nodiscard]] bool has_input_parameter(
    godot::StringName const &p_name
  ) const;
  [[nodiscard]] godot::Ref<InputParameter> get_input_parameter(
    godot::StringName const &p_name
  ) const;

  [[nodiscard]] godot::TypedArray<godot::StringName> list_parameters() const;
};

#endif // VDOT_PARAMETER_CONTEXT_H
