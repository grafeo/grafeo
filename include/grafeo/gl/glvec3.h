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
#ifndef GRF_GL_VEC3_H
#define GRF_GL_VEC3_H
#include <math.h>
#define grf_to_rad(angle) (angle / 180.0 * M_PI  )
#define grf_to_deg(angle) (angle * 180.0 * M_1_PI)
typedef struct _GrfGLVec3{
  double data[3];
}GrfGLVec3;

/**
 * @brief grf_gl_vec3_add
 * @param vec1
 * @param vec2
 */
void
grf_gl_vec3_add(GrfGLVec3* vec1, GrfGLVec3* vec2);

/**
 * @brief grf_gl_vec3_subtract
 * @param vec1
 * @param vec2
 */
void
grf_gl_vec3_subtract(GrfGLVec3* vec1, GrfGLVec3* vec2);

/**
 * @brief grf_gl_vec3_multiply
 * @param vec1
 * @param vec2
 */
void
grf_gl_vec3_multiply(GrfGLVec3* vec1, GrfGLVec3* vec2);

/**
 * @brief grf_gl_vec3_divide
 * @param vec1
 * @param vec2
 */
void
grf_gl_vec3_divide(GrfGLVec3* vec1, GrfGLVec3* vec2);

/**
 * @brief grf_gl_vec3_dot
 * @param vec1
 * @param vec2
 * @return
 */
double
grf_gl_vec3_dot(GrfGLVec3* vec1, GrfGLVec3* vec2);
/**
 * @brief grf_gl_vec3_cross
 * @param vec1
 * @param vec2
 * @return
 */
GrfGLVec3
grf_gl_vec3_cross(GrfGLVec3* vec1, GrfGLVec3* vec2);

/**
 * @brief grf_gl_vec3_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
grf_gl_vec3_multiply_scalar(GrfGLVec3* vec1, double scalar);
/**
 * @brief grf_gl_vec3_normalize
 * @param vec
 */
void
grf_gl_vec3_normalize(GrfGLVec3* vec);
/**
 * @brief grf_gl_vec3_get_magnitude
 * @param vec
 * @return
 */
double
grf_gl_vec3_get_magnitude(GrfGLVec3* vec);

#endif
