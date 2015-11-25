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
#include <string.h>
/*=================================
 * PRIVATE API
 *=================================*/
#define MATRIX_SIZE 16*sizeof(float)
typedef struct _GrfGLProjectionPrivate{
  float matrix[16];
}GrfGLProjectionPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GrfGLProjection, grf_gl_projection,G_TYPE_OBJECT)


/*=================================
 * PUBLIC API
 *=================================*/
float*
grf_gl_projection_get_matrix(GrfGLProjection* projection){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  g_autofree float* matrix = g_slice_copy(MATRIX_SIZE,priv->matrix);
  return g_steal_pointer(matrix);
}
float*
grf_gl_projection_get_matrix_ptr(GrfGLProjection* projection){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  return priv->matrix;
}

void
grf_gl_projection_set_matrix(GrfGLProjection* projection, float* matrix){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  memcpy(priv->matrix,matrix,MATRIX_SIZE);
}
