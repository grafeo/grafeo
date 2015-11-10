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

static void test_grf_chart_container(void** state){
  (void) state;

  // New container
  GrfChartContainer* container = grf_chart_container_new();
  assert_non_null(container);
  assert_int_equal(grf_chart_container_get_dim(container),2);
  assert_int_equal(grf_chart_container_get_num_components(container),0);
  assert_int_equal(grf_chart_container_get_num_leafs(container,FALSE),0);
  assert_null(grf_chart_container_get_title(container));
  grfsize_t *size = grf_chart_container_get_size(container);
  assert_int_equal(size[0],0);
  assert_int_equal(size[1],0);

  // Add a leaf
  GrfChartPanel* chart_panel = grf_chart_panel_new();
  grf_chart_container_add_component(container,GRF_CHART_COMPONENT(chart_panel));
  assert_int_equal(grf_chart_container_get_num_components(container),1);
  assert_int_equal(grf_chart_container_get_num_leafs(container,FALSE), 1);
  size = grf_chart_container_get_size(container);
  assert_int_equal(size[0],1);
  assert_int_equal(size[1],1);

  // Add a container
  GrfChartContainer* chart_container = grf_chart_container_new();
  grf_chart_container_add_component(container, GRF_CHART_COMPONENT(chart_container));
  assert_int_equal(grf_chart_container_get_num_components(container),2);
  assert_int_equal(grf_chart_container_get_num_leafs(container,FALSE), 1);
  assert_int_equal(grf_chart_container_get_num_leafs(container,TRUE), 1);
  size = grf_chart_container_get_size(container);
  assert_int_equal(size[0],1);
  assert_int_equal(size[1],2);

  // Get a component
  GrfChartComponent* component = grf_chart_container_get_component(container, 1);
  assert_non_null(component);
  assert_true(GRF_IS_CHART_CONTAINER(component));

  // Remove a component
  grf_chart_container_remove_component(container, GRF_CHART_COMPONENT(chart_panel));
  assert_int_equal(grf_chart_container_get_num_components(container), 1);
  assert_int_equal(grf_chart_container_get_num_leafs(container,FALSE),0);
  assert_int_equal(grf_chart_container_get_num_leafs(container,TRUE),0);
  size = grf_chart_container_get_size(container);
  assert_int_equal(size[0],1);
  assert_int_equal(size[1],1);



}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_chart_container),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
