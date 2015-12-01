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
  (void) state;
  // Creating dummy data
  grfdim_t  dim         = 1;
  grfsize_t size        = 5;
  uint8_t   dados[5] = {5,4,6,2,1};
  GrfNDArray* array       = grf_ndarray_from_data(dados,dim,&size,GRF_UINT8);

  // Creating our chart
  GrfChart* chart = grf_chart_new();
  assert_non_null(chart);
  assert_int_equal(grf_chart_get_num_charts(chart),0);
  assert_int_equal(grf_chart_container_get_dim(GRF_CHART_CONTAINER(chart)),2);
  assert_int_equal(grf_chart_container_get_num_components(GRF_CHART_CONTAINER(chart)),0);
  assert_non_null(grf_chart_container_get_size(GRF_CHART_CONTAINER(chart)));
  grfsize_t* size_container = grf_chart_container_get_size(GRF_CHART_CONTAINER(chart));
  assert_int_equal(size_container[0], 0);
  assert_int_equal(size_container[1], 0);

  // Adding a 2D line plot
  grf_chart_plot(chart,array);
  assert_int_equal(grf_chart_get_num_charts(chart),1);
  assert_int_equal(grf_chart_container_get_dim(GRF_CHART_CONTAINER(chart)),2);
  assert_int_equal(grf_chart_container_get_num_components(GRF_CHART_CONTAINER(chart)),1);
  size_container = grf_chart_container_get_size(GRF_CHART_CONTAINER(chart));
  assert_int_equal(size_container[0], 1);
  assert_int_equal(size_container[1], 1);
  GrfAxis* axis_x = grf_chart_get_axis(chart,0);
  GrfAxis* axis_y = grf_chart_get_axis(chart,1);
  assert_non_null(axis_x);
  assert_non_null(axis_y);
  assert_true(grf_axis_get_visible(axis_x));
  assert_true(grf_axis_get_visible(axis_y));
  assert_null(grf_chart_get_title(chart));

  grf_chart_set_title(chart, "Título legal");
  assert_string_equal(grf_chart_get_title(chart), "Título legal");

  GrfRange range[2];
  grf_range_from_to(range,0,5);
  grf_range_from_to(range,0,20);
  //grf_chart_set_axis_range(chart, range);
  //GrfAxis* axis = grf_chart_get_axis(chart);
//  range[0] = grf_axis_get_range(axis);
//  range[1] = grf_axis_get_range(axis);
//  assert_int_equal(range[0].from->value,0);
//  assert_int_equal(range[0].to->value,5);
//  assert_int_equal(range[1].from->value,0);
//  assert_int_equal(range[1].to->value,20);

//  chart->size[0]    = 0;


//  grf_chart_plot();
//  grf_chart_plot();
  grf_ndarray_free(array);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_chart_plot_1d),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
