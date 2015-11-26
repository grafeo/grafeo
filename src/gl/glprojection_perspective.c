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

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfGLProjectionPerspective*
grf_gl_projection_perspective(double fov, double aspect, double near, double far){

}

/* Accessors
 *=================================*/
double
grf_gl_projection_perspective_get_fov(GrfGLProjectionPerspective* proj){

}

double
grf_gl_projection_perspective_get_aspect(GrfGLProjectionPerspective* proj){

}

double
grf_gl_projection_perspective_get_near(GrfGLProjectionPerspective* proj){

}

double
grf_gl_projection_perspective_get_far(GrfGLProjectionPerspective* proj){

}

/* Mutators
 *=================================*/
void
grf_gl_projection_perspective_set_fov(GrfGLProjectionPerspective* proj, double fov){

}

void
grf_gl_projection_perspective_set_aspect(GrfGLProjectionPerspective* proj, double aspect){

}

void
grf_gl_projection_perspective_set_near(GrfGLProjectionPerspective* proj, double near){

}

void
grf_gl_projection_perspective_set_far(GrfGLProjectionPerspective* proj, double far){

}

void
grf_gl_projection_perspective_set_all(double fov, double aspect, double near, double far){

}

void
grf_gl_projection_perspective_update(GrfGLProjectionPerspective* proj){

}
