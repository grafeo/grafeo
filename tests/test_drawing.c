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
#include <grafeo/drawing.h>

static void test_drawing_line(void** state){
  (void) state;
  Array* array = array_new_2D(10,10);
  array_fill(array,0);

  // Line Properties
  GrfScalar2D p0        = {1, 1},
              p1        = {5,3};
  GrfScalar4D color     = {255,0,0,255};
  int         thickness = 1;
  int         line_type = 0;
  int         shift     = 0;

  // Draw Line
  grf_array_draw_line(array, p0, p1, &color, thickness, line_type, shift);

  // Free Array
  array_free(array);
}

static void test_drawing_circle(void** state){
  (void) state;
}

static void test_drawing_polyline(void** state){
  (void) state;
}

static void test_drawing_line_arrow(void** state){
  (void) state;
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[4]={
    cmocka_unit_test(test_drawing_line),
    cmocka_unit_test(test_drawing_circle),
    cmocka_unit_test(test_drawing_polyline),
    cmocka_unit_test(test_drawing_line_arrow),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
