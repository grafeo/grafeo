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
#ifndef GRF_GL_CAMERA_H
#define GRF_GL_CAMERA_H
#include <glib-object.h>
#include <grafeo/gl.h>

/*=================================
 * CLASS DECLARATION
 *=================================*/
#define GRF_TYPE_GL_CAMERA grf_gl_camera_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLCamera, grf_gl_camera, GRF, GL_CAMERA, GObject)
typedef struct _GrfGLCameraClass{
  GObjectClass parent_class;
}GrfGLCameraClass;

/*=================================
 * METHODS
 *=================================*/
/**
 * @brief Creates a new camera with empty metadata
 * @return a new camera
 */
GrfGLCamera*
grf_gl_camera_new();
/**
 * @brief Gets a copy of the name.
 *
 * You must free the copy (or use g_autofree)
 *
 * @param camera
 * @return
 */
char*
grf_gl_camera_get_name(GrfGLCamera* camera);
/**
 * @brief grf_gl_camera_set_name
 * @param camera
 * @param name
 */
void
grf_gl_camera_set_name(GrfGLCamera* camera, char* name);
/**
 * @brief grf_gl_camera_move_left
 * @param camera
 */
void
grf_gl_camera_move_left(GrfGLCamera* camera, double units);
/**
 * @brief grf_gl_camera_move_right
 * @param camera
 */
void
grf_gl_camera_move_right(GrfGLCamera* camera, double units);
/**
 * @brief grf_gl_camera_move_forward
 * @param camera
 */
void
grf_gl_camera_move_forward(GrfGLCamera* camera, double units);
/**
 * @brief grf_gl_camera_move_behind
 * @param camera
 */
void
grf_gl_camera_move_behind(GrfGLCamera* camera, double units);
/**
 * @brief grf_gl_camera_move_up
 * @param camera
 * @param units
 */
void
grf_gl_camera_move_up(GrfGLCamera* camera, double units);
/**
 * @brief grf_gl_camera_move_down
 * @param camera
 * @param units
 */
void
grf_gl_camera_move_down(GrfGLCamera* camera, double units);


/**
 * @brief grf_gl_camera_pitch
 * @param camera
 */
void
grf_gl_camera_pitch(GrfGLCamera* camera, double angle);
/**
 * @brief grf_gl_camera_yaw
 * @param camera
 */
void
grf_gl_camera_yaw(GrfGLCamera* camera, double angle);
/**
 * @brief grf_gl_camera_roll
 * @param camera
 */
void
grf_gl_camera_roll(GrfGLCamera* camera, double angle);
/**
 * @brief grf_gl_camera_rotate
 * @param camera
 */
void
grf_gl_camera_rotate(GrfGLCamera* camera, double angle, double x, double y, double z);
/**
 * @brief grf_gl_camera_set_position
 * @param camera
 */
void
grf_gl_camera_set_position(GrfGLCamera* camera, GrfGLVec3* position);
/**
 * @brief grf_gl_camera_lookat
 * @param camera
 */
void
grf_gl_camera_lookat(GrfGLCamera* camera, GrfGLVec3* position);
/**
 * @brief grf_gl_camera_get_view_matrix
 * @param camera
 * @return
 */
GrfGLMat4
grf_gl_camera_get_view(GrfGLCamera* camera);
/**
 * @brief grf_gl_camera_get_position
 * @return
 */
GrfGLVec3
grf_gl_camera_get_position(GrfGLCamera* camera);
/**
 * @brief return the projection
 * @param camera
 * @return
 */
GrfGLProjection*
grf_gl_camera_get_projection(GrfGLCamera* camera);
/**
 * @brief grf_gl_camera_set_projection
 * @param camera
 * @param projection
 */
void
grf_gl_camera_set_projection(GrfGLCamera* camera, GrfGLProjection* projection);
/**
 * @brief grf_gl_camera_reset_view
 * @param position
 * @param forward
 * @param up
 */
void
grf_gl_camera_reset_view(GrfGLCamera* camera, GrfGLVec3 position, GrfGLVec3 forward, GrfGLVec3 up);
#endif
