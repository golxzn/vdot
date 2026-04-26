// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 MizunagiKB <mizukb@live.jp>

// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024 northernpaws

#ifndef VDOT_CUBISM_EFFECT_H
#define VDOT_CUBISM_EFFECT_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

#include "cubism.hpp"
#include "cubism_model.hpp"

class CubismModelProxy;

class CubismEffect : public godot::Node {
  GDCLASS(CubismEffect, godot::Node)

protected:
  bool _active = true;

protected:
  static void _bind_methods();

public:
  void set_active(bool const active) { this->_active = active; }

  [[nodiscard]] bool get_active() const { return this->_active; }

  virtual void _cubism_init(CubismModelProxy *model);
  virtual void _cubism_term(CubismModelProxy *model);
  virtual void _cubism_prologue(CubismModelProxy *model, float const delta);
  virtual void _cubism_process(CubismModelProxy *model, float const delta);
  virtual void _cubism_epilogue(CubismModelProxy *model, float const delta);

  void _enter_tree() override;
  void _exit_tree() override;
};

#endif // VDOT_CUBISM_EFFECT_H
