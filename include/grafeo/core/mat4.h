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
#include <grafeo/core.h>
typedef struct _GrfMat4{
  double data[16];
}GrfMat4;
/**
 * @brief grf_mat4_eye
 * @return
 */
GrfMat4
grf_mat4_eye();
/**
 * @brief grf_mat4_rotate_vec3
 * @param vec
 * @param angle
 * @param axis
 */
void
grf_mat4_rotate_vec3(GrfVec3* vec, double angle, GrfVec3 axis);
/**
 * @brief grf_mat4_rotate_mat4
 * @param mat
 * @param angle
 * @param axis
 */
void
grf_mat4_rotate_mat4(GrfMat4* mat, double angle, GrfVec3 axis);
/**
 * @brief grf_mat4_mult_vec3
 * @param mat
 * @param vec
 */
void
grf_mat4_mult_vec3(GrfMat4* mat, GrfVec3* vec);
/**
 * @brief grf_mat4_mult_mat4
 * @param mat1
 * @param mat2
 * @param output
 */
void
grf_mat4_mult_mat4(GrfMat4* mat1, GrfMat4* mat2, GrfMat4* output);
#endif
