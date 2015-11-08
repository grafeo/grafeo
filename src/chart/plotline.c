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
typedef struct _GrfPlotLinePrivate{
  char            * format;
  GrfChartCapStyle  dash_capstyle;
  GrfChartJoinStyle dash_joinstyle;
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
  GrfArray*         data;
} GrfPlotLinePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfPlotLine, grf_plot_line, GRF_TYPE_PLOT)

static void
grf_plot_line_init(GrfPlotLine *self){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(self);
  priv->data = NULL;
  priv->linestyle = "";
}

static void
grf_plot_line_class_init(GrfPlotLineClass *klass){

}

/*=========================
 *      PUBLIC API
 *=======================*/
GrfPlotLine*
grf_plot_line_new(){
  return g_object_new(GRF_TYPE_PLOT_LINE, NULL);
}

GrfPlotLine*
grf_plot_line_new_with_data(GrfArray* data){
  GrfPlotLine* plot_line = grf_plot_line_new();
  grf_plot_line_set_data(plot_line, data);
  return plot_line;
}

char*
grf_plot_line_get_format(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->format;
}

GrfChartCapStyle
grf_plot_line_get_dash_capstyle(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->dash_capstyle;
}

GrfChartJoinStyle
grf_plot_line_get_dash_joinstyle(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->dash_joinstyle;
}

grfbool_t
grf_plot_line_get_antialiased(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->antialiased;
}

GrfScalar4D
grf_plot_line_get_color(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->color;
}

uint8_t*
grf_plot_line_get_dashes(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->dashes;
}

GrfChartDrawStyle
grf_plot_line_get_drawstyle(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->drawstyle;
}

GrfChartFillStyle
grf_plot_line_get_fillstyle(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->fillstyle;
}

char*
grf_plot_line_get_linestyle(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->linestyle;
}

uint8_t
grf_plot_line_get_linewidth(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->linewidth;
}

GrfChartMarker
grf_plot_line_get_marker(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker;
}

GrfScalar4D
grf_plot_line_get_marker_edge_color(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_edge_color;
}

uint8_t
grf_plot_line_get_marker_edge_width(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_edge_width;
}

GrfScalar4D
grf_plot_line_get_marker_face_color(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_face_color;
}

GrfScalar4D
grf_plot_line_get_marker_face_color_alt(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_face_color_alt;
}

uint8_t
grf_plot_line_get_marker_size(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_size;
}

uint8_t
grf_plot_line_get_marker_every(GrfPlotLine* plot_line){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  return priv->marker_every;
}

void
grf_plot_line_set_format(GrfPlotLine* plot_line, char* format){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->format = format;
}

void
grf_plot_line_set_dash_capstyle(GrfPlotLine* plot_line, GrfChartCapStyle dash_capstyle){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->dash_capstyle = dash_capstyle;
}

void
grf_plot_line_set_dash_joinstyle(GrfPlotLine* plot_line, GrfChartJoinStyle dash_joinstyle){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->dash_joinstyle = dash_joinstyle;
}

void
grf_plot_line_set_antialiased(GrfPlotLine* plot_line, grfbool_t antialiased){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->antialiased = antialiased;
}

void
grf_plot_line_set_color(GrfPlotLine* plot_line, GrfScalar4D color){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->color = color;
}

void
grf_plot_line_set_dashes(GrfPlotLine* plot_line, uint8_t* dashes){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->dashes = dashes;
}


void
grf_plot_line_set_drawstyle(GrfPlotLine* plot_line, GrfChartDrawStyle drawstyle){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->drawstyle = drawstyle;
}


void
grf_plot_line_set_fillstyle(GrfPlotLine* plot_line, GrfChartFillStyle fillstyle){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->fillstyle = fillstyle;
}


void
grf_plot_line_set_linestyle(GrfPlotLine* plot_line, char* linestyle){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->linestyle = linestyle;
}


void
grf_plot_line_set_linewidth(GrfPlotLine* plot_line, uint8_t linewidth){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->linewidth = linewidth;
}

void
grf_plot_line_set_marker(GrfPlotLine* plot_line, GrfChartMarker marker){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker = marker;
}

void
grf_plot_line_set_marker_edge_color(GrfPlotLine* plot_line, GrfScalar4D marker_edge_color){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_edge_color = marker_edge_color;
}

void
grf_plot_line_set_marker_edge_width(GrfPlotLine* plot_line, uint8_t marker_edge_width){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_edge_width = marker_edge_width;
}

void
grf_plot_line_set_marker_face_color(GrfPlotLine* plot_line, GrfScalar4D marker_face_color){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_face_color = marker_face_color;
}

void
grf_plot_line_set_marker_face_color_alt(GrfPlotLine* plot_line, GrfScalar4D marker_face_color_alt){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_face_color_alt = marker_face_color_alt;
}

void
grf_plot_line_set_marker_size(GrfPlotLine* plot_line, uint8_t marker_size){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_size = marker_size;
}

void
grf_plot_line_set_marker_every(GrfPlotLine* plot_line, uint8_t marker_every){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->marker_every = marker_every;
}

void
grf_plot_line_set_data(GrfPlotLine* plot_line, GrfArray* data){
  GrfPlotLinePrivate* priv = grf_plot_line_get_instance_private(plot_line);
  priv->data = data;
}
