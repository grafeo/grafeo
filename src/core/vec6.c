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
#include <grafeo/core.h>

void
grf_vec6_add(GrfVec6* vec1, GrfVec6* vec2){
  vec1->data[0] += vec2->data[0];
  vec1->data[1] += vec2->data[1];
  vec1->data[2] += vec2->data[2];
  vec1->data[3] += vec2->data[3];
  vec1->data[4] += vec2->data[4];
  vec1->data[5] += vec2->data[5];
}

void
grf_vec6_subtract(GrfVec6* vec1, GrfVec6* vec2){
  vec1->data[0] -= vec2->data[0];
  vec1->data[1] -= vec2->data[1];
  vec1->data[2] -= vec2->data[2];
  vec1->data[3] -= vec2->data[3];
  vec1->data[4] -= vec2->data[4];
  vec1->data[5] -= vec2->data[5];
}

void
grf_vec6_multiply(GrfVec6* vec1, GrfVec6* vec2){
  vec1->data[0] *= vec2->data[0];
  vec1->data[1] *= vec2->data[1];
  vec1->data[2] *= vec2->data[2];
  vec1->data[3] *= vec2->data[3];
  vec1->data[4] *= vec2->data[4];
  vec1->data[5] *= vec2->data[5];
}

void
grf_vec6_divide(GrfVec6* vec1, GrfVec6* vec2){
  vec1->data[0] /= vec2->data[0];
  vec1->data[1] /= vec2->data[1];
  vec1->data[2] /= vec2->data[2];
  vec1->data[3] /= vec2->data[3];
  vec1->data[4] /= vec2->data[4];
  vec1->data[5] /= vec2->data[5];
}

double
grf_vec6_dot(GrfVec6* vec1, GrfVec6* vec2){
  return  vec1->data[0]*vec2->data[0] +
          vec1->data[1]*vec2->data[1] +
          vec1->data[2]*vec2->data[2] +
          vec1->data[3]*vec2->data[3] +
          vec1->data[4]*vec2->data[4] +
          vec1->data[5]*vec2->data[5];
}
void
grf_vec6_multiply_scalar(GrfVec6* vec1, double scalar){
  vec1->data[0] *= scalar;
  vec1->data[1] *= scalar;
  vec1->data[2] *= scalar;
  vec1->data[3] *= scalar;
  vec1->data[4] *= scalar;
  vec1->data[5] *= scalar;
}
void
grf_vec6_normalize(GrfVec6* vec){
  double magnitude = grf_vec6_get_magnitude(vec);
  grf_vec6_multiply_scalar(vec,1.0/magnitude);
}
double
grf_vec6_get_magnitude(GrfVec6* vec){
  return sqrt(grf_vec6_dot(vec,vec));
}