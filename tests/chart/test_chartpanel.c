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

static void test_grf_chart_panel(void** state){
  (void) state;

  GrfChartPanel* panel = grf_chart_panel_new();
  assert_non_null(panel);
  assert_null(grf_chart_panel_get_title(panel));
  assert_null(grf_chart_panel_get_axis(panel,0));
  assert_int_equal(grf_chart_panel_get_dim(panel),0);
  assert_null(grf_chart_panel_get_legend(panel));
  GrfQueue* plots = grf_chart_panel_get_plots(panel);
  assert_null(plots->begin);
  assert_null(plots->end);
  assert_int_equal(plots->length,0);

}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_chart_panel),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
