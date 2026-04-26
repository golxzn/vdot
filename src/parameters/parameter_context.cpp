#include "parameter_context.hpp"

#include "parameter_input.hpp"

void ParameterContext::_bind_methods() {
  ADD_SIGNAL(
    godot::MethodInfo(
      SIGNAL_PARAMETER_ADDED,
      godot::PropertyInfo(godot::Variant::STRING_NAME, "context_id"),
      godot::PropertyInfo(
        godot::Variant::OBJECT,
        "parameter",
        godot::PROPERTY_HINT_TYPE_STRING,
        Parameter::get_class_static()
      )
    )
  );

  ADD_SIGNAL(
    godot::MethodInfo(
      SIGNAL_PARAMETER_REMOVED,
      godot::PropertyInfo(godot::Variant::STRING_NAME, "context_id"),
      godot::PropertyInfo(
        godot::Variant::OBJECT,
        "parameter",
        godot::PROPERTY_HINT_TYPE_STRING,
        Parameter::get_class_static()
      )
    )
  );

  ADD_SIGNAL(
    godot::MethodInfo(
      "parameters_updated",
      godot::PropertyInfo(
        godot::Variant::ARRAY,
        "parameters",
        godot::PROPERTY_HINT_ARRAY_TYPE,
        Parameter::get_class_static()
      )
    )
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("get_context_id"), &ParameterContext::get_context_id
  );
  ADD_PROPERTY(
    godot::PropertyInfo(godot::Variant::STRING, "context_id"),
    godot::String(),
    "get_context_id"
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("get_context_label"), &ParameterContext::get_context_label
  );
  ADD_PROPERTY(
    godot::PropertyInfo(godot::Variant::STRING, "context_label"),
    godot::String(),
    "get_context_label"
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("get_context_description"),
    &ParameterContext::get_context_description
  );
  ADD_PROPERTY(
    godot::PropertyInfo(godot::Variant::STRING, "context_description"),
    godot::String(),
    "get_context_description"
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("add_input_parameter", "input_parameter"),
    &ParameterContext::add_input_parameter
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("remove_input_parameter", "parameter_name"),
    &ParameterContext::remove_input_parameter
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("get_input_parameter", "parameter_name"),
    &ParameterContext::get_input_parameter
  );

  godot::ClassDB::bind_method(
    godot::D_METHOD("list_parameters"), &ParameterContext::list_parameters
  );
}

godot::String ParameterContext::get_context_id() const { return context_id; }

godot::String ParameterContext::get_context_label() const {
  return context_label;
}

godot::String ParameterContext::get_context_description() const {
  return context_description;
}

godot::HashMap<godot::StringName, godot::Ref<InputParameter>>
ParameterContext::get_context_parameters() const {
  return context_parameters;
}

void ParameterContext::set_context_parameters(
  godot::HashMap<godot::StringName, godot::Ref<InputParameter>> const
    &p_parameters
) {
  context_parameters = p_parameters;
}

void ParameterContext::add_input_parameter(
  godot::Ref<InputParameter> const &p_parameter
) {
  context_parameters[p_parameter->get_parameter_id()] = p_parameter;
  emit_signal(SIGNAL_PARAMETER_ADDED, context_id, p_parameter);
}

void ParameterContext::remove_input_parameter(
  godot::StringName const &p_parameter_id
) {
  ERR_FAIL_COND_MSG(
    !context_parameters.has(p_parameter_id),
    "Requested parameter to remove not found."
  );

  emit_signal(
    SIGNAL_PARAMETER_REMOVED, context_id, context_parameters[p_parameter_id]
  );

  context_parameters.erase(p_parameter_id);
}

bool ParameterContext::has_input_parameter(
  godot::StringName const &p_parameter_id
) const {
  return context_parameters.has(p_parameter_id);
}

godot::Ref<InputParameter> ParameterContext::get_input_parameter(
  godot::StringName const &p_parameter_id
) const {
  ERR_FAIL_COND_V_MSG(
    !context_parameters.has(p_parameter_id),
    nullptr,
    godot::vformat("Requested parameter '%s' does not exist.", p_parameter_id)
  );

  return context_parameters[p_parameter_id];
}

godot::TypedArray<godot::StringName> ParameterContext::
  list_parameters() const {
  godot::TypedArray<godot::StringName> parameter_ids;
  for (godot::KeyValue<godot::StringName, godot::Ref<InputParameter>> const
         &context_parameter : context_parameters) {
    parameter_ids.append(context_parameter.key);
  }

  return parameter_ids;
}
