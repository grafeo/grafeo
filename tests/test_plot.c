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

static void test_grf_plotline(void** state){
  (void) state;
  GrfPlotLine* plotline = grf_plot_line_new();
  assert_non_null(plotline);
  assert_true(grf_plot_line_get_antialiased(plotline));
  GrfScalar4D color = grf_plot_line_get_color(plotline);
  assert_int_equal(color.x,0);
  assert_int_equal(color.y,0);
  assert_int_equal(color.z,0);
  assert_int_equal(color.w,255);

  assert_null(grf_plot_line_get_dashes(plotline));
  assert_int_equal(grf_plot_line_get_dash_capstyle(plotline),GRF_CAPSTYLE_BUTT);
  assert_int_equal(grf_plot_line_get_dash_joinstyle(plotline),GRF_JOINSTYLE_MITTER);
  assert_int_equal(grf_plot_line_get_drawstyle(plotline),GRF_DRAWSTYLE_DEFAULT);
  assert_int_equal(grf_plot_line_get_fillstyle(plotline),GRF_FILLSTYLE_FULL);
  assert_null(grf_plot_line_get_format(plotline));
  assert_null(grf_plot_line_get_linestyle(plotline));
  assert_int_equal(grf_plot_line_get_linewidth(plotline),1);
  assert_int_equal(grf_plot_line_get_marker(plotline),GRF_CHARTMARKER_POINT);
  color = grf_plot_line_get_marker_edge_color(plotline);
  assert_int_equal(color.x, 0);
  assert_int_equal(color.y, 0);
  assert_int_equal(color.z, 0);
  assert_int_equal(color.w, 255);
  assert_int_equal(grf_plot_line_get_marker_edge_width(plotline),1);
  assert_int_equal(grf_plot_line_get_marker_every(plotline),1);
  color = grf_plot_line_get_marker_face_color(plotline);
  assert_int_equal(color.x, 0);
  assert_int_equal(color.y, 0);
  assert_int_equal(color.z, 0);
  assert_int_equal(color.w, 255);
  color = grf_plot_line_get_marker_face_color_alt(plotline);
  assert_int_equal(color.x, 0);
  assert_int_equal(color.y, 0);
  assert_int_equal(color.z, 0);
  assert_int_equal(color.w, 255);
  assert_int_equal(grf_plot_line_get_marker_size(plotline),1);
  assert_null(grf_plot_line_get_data(plotline));

  g_clear_object(&plotline);
}

static void test_grf_plotbar(void** state){
  (void) state;
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_grf_plotline),
    cmocka_unit_test(test_grf_plotbar),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
