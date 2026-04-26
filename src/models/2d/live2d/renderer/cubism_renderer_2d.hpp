// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023 MizunagiKB <mizukb@live.jp>

// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2024 northernpaws

#ifndef VDOT_CUBISM_RENDERER_2D_H
#define VDOT_CUBISM_RENDERER_2D_H

#include <Rendering/CubismRenderer.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/texture2d.hpp>

#include "cubism_renderer.hpp"
#include "models/2d/live2d/cubism.hpp"
#include "models/2d/live2d/cubism_model.hpp"

class CubismRenderer2D
  : public Live2D::Cubism::Framework::Rendering::CubismRenderer {
  friend class ::CubismRenderer;

public:
  CubismRenderer2D();
  ~CubismRenderer2D() override;

private:
  godot::Ref<godot::ShaderMaterial> make_ShaderMaterial(
    Csm::CubismModel const *model,
    Csm::csmInt32 const     index,
    ::CubismRenderer const &res
  ) const;
  void make_ArrayMesh_prepare(
    Csm::CubismModel const *model,
    ::CubismRenderer       &res
  );

  godot::Ref<godot::ArrayMesh> make_ArrayMesh(
    Csm::CubismModel const *model,
    Csm::csmInt32 const     index,
    bool const              makemask,
    ::CubismRenderer const &res
  ) const;

  void update_mask(
    godot::SubViewport *viewport,
    Csm::csmInt32 const index,
    ::CubismRenderer   &res
  );

public:
  [[nodiscard]] Csm::csmInt32 calc_viewport_count() const;
  [[nodiscard]] Csm::csmInt32 calc_mesh_instance_count() const;
  godot::Vector2              get_size(Csm::CubismModel const *model) const;
  godot::Vector2              get_origin(Csm::CubismModel const *model) const;
  float                       get_ppunit(Csm::CubismModel const *model) const;

  void update(::CubismRenderer &res);
  void update(
    ::CubismRenderer &res,
    bool const        update_node,
    bool const        update_mesh
  );

  void Initialize(
    Csm::CubismModel *model,
    Csm::csmInt32     maskBufferCount
  ) override;
  void DoDrawModel() override;
  void SaveProfile() override;
  void RestoreProfile() override;
};

#endif // VDOT_CUBISM_RENDERER_2D_H
