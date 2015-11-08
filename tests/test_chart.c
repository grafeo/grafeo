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

static void test_grf_chart_plot_1d(void** state){
  // Creating dummy data
  grfdim_t  dim         = 1;
  grfsize_t size        = 5;
  uint8_t   dados[5] = {5,4,6,2,1};
  GrfArray* array       = grf_array_from_data(dados,dim,&size,GRF_UINT8);

  // Creating our chart
  GrfChart* chart = grf_chart_new();
  assert_non_null(chart);
  assert_int_equal(grf_chart_get_num_charts(chart),0);
  assert_int_equal(grf_chart_container_get_dim(GRF_CHART_CONTAINER(chart)),0);
  assert_int_equal(grf_chart_container_get_num_components(GRF_CHART_CONTAINER(chart)),0);
  assert_null(grf_chart_container_get_size(GRF_CHART_CONTAINER(chart)));

  // Adding a 2D line plot
  grf_chart_plot(chart,array);
  assert_int_equal(grf_chart_get_num_charts(chart),1);
  assert_int_equal(grf_chart_container_get_dim(GRF_CHART_CONTAINER(chart)),1);
  assert_int_equal(grf_chart_container_get_num_components(GRF_CHART_CONTAINER(chart)),1);
  grfsize_t* size_container = grf_chart_container_get_size(GRF_CHART_CONTAINER(chart));
  assert_int_equal(size_container[0], 1);
  assert_string_equal(grf_chart_get_title(chart), NULL);
//  chart->size[0]    = 0;


//  grf_chart_plot();
//  grf_chart_plot();
  grf_array_free(array);
}

static void test_grf_chart_window(void** state){
  GrfChartWindow* window = grf_chart_window_new();
  grf_chart_window_show(window);
  uint8_t key = 0;
  while(key != 27) key = grf_chart_window_waitkey();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_grf_chart_plot_1d),
    cmocka_unit_test(test_grf_chart_window),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
