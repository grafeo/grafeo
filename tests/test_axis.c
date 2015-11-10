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
#include <grafeo/chart.h>

static void test_grf_axis(void** state){
  (void) state;

  // Testing initialization
  GrfAxis* axis = grf_axis_new();
  assert_non_null(axis);
  GrfScalar4D color = grf_axis_get_color(axis);
  assert_int_equal(color.x,  0);
  assert_int_equal(color.y,  0);
  assert_int_equal(color.z,  0);
  assert_int_equal(color.w,255);
  assert_int_equal(grf_axis_get_grid_style(axis),GRF_GRID_STYLE_SOLID);
  assert_true(grf_axis_get_grid_visible(axis));
  assert_null(grf_axis_get_label(axis));
  assert_int_equal(grf_axis_get_limitmode(axis),GRF_AXIS_LIM_AUTO);
  assert_int_equal(grf_axis_get_linewidth(axis),1);
  assert_int_equal(grf_axis_get_location(axis),GRF_AXIS_LOC_ORIGIN);
  GrfRange range = grf_axis_get_range(axis);
  assert_null(range.from);
  assert_null(range.to);
  color = grf_axis_get_tick_color(axis);
  assert_int_equal(color.x, 127);
  assert_int_equal(color.y, 127);
  assert_int_equal(color.z, 127);
  assert_int_equal(color.w, 255);
  assert_int_equal(grf_axis_get_tick_direction(axis),GRF_TICK_DIR_IN);
  assert_int_equal(grf_axis_get_tick_linewidth(axis), 1);
  assert_null(grf_axis_get_tick_positions(axis));
  assert_true(grf_axis_get_visible(axis));

  // Testing some setters
  grf_scalar4D_fill(color, 200, 50, 20, 84);
  grf_axis_set_color(axis,color);
  color = grf_axis_get_color(axis);
  assert_int_equal(color.x, 200);
  assert_int_equal(color.y, 50);
  assert_int_equal(color.z, 20);
  assert_int_equal(color.w, 84);

  grf_axis_set_grid_style(axis, GRF_GRID_STYLE_DASH_DOTTED);
  assert_int_equal(grf_axis_get_grid_style(axis),GRF_GRID_STYLE_DASH_DOTTED);
  grf_axis_set_grid_visible(axis,FALSE);
  assert_false(grf_axis_get_grid_visible(axis));
  grf_axis_set_label(axis, "Hello");
  assert_string_equal(grf_axis_get_label(axis),"Hello");
  grf_axis_set_limitmode(axis,GRF_AXIS_LIM_MANUAL);
  assert_int_equal(grf_axis_get_limitmode(axis), GRF_AXIS_LIM_MANUAL);
  grf_axis_set_linewidth(axis, 2);
  assert_int_equal(grf_axis_get_linewidth(axis),2);
  grf_axis_set_location(axis,GRF_AXIS_LOC_RIGHT);
  assert_int_equal(grf_axis_get_location(axis), GRF_AXIS_LOC_RIGHT);
  grf_range_from_to(&range,0,5);
  grf_axis_set_range(axis,range);
  range = grf_axis_get_range(axis);
  assert_int_equal(range.from->value, 0);
  assert_int_equal(range.to->value, 5);

  grf_axis_free(axis);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_axis),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
