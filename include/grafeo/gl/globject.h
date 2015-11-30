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
#ifndef GRF_GL_OBJECT_H
#define GRF_GL_OBJECT_H
#include <glib-object.h>
#include <grafeo/gl.h>
#define GRF_TYPE_GL_OBJECT grf_gl_object_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLObject, grf_gl_object, GRF, GL_OBJECT, GObject)
typedef struct _GrfGLObjectClass{
  GObjectClass parent_class;
}GrfGLObjectClass;
/**
 * @brief grf_gl_object_new
 * @return
 */
GrfGLObject*
grf_gl_object_new();
/**
 * @brief grf_gl_object_new_with_name
 * @param name
 * @return
 */
GrfGLObject*
grf_gl_object_new_with_name(char* name);
/**
 * @brief grf_gl_object_get_model
 * @param object
 * @return
 */
GrfGLMat4
grf_gl_object_get_model(GrfGLObject* object);
/**
 * @brief grf_gl_object_set_model
 * @param object
 * @param model
 */
void
grf_gl_object_set_model(GrfGLObject* object, GrfGLMat4 model);
/**
 * @brief grf_gl_object_get_position
 * @param object
 * @return
 */
GrfGLVec3
grf_gl_object_get_position(GrfGLObject* object);
/**
 * @brief grf_gl_object_set_position
 * @param object
 * @param position
 */
void
grf_gl_object_set_position(GrfGLObject* object, GrfGLVec3 position);
/**
 * @brief grf_gl_object_get_scale
 * @param object
 * @return
 */
GrfGLVec3
grf_gl_object_get_scale(GrfGLObject* object);
/**
 * @brief grf_gl_object_set_scale
 * @param object
 * @param scale
 */
void
grf_gl_object_set_scale(GrfGLObject* object, GrfGLVec3 scale);
/**
 * @brief grf_gl_object_translate
 * @param object
 * @param amount
 */
void
grf_gl_object_translate(GrfGLObject* object, GrfGLVec3 amount);
/**
 * @brief grf_gl_object_scale
 * @param object
 * @param amount
 */
void
grf_gl_object_scale(GrfGLObject* object, GrfGLVec3 amount);
/**
 * @brief grf_gl_object_rotate
 * @param object
 * @param angle
 * @param axis
 */
void
grf_gl_object_rotate(GrfGLObject* object, double angle, GrfGLVec3 axis);
/**
 * @brief grf_gl_object_roll
 * @param object
 * @param amount
 */
void
grf_gl_object_roll(GrfGLObject* object, double amount);
/**
 * @brief grf_gl_object_pitch
 * @param object
 * @param amount
 */
void
grf_gl_object_pitch(GrfGLObject* object, double amount);
/**
 * @brief grf_gl_object_yaw
 * @param object
 * @param amount
 */
void
grf_gl_object_yaw(GrfGLObject* object, double amount);



#endif
