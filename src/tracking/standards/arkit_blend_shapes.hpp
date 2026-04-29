#ifndef VDOT_BLEND_SHAPES_H
#define VDOT_BLEND_SHAPES_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vmap.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace ARKit {

/**
 * BlendShapes defines an enum of all possible blend shape names used by
 * ARKit's face tracking.
 *
 * Coincidentally, since this is primarily used by the LiveLinkFace server, the
 * enum values map to the corresponding blend shape value in a LiveLinkFace
 * tracking packet.
 *
 * @see
 * https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation
 * @see https://arkit-face-blendshapes.com
 */
enum BlendShape {
  EyeBlinkLeft        = 0,
  EyeLookDownLeft     = 1,
  EyeLookInLeft       = 2,
  EyeLookOutLeft      = 3,
  EyeLookUpLeft       = 4,
  EyeSquintLeft       = 5,
  EyeWideLeft         = 6,

  EyeBlinkRight       = 7,
  EyeLookDownRight    = 8,
  EyeLookInRight      = 9,
  EyeLookOutRight     = 10,
  EyeLookUpRight      = 11,
  EyeSquintRight      = 12,
  EyeWideRight        = 13,

  JawForward          = 14,
  JawRight            = 15,
  JawLeft             = 16,
  JawOpen             = 17,

  MouthClose          = 18,
  MouthFunnel         = 19,
  MouthPucker         = 20,
  MouthRight          = 21,
  MouthLeft           = 22,

  MouthSmileLeft      = 23,
  MouthSmileRight     = 24,

  MouthFrownLeft      = 25,
  MouthFrownRight     = 26,

  MouthDimpleLeft     = 27,
  MouthDimpleRight    = 28,

  MouthStretchLeft    = 29,
  MouthStretchRight   = 30,

  MouthRollLower      = 31,
  MouthRollUpper      = 32,
  MouthShrugLower     = 33,
  MouthShrugUpper     = 34,

  MouthPressLeft      = 35,
  MouthPressRight     = 36,

  MouthLowerDownLeft  = 37,
  MouthLowerDownRight = 38,

  MouthUpperUpLeft    = 39,
  MouthUpperUpRight   = 40,

  BrowDownLeft        = 41,
  BrowDownRight       = 42,

  BrowInnerUp         = 43,

  BrowOuterUpLeft     = 44,
  BrowOuterUpRight    = 45,

  CheekPuff           = 46,

  CheekSquintLeft     = 47,
  CheekSquintRight    = 48,

  NoseSneerLeft       = 49,
  NoseSneerRight      = 50,

  TongueOut           = 51,

  // limit used for array sizing.
  Max                 = 52,
};

godot::HashMap<godot::StringName, BlendShape> const ShapeNames{
  {        "EyeBlinkLeft",        BlendShape::EyeBlinkLeft },
  {     "EyeLookDownLeft",     BlendShape::EyeLookDownLeft },
  {       "EyeLookInLeft",       BlendShape::EyeLookInLeft },
  {      "EyeLookOutLeft",      BlendShape::EyeLookOutLeft },
  {       "EyeLookUpLeft",       BlendShape::EyeLookUpLeft },
  {       "EyeSquintLeft",       BlendShape::EyeSquintLeft },
  {         "EyeWideLeft",         BlendShape::EyeWideLeft },

  {       "EyeBlinkRight",       BlendShape::EyeBlinkRight },
  {    "EyeLookDownRight",    BlendShape::EyeLookDownRight },
  {      "EyeLookInRight",      BlendShape::EyeLookInRight },
  {     "EyeLookOutRight",     BlendShape::EyeLookOutRight },
  {      "EyeLookUpRight",      BlendShape::EyeLookUpRight },
  {      "EyeSquintRight",      BlendShape::EyeSquintRight },
  {        "EyeWideRight",        BlendShape::EyeWideRight },

  {          "JawForward",          BlendShape::JawForward },
  {            "JawRight",            BlendShape::JawRight },
  {             "JawLeft",             BlendShape::JawLeft },
  {             "JawOpen",             BlendShape::JawOpen },

  {          "MouthClose",          BlendShape::MouthClose },
  {         "MouthFunnel",         BlendShape::MouthFunnel },
  {         "MouthPucker",         BlendShape::MouthPucker },
  {          "MouthRight",          BlendShape::MouthRight },
  {           "MouthLeft",           BlendShape::MouthLeft },

  {      "MouthSmileLeft",      BlendShape::MouthSmileLeft },
  {     "MouthSmileRight",     BlendShape::MouthSmileRight },

  {      "MouthFrownLeft",      BlendShape::MouthFrownLeft },
  {     "MouthFrownRight",     BlendShape::MouthFrownRight },

  {     "MouthDimpleLeft",     BlendShape::MouthDimpleLeft },
  {    "MouthDimpleRight",    BlendShape::MouthDimpleRight },

  {    "MouthStretchLeft",    BlendShape::MouthStretchLeft },
  {   "MouthStretchRight",   BlendShape::MouthStretchRight },

  {      "MouthRollLower",      BlendShape::MouthRollLower },
  {      "MouthRollUpper",      BlendShape::MouthRollUpper },
  {     "MouthShrugLower",     BlendShape::MouthShrugLower },
  {     "MouthShrugUpper",     BlendShape::MouthShrugUpper },

  {      "MouthPressLeft",      BlendShape::MouthPressLeft },
  {     "MouthPressRight",     BlendShape::MouthPressRight },

  {  "MouthLowerDownLeft",  BlendShape::MouthLowerDownLeft },
  { "MouthLowerDownRight", BlendShape::MouthLowerDownRight },

  {    "MouthUpperUpLeft",    BlendShape::MouthUpperUpLeft },
  {   "MouthUpperUpRight",   BlendShape::MouthUpperUpRight },

  {        "BrowDownLeft",        BlendShape::BrowDownLeft },
  {       "BrowDownRight",       BlendShape::BrowDownRight },

  {         "BrowInnerUp",         BlendShape::BrowInnerUp },

  {     "BrowOuterUpLeft",     BlendShape::BrowOuterUpLeft },
  {    "BrowOuterUpRight",    BlendShape::BrowOuterUpRight },

  {           "CheekPuff",           BlendShape::CheekPuff },

  {     "CheekSquintLeft",     BlendShape::CheekSquintLeft },
  {    "CheekSquintRight",    BlendShape::CheekSquintRight },

  {       "NoseSneerLeft",       BlendShape::NoseSneerLeft },
  {      "NoseSneerRight",      BlendShape::NoseSneerRight },

  {           "TongueOut",           BlendShape::TongueOut },
};

godot::HashMap<BlendShape, godot::StringName> const shape_to_name{
  {        BlendShape::EyeBlinkLeft,        "eyeBlinkLeft" },
  {     BlendShape::EyeLookDownLeft,     "eyeLookDownLeft" },
  {       BlendShape::EyeLookInLeft,       "eyeLookInLeft" },
  {      BlendShape::EyeLookOutLeft,      "eyeLookOutLeft" },
  {       BlendShape::EyeLookUpLeft,       "eyeLookUpLeft" },
  {       BlendShape::EyeSquintLeft,       "eyeSquintLeft" },
  {         BlendShape::EyeWideLeft,         "eyeWideLeft" },

  {       BlendShape::EyeBlinkRight,       "eyeBlinkRight" },
  {    BlendShape::EyeLookDownRight,    "eyeLookDownRight" },
  {      BlendShape::EyeLookInRight,      "eyeLookInRight" },
  {     BlendShape::EyeLookOutRight,     "eyeLookOutRight" },
  {      BlendShape::EyeLookUpRight,      "eyeLookUpRight" },
  {      BlendShape::EyeSquintRight,      "eyeSquintRight" },
  {        BlendShape::EyeWideRight,        "eyeWideRight" },

  {          BlendShape::JawForward,          "jawForward" },
  {            BlendShape::JawRight,            "jawRight" },
  {             BlendShape::JawLeft,             "jawLeft" },
  {             BlendShape::JawOpen,             "jawOpen" },

  {          BlendShape::MouthClose,          "mouthClose" },
  {         BlendShape::MouthFunnel,         "mouthFunnel" },
  {         BlendShape::MouthPucker,         "mouthPucker" },
  {          BlendShape::MouthRight,          "mouthRight" },
  {           BlendShape::MouthLeft,           "mouthLeft" },

  {      BlendShape::MouthSmileLeft,      "mouthSmileLeft" },
  {     BlendShape::MouthSmileRight,     "mouthSmileRight" },

  {      BlendShape::MouthFrownLeft,      "mouthFrownLeft" },
  {     BlendShape::MouthFrownRight,     "mouthFrownRight" },

  {     BlendShape::MouthDimpleLeft,     "mouthDimpleLeft" },
  {    BlendShape::MouthDimpleRight,    "mouthDimpleRight" },

  {    BlendShape::MouthStretchLeft,    "mouthStretchLeft" },
  {   BlendShape::MouthStretchRight,   "mouthStretchRight" },

  {      BlendShape::MouthRollLower,      "mouthRollLower" },
  {      BlendShape::MouthRollUpper,      "mouthRollUpper" },
  {     BlendShape::MouthShrugLower,     "mouthShrugLower" },
  {     BlendShape::MouthShrugUpper,     "mouthShrugUpper" },

  {      BlendShape::MouthPressLeft,      "mouthPressLeft" },
  {     BlendShape::MouthPressRight,     "mouthPressRight" },

  {  BlendShape::MouthLowerDownLeft,  "mouthLowerDownLeft" },
  { BlendShape::MouthLowerDownRight, "mouthLowerDownRight" },

  {    BlendShape::MouthUpperUpLeft,    "mouthUpperUpLeft" },
  {   BlendShape::MouthUpperUpRight,   "mouthUpperUpRight" },

  {        BlendShape::BrowDownLeft,        "browDownLeft" },
  {       BlendShape::BrowDownRight,       "browDownRight" },

  {         BlendShape::BrowInnerUp,         "browInnerUp" },

  {     BlendShape::BrowOuterUpLeft,     "browOuterUpLeft" },
  {    BlendShape::BrowOuterUpRight,    "browOuterUpRight" },

  {           BlendShape::CheekPuff,           "cheekPuff" },

  {     BlendShape::CheekSquintLeft,     "cheekSquintLeft" },
  {    BlendShape::CheekSquintRight,    "cheekSquintRight" },

  {       BlendShape::NoseSneerLeft,       "noseSneerLeft" },
  {      BlendShape::NoseSneerRight,      "noseSneerRight" },

  {           BlendShape::TongueOut,           "tongueOut" },
};


} // namespace ARKit

VARIANT_ENUM_CAST(ARKit::BlendShape);

#endif // VDOT_BLEND_SHAPES_H
