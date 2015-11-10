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
  GrfQueue            plots;      /**< List of M plots, M = num_plots */
  GrfQueue            axis;       /**< List of N axis, N = dim*/
  GrfLegend         * legend;     /**< Legend configuration of the chart */
  GrfChartProjection  projection; /**< Cartesian or Spherical (polar in 2D) */
}GrfChartPanelPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfChartPanel, grf_chart_panel, GRF_TYPE_CHART_COMPONENT)

static void
grf_chart_panel_init(GrfChartPanel *self){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(self);
  priv->dim          = 0;
  priv->plots.begin  = NULL;
  priv->plots.end    = NULL;
  priv->plots.length = 0;
  priv->axis.begin   = NULL;
  priv->axis.end     = NULL;
  priv->axis.length  = 0;
  priv->legend       = NULL;
  priv->projection   = GRF_PROJECTION_CARTESIAN;
}

static void
grf_chart_panel_class_init(GrfChartPanelClass *klass){
  klass->get_dim        = grf_chart_panel_get_dim;
  klass->get_axis       = grf_chart_panel_get_axis;
  klass->get_legend     = grf_chart_panel_get_legend;
  klass->get_num_plots  = grf_chart_panel_get_num_plots;
  klass->get_projection = grf_chart_panel_get_projection;
  klass->get_title      = grf_chart_panel_get_title;

  klass->set_dim        = grf_chart_panel_set_dim;
  klass->append_axis    = grf_chart_panel_append_axis;
  klass->prepend_axis   = grf_chart_panel_prepend_axis;
  klass->remove_axis    = grf_chart_panel_remove_axis;
  klass->set_legend     = grf_chart_panel_set_legend;
  klass->set_projection = grf_chart_panel_set_projection;
  klass->set_title      = grf_chart_panel_set_title;
}

/*=========================
 *      PUBLIC API
 *=======================*/
GrfChartPanel*
grf_chart_panel_new(){
  return g_object_new(GRF_TYPE_CHART_PANEL, NULL);
}

grfdim_t
grf_chart_panel_get_dim(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  return priv->dim;
}

grfsize_t
grf_chart_panel_get_num_plots(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  return priv->plots.length;
}

GrfAxis*
grf_chart_panel_get_axis(GrfChartPanel* chart_panel, uint8_t index){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  if(index >= priv->axis.length) return NULL;
  return GRF_AXIS(grf_queue_value_at(&priv->axis,(uint32_t)index));
}

GrfLegend*
grf_chart_panel_get_legend(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  return priv->legend;
}

char*
grf_chart_panel_get_title(GrfChartPanel *chart_panel){
  return grf_chart_component_get_title(GRF_CHART_COMPONENT(chart_panel));
}

GrfChartProjection
grf_chart_panel_get_projection(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  return priv->projection;
}

GrfLegend*
grf_chart_panel_add_legend(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  priv->legend = grf_legend_new();
  return priv->legend;
}

void
grf_chart_panel_add_plot(GrfChartPanel* chart_panel, GrfPlot* plot){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  grf_queue_append(&priv->plots, plot);
}

void
grf_chart_panel_remove_plot(GrfChartPanel* chart_panel, GrfPlot* plot){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  grf_queue_remove(&priv->plots, plot);
}

GrfQueue*
grf_chart_panel_get_plots(GrfChartPanel* chart_panel){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  return &priv->plots;
}

void
grf_chart_panel_set_dim(GrfChartPanel* chart_panel, grfdim_t dim){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  priv->dim = dim;
}

void
grf_chart_panel_set_legend(GrfChartPanel* chart_panel, GrfLegend* legend){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  priv->legend = legend;
}

void
grf_chart_panel_append_axis(GrfChartPanel* chart_panel, GrfAxis* axis){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  grf_queue_append(&priv->axis,axis);
}

void
grf_chart_panel_prepend_axis(GrfChartPanel* chart_panel, GrfAxis* axis){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  grf_queue_prepend(&priv->axis,axis);
}

void
grf_chart_panel_remove_axis(GrfChartPanel* chart_panel, GrfAxis* axis){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  grf_queue_remove(&priv->axis,axis);
}

void
grf_chart_panel_set_projection(GrfChartPanel* chart_panel, GrfChartProjection projection){
  GrfChartPanelPrivate* priv = grf_chart_panel_get_instance_private(chart_panel);
  priv->projection = projection;
}

void
grf_chart_panel_set_title(GrfChartPanel* chart_panel, char* title){
  grf_chart_component_set_title(GRF_CHART_COMPONENT(chart_panel), title);
}
