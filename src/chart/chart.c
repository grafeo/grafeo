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
#include <grafeo/chart.h>
/*=========================
 *      PRIVATE API
 *=======================*/
typedef struct _GrfChartPrivate{

}GrfChartPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfChart, grf_chart, GRF_TYPE_CHART_CONTAINER)

static void
grf_chart_class_init(GrfChartClass *klass){

}

static void
grf_chart_init(GrfChart *self){

}


/*=========================
 *      PUBLIC API
 *=======================*/


GrfChart*
grf_chart_new(){
  return g_object_new(GRF_TYPE_CHART, NULL);
}

GrfPlot*
grf_chart_plot(GrfChart *chart, GrfArray *data_y){
  return NULL;
}

grfsize_t
grf_chart_get_num_charts(GrfChart *chart){
  return grf_chart_container_get_num_panels(GRF_CHART_CONTAINER(chart),TRUE);
}

char*
grf_chart_get_title(GrfChart* chart){
  return grf_chart_container_get_title(GRF_CHART_CONTAINER(chart));
}
