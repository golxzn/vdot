// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 MizunagiKB <mizukb@live.jp>

// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024 northernpaws

#ifndef VDOT_CUBISM_MODEL_PROXY_H
#define VDOT_CUBISM_MODEL_PROXY_H

#include <CubismFramework.hpp>
#include <CubismModelSettingJson.hpp>
#include <Model/CubismUserModel.hpp>
#include <Motion/CubismMotion.hpp>
#include <Motion/CubismMotionQueueManager.hpp>
#include <Utils/CubismString.hpp>

#include "cubism_renderer.hpp"
#include "models/2d/live2d/cubism.hpp"

class CubismEffectBreath;
class CubismEffectCustom;
class CubismEffectEyeBlink;
class CubismEffectHitArea;

class CubismModelProxy : public Csm::CubismUserModel {
  friend CubismModel;
  friend CubismEffectBreath;
  friend CubismEffectCustom;
  friend CubismEffectEyeBlink;
  friend CubismEffectHitArea;

  enum EFFECT_CALL {
    EFFECT_CALL_PROLOGUE,
    EFFECT_CALL_PROCESS,
    EFFECT_CALL_EPILOGUE
  };

public:
  CubismModelProxy(CubismModel *owner_viewport, godot::Node *parent_node);
  ~CubismModelProxy() override;

public:
  CubismModel *_owner_viewport = nullptr;
  godot::Node *_parent_node    = nullptr;

  //  private:
  CubismRenderer                      _renderer_resource;
  CubismModel::moc3FileFormatVersion  _moc3_file_format_version;
  godot::String                       _model_pathname;
  Csm::ICubismModelSetting           *_model_setting;
  Csm::csmVector<Csm::CubismIdHandle> _list_eye_blink;
  Csm::csmVector<Csm::CubismIdHandle> _list_lipsync;
  Csm::csmMap<Csm::csmString, Csm::CubismExpressionMotion *> _map_expression;
  Csm::csmMap<Csm::csmString, Csm::CubismMotion *>           _map_motion;

public:
  bool model_load(godot::String const &model_pathname);
  void model_load_resource();
  void pro_update(float const delta);
  void efx_update(float const delta);
  void epi_update(float const delta);
  void update_node();
  void clear();

  void stop();

  void expression_set(char const *expression_id);
  void expression_stop();

  Live2D::Cubism::Framework::CubismMotionQueueEntryHandle motion_start(
    char const   *group,
    int32_t const no,
    int32_t const priority,
    bool const    loop,
    bool const    loop_fade_in,
    void         *custom_data
  );
  void motion_stop();

  void MotionEventFired(Csm::csmString const &eventValue) override;

private:
  void expression_load();
  void physics_load();
  void pose_load();
  void userdata_load();
  void motion_load();

  void effect_init();
  void effect_term();
  void effect_batch(float const delta, const EFFECT_CALL efx_call);
};

#endif // VDOT_CUBISM_MODEL_PROXY_H
