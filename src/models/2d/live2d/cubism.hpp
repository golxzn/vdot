#ifndef VDOT_CUBISM_H
#define VDOT_CUBISM_H

#if defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif

enum CubismShader {
  CUBISM_SHADER_NORM_ADD,
  CUBISM_SHADER_NORM_MIX,
  CUBISM_SHADER_NORM_MUL,
  CUBISM_SHADER_MASK,
  CUBISM_SHADER_MASK_ADD,
  CUBISM_SHADER_MASK_ADD_INV,
  CUBISM_SHADER_MASK_MIX,
  CUBISM_SHADER_MASK_MIX_INV,
  CUBISM_SHADER_MASK_MUL,
  CUBISM_SHADER_MASK_MUL_INV,
  CUBISM_SHADER_MAX
};

static int const MAX_PRINTLOG_LENGTH              = 256;

static bool const DEFAULT_PROP_ANIM_LOOP          = false;
static bool const DEFAULT_PROP_ANIM_LOOP_FADE_IN  = true;

static char const *PROP_ANIM_GROUP                = "Animation";
static char const *PROP_ANIM_EXPRESSION           = "anim_expression";
static char const *PROP_ANIM_MOTION               = "anim_motion";
static char const *PROP_ANIM_LOOP                 = "anim_loop";
static char const *PROP_ANIM_LOOP_FADE_IN         = "anim_loop_fade_in";
static char const *PROP_PARAMETER_GROUP           = "Parameter";
static char const *PROP_PART_OPACITY_GROUP        = "PartOpacity";

static char const *SIGNAL_EFFECT_HIT_AREA_ENTERED = "hit_area_entered";
static char const *SIGNAL_EFFECT_HIT_AREA_EXITED  = "hit_area_exited";

#ifdef CUBISM_MOTION_CUSTOMDATA
static char const *SIGNAL_MOTION_FINISHED = "motion_finished";
#endif // CUBISM_MOTION_CUSTOMDATA

#if defined(__GNUC__) || defined(__GNUG__)
#  pragma GCC diagnostic pop
#endif

#endif // VDOT_CUBISM_H
