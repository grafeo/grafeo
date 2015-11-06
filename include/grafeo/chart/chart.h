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

/**
 * @brief
 */
typedef struct _GrfAxis{
  ticks
}GrfAxis;

/**
 * @brief
 */
typedef struct _GrfLegend{

}GrfLegend;

/**
 * @brief
 */
typedef struct _GrfPlot{
  char     * label;
}GrfPlot;

/**
 * @brief
 */
typedef struct _GrfPlotLine{
  GrfPlot           parent_instance;
  char            * format;
  GrfChartCapStyle  capstyle;
  grfbool_t         antialiased;
  GrfScalar4D       color;
  uint8_t         * dashes;
  GrfChartDrawStyle drawstyle;
  GrfChartFillStyle fillstyle;
  char            * linestyle;
  uint8_t           linewidth;
  GrfChartMarker    marker;
  GrfScalar4D       marker_edge_color;
  uint8_t           marker_edge_width;
  GrfScalar4D       marker_face_color;
  GrfScalar4D       marker_face_color_alt;
  uint8_t           marker_size;
  uint8_t           marker_every;

  GrfArray        * data_x;
  GrfArray        * data_y;

}GrfPlotLine;

typedef struct _GrfPlotBar{
  GrfPlot           parent_instance;
  GrfScalar4D       color;
  GrfScalar4D       edge_color;
  uint8_t           linewidth;
  char**            tick_labels;
  GrfOrientation    orientation;
  GrfArray        * data_y;
}GrfPlotBar;

/**
 * @brief
 */
typedef struct _GrfAxes{
  GrfAxis         * axis;

}GrfAxes;

typedef enum{
  GRF_PROJECTION_CARTESIAN,
  GRF_PROJECTION_SPHERICAL
} GrfChartProjection;

/**
 * @brief
 */
typedef struct _GrfSubChart{
  grfdim_t   dim;   /**< Dimension of a chart (2 or 3) */
  GrfAxes  * axes;  /**< List of N axis, N = dim*/
  grfsize_t  num_plots;
  GrfPlot  * plots;  /**< List of M plots, M = num_plots */
  char     * title;  /**< Title of the chart */
  GrfLegend* legend; /**< Legend configuration of the chart */
}GrfSubChart;

/**
 * Base class for supporting multiple charts
 */
typedef struct _GrfChartComponent{
}GrfChartComponent;

/**
 * Leaf of the tree structure of charts
 */
typedef struct _GrfChartPanel{
  GrfChartComponent parent_instance;
  grfdim_t   dim;   /**< Dimension of a chart (2 or 3) */
  grfsize_t  num_plots; /**< Number of plots inside the chart */
  GrfAxes  * axes;  /**< List of N axis, N = dim*/
  GrfPlot  * plots;  /**< List of M plots, M = num_plots */
  char     * title;  /**< Title of the chart */
  GrfLegend* legend; /**< Legend configuration of the chart */
  GrfChartProjection projection; /** Cartesian or Spherical (polar in 2D) */
}GrfChartElement;

/**
 * Composite of charts
 * To get the
 */
typedef struct _GrfChartContainer{
  GrfChartComponent  parent_instance;
  grfdim_t           dim;          /**< Layout of container: 1D (row or column) or 2D (grid) */
  grfsize_t          num_children; /**< Total number of components */
  grfsize_t        * size;         /**< Number of components in each dimension */
  GrfChartComponent* children;     /**< List of components */
  char             * title;        /**< Title of the component */

}GrfChartContainer;


/**
 * @brief Main container for a chart
 *
 * It may contain several subcharts. Each subchart may
 * contain several plots
 */
typedef struct _GrfChart{
  GrfChartContainer parent_instance;
} GrfChart;

/* =======================
 * CREATION
 * =======================*/
/**
 * @brief Creates a new chart instance
 * @return
 */
GrfChart*
grf_chart_new();

/**
 * @brief Creates a default 2D line plot in a chart
 * @param chart
 * @param data_y
 */
void
grf_chart_plot(GrfChart* chart, Array* data_y);

/**
 * @brief Same as grf_chart_plot, but with custom line and marker settings
 * @param data_y
 * @param format
 * @return
 */
GrfChart*
grf_chart_plot_with_format(Array* data_y,const char* format);

/**
 * @brief Creates a default 2D line
 * @param data_x
 * @param data_y
 * @return
 */
GrfChart*
grf_chart_plot2(Array* data_x, Array* data_y);

/**
 * @brief Creates a default 2D line plot with custom line and marker settings
 * @param data_y
 * @param format
 * @return
 */
GrfChart*
grf_chart_plot2_with_format(Array* data_y,const char* format);

/* =======================
 * MUTATORS
 * =======================*/
void
grf_chart_set_ylabel(GrfChart* chart, const char* ylabel);

void
grf_chart_set_xlabel(GrfChart* chart, const char* xlabel);

void
grf_chart_set_antialiased(GrfChart* chart, grfbool antialiased);

void
grf_chart_set_dash_capstyle(GrfChart* chart);

void
grf_chart_set_joinstyle(GrfChart* chart);



void
grf_chart_show(GrfChart* chart);

uint8_t
grf_chart_waitkey();


#endif
