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
#define ORTHOGONAL_PROJECTION_LEFT   0
#define ORTHOGONAL_PROJECTION_RIGHT  1
#define ORTHOGONAL_PROJECTION_BOTTOM 2
#define ORTHOGONAL_PROJECTION_TOP    3
#define ORTHOGONAL_PROJECTION_NEAR   4
#define ORTHOGONAL_PROJECTION_FAR    5
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _GrfGLProjectionOrthogonalPrivate{
  double params[6];
}GrfGLProjectionOrthogonalPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfGLProjectionOrthogonal,
                           grf_gl_projection_orthogonal,
                           GRF_TYPE_GL_PROJECTION)
static void
grf_gl_projection_orthogonal_init(GrfGLProjectionOrthogonal *self){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  memset(priv->params,0,6*sizeof(double));
}

static void
grf_gl_projection_orthogonal_class_init(GrfGLProjectionOrthogonalClass *klass){

}

grf_gl_projection_orthogonal_update_left_right(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  double* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix[0]    =   2.0f /
                   (priv->params[ORTHOGONAL_PROJECTION_RIGHT] - priv->params[ORTHOGONAL_PROJECTION_LEFT]);
  matrix[12]   = - (right + left) /
                   (priv->params[ORTHOGONAL_PROJECTION_RIGHT] - priv->params[ORTHOGONAL_PROJECTION_LEFT]);
}

grf_gl_projection_orthogonal_update_bottom_top(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  double* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix[5]    =   2.0f /
                   (priv->params[ORTHOGONAL_PROJECTION_TOP] - priv->params[ORTHOGONAL_PROJECTION_BOTTOM]);
  matrix[13]   = - (top + bottom) /
                   (priv->params[ORTHOGONAL_PROJECTION_TOP] - priv->params[ORTHOGONAL_PROJECTION_BOTTOM]);
}

grf_gl_projection_orthogonal_update_near_far(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  double* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix[10]   = - 2.0f /
                   (priv->params[ORTHOGONAL_PROJECTION_FAR] - priv->params[ORTHOGONAL_PROJECTION_NEAR]);
  matrix[14]   = - (priv->params[ORTHOGONAL_PROJECTION_FAR] + priv->params[ORTHOGONAL_PROJECTION_NEAR]) /
                   (priv->params[ORTHOGONAL_PROJECTION_FAR] - priv->params[ORTHOGONAL_PROJECTION_NEAR]);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfGLProjectionOrthogonal*
grf_gl_projection_orthogonal(double left, double right, double bottom, double top, double near, double far){
  GrfGLProjectionOrthogonal* proj = g_object_new(GRF_TYPE_GL_PROJECTION_ORTHOGONAL, NULL);
  grf_gl_projection_orthogonal_set_all(proj,left,right,bottom,top,near,far);
  return proj;
}
/* Accessors
 *=================================*/
double
grf_gl_projection_orthogonal_get_left(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_LEFT];
}

double
grf_gl_projection_orthogonal_get_right(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_RIGHT];
}

double
grf_gl_projection_orthogonal_get_top(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_TOP];
}

double
grf_gl_projection_orthogonal_get_bottom(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_BOTTOM];
}

double
grf_gl_projection_orthogonal_get_near(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_NEAR];
}

double
grf_gl_projection_orthogonal_get_far(GrfGLProjectionOrthogonal* proj){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  return priv->params[ORTHOGONAL_PROJECTION_FAR];
}

/* Mutators
 *=================================*/
void
grf_gl_projection_orthogonal_set_left(GrfGLProjectionOrthogonal* proj,
                                      double left){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_LEFT] = left;
  grf_gl_projection_orthogonal_update_left_right(proj);
}

void
grf_gl_projection_orthogonal_set_right(GrfGLProjectionOrthogonal* proj,
                                       double right){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_RIGHT] = right;
  grf_gl_projection_orthogonal_update_left_right(proj);
}

void
grf_gl_projection_orthogonal_set_top(GrfGLProjectionOrthogonal* proj,
                                     double top){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_TOP] = top;
  grf_gl_projection_orthogonal_update_bottom_top(proj);
}

void
grf_gl_projection_orthogonal_set_bottom(GrfGLProjectionOrthogonal* proj,
                                        double bottom){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_BOTTOM] = bottom;
  grf_gl_projection_orthogonal_update_bottom_top(proj);
}

void
grf_gl_projection_orthogonal_set_near(GrfGLProjectionOrthogonal* proj,
                                      double near){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_NEAR] = near;
  grf_gl_projection_orthogonal_update_near_far(proj);

}

void
grf_gl_projection_orthogonal_set_far(GrfGLProjectionOrthogonal* proj,
                                     double far){
  GrfGLProjectionOrthogonalPrivate* priv = grf_gl_projection_orthogonal_get_instance_private(self);
  priv->params[ORTHOGONAL_PROJECTION_FAR] = far;
  grf_gl_projection_orthogonal_update_near_far(proj);
}

void
grf_gl_projection_orthogonal_update(GrfGLProjectionOrthogonal* proj){
  double* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  memset(matrix,0,sizeof(matrix)*16);
  grf_gl_projection_orthogonal_update_near_far(proj);
  grf_gl_projection_orthogonal_update_bottom_top(proj);
  grf_gl_projection_orthogonal_update_left_right(proj);
}
