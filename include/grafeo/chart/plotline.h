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
#ifndef GRF_PLOT_LINE_H
#define GRF_PLOT_LINE_H
#include <grafeo/core.h>
#include <grafeo/chart.h>
#include <glib-object.h>

BEGIN_DECLS

#define GRF_TYPE_PLOT_LINE grf_plot_line_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfPlotLine, grf_plot_line, GRF, PLOT_LINE, GrfPlot)

typedef struct _GrfPlotLineClass{
  GrfPlotClass parent_class;
}GrfPlotLineClass;

/**
 * @brief grf_plot_line_new
 * @return
 */
GrfPlotLine*
grf_plot_line_new();

/**
 * @brief grf_plot_line_new_with_data
 * @param array
 * @return
 */
GrfPlotLine*
grf_plot_line_new_with_data(GrfArray* array);

/**
 * @brief grf_plot_line_get_format
 * @param plot_line
 * @return
 */
char*
grf_plot_line_get_format(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_dash_capstyle
 * @param plot_line
 * @return
 */
GrfChartCapStyle
grf_plot_line_get_dash_capstyle(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_dash_joinstyle
 * @param plot_line
 * @return
 */
GrfChartJoinStyle
grf_plot_line_get_dash_joinstyle(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_antialiased
 * @param plot_line
 * @return
 */
grfbool_t
grf_plot_line_get_antialiased(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_color
 * @param plot_line
 * @return
 */
GrfScalar4D
grf_plot_line_get_color(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_dashes
 * @param plot_line
 * @return
 */
uint8_t*
grf_plot_line_get_dashes(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_drawstyle
 * @param plot_line
 * @return
 */
GrfChartDrawStyle
grf_plot_line_get_drawstyle(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_fillstyle
 * @param plot_line
 * @return
 */
GrfChartFillStyle
grf_plot_line_get_fillstyle(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_linestyle
 * @param plot_line
 * @return
 */
char*
grf_plot_line_get_linestyle(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_linewidth
 * @param plot_line
 * @return
 */
uint8_t
grf_plot_line_get_linewidth(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker
 * @param plot_line
 * @return
 */
GrfChartMarker
grf_plot_line_get_marker(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_edge_color
 * @param plot_line
 * @return
 */
GrfScalar4D
grf_plot_line_get_marker_edge_color(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_edge_width
 * @param plot_line
 * @return
 */
uint8_t
grf_plot_line_get_marker_edge_width(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_face_color
 * @param plot_line
 * @return
 */
GrfScalar4D
grf_plot_line_get_marker_face_color(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_face_color_alt
 * @param plot_line
 * @return
 */
GrfScalar4D
grf_plot_line_get_marker_face_color_alt(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_size
 * @param plot_line
 * @return
 */
uint8_t
grf_plot_line_get_marker_size(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_get_marker_every
 * @param plot_line
 * @return
 */
uint8_t
grf_plot_line_get_marker_every(GrfPlotLine* plot_line);

/**
 * @brief grf_plot_line_set_format
 * @param plot_line
 * @param format
 */
void
grf_plot_line_set_format(GrfPlotLine* plot_line, char* format);

/**
 * @brief grf_plot_line_set_dash_capstyle
 * @param plot_line
 * @param dash_capstyle
 */
void
grf_plot_line_set_dash_capstyle(GrfPlotLine* plot_line, GrfChartCapStyle dash_capstyle);

/**
 * @brief grf_plot_line_set_dash_joinstyle
 * @param plot_line
 * @param dash_joinstyle
 */
void
grf_plot_line_set_dash_joinstyle(GrfPlotLine* plot_line, GrfChartJoinStyle dash_joinstyle);

/**
 * @brief grf_plot_line_set_antialiased
 * @param plot_line
 * @param antialiased
 */
void
grf_plot_line_set_antialiased(GrfPlotLine* plot_line, grfbool_t antialiased);

/**
 * @brief grf_plot_line_set_color
 * @param plot_line
 * @param color
 */
void
grf_plot_line_set_color(GrfPlotLine* plot_line, GrfScalar4D color);

/**
 * @brief grf_plot_line_set_dashes
 * @param plot_line
 * @param dashes
 */
void
grf_plot_line_set_dashes(GrfPlotLine* plot_line, uint8_t* dashes);

/**
 * @brief grf_plot_line_set_drawstyle
 * @param plot_line
 * @param drawstyle
 */
void
grf_plot_line_set_drawstyle(GrfPlotLine* plot_line, GrfChartDrawStyle drawstyle);

/**
 * @brief grf_plot_line_set_fillstyle
 * @param plot_line
 * @param fillstyle
 */
void
grf_plot_line_set_fillstyle(GrfPlotLine* plot_line, GrfChartFillStyle fillstyle);

/**
 * @brief grf_plot_line_set_linestyle
 * @param plot_line
 * @param linestyle
 */
void
grf_plot_line_set_linestyle(GrfPlotLine* plot_line, char* linestyle);

/**
 * @brief grf_plot_line_set_linewidth
 * @param plot_line
 * @param linewidth
 */
void
grf_plot_line_set_linewidth(GrfPlotLine* plot_line, uint8_t linewidth);

/**
 * @brief grf_plot_line_set_marker
 * @param plot_line
 * @param marker
 */
void
grf_plot_line_set_marker(GrfPlotLine* plot_line, GrfChartMarker marker);

/**
 * @brief grf_plot_line_set_marker_edge_color
 * @param plot_line
 * @param marker_edge_color
 */
void
grf_plot_line_set_marker_edge_color(GrfPlotLine* plot_line, GrfScalar4D marker_edge_color);

/**
 * @brief grf_plot_line_set_marker_edge_width
 * @param plot_line
 * @param marker_edge_width
 */
void
grf_plot_line_set_marker_edge_width(GrfPlotLine* plot_line, uint8_t marker_edge_width);

/**
 * @brief grf_plot_line_set_marker_face_color
 * @param plot_line
 * @param marker_face_color
 */
void
grf_plot_line_set_marker_face_color(GrfPlotLine* plot_line, GrfScalar4D marker_face_color);

/**
 * @brief grf_plot_line_set_marker_face_color_alt
 * @param plot_line
 * @param marker_face_color_alt
 */
void
grf_plot_line_set_marker_face_color_alt(GrfPlotLine* plot_line, GrfScalar4D marker_face_color_alt);

/**
 * @brief grf_plot_line_set_marker_size
 * @param plot_line
 * @param marker_size
 */
void
grf_plot_line_set_marker_size(GrfPlotLine* plot_line, uint8_t marker_size);

/**
 * @brief grf_plot_line_set_marker_every
 * @param plot_line
 * @param marker_every
 */
void
grf_plot_line_set_marker_every(GrfPlotLine* plot_line, uint8_t marker_every);

/**
 * @brief grf_plot_line_set_data
 * @param plot_line
 * @param data
 */
void
grf_plot_line_set_data(GrfPlotLine* plot_line, GrfArray* data);

END_DECLS

#endif
