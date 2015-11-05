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
  grfdim_t  dim  = 1;
  grfsize_t size = 5;
  uint8_t dados[size] = {5,4,6,2,1};
  GrfArray* array = grf_array_from_data(dados,dim,&size,GRF_UINT8);

  // Creating our chart
  GrfChart* chart = grf_chart_new();
  grf_chart_plot(chart,array);
  chart->dim        = 1;
  chart->num_charts = 1;
  chart->size[0]    = 0;


  grf_chart_plot();
  grf_chart_plot();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[24]={
    cmocka_unit_test(test_grf_chart_plot_1d),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
