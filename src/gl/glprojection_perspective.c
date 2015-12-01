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
#define PERSPECTIVE_FOV    0
#define PERSPECTIVE_ASPECT 1
#define PERSPECTIVE_NEAR   2
#define PERSPECTIVE_FAR    3
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _GrfGLProjectionPerspectivePrivate{
  double params[4];
}GrfGLProjectionPerspectivePrivate;
G_DEFINE_TYPE_WITH_PRIVATE(GrfGLProjectionPerspective, grf_gl_projection_perspective, GRF_TYPE_GL_PROJECTION)

static void
grf_gl_projection_perspective_init(GrfGLProjectionPerspective *self){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(self);
  memset(priv->params,0,4*sizeof(double));
}

static void
grf_gl_projection_perspective_class_init(GrfGLProjectionPerspectiveClass *klass){

}
static void
grf_gl_projection_perspective_update_fov_aspect(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  GrfMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  double fov    = priv->params[PERSPECTIVE_FOV];
  double aspect = priv->params[PERSPECTIVE_ASPECT];
  double tanHalfFovy = tan(fov/2.0f);
  matrix->data[0 ] = 1.0f/(aspect*tanHalfFovy);
  matrix->data[5 ] = 1.0f/(tanHalfFovy);
}
static void
grf_gl_projection_perspective_update_near_far(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  GrfMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  double near   = priv->params[PERSPECTIVE_NEAR];
  double far    = priv->params[PERSPECTIVE_FAR];
  matrix->data[10] = -(far+near)/(far-near);
  matrix->data[14] = -(2.0f*far*near)/(far-near);
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfGLProjectionPerspective*
grf_gl_projection_perspective(double fov, double aspect, double near, double far){
  GrfGLProjectionPerspective* proj = g_object_new(GRF_TYPE_GL_PROJECTION_PERSPECTIVE, NULL);
  grf_gl_projection_perspective_set_all(proj,fov,aspect,near,far);
  return proj;
}

/* Accessors
 *=================================*/
double
grf_gl_projection_perspective_get_fov(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  return priv->params[PERSPECTIVE_FOV];
}

double
grf_gl_projection_perspective_get_aspect(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  return priv->params[PERSPECTIVE_ASPECT];
}

double
grf_gl_projection_perspective_get_near(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  return priv->params[PERSPECTIVE_NEAR];
}

double
grf_gl_projection_perspective_get_far(GrfGLProjectionPerspective* proj){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  return priv->params[PERSPECTIVE_FAR];
}

/* Mutators
 *=================================*/
void
grf_gl_projection_perspective_set_fov(GrfGLProjectionPerspective* proj, double fov){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  priv->params[PERSPECTIVE_FOV] = fov;
  grf_gl_projection_perspective_update_fov_aspect(proj);
}

void
grf_gl_projection_perspective_set_aspect(GrfGLProjectionPerspective* proj, double aspect){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  priv->params[PERSPECTIVE_FOV] = aspect;
  grf_gl_projection_perspective_update_fov_aspect(proj);
}

void
grf_gl_projection_perspective_set_near(GrfGLProjectionPerspective* proj, double near){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  priv->params[PERSPECTIVE_FOV] = near;
  grf_gl_projection_perspective_update_near_far(proj);
}

void
grf_gl_projection_perspective_set_far(GrfGLProjectionPerspective* proj, double far){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  priv->params[PERSPECTIVE_FOV] = far;
  grf_gl_projection_perspective_update_near_far(proj);
}

void
grf_gl_projection_perspective_set_all(GrfGLProjectionPerspective* proj,
                                      double fov, double aspect,
                                      double near, double far){
  GrfGLProjectionPerspectivePrivate* priv = grf_gl_projection_perspective_get_instance_private(proj);
  priv->params[PERSPECTIVE_FOV]    = fov;
  priv->params[PERSPECTIVE_ASPECT] = aspect;
  priv->params[PERSPECTIVE_NEAR]   = near;
  priv->params[PERSPECTIVE_FAR]    = far;
  grf_gl_projection_perspective_update(proj);
}

void
grf_gl_projection_perspective_update(GrfGLProjectionPerspective* proj){
  grf_gl_projection_fill(GRF_GL_PROJECTION(proj),0);
  GrfMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj));
  grf_gl_projection_perspective_update_fov_aspect(proj);
  grf_gl_projection_perspective_update_near_far(proj);
  matrix->data[11] = -1.0f;
}
