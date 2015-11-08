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
#ifndef GRF_CHART_PANEL_H
#define GRF_CHART_PANEL_H

#include <glib-object.h>
#include <grafeo/chart.h>
#include <grafeo/core.h>

G_BEGIN_DECLS

#define GRF_TYPE_CHART_PANEL grf_chart_panel_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfChartPanel, grf_chart_panel, GRF, CHART_PANEL, GrfChartComponent)

typedef struct _GrfChartPanelClass{
  GrfChartComponentClass parent_instance;
  grfdim_t           (*get_dim)        (GrfChartPanel* self);
  grfsize_t          (*get_num_plots)  (GrfChartPanel* self);
  GrfAxis*           (*get_axis)       (GrfChartPanel* self);
  GrfLegend*         (*get_legend)     (GrfChartPanel* self);
  GrfChartProjection (*get_projection) (GrfChartPanel* self);


}GrfChartPanelClass;

/**
 * @brief grf_chart_panel_new
 * @return
 */
GrfChartPanel*
grf_chart_panel_new();

/**
 * @brief grf_chart_panel_get_dim
 * @param chart_panel
 * @return
 */
grfdim_t
grf_chart_panel_get_dim(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_get_num_plots
 * @param chart_panel
 * @return
 */
grfsize_t
grf_chart_panel_get_num_plots(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_get_axis
 * @param chart_panel
 * @return
 */
GrfAxis*
grf_chart_panel_get_axis(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_get_legend
 * @param chart_panel
 * @return
 */
GrfLegend*
grf_chart_panel_get_legend(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_get_projection
 * @param chart_panel
 * @return
 */
GrfChartProjection
grf_chart_panel_get_projection(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_add_legend
 * @param chart_panel
 * @return
 */
GrfLegend*
grf_chart_panel_add_legend(GrfChartPanel* chart_panel);

/**
 * @brief grf_chart_panel_add_plot
 * @param chart_panel
 * @param plot
 */
void
grf_chart_panel_add_plot(GrfChartPanel* chart_panel, GrfPlot* plot);

/**
 * @brief grf_chart_panel_remove_plot
 * @param chart_panel
 * @param plot
 */
void
grf_chart_panel_remove_plot(GrfChartPanel* chart_panel, GrfPlot* plot);

G_END_DECLS

#endif
