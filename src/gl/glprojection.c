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
#define MATRIX_SIZE sizeof(GrfMat4)
typedef struct _GrfGLProjectionPrivate{
  GrfMat4 matrix;
}GrfGLProjectionPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GrfGLProjection, grf_gl_projection,G_TYPE_INITIALLY_UNOWNED)
static void
grf_gl_projection_init(GrfGLProjection *self){
  grf_gl_projection_fill(self, 0);
}
static void
grf_gl_projection_class_init(GrfGLProjectionClass *klass){

}
/*=================================
 * PUBLIC API
 *=================================*/
GrfMat4*
grf_gl_projection_get_matrix(GrfGLProjection* projection){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  GrfMat4* matrix = g_malloc(MATRIX_SIZE);
  memcpy(matrix,&priv->matrix,MATRIX_SIZE);
  return matrix;
}
GrfMat4*
grf_gl_projection_get_matrix_ptr(GrfGLProjection* projection){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  return &priv->matrix;
}

void
grf_gl_projection_set_matrix(GrfGLProjection* projection, GrfMat4* matrix){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(projection);
  memcpy(priv->matrix.data,matrix->data,MATRIX_SIZE);
}

void
grf_gl_projection_fill(GrfGLProjection* proj, int value){
  GrfGLProjectionPrivate* priv = grf_gl_projection_get_instance_private(proj);
  memset(priv->matrix.data,value,MATRIX_SIZE);
}
