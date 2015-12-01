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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <grafeo/gl.h>

static void test_gl_projection_orthographic(void** state){
  g_autoptr(GrfGLProjectionOrthographic) proj_ortho = grf_gl_projection_orthographic(-100,100,-100,100,-100,100);
  assert_non_null(proj_ortho);
  // Testing parameters
  assert_int_equal(grf_gl_projection_orthographic_get_left(proj_ortho),-100);
  assert_int_equal(grf_gl_projection_orthographic_get_right(proj_ortho),100);
  assert_int_equal(grf_gl_projection_orthographic_get_bottom(proj_ortho),-100);
  assert_int_equal(grf_gl_projection_orthographic_get_top(proj_ortho),100);
  assert_int_equal(grf_gl_projection_orthographic_get_near(proj_ortho),-100);
  assert_int_equal(grf_gl_projection_orthographic_get_far(proj_ortho),100);

  // Testing projection matrix
  GrfMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj_ortho));
  double verdades[16] = {0.01,    0.0,   0.0,  0.0,// 1st column
                         0.0,    0.01,   0.0,  0.0,// 2nd column
                         0.0,     0.0, -0.01,  0.0,// 3rd column
                        -0.0,    -0.0,  -0.0,  1.0,// 4th column
                        };
  assert_memory_equal(verdades, matrix->data, 16*sizeof(double));

  // Testing get_matrix, which gives a matrix clone
  g_autofree GrfMat4* matrix2 = grf_gl_projection_get_matrix(GRF_GL_PROJECTION(proj_ortho));
  matrix2->data[0] = 500.0;
  assert_false(matrix2->data[0] == matrix->data[0]);

  // Testing setting another matrix
  grf_gl_projection_set_matrix(GRF_GL_PROJECTION(proj_ortho),matrix2);
  verdades[0] = 500.0;
  assert_memory_equal(verdades, matrix->data, 16*sizeof(double));

  // Testing setters
  grf_gl_projection_orthographic_set_all(proj_ortho,-200,200,-200,200,-200,200);
  double verdades2[16] ={0.005,    0.0,   0.0,  0.0,// 1st column
                         0.0,    0.005,   0.0,  0.0,// 2nd column
                         0.0,     0.0, -0.005,  0.0,// 3rd column
                        -0.0,    -0.0,  -0.0,  1.0,// 4th column
                        };
  assert_memory_equal(verdades2, matrix->data, 16*sizeof(double));
}

static void test_gl_projection_perspective(void** state){
  double angle      = 45.0;
  double aspect     = 640.0/480.0;
  double near       = 0.1;
  double far        = 100.0;
  double halftanfov = tan(angle/2.0);

  g_autoptr(GrfGLProjectionPerspective) proj_persp = grf_gl_projection_perspective(angle, aspect, near, far);
  assert_non_null(proj_persp);
  // Testing parameters
  assert_true(grf_gl_projection_perspective_get_fov(proj_persp)    == angle);
  assert_true(grf_gl_projection_perspective_get_aspect(proj_persp) == aspect);
  assert_true(grf_gl_projection_perspective_get_near(proj_persp)   == near);
  assert_true(grf_gl_projection_perspective_get_far(proj_persp)    == far);

  // Testing projection matrix
  GrfMat4* matrix = grf_gl_projection_get_matrix_ptr(GRF_GL_PROJECTION(proj_persp));

  double verdades[16] = {1.0/(aspect*halftanfov),    0.0,   0.0,  0.0,// 1st column
                         0.0,     1.0/halftanfov,   0.0,  0.0,// 2nd column
                         0.0,     0.0, (far+near)/(near-far),  -1.0,// 3rd column
                         0.0,     0.0,  2*far*near/(near-far),  0.0,// 4th column
                        };
  assert_memory_equal(verdades, matrix->data, sizeof(GrfMat4));

  // Testing get_matrix, which gives a matrix clone
  g_autofree GrfMat4* matrix2 = grf_gl_projection_get_matrix(GRF_GL_PROJECTION(proj_persp));
  matrix2->data[0] = 500.0;
  assert_false(matrix2->data[0] == matrix->data[0]);

  // Testing setting another matrix
  grf_gl_projection_set_matrix(GRF_GL_PROJECTION(proj_persp),matrix2);
  verdades[0] = 500.0;
  assert_memory_equal(verdades, matrix->data, sizeof(GrfMat4));

  // Testing setters
  grf_gl_projection_perspective_set_all(proj_persp,angle,aspect,near,far);
  verdades[0] = 1.0/(aspect*halftanfov);
  assert_memory_equal(verdades, matrix->data, sizeof(GrfMat4));
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_gl_projection_orthographic),
    cmocka_unit_test(test_gl_projection_perspective),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
