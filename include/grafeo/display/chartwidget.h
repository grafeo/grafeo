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
#ifndef GRF_CHART_WIDGET_H
#define GRF_CHART_WIDGET_H
#include <grafeo/chart.h>
#include <grafeo/display.h>
#include <gtk/gtk.h>
#include <goocanvas.h>
G_BEGIN_DECLS

/*====================*
    CLASS DEFINITION
 *====================*/
#define GRF_TYPE_CHART_WIDGET grf_chart_widget_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfChartWidget, grf_chart_widget, GRF, CHART_WIDGET, GooCanvas)

typedef struct _GrfChartWidgetClass{
  GooCanvasClass parent_class;

  void (*set_scale)       (GrfChartWidget* widget, float scale, gboolean invalidate);
  void (*set_translation) (GrfChartWidget* widget, float x, float y, gboolean invalidate);
  void (*set_rotation)    (GrfChartWidget* widget, float angle, gboolean invalidate);
  void (*set_transform)   (GrfChartWidget* widget, float* transform, gboolean invalidate);
}GrfChartWidgetClass;

/*====================*
      CONSTRUCTORS
 *====================*/
/**
 * @brief grf_chart_widget_new
 * @return
 */
GrfChartWidget*
grf_chart_widget_new();
/*====================*
       ACCESSORS
 *====================*/
/**
 * @brief grf_chart_widget_get_chart
 * @param chart_widget
 * @return
 */
GrfChart*
grf_chart_widget_get_chart(GrfChartWidget* chart_widget);

/*====================*
        MUTATORS
 *====================*/
/**
 * @brief grf_chart_widget_set_chart
 * @param chart_widget
 * @param chart
 */
void
grf_chart_widget_set_chart(GrfChartWidget* chart_widget, GrfChart* chart);

/*====================*
        ACTIONS
 *====================*/
void
grf_chart_widget_zoom();

void
grf_chart_widget_zoom_in();

void
grf_chart_widget_zoom_out();

void
grf_chart_widget_translate();


G_END_DECLS
#endif
