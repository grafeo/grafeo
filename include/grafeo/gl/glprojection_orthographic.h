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
/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define GRF_TYPE_GL_PROJECTION_ORTHOGONAL grf_gl_projection_orthogonal_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLProjectionOrthogonal, grf_gl_projection_orthogonal,
                         GRF, GL_PROJECTION_ORTHOGONAL, GrfGLProjection)
typedef struct _GrfGLProjectionOrthogonalClass{
  GrfGLProjectionClass parent_class;
}GrfGLProjectionOrthogonalClass;

/*===========================================================================
 * MEMBERS
 *===========================================================================*/
GrfGLProjectionOrthogonal*
grf_gl_projection_orthogonal(double left  , double right,
                             double bottom, double top,
                             double near  , double far);

/* Accessors
 *=================================*/
/**
 * @brief grf_gl_projection_orthogonal_get_left
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_left(GrfGLProjectionOrthogonal* proj);
/**
 * @brief grf_gl_projection_orthogonal_get_right
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_right(GrfGLProjectionOrthogonal* proj);
/**
 * @brief grf_gl_projection_orthogonal_get_top
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_top(GrfGLProjectionOrthogonal* proj);
/**
 * @brief grf_gl_projection_orthogonal_get_bottom
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_bottom(GrfGLProjectionOrthogonal* proj);
/**
 * @brief grf_gl_projection_orthogonal_get_near
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_near(GrfGLProjectionOrthogonal* proj);
/**
 * @brief grf_gl_projection_orthogonal_get_far
 * @param proj
 * @return
 */
double
grf_gl_projection_orthogonal_get_far(GrfGLProjectionOrthogonal* proj);

/* Mutators
 *=================================*/
/**
 * @brief grf_gl_projection_orthogonal_set_left
 * @param proj
 * @param left
 */
void
grf_gl_projection_orthogonal_set_left(GrfGLProjectionOrthogonal* proj,
                                      double left);
/**
 * @brief grf_gl_projection_orthogonal_set_right
 * @param proj
 * @param right
 */
void
grf_gl_projection_orthogonal_set_right(GrfGLProjectionOrthogonal* proj,
                                       double right);
/**
 * @brief grf_gl_projection_orthogonal_set_top
 * @param proj
 * @param top
 */
void
grf_gl_projection_orthogonal_set_top(GrfGLProjectionOrthogonal* proj,
                                     double top);
/**
 * @brief grf_gl_projection_orthogonal_set_bottom
 * @param proj
 * @param bottom
 */
void
grf_gl_projection_orthogonal_set_bottom(GrfGLProjectionOrthogonal* proj,
                                        double bottom);
/**
 * @brief grf_gl_projection_orthogonal_set_near
 * @param proj
 * @param near
 */
void
grf_gl_projection_orthogonal_set_near(GrfGLProjectionOrthogonal* proj,
                                      double near);
/**
 * @brief grf_gl_projection_orthogonal_set_far
 * @param proj
 * @param far
 */
void
grf_gl_projection_orthogonal_set_far(GrfGLProjectionOrthogonal* proj,
                                     double far);
/**
 * @brief grf_gl_projection_orthogonal_set_all
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
void
grf_gl_projection_orthogonal_set_all(GrfGLProjectionOrthogonal* proj,
                                     double left  , double right,
                                     double bottom, double top,
                                     double near  , double far);
/**
 * @brief grf_gl_projection_orthogonal_update
 * @param proj
 */
void
grf_gl_projection_orthogonal_update(GrfGLProjectionOrthogonal* proj);

#endif
