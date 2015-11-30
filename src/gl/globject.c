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
  memcpy(&priv->matrix_model.data[12],priv->position.data, sizeof(GrfGLVec3));
}

void
grf_gl_object_scale(GrfGLObject* object, GrfGLVec3 amount){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  grf_gl_vec3_multiply(&priv->scale,&amount);
  priv->matrix_model.data[(0<<2)+0] = priv->scale.data[0];
  priv->matrix_model.data[(1<<2)+1] = priv->scale.data[1];
  priv->matrix_model.data[(2<<2)+2] = priv->scale.data[2];
}

void
grf_gl_object_rotate(GrfGLObject* object, double angle, GrfGLVec3 axis){
  GrfGLObjectPrivate* priv = grf_gl_object_get_instance_private(object);
  grf_gl_mat4_rotate_mat4(&priv->matrix_model,angle,axis);
}

void
grf_gl_object_roll(GrfGLObject* object, double amount){
  grf_gl_object_rotate(object,amount,(GrfGLVec3){{0,0,1}});
}

void
grf_gl_object_pitch(GrfGLObject* object, double amount){
  grf_gl_object_rotate(object,amount,(GrfGLVec3){{1,0,0}});
}

void
grf_gl_object_yaw(GrfGLObject* object, double amount){
  grf_gl_object_rotate(object,amount,(GrfGLVec3){{0,1,0}});
}
