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
#ifndef GRF_CHART_H
#define GRF_CHART_H
#include <grafeo/core.h>
#include <grafeo/chart.h>
#include <glib-object.h>

#define GRF_TYPE_CHART grf_chart_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfChart, grf_chart, GRF, CHART, GrfChartContainer)

typedef struct _GrfChartClass{
  GrfChartContainerClass parent_class;
}GrfChartClass;




///**
// * @brief 2D Line plot
// */
//typedef struct _GrfPlotLine{
//  GrfPlot           parent_instance;
//  char            * format;
//  GrfChartCapStyle  dash_capstyle;
//  GrfChartJoinStyle dash_joinstyle;
//  grfbool_t         antialiased;
//  GrfScalar4D       color;
//  uint8_t         * dashes;
//  GrfChartDrawStyle drawstyle;
//  GrfChartFillStyle fillstyle;
//  char            * linestyle;
//  uint8_t           linewidth;
//  GrfChartMarker    marker;
//  GrfScalar4D       marker_edge_color;
//  uint8_t           marker_edge_width;
//  GrfScalar4D       marker_face_color;
//  GrfScalar4D       marker_face_color_alt;
//  uint8_t           marker_size;
//  uint8_t           marker_every;

//  GrfArray        * data_x;
//  GrfArray        * data_y;

//}GrfPlotLine;


///**
// * @brief 2D Bar plot
// */
//typedef struct _GrfPlotBar{
//  GrfPlot           parent_instance;/**< */
//  GrfScalar4D       color;          /**< */
//  GrfScalar4D       edge_color;     /**< */
//  uint8_t           linewidth;      /**< */
//  char**            tick_labels;    /**< */
//  GrfOrientation    orientation;    /**< */
//  GrfArray        * data_y;         /**< */
//}GrfPlotBar;

/* =======================
 * CREATION
 * =======================*/
/**
 * @brief Creates a new chart instance
 * @return
 */
GrfChart*
grf_chart_new();

/* =======================
 * MUTATORS
 * =======================*/
/**
 * @brief grf_chart_set_ylabel
 * @param chart
 * @param ylabel
 */
void
grf_chart_set_ylabel(GrfChart* chart, const char* ylabel);
/**
 * @brief grf_chart_set_xlabel
 * @param chart
 * @param xlabel
 */
void
grf_chart_set_xlabel(GrfChart* chart, const char* xlabel);
/**
 * @brief grf_chart_set_antialiased
 * @param chart
 * @param antialiased
 */
void
grf_chart_set_antialiased(GrfChart* chart, grfbool_t antialiased);
/**
 * @brief grf_chart_set_dash_capstyle
 * @param chart
 */
void
grf_chart_set_dash_capstyle(GrfChart* chart);
/**
 * @brief grf_chart_set_joinstyle
 * @param chart
 */
void
grf_chart_set_joinstyle(GrfChart* chart);
/**
 * @brief grf_chart_set_title
 * @param chart
 * @param title
 */
void
grf_chart_set_title(GrfChart* chart, char* title);

/* =======================
 * ACCESSORS
 * =======================*/
/**
 * @brief grf_chart_get_num_charts
 * @param chart
 * @return
 */
grfsize_t
grf_chart_get_num_charts(GrfChart* chart);


/**
 * @brief grf_chart_get_title
 * @param chart
 * @return
 */
char*
grf_chart_get_title(GrfChart* chart);
/**
 * @brief grf_chart_panel_get_title
 * @param chart_panel
 * @return
 */
char*
grf_chart_panel_get_title(GrfChartPanel* chart_panel);
/**
 * @brief grf_chart_get_last_panel
 * @param chart
 * @return
 */
GrfChartPanel*
grf_chart_get_last_panel(GrfChart* chart);

/* =======================
 * OPERATIONS
 * =======================*/

/**
 * @brief Creates a default 2D line plot in a chart
 * @param chart
 * @param data_y
 */
GrfPlot*
grf_chart_plot(GrfChart* chart, GrfArray* data_y);

/**
 * @brief Same as grf_chart_plot, but with custom line and marker settings
 * @param data_y
 * @param format
 * @return
 */
GrfChart*
grf_chart_plot_with_format(GrfArray* data_y,const char* format);

/**
 * @brief Creates a default 2D line
 * @param data_x
 * @param data_y
 * @return
 */
GrfChart*
grf_chart_plot2(GrfArray* data_x, GrfArray* data_y);

/**
 * @brief Creates a default 2D line plot with custom line and marker settings
 * @param data_y
 * @param format
 * @return
 */
GrfChart*
grf_chart_plot2_with_format(GrfArray* data_y,const char* format);

/**
 * @brief grf_chart_get_axis
 * @param chart
 * @param index
 * @return
 */
GrfAxis*
grf_chart_get_axis(GrfChart* chart, uint8_t index);
/**
 * @brief grf_chart_set_axis
 * @param chart
 * @param axis
 */
void
grf_chart_set_axis_range(GrfChart* chart, GrfRange* range);


void
grf_chart_show(GrfChart* chart);

uint8_t
grf_chart_waitkey();

#endif
