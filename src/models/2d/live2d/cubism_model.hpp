// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 MizunagiKB <mizukb@live.jp>

// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024 northernpaws

#ifndef VDOT_CUBISM_USER_MODEL_H
#define VDOT_CUBISM_USER_MODEL_H

#include <CubismFramework.hpp>
#include <Math/CubismVector2.hpp>
#include <Motion/ACubismMotion.hpp>
#include <Motion/CubismMotionQueueEntry.hpp>
#include <godot_cpp/classes/canvas_group.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "cubism.hpp"
#include "cubism_effect.hpp"
#include "cubism_motion_entry.hpp"
#include "models/2d/model_2d.hpp"

class CubismModelProxy;
class CubismEffect;

struct anim_expression {
public:
  godot::String expression_id;

public:
  anim_expression() = default;

  explicit anim_expression(Csm::csmChar const *c_expression_id)
    : expression_id(godot::String(c_expression_id)) {}

  [[nodiscard]] godot::String to_string() const {
    return godot::String(this->expression_id);
  }
};

class anim_motion {
public:
  godot::String group;
  int32_t       no = 0;

public:
  anim_motion() = default;

  anim_motion(Csm::csmChar const *c_group, int32_t const c_no)
    : group(godot::String(c_group))
    , no(c_no) {}

  [[nodiscard]] godot::String to_string() const {
    godot::Array ary;
    ary.append(this->group);
    ary.append(this->no);

    return godot::String("{0}_{1}").format(ary);
  }
};

class CubismModel : public Model2D {
  GDCLASS(CubismModel, Model2D)

public:
  CubismModel();
  ~CubismModel() override;

public:
  enum moc3FileFormatVersion {
    CSM_MOC_VERSION_UNKNOWN = Live2D::Cubism::Core::csmMocVersion_Unknown,
    CSM_MOC_VERSION_30      = Live2D::Cubism::Core::csmMocVersion_30,
    CSM_MOC_VERSION_33      = Live2D::Cubism::Core::csmMocVersion_33,
    CSM_MOC_VERSION_40      = Live2D::Cubism::Core::csmMocVersion_40,
    CSM_MOC_VERSION_42      = Live2D::Cubism::Core::csmMocVersion_42,
    CSM_MOC_VERSION_50      = Live2D::Cubism::Core::csmMocVersion_50
  };

  enum Priority {
    PRIORITY_NONE   = 0,
    PRIORITY_IDLE   = 1,
    PRIORITY_NORMAL = 2,
    PRIORITY_FORCE  = 3
  };

  enum ParameterMode { FULL_PARAMETER = 0, NONE_PARAMETER = 1 };

  enum MotionProcessCallback { PHYSICS = 0, IDLE = 1, MANUAL = 2 };

  godot::String     assets;
  CubismModelProxy *proxy_model;
  bool              enable_load_expressions;
  bool              enable_load_motions;

  float           speed_scale;
  bool            auto_scale;
  float           adjust_scale;
  godot::Vector2  adjust_pos;
  godot::Vector2i mask_viewport_size;

  ParameterMode         parameter_mode;
  MotionProcessCallback playback_process_mode;

  godot::Array ary_shader;
  godot::Array ary_parameter;
  godot::Array ary_part_opacity;

  Csm::csmBool                                anim_loop;
  Csm::csmBool                                anim_loop_fade_in;
  Csm::csmMap<godot::String, anim_expression> dict_anim_expression;
  godot::String                               curr_anim_expression_key;
  Csm::csmMap<godot::String, anim_motion>     dict_anim_motion;
  godot::String                               curr_anim_motion_key;

  Csm::csmVector<CubismEffect *> _list_cubism_effect;
  bool                           cubism_effect_dirty;

protected:
  static void _bind_methods();

public:
  godot::Dictionary     csm_get_version();
  moc3FileFormatVersion csm_get_latest_moc_version();
  moc3FileFormatVersion csm_get_moc_version();

  void                        set_assets(godot::String const assets);
  [[nodiscard]] godot::String get_assets() const;

  void               set_load_expressions(bool const enable);
  [[nodiscard]] bool get_load_expressions() const;
  void               set_load_motions(bool const enable);
  [[nodiscard]] bool get_load_motions() const;

  [[nodiscard]] godot::Dictionary get_canvas_info() const;

  [[nodiscard]] bool is_initialized() const;
  void               clear();

  void set_parameter_mode(ParameterMode const value);
  [[nodiscard]] CubismModel::ParameterMode get_parameter_mode() const;

  void set_process_callback(MotionProcessCallback const value);
  [[nodiscard]] CubismModel::MotionProcessCallback
  get_process_callback() const;

  void                set_speed_scale(float const speed);
  [[nodiscard]] float get_speed_scale() const;

  void               set_auto_scale(bool const value);
  [[nodiscard]] bool get_auto_scale() const;

  void set_adjust_scale(float const scale) { this->adjust_scale = scale; }

  [[nodiscard]] float get_adjust_scale() const { return this->adjust_scale; }

  void set_adjust_position(godot::Vector2 const pos) {
    this->adjust_pos = pos;
  }

  godot::Vector2 get_adjust_position() const { return this->adjust_pos; }

  void set_mask_viewport_size(godot::Vector2i const size) {
    this->mask_viewport_size = size;
  }

  godot::Vector2i get_mask_viewport_size() const {
    return this->mask_viewport_size;
  }

  [[nodiscard]] godot::Dictionary          get_motions() const;
  godot::Ref<CubismMotionQueueEntryHandle> start_motion(
    godot::String const str_group,
    int32_t const       no,
    Priority const      priority
  );
  godot::Ref<CubismMotionQueueEntryHandle> start_motion_loop(
    godot::String const str_group,
    int32_t const       no,
    Priority const      priority,
    bool const          loop,
    bool const          loop_fade_in
  );
  [[nodiscard]] godot::Array get_cubism_motion_queue_entries() const;
  void                       stop_motion();

  [[nodiscard]] godot::Array get_expressions() const;
  void start_expression(godot::String const expression_id);
  void stop_expression();

  [[nodiscard]] godot::Array get_hit_areas() const;

  [[nodiscard]] godot::Array get_parameters() const;
  [[nodiscard]] godot::Array get_part_opacities() const;

  [[nodiscard]] godot::Dictionary get_meshes() const;

  void set_shader(
    CubismShader const               e,
    godot::Ref<godot::Shader> const &shader
  ) {
    this->ary_shader[e] = shader;
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader(
    CubismShader const e
  ) const {
    return this->ary_shader[e];
  }

  // NormAdd
  void set_shader_add(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_NORM_ADD, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_add() const {
    return this->get_shader(CUBISM_SHADER_NORM_ADD);
  }

  // NormMix
  void set_shader_mix(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_NORM_MIX, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mix() const {
    return this->get_shader(CUBISM_SHADER_NORM_MIX);
  }

  // NormMul
  void set_shader_mul(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_NORM_MUL, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mul() const {
    return this->get_shader(CUBISM_SHADER_NORM_MUL);
  }

  // Mask
  void set_shader_mask(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask() const {
    return this->get_shader(CUBISM_SHADER_MASK);
  }

  // MaskAdd
  void set_shader_mask_add(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_ADD, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_add() const {
    return this->get_shader(CUBISM_SHADER_MASK_ADD);
  }

  void set_shader_mask_add_inv(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_ADD_INV, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_add_inv() const {
    return this->get_shader(CUBISM_SHADER_MASK_ADD_INV);
  }

  // MaskMix
  void set_shader_mask_mix(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_MIX, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_mix() const {
    return this->get_shader(CUBISM_SHADER_MASK_MIX);
  }

  void set_shader_mask_mix_inv(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_MIX_INV, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_mix_inv() const {
    return this->get_shader(CUBISM_SHADER_MASK_MIX_INV);
  }

  // MaskMul
  void set_shader_mask_mul(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_MUL, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_mul() const {
    return this->get_shader(CUBISM_SHADER_MASK_MUL);
  }

  void set_shader_mask_mul_inv(godot::Ref<godot::Shader> const &shader) {
    this->set_shader(CUBISM_SHADER_MASK_MUL_INV, shader);
  }

  [[nodiscard]] godot::Ref<godot::Shader> get_shader_mask_mul_inv() const {
    return this->get_shader(CUBISM_SHADER_MASK_MUL_INV);
  }

  // for Signal
  static void on_motion_finished(Csm::ACubismMotion *motion);

  void _update(float const delta);

  void advance(float const delta);

  [[nodiscard]] bool check_cubism_effect_dirty() const;
  void               cubism_effect_dirty_reset();

  // Properties
  void setup_property();

  bool _set(godot::StringName const &p_name, godot::Variant const &p_value);
  bool _get(godot::StringName const &p_name, godot::Variant &r_ret) const;
  bool _property_can_revert(godot::StringName const &p_name) const;
  bool _property_get_revert(
    godot::StringName const &p_name,
    godot::Variant          &r_property
  ) const;
  void _get_property_list(godot::List<godot::PropertyInfo> *p_list);

  void _ready() override;
  void _enter_tree() override;
  void _exit_tree() override;
  void _process(double delta) override;
  void _physics_process(double delta) override;

  void _on_append_child_act(CubismEffect *node);
  void _on_remove_child_act(CubismEffect *node);

  godot::Ref<ModelBundle> pack_bundle() override;
};

VARIANT_ENUM_CAST(CubismModel::moc3FileFormatVersion);
VARIANT_ENUM_CAST(CubismModel::Priority);
VARIANT_ENUM_CAST(CubismModel::ParameterMode);
VARIANT_ENUM_CAST(CubismModel::MotionProcessCallback);

#endif // VDOT_CUBISM_USER_MODEL_H
