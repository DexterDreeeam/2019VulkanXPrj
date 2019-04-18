
/*
 *
 *  //-- DexterDreeeam copyright --//
 *
 *  File Name:
 *
 *    >> math_x.hpp
 *
 *  Abstract:
 *
 *    >> math libs types
 *
 */

#if !defined(__MATH_X_HPP__)
#define __MATH_X_HPP__

#include "../core/_Macros.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "./glm/vec4.hpp"
#include "./glm/mat4x4.hpp"

_x_NS_START_

typedef glm::mat4 t_Vec4;
typedef glm::mat4 t_Mat4;

_x_NS_END_

#endif