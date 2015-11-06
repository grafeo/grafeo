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
/**
 * Leaf of the tree structure of charts
 */
typedef struct _GrfChartPanelPrivate{
  grfdim_t            dim;        /**< Dimension of a chart (2 or 3) */
  grfsize_t           num_plots;  /**< Number of plots inside the chart */
  GrfAxis           * axis;       /**< List of N axis, N = dim*/
  GrfPlot           * plots;      /**< List of M plots, M = num_plots */
  GrfLegend         * legend;     /**< Legend configuration of the chart */
  GrfChartProjection  projection; /**< Cartesian or Spherical (polar in 2D) */
}GrfChartPanelPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfChartPanel, grf_chart_panel, GRF_TYPE_CHART_COMPONENT)

static void
grf_chart_panel_init(GrfChartPanel *self){

}

static void
grf_chart_panel_class_init(GrfChartPanelClass *klass){

}

/*=========================
 *      PUBLIC API
 *=======================*/
GrfChartPanel*
grf_chart_panel_new(){
  return NULL;
}

grfdim_t
grf_chart_panel_get_dim(GrfChartPanel* chart_panel){
  return 0;
}

grfsize_t
grf_chart_panel_get_num_plots(GrfChartPanel* chart_panel){
  return 0;
}

GrfAxis*
grf_chart_panel_get_axis(GrfChartPanel* chart_panel){
  return NULL;
}

GrfLegend*
grf_chart_panel_get_legend(GrfChartPanel* chart_panel){
  return NULL;
}

GrfChartProjection
grf_chart_panel_get_projection(GrfChartPanel* chart_panel){
  return GRF_PROJECTION_CARTESIAN;
}
