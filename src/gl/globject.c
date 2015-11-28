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
/*=================================
 * PRIVATE API
 *=================================*/
typedef struct _GrfGLObjectPrivate{
  GrfGLMat4 matrix_model;
  GrfGLVec3 position;
  GrfGLVec3 scale;
  GrfGLVec3 rotation;
}GrfGLObjectPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfGLObject, grf_gl_object, G_TYPE_OBJECT)

static void
grf_gl_object_init(GrfGLObject *self){

}

static void
grf_gl_object_class_init(GrfGLObjectClass *klass){

}

/*=================================
 * PUBLIC API
 *=================================*/
GrfGLObject*
grf_gl_object_new(){
  return g_object_new(GRF_TYPE_GL_OBJECT, NULL);
}

GrfGLObject*
grf_gl_object_new_with_name(char* name){
  GrfGLObject* object = grf_gl_object_new();
  return object;
}

GrfGLMat4
grf_gl_object_get_model(GrfGLObject* object){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  return priv->matrix_model;
}

void
grf_gl_object_set_model(GrfGLObject* object, GrfGLMat4 model){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  priv->matrix_model = model;
}

GrfGLVec3
grf_gl_object_get_position(GrfGLObject* object){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  return priv->position;
}

void
grf_gl_object_set_position(GrfGLObject* object, GrfGLVec3 position){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  priv->position = position;
}

GrfGLVec3
grf_gl_object_get_scale(GrfGLObject* object){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  return priv->scale;
}

void
grf_gl_object_set_scale(GrfGLObject* object, GrfGLVec3 scale){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  priv->scale = scale;
}

void
grf_gl_object_translate(GrfGLObject* object, GrfGLVec3 amount){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  grf_gl_vec3_add(&priv->position,&amount);
  memcpy(&priv->matrix_model[12],priv->position.data, sizeof(GrfGLVec3));
}

void
grf_gl_object_scale(GrfGLObject* object, GrfGLVec3 amount){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  grf_gl_vec3_multiply(&priv->scale,&amount);
  priv->matrix_model[(0<<2)+0] = priv->scale[0];
  priv->matrix_model[(1<<2)+1] = priv->scale[1];
  priv->matrix_model[(2<<2)+2] = priv->scale[2];
}

void
grf_gl_object_rotate(GrfGLObject* object, double angle, GrfGLVec3 axis){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  float a            = grf_to_rad(angle);
  float c            = cos(a);
  float cc           = 1-c;
  float s            = sin(a);

  uint32_t  size     = 3;
  uint32_t  sizes[2] = {4,4};
  float     ve[3]    = {x,y,z};
  GrfArray* axisn    = grf_array_from_data(ve,1,&size,GRF_FLOAT);
  GrfArray* axis     = grf_array_normalize(axisn);

  float u  = axis->data_float[0];
  float v  = axis->data_float[1];
  float w  = axis->data_float[2];
  float u2 = u*u;
  float v2 = v*v;
  float w2 = w*w;
  float uv = u*v;
  float uw = u*w;
  float vw = v*w;

  GrfArray* rotation = grf_array_zeros(2,sizes,GRF_FLOAT);

  rotation->data_float[0]  = u2+(1-u2)*c;
  rotation->data_float[1]  = uv*cc - w*s;
  rotation->data_float[2]  = uw*cc + v*s;

  rotation->data_float[4]  = uv*cc + w*s;
  rotation->data_float[5]  = v2+(1-v2)*c;
  rotation->data_float[6]  = vw*cc - u*s;

  rotation->data_float[8]  = uw*cc - v*s;
  rotation->data_float[9]  = vw*cc + u*s;
  rotation->data_float[10] = w2+(1-w2)*c;

  rotation->data_float[15] = 1;

  if(!array) return rotation;
  GrfArray* result = grf_array_dot(array,rotation);
  grf_array_free(rotation);
  return result;
}

void
grf_gl_object_roll(GrfGLObject* object, double amount){

}

void
grf_gl_object_pitch(GrfGLObject* object, double amount){

}

void
grf_gl_object_yaw(GrfGLObject* object, double amount){

}
