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
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfGLMat4
grf_gl_mat4_eye(){
  return (GrfGLMat4){{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
}

void
grf_gl_mat4_rotate_vec3(GrfGLVec3* vec, double angle, GrfGLVec3 axis){
  double a            = grf_to_rad(angle);
  double c            = cos(a);
  double cc           = 1-c;
  double s            = sin(a);

  grf_gl_vec3_normalize(&axis);

  //uint32_t  size     = 3;
  //uint32_t  sizes[2] = {4,4};
  //double     ve[3]    = {x,y,z};
  //GrfNDArray* axisn    = grf_ndarray_from_data(ve,1,&size,GRF_double);
  //GrfNDArray* axis     = grf_ndarray_normalize(axisn);

  double u  = axis.data[0];
  double v  = axis.data[1];
  double w  = axis.data[2];
  double u2 = u*u;
  double v2 = v*v;
  double w2 = w*w;
  double uv = u*v;
  double uw = u*w;
  double vw = v*w;

  GrfGLMat4 rotation = grf_gl_mat4_eye();
  //GrfNDArray* rotation = grf_ndarray_zeros(2,sizes,GRF_double);

  rotation.data[0]  = u2*cc + c;
  rotation.data[1]  = uv*cc + w*s;
  rotation.data[2]  = uw*cc - v*s;

  rotation.data[4]  = uv*cc - w*s;
  rotation.data[5]  = v2*cc + c;
  rotation.data[6]  = vw*cc + u*s;

  rotation.data[8]  = uw*cc + v*s;
  rotation.data[9]  = vw*cc - u*s;
  rotation.data[10] = w2*cc + c;

  grf_gl_mat4_mult_vec3(&rotation,vec);
}
void
grf_gl_mat4_mult_vec3(GrfGLMat4* mat, GrfGLVec3* vec){
  GrfGLVec3 res = *vec;
  u_int8_t i;
  GrfGLVec3 rows[3] = {{{mat->data[0],mat->data[4],mat->data[8]}},
                       {{mat->data[1],mat->data[5],mat->data[9]}},
                       {{mat->data[2],mat->data[6],mat->data[10]}}};
  for(i = 0; i < 3; i++)
    vec->data[i] = grf_gl_vec3_dot(&res,&rows[i]) + mat->data[12+i];
}
