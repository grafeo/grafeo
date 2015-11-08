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
#ifndef GRF_LEGEND_H
#define GRF_LEGEND_H

#include <glib-object.h>
#include <grafeo/chart.h>

G_BEGIN_DECLS

#define GRF_TYPE_LEGEND grf_legend_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfLegend, grf_legend, GRF, LEGEND, GObject)

typedef struct _GrfLegendClass{
  GObjectClass parent_class;
}GrfLegendClass;

/**
 * @brief grf_legend_new
 * @return
 */
GrfLegend*
grf_legend_new();
/**
 * @brief grf_legend_get_num_cols
 * @param legend
 * @return
 */
uint8_t
grf_legend_get_num_cols(GrfLegend* legend);
/**
 * @brief grf_legend_get_num_points
 * @param legend
 * @return
 */
uint8_t
grf_legend_get_num_points(GrfLegend* legend);
/**
 * @brief grf_legend_get_scatter_points
 * @param legend
 * @return
 */
uint8_t
grf_legend_get_scatter_points(GrfLegend* legend);
/**
 * @brief grf_legend_get_with_frame
 * @param legend
 * @return
 */
grfbool_t
grf_legend_get_with_frame(GrfLegend* legend);
/**
 * @brief grf_legend_get_with_round_edges
 * @param legend
 * @return
 */
grfbool_t
grf_legend_get_with_round_edges(GrfLegend* legend);
/**
 * @brief grf_legend_get_with_shadow
 * @param legend
 * @return
 */
grfbool_t
grf_legend_get_with_shadow(GrfLegend* legend);
/**
 * @brief grf_legend_get_frame_alpha
 * @param legend
 * @return
 */
float
grf_legend_get_frame_alpha(GrfLegend* legend);
/**
 * @brief grf_legend_get_title
 * @param legend
 * @return
 */
char*
grf_legend_get_title(GrfLegend* legend);
/**
 * @brief grf_legend_set_num_cols
 * @param legend
 * @param num_cols
 */
void
grf_legend_set_num_cols(GrfLegend* legend, uint8_t num_cols);
/**
 * @brief grf_legend_set_num_points
 * @param legend
 * @param num_points
 */
void
grf_legend_set_num_points(GrfLegend* legend, uint8_t num_points);
/**
 * @brief grf_legend_set_scatter_points
 * @param legend
 * @param scatter_points
 */
void
grf_legend_set_scatter_points(GrfLegend* legend, uint8_t scatter_points);
/**
 * @brief grf_legend_set_with_frame
 * @param legend
 * @param with_frame
 */
void
grf_legend_set_with_frame(GrfLegend* legend, grfbool_t with_frame);
/**
 * @brief grf_legend_set_with_round_edges
 * @param legend
 * @param with_round_edges
 */
void
grf_legend_set_with_round_edges(GrfLegend* legend, grfbool_t with_round_edges);
/**
 * @brief grf_legend_set_with_shadow
 * @param legend
 * @param with_shadow
 */
void
grf_legend_set_with_shadow(GrfLegend* legend, grfbool_t with_shadow);
/**
 * @brief grf_legend_set_frame_alpha
 * @param legend
 * @param frame_alpha
 */
void
grf_legend_set_frame_alpha(GrfLegend* legend, float frame_alpha);
/**
 * @brief grf_legend_set_frame_title
 * @param legend
 * @param title
 */
void
grf_legend_set_frame_title(GrfLegend* legend, char* title);

G_END_DECLS
#endif
