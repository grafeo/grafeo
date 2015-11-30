/* ===================================================================
#   Copyright (C) 2015-2015
#   Anderson Tavares <nocturne.pe at gmail.com> PK 0x38e7bfc5c2def8ff
#   Lucy Mansilla    <lucyacm at gmail.com>
#   Caio de Braz     <caiobraz at gmail.com>
#   Hans Harley      <hansbecc at gmail.com>
#   Paulo Miranda    <pavmbr at yahoo.com.br>
#
#   Institute of Mathematics and Statistics - IME
#   University of Sao Paulo - USP
#
#   This file is part of Grafeo.
#
#   Grafeo is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation, either version
#   3 of the License, or (at your option) any later version.
#
#   Grafeo is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty
#   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#   See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with Grafeo.  If not, see
#   <http://www.gnu.org/licenses/>.
# ===================================================================*/
#ifndef GRF_GL_MAT4_H
#define GRF_GL_MAT4_H
#include <grafeo/gl.h>
typedef struct _GrfGLMat4{
  double data[16];
}GrfGLMat4;
/**
 * @brief grf_gl_mat4_eye
 * @return
 */
GrfGLMat4
grf_gl_mat4_eye();
/**
 * @brief grf_gl_mat4_rotate_vec3
 * @param vec
 * @param angle
 * @param axis
 */
void
grf_gl_mat4_rotate_vec3(GrfGLVec3* vec, double angle, GrfGLVec3 axis);
/**
 * @brief grf_gl_mat4_rotate_mat4
 * @param mat
 * @param angle
 * @param axis
 */
void
grf_gl_mat4_rotate_mat4(GrfGLMat4* mat, double angle, GrfGLVec3 axis);
/**
 * @brief grf_gl_mat4_mult_vec3
 * @param mat
 * @param vec
 */
void
grf_gl_mat4_mult_vec3(GrfGLMat4* mat, GrfGLVec3* vec);
/**
 * @brief grf_gl_mat4_mult_mat4
 * @param mat1
 * @param mat2
 * @param output
 */
void
grf_gl_mat4_mult_mat4(GrfGLMat4* mat1, GrfGLMat4* mat2, GrfGLMat4* output);
#endif
