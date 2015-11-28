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
/**
 * @brief Private structure (fields)
 *
 * You must initialize everything in grf_gl_camera_init
 */
typedef struct _GrfGLCameraPrivate{
  GrfGLMat4 matrix_view;        /**< view matrix
                                     (camera extrinsic parameters) */
  GrfGLProjection* projection;  /**< projection matrix
                                     (camera intrinsic parameters) */

  GrfGLVec3  position;          /**< Location of the camera */

  GrfGLVec3  right;             /**< Cemras's right direction */
  GrfGLVec3  forward;           /**< Camera's front direction */
  GrfGLVec3  up;                /**< Camera's up direction */

  double     pitch;             /**< Camera's pitch direction */
  double     yaw;               /**< Camera's yaw direction */
  double     roll;              /**< Camera's roll direction */

  char*      name;              /**< We can have several cameras */
}GrfGLCameraPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(GrfGLCamera, grf_gl_camera, G_TYPE_OBJECT)

/**
 * @brief Destructor-Dispose: Clear all referenced objects in
 *        private structure
 * @param the GrfGLCamera object
 */
static void
grf_gl_camera_dispose(GObject* object){
  // Get members
  GrfGLCamera* camera = GRF_GL_CAMERA(object);
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);

  // Clear members
  g_clear_object(&priv->projection);

  // Continue process with superclasses
  G_OBJECT_CLASS (grf_gl_camera_parent_class)->dispose (object);
}

/**
 * @brief Destructor-Finalize: Clear referenced memory (not objects)
 *        in private structure
 * @param the GrfGLCamera object
 */
static void
grf_gl_camera_finalize(GObject* object){
  // Get members
  GrfGLCamera* camera = GRF_GL_CAMERA(object);
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);

  // Clear members
  g_free(priv->name);

  // Continue process with superclasses
  G_OBJECT_CLASS (grf_gl_camera_parent_class)->finalize (object);
}

/**
 * @brief Constructor: initialize members
 */
static void
grf_gl_camera_init(GrfGLCamera *self){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(self);
  priv->projection         = NULL;
  memset(&priv->position,0,sizeof(GrfGLVec3));
  memset(&priv->matrix_view,0,sizeof(GrfGLMat4));
  priv->name              = NULL;
}

/**
 * Clear all referenced objects in private structure
 */
static void
grf_gl_camera_class_init(GrfGLCameraClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = grf_gl_camera_dispose;
  object_class->finalize = grf_gl_camera_finalize;
}


/*=================================
 * PUBLIC API
 *=================================*/
GrfGLCamera*
grf_gl_camera_new(){
  return g_object_new(GRF_TYPE_GL_CAMERA, NULL);
}

char*
grf_gl_camera_get_name(GrfGLCamera* camera){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  gchar* name = g_strdup(priv->name);
  return name;
}

void
grf_gl_camera_set_name(GrfGLCamera *camera, char *name){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  if(priv->name) g_free(priv->name);
  priv->name = g_strdup(name);
}

// Movement

void
grf_gl_camera_move_right(GrfGLCamera *camera, double units){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  // priv->position += priv->right * units;
  GrfGLVec3 right_scaled = priv->right;
  grf_gl_vec3_multiply_scalar(&right_scaled, units);    // priv->right * units;
  grf_gl_vec3_add(&priv->position, &right_scaled);
}

void
grf_gl_camera_move_left(GrfGLCamera *camera, double units){
  grf_gl_camera_move_right(camera, -units);
}

void
grf_gl_camera_move_forward(GrfGLCamera *camera, double units){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);

  // priv->position += priv->forward * units;
  GrfGLVec3 forward_scaled = priv->forward;
  grf_gl_vec3_multiply_scalar(&forward_scaled, units);    // priv->forward * units;
  grf_gl_vec3_add(&priv->position, &forward_scaled);
}

void
grf_gl_camera_move_behind(GrfGLCamera *camera, double units){
  grf_gl_camera_move_forward(camera, -units);
}

void
grf_gl_camera_move_up(GrfGLCamera *camera, double units){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);

  // priv->position += priv->forward * units;
  GrfGLVec3 up_scaled = priv->up;
  grf_gl_vec3_multiply_scalar(&up_scaled, units);    // priv->forward * units;
  grf_gl_vec3_add(&priv->position, &up_scaled);
}

void
grf_gl_camera_move_down(GrfGLCamera *camera, double units){
  grf_gl_camera_move_up(camera, -units);
}

// Rotation

void
grf_gl_camera_pitch(GrfGLCamera *camera, double angle){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  priv->pitch += angle;
}

void
grf_gl_camera_yaw(GrfGLCamera *camera, double angle){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  priv->yaw += angle;
}

void
grf_gl_camera_roll(GrfGLCamera *camera, double angle){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  priv->roll += angle;
}
GrfGLVec3
grf_gl_camera_get_position(GrfGLCamera *camera){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  return priv->position;
}
GrfGLProjection*
grf_gl_camera_get_projection(GrfGLCamera* camera){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  return priv->projection;
}

GrfGLMat4
grf_gl_camera_get_view(GrfGLCamera* camera){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  return priv->matrix_view;
}

void
grf_gl_camera_set_projection(GrfGLCamera* camera, GrfGLProjection* projection){
  GrfGLCameraPrivate* priv = grf_gl_camera_get_instance_private(camera);
  g_set_object(&priv->projection,projection);
}
