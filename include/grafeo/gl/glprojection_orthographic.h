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
#ifndef GRF_GL_PROJECTION_ORTHOGONAL_H
#define GRF_GL_PROJECTION_ORTHOGONAL_H
#include<grafeo/gl.h>
#include<glib-object.h>
#include<string.h>
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define GRF_TYPE_GL_PROJECTION_ORTHOGONAL grf_gl_projection_orthographic_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLProjectionOrthographic, grf_gl_projection_orthographic,
                         GRF, GL_PROJECTION_ORTHOGONAL, GrfGLProjection)
typedef struct _GrfGLProjectionOrthographicClass{
  GrfGLProjectionClass parent_class;
}GrfGLProjectionOrthographicClass;

/*===========================================================================
 * MEMBERS
 *===========================================================================*/
GrfGLProjectionOrthographic*
grf_gl_projection_orthographic(double left  , double right,
                             double bottom, double top,
                             double near  , double far);

/* Accessors
 *=================================*/
/**
 * @brief grf_gl_projection_orthographic_get_left
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_left(GrfGLProjectionOrthographic* proj);
/**
 * @brief grf_gl_projection_orthographic_get_right
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_right(GrfGLProjectionOrthographic* proj);
/**
 * @brief grf_gl_projection_orthographic_get_top
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_top(GrfGLProjectionOrthographic* proj);
/**
 * @brief grf_gl_projection_orthographic_get_bottom
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_bottom(GrfGLProjectionOrthographic* proj);
/**
 * @brief grf_gl_projection_orthographic_get_near
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_near(GrfGLProjectionOrthographic* proj);
/**
 * @brief grf_gl_projection_orthographic_get_far
 * @param proj
 * @return
 */
double
grf_gl_projection_orthographic_get_far(GrfGLProjectionOrthographic* proj);

/* Mutators
 *=================================*/
/**
 * @brief grf_gl_projection_orthographic_set_left
 * @param proj
 * @param left
 */
void
grf_gl_projection_orthographic_set_left(GrfGLProjectionOrthographic* proj,
                                      double left);
/**
 * @brief grf_gl_projection_orthographic_set_right
 * @param proj
 * @param right
 */
void
grf_gl_projection_orthographic_set_right(GrfGLProjectionOrthographic* proj,
                                       double right);
/**
 * @brief grf_gl_projection_orthographic_set_top
 * @param proj
 * @param top
 */
void
grf_gl_projection_orthographic_set_top(GrfGLProjectionOrthographic* proj,
                                     double top);
/**
 * @brief grf_gl_projection_orthographic_set_bottom
 * @param proj
 * @param bottom
 */
void
grf_gl_projection_orthographic_set_bottom(GrfGLProjectionOrthographic* proj,
                                        double bottom);
/**
 * @brief grf_gl_projection_orthographic_set_near
 * @param proj
 * @param near
 */
void
grf_gl_projection_orthographic_set_near(GrfGLProjectionOrthographic* proj,
                                      double near);
/**
 * @brief grf_gl_projection_orthographic_set_far
 * @param proj
 * @param far
 */
void
grf_gl_projection_orthographic_set_far(GrfGLProjectionOrthographic* proj,
                                     double far);
/**
 * @brief grf_gl_projection_orthographic_set_all
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
void
grf_gl_projection_orthographic_set_all(GrfGLProjectionOrthographic* proj,
                                     double left  , double right,
                                     double bottom, double top,
                                     double near  , double far);
/**
 * @brief grf_gl_projection_orthographic_update
 * @param proj
 */
void
grf_gl_projection_orthographic_update(GrfGLProjectionOrthographic* proj);

#endif
