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
#define ORTHOGRAPHIC_PROJECTION_LEFT   0
#define ORTHOGRAPHIC_PROJECTION_RIGHT  1
#define ORTHOGRAPHIC_PROJECTION_BOTTOM 2
#define ORTHOGRAPHIC_PROJECTION_TOP    3
#define ORTHOGRAPHIC_PROJECTION_NEAR   4
#define ORTHOGRAPHIC_PROJECTION_FAR    5
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _GrfGLProjectionOrthographicPrivate{
  double params[6];
}GrfGLProjectionOrthographicPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfGLProjectionOrthographic,
                           grf_gl_projection_orthographic,
                           GRF_TYPE_GL_PROJECTION)
static void
grf_gl_projection_orthographic_init(GrfGLProjectionOrthographic *self){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(self);
  memset(priv->params,0,6*sizeof(double));
}

static void
grf_gl_projection_orthographic_class_init(GrfGLProjectionOrthographicClass *klass){

}
static void
grf_gl_projection_orthographic_update_left_right(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  GrfGLMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix->data[0]   =   2.0f /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT] - priv->params[ORTHOGRAPHIC_PROJECTION_LEFT]);
  matrix->data[12]  = - (priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT] + priv->params[ORTHOGRAPHIC_PROJECTION_LEFT]) /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT] - priv->params[ORTHOGRAPHIC_PROJECTION_LEFT]);
}
static void
grf_gl_projection_orthographic_update_bottom_top(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  GrfGLMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix->data[5]    =   2.0f /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_TOP] - priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM]);
  matrix->data[13]   = - (priv->params[ORTHOGRAPHIC_PROJECTION_TOP] + priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM]) /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_TOP] - priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM]);
}
static void
grf_gl_projection_orthographic_update_near_far(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  GrfGLMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  matrix->data[10]   = - 2.0f /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_FAR] - priv->params[ORTHOGRAPHIC_PROJECTION_NEAR]);
  matrix->data[14]   = - (priv->params[ORTHOGRAPHIC_PROJECTION_FAR] + priv->params[ORTHOGRAPHIC_PROJECTION_NEAR]) /
                   (priv->params[ORTHOGRAPHIC_PROJECTION_FAR] - priv->params[ORTHOGRAPHIC_PROJECTION_NEAR]);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfGLProjectionOrthographic*
grf_gl_projection_orthographic(double left, double right, double bottom, double top, double near, double far){
  GrfGLProjectionOrthographic* proj = g_object_new(GRF_TYPE_GL_PROJECTION_ORTHOGONAL, NULL);
  grf_gl_projection_orthographic_set_all(proj,left,right,bottom,top,near,far);
  return proj;
}
/* Accessors
 *=================================*/
double
grf_gl_projection_orthographic_get_left(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_LEFT];
}

double
grf_gl_projection_orthographic_get_right(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT];
}

double
grf_gl_projection_orthographic_get_top(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_TOP];
}

double
grf_gl_projection_orthographic_get_bottom(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM];
}

double
grf_gl_projection_orthographic_get_near(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_NEAR];
}

double
grf_gl_projection_orthographic_get_far(GrfGLProjectionOrthographic* proj){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  return priv->params[ORTHOGRAPHIC_PROJECTION_FAR];
}

/* Mutators
 *=================================*/
void
grf_gl_projection_orthographic_set_left(GrfGLProjectionOrthographic* proj,
                                      double left){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_LEFT] = left;
  grf_gl_projection_orthographic_update_left_right(proj);
}

void
grf_gl_projection_orthographic_set_right(GrfGLProjectionOrthographic* proj,
                                       double right){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT] = right;
  grf_gl_projection_orthographic_update_left_right(proj);
}

void
grf_gl_projection_orthographic_set_top(GrfGLProjectionOrthographic* proj,
                                     double top){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_TOP] = top;
  grf_gl_projection_orthographic_update_bottom_top(proj);
}

void
grf_gl_projection_orthographic_set_bottom(GrfGLProjectionOrthographic* proj,
                                        double bottom){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM] = bottom;
  grf_gl_projection_orthographic_update_bottom_top(proj);
}

void
grf_gl_projection_orthographic_set_near(GrfGLProjectionOrthographic* proj,
                                      double near){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_NEAR] = near;
  grf_gl_projection_orthographic_update_near_far(proj);

}

void
grf_gl_projection_orthographic_set_far(GrfGLProjectionOrthographic* proj,
                                     double far){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_FAR] = far;
  grf_gl_projection_orthographic_update_near_far(proj);
}

void
grf_gl_projection_orthographic_set_all(GrfGLProjectionOrthographic* proj,
                                     double left  , double right,
                                     double bottom, double top,
                                     double near  , double far){
  GrfGLProjectionOrthographicPrivate* priv = grf_gl_projection_orthographic_get_instance_private(proj);
  priv->params[ORTHOGRAPHIC_PROJECTION_LEFT]   = left;
  priv->params[ORTHOGRAPHIC_PROJECTION_RIGHT]  = right;
  priv->params[ORTHOGRAPHIC_PROJECTION_BOTTOM] = bottom;
  priv->params[ORTHOGRAPHIC_PROJECTION_TOP]    = top;
  priv->params[ORTHOGRAPHIC_PROJECTION_NEAR]   = near;
  priv->params[ORTHOGRAPHIC_PROJECTION_FAR]    = far;
  grf_gl_projection_orthographic_update(proj);
}

void
grf_gl_projection_orthographic_update(GrfGLProjectionOrthographic* proj){
  GrfGLMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  grf_gl_projection_fill(GRF_GL_PROJECTION(proj), 0);
  grf_gl_projection_orthographic_update_near_far(proj);
  grf_gl_projection_orthographic_update_bottom_top(proj);
  grf_gl_projection_orthographic_update_left_right(proj);
  matrix->data[15] = 1.0;
}
