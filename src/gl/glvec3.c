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
#include <grafeo/gl.h>

void
grf_gl_vec3_add(GrfGLVec3* vec1, GrfGLVec3* vec2){
  vec1->data[0] += vec2->data[0];
  vec1->data[1] += vec2->data[1];
  vec1->data[2] += vec2->data[2];
}

void
grf_gl_vec3_subtract(GrfGLVec3* vec1, GrfGLVec3* vec2){
  vec1->data[0] -= vec2->data[0];
  vec1->data[1] -= vec2->data[1];
  vec1->data[2] -= vec2->data[2];
}

void
grf_gl_vec3_multiply(GrfGLVec3* vec1, GrfGLVec3* vec2){
  vec1->data[0] *= vec2->data[0];
  vec1->data[1] *= vec2->data[1];
  vec1->data[2] *= vec2->data[2];
}

void
grf_gl_vec3_divide(GrfGLVec3* vec1, GrfGLVec3* vec2){
  vec1->data[0] /= vec2->data[0];
  vec1->data[1] /= vec2->data[1];
  vec1->data[2] /= vec2->data[2];
}

double
grf_gl_vec3_dot(GrfGLVec3* vec1, GrfGLVec3* vec2){
  return  vec1->data[0]*vec2->data[0] +
          vec1->data[1]*vec2->data[1] +
          vec1->data[2]*vec2->data[2];
}
void
grf_gl_vec3_multiply_scalar(GrfGLVec3* vec1, double scalar){
  vec1->data[0] *= scalar;
  vec1->data[1] *= scalar;
  vec1->data[2] *= scalar;
}
void
grf_gl_vec3_normalize(GrfGLVec3* vec){
  double magnitude = grf_gl_vec3_get_magnitude(vec);
  grf_gl_vec3_multiply_scalar(vec,1.0/magnitude);
}
double
grf_gl_vec3_get_magnitude(GrfGLVec3* vec){
  return sqrt(vec->data[0]*vec->data[0] + vec->data[1]*vec->data[1] + vec->data[2]*vec->data[2]);
}
GrfGLVec3
grf_gl_vec3_cross(GrfGLVec3* vec1, GrfGLVec3* vec2){
  double* d1 = vec1->data;
  double* d2 = vec2->data;
  GrfGLVec3 result;
  double* d = result.data;
  d[0] = d1[1] * d2[2] - d2[1] * d1[2];
  d[1] = d1[2] * d2[0] - d2[2] * d1[0];
  d[2] = d1[0] * d2[1] - d2[0] * d1[1];
  return result;
}
