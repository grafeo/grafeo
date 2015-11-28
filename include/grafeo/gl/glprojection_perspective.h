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
#ifndef GRF_GL_PROJECTION_PERSPECTIVE_H
#define GRF_GL_PROJECTION_PERSPECTIVE_H
#include <grafeo/gl.h>
#include <glib-object.h>
#include <string.h>
#include <math.h>
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define GRF_TYPE_GL_PROJECTION_PERSPECTIVE grf_gl_projection_perspective_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLProjectionPerspective, grf_gl_projection_perspective, GRF, GL_PROJECTION_PERSPECTIVE, GrfGLProjection)
typedef struct _GrfGLProjectionPerspectiveClass{
  GrfGLProjectionClass parent_class;
}GrfGLProjectionPerspectiveClass;
/*===========================================================================
 * MEMBERS
 *===========================================================================*/
GrfGLProjectionPerspective*
grf_gl_projection_perspective(double fov, double aspect, double near, double far);

/* Accessors
 *=================================*/
/**
 * @brief grf_gl_projection_perspective_get_fov
 * @param proj
 * @return
 */
double
grf_gl_projection_perspective_get_fov(GrfGLProjectionPerspective* proj);
/**
 * @brief grf_gl_projection_perspective_get_aspect
 * @param proj
 * @return
 */
double
grf_gl_projection_perspective_get_aspect(GrfGLProjectionPerspective* proj);
/**
 * @brief grf_gl_projection_perspective_get_near
 * @param proj
 * @return
 */
double
grf_gl_projection_perspective_get_near(GrfGLProjectionPerspective* proj);
/**
 * @brief grf_gl_projection_perspective_get_far
 * @param proj
 * @return
 */
double
grf_gl_projection_perspective_get_far(GrfGLProjectionPerspective* proj);

/* Mutators
 *=================================*/

/**
 * @brief grf_gl_projection_perspective_set_fov
 * @param proj
 * @param fov
 */
void
grf_gl_projection_perspective_set_fov(GrfGLProjectionPerspective* proj, double fov);
/**
 * @brief grf_gl_projection_perspective_set_aspect
 * @param proj
 * @param aspect
 */
void
grf_gl_projection_perspective_set_aspect(GrfGLProjectionPerspective* proj, double aspect);
/**
 * @brief grf_gl_projection_perspective_set_near
 * @param proj
 * @param near
 */
void
grf_gl_projection_perspective_set_near(GrfGLProjectionPerspective* proj, double near);
/**
 * @brief grf_gl_projection_perspective_set_far
 * @param proj
 * @param far
 */
void
grf_gl_projection_perspective_set_far(GrfGLProjectionPerspective* proj, double far);
/**
 * @brief grf_gl_projection_perspective_set_all
 * @param proj
 * @param fov
 * @param aspect
 * @param near
 * @param far
 */
void
grf_gl_projection_perspective_set_all(GrfGLProjectionPerspective* proj,
                                      double fov, double aspect,
                                      double near, double far);
/**
 * @brief grf_gl_projection_perspective_update
 * @param proj
 */
void
grf_gl_projection_perspective_update(GrfGLProjectionPerspective* proj);
#endif
