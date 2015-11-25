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
#ifndef GRF_GL_PROJECTION_H
#define GRF_GL_PROJECTION_H
#include <grafeo/gl.h>
#include <glib-object.h>

#define GRF_TYPE_GL_PROJECTION grf_gl_projection_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfGLProjection, grf_gl_projection, GRF, GL_PROJECTION, GObject)
typedef struct _GrfGLProjectionClass{
  GObjectClass parent_class;
}GrfGLProjectionClass;
/**
 * @brief grf_gl_projection_get_matrix
 * @param projection
 * @return
 */
float*
grf_gl_projection_get_matrix(GrfGLProjection* projection);
/**
 * @brief grf_gl_projection_set_matrix
 * @param projection
 * @param matrix
 */
void
grf_gl_projection_set_matrix(GrfGLProjection* projection, float* matrix);
#endif
