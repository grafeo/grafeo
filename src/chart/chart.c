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

G_DEFINE_TYPE(GrfChart, grf_chart, GRF_TYPE_CHART_CONTAINER)

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
grf_chart_plot(GrfChart *chart, GrfNDArray *data_y){
  // Create the plot
  GrfPlotLine  * plot_line = grf_plot_line_new_with_data(data_y);
  GrfChartPanel* panel;

  // If the container is empty, create a 2D panel.
  if(grf_chart_container_is_empty(GRF_CHART_CONTAINER(chart))){
    panel = grf_chart_panel_new();
    grfsize_t size[2] = {1,1};
    grf_chart_container_add_component(GRF_CHART_CONTAINER(chart),GRF_CHART_COMPONENT(panel));
    grf_chart_container_set_size(GRF_CHART_CONTAINER(chart),2,size);
    GrfAxis* axis_x = grf_axis_new();
    GrfAxis* axis_y = grf_axis_new();
    grf_chart_panel_append_axis(panel,axis_x);
    grf_chart_panel_append_axis(panel,axis_y);
  }else{
    panel = grf_chart_get_last_panel(chart);
  }

  grf_chart_panel_add_plot(panel, GRF_PLOT(plot_line));
  return GRF_PLOT(plot_line);
}

grfsize_t
grf_chart_get_num_charts(GrfChart *chart){
  return grf_chart_container_get_num_leafs(GRF_CHART_CONTAINER(chart),TRUE);
}

char*
grf_chart_get_title(GrfChart* chart){
  return grf_chart_container_get_title(GRF_CHART_CONTAINER(chart));
}

GrfChartPanel*
grf_chart_get_last_panel(GrfChart* chart){
  return grf_chart_container_get_last_leaf(GRF_CHART_CONTAINER(chart));
}

GrfAxis*
grf_chart_get_axis(GrfChart* chart, uint8_t index){
  GrfChartComponent* component = grf_chart_container_get_first_leaf(GRF_CHART_CONTAINER(chart));
  if(component && GRF_IS_CHART_PANEL(component))
    return grf_chart_panel_get_axis(GRF_CHART_PANEL(component),index);
  return NULL;
}

void
grf_chart_set_title(GrfChart* chart, char* title){
  grf_chart_container_set_title(GRF_CHART_CONTAINER(chart),title);
}
