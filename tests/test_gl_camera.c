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

static void test_gl_camera(void** state){
  // New camera
  g_autoptr(GrfGLCamera) camera = grf_gl_camera_new();
  assert_non_null(camera);

  // Naming camera
  g_autofree char* name = grf_gl_camera_get_name(camera);
  assert_null(name);

  // Reset camera view
  grf_gl_camera_reset_view(camera,
                           (GrfGLVec3){{0,0,0}},  // Position
                           (GrfGLVec3){{0,0,-1}}, // Target
                           (GrfGLVec3){{0,1,0}}); // Up

  GrfGLMat4 view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct = grf_gl_mat4_eye();
  u_int8_t i;
  for(i = 0; i < 16; i++) assert_true(view.data[i] == view_correct.data[i]);

  // Moving camera behind
  grf_gl_camera_move_behind(camera,5.0);

  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct2 = {{1,0,0,0, 0,1,0,0, 0,0,1,-5.0, 0.0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(view.data[i] == view_correct2.data[i]);

  // Moving camera right
  grf_gl_camera_move_right(camera, 3.5);
  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct3 = {{1,0,0,-3.5, 0,1,0,0, 0,0,1,-5.0, 0.0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(view.data[i] == view_correct3.data[i]);

  // Moving camera down
  double down = 7.6;
  grf_gl_camera_move_down(camera, down);
  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct4 = {{1,0,0,-3.5, 0,1,0,down, 0,0,1,-5.0, 0.0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(view.data[i] == view_correct4.data[i]);

  // Yaw (rotate at y-axis)
  // 90 deg:
  //  right => forward
  //  up    => up
  //  forward => -right
  grf_gl_camera_yaw(camera, 90.0);
  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct5 = {{0,0,-1,5, 0,1,0,down, 1,0,0,-3.5, 0.0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(fabs(view.data[i] - view_correct5.data[i]) < 1e-10);

  // Pitch (rotate at x-axis)
  // 90 deg:
  //  right   => right
  //  up      => -forward
  //  forward => up
  grf_gl_camera_pitch(camera, 90.0);
  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct6 = {{0,0,-1,5, 1,0,0,-3.5, 0,-1,0,-down, 0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(fabs(view.data[i] - view_correct6.data[i]) < 1e-10);

  // Roll (rotate at z-axis)
  // 90 deg:
  //  forward => forward
  //  up => right
  //  right => -up
  grf_gl_camera_roll(camera, 90.0);
  view         = grf_gl_camera_get_view(camera);
  GrfGLMat4 view_correct7 = {{-1,0,0,3.5, 0,0,-1,5, 0,-1,0,-down, 0,0,0,1}};
  for(i = 0; i < 16; i++) assert_true(fabs(view.data[i] - view_correct7.data[i]) < 1e-10);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_gl_camera),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
