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
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
static void
grf_gl_create_rotation_matrix(GrfMat4* rotation, double angle, GrfVec3 axis){
  double a            = grf_to_rad(angle);
  double c            = cos(a);
  double cc           = 1-c;
  double s            = sin(a);

  grf_vec3_normalize(&axis);

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

  rotation->data[0]  = u2*cc + c;
  rotation->data[1]  = uv*cc + w*s;
  rotation->data[2]  = uw*cc - v*s;

  rotation->data[4]  = uv*cc - w*s;
  rotation->data[5]  = v2*cc + c;
  rotation->data[6]  = vw*cc + u*s;

  rotation->data[8]  = uw*cc + v*s;
  rotation->data[9]  = vw*cc - u*s;
  rotation->data[10] = w2*cc + c;
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfMat4
grf_mat4_eye(){
  return (GrfMat4){{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
}

void
grf_mat4_rotate_vec3(GrfVec3* vec, double angle, GrfVec3 axis){
  GrfMat4 rotation = grf_mat4_eye();
  grf_gl_create_rotation_matrix(&rotation,angle,axis);
  grf_mat4_mult_vec3(&rotation,vec);
}

void
grf_mat4_rotate_mat4(GrfMat4* mat, double angle, GrfVec3 axis){
  GrfMat4 rotation = grf_mat4_eye();
  grf_gl_create_rotation_matrix(&rotation,angle,axis);
  grf_mat4_mult_mat4(&rotation,mat,mat);
}

void
grf_mat4_mult_vec3(GrfMat4* mat, GrfVec3* vec){
  GrfVec3 res = *vec;
  u_int8_t i;
  GrfVec3 rows[3] = {{{mat->data[0],mat->data[4],mat->data[8]}},
                       {{mat->data[1],mat->data[5],mat->data[9]}},
                       {{mat->data[2],mat->data[6],mat->data[10]}}};
  for(i = 0; i < 3; i++)
    vec->data[i] = grf_vec3_dot(&res,&rows[i]) + mat->data[12+i];
}
void
grf_mat4_mult_mat4(GrfMat4* mat1, GrfMat4* mat2, GrfMat4* output){
  GrfVec4 rows1[4] = {{{mat1->data[0],mat1->data[4],mat1->data[8] ,mat1->data[12]}},
                        {{mat1->data[1],mat1->data[5],mat1->data[9] ,mat1->data[13]}},
                        {{mat1->data[2],mat1->data[6],mat1->data[10],mat1->data[14]}},
                        {{mat1->data[3],mat1->data[7],mat1->data[11],mat1->data[15]}}};
  GrfVec4 columns2[4] = {*(GrfVec4*)&mat2->data[0],
                           *(GrfVec4*)&mat2->data[4],
                           *(GrfVec4*)&mat2->data[8],
                           *(GrfVec4*)&mat2->data[12]};
  u_int8_t i, j, k = 0;
  for(j = 0; j < 4; j++)
    for(i = 0; i < 4; i++, k++)
      output->data[k] = grf_vec4_dot(&rows1[i],&columns2[j]);
}
