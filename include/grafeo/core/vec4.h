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
#ifndef GRF_GL_VEC4_H
#define GRF_GL_VEC4_H
#include <math.h>
#define grf_to_rad(angle) (angle / 180.0 * M_PI  )
#define grf_to_deg(angle) (angle * 180.0 * M_1_PI)
typedef struct _GrfVec4{
  double data[4];
}GrfVec4;

/**
 * @brief grf_vec4_add
 * @param vec1
 * @param vec2
 */
void
grf_vec4_add(GrfVec4* vec1, GrfVec4* vec2);

/**
 * @brief grf_vec4_subtract
 * @param vec1
 * @param vec2
 */
void
grf_vec4_subtract(GrfVec4* vec1, GrfVec4* vec2);

/**
 * @brief grf_vec4_multiply
 * @param vec1
 * @param vec2
 */
void
grf_vec4_multiply(GrfVec4* vec1, GrfVec4* vec2);

/**
 * @brief grf_vec4_divide
 * @param vec1
 * @param vec2
 */
void
grf_vec4_divide(GrfVec4* vec1, GrfVec4* vec2);

/**
 * @brief grf_vec4_dot
 * @param vec1
 * @param vec2
 * @return
 */
double
grf_vec4_dot(GrfVec4* vec1, GrfVec4* vec2);

/**
 * @brief grf_vec4_multiply_scalar
 * @param vec1
 * @param scalar
 */
void
grf_vec4_multiply_scalar(GrfVec4* vec1, double scalar);
/**
 * @brief grf_vec4_normalize
 * @param vec
 */
void
grf_vec4_normalize(GrfVec4* vec);
/**
 * @brief grf_vec4_get_magnitude
 * @param vec
 * @return
 */
double
grf_vec4_get_magnitude(GrfVec4* vec);

#endif
