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
typedef struct _GrfLegendPrivate{
  GrfLegendLocation location;
  uint8_t           num_cols;
  uint8_t           num_points;
  uint8_t           scatter_points;
  grfbool_t         with_frame;
  grfbool_t         with_round_edges;
  grfbool_t         with_shadow;
  float             frame_alpha;
  char*             title;
}GrfLegendPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GrfLegend, grf_legend, G_TYPE_OBJECT)


static void
grf_legend_init(GrfLegend *self){

}

static void
grf_legend_class_init(GrfLegendClass *klass){

}


/*=========================
 *      PUBLIC API
 *=======================*/
GrfLegend*
grf_legend_new(){
  return g_object_new(GRF_TYPE_LEGEND, NULL);
}

uint8_t
grf_legend_get_num_cols(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->num_cols;
}

uint8_t
grf_legend_get_num_points(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->num_points;
}

uint8_t
grf_legend_get_scatter_points(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->scatter_points;
}

grfbool_t
grf_legend_get_with_frame(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->with_frame;
}

grfbool_t
grf_legend_get_with_round_edges(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->with_round_edges;
}

grfbool_t
grf_legend_get_with_shadow(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->with_shadow;
}

float
grf_legend_get_frame_alpha(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->frame_alpha;
}

char*
grf_legend_get_title(GrfLegend* legend){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  return priv->title;
}

void
grf_legend_set_num_cols(GrfLegend* legend, uint8_t num_cols){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->num_cols = num_cols;
}

void
grf_legend_set_num_points(GrfLegend* legend, uint8_t num_points){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->num_points = num_points;
}

void
grf_legend_set_scatter_points(GrfLegend* legend, uint8_t scatter_points){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->scatter_points = scatter_points;
}

void
grf_legend_set_with_frame(GrfLegend* legend, grfbool_t with_frame){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->with_frame = with_frame;
}

void
grf_legend_set_with_round_edges(GrfLegend* legend, grfbool_t with_round_edges){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->with_round_edges = with_round_edges;
}

void
grf_legend_set_with_shadow(GrfLegend* legend, grfbool_t with_shadow){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->with_shadow = with_shadow;
}

void
grf_legend_set_frame_alpha(GrfLegend* legend, float frame_alpha){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->frame_alpha = frame_alpha;
}

void
grf_legend_set_frame_title(GrfLegend* legend, char* title){
  GrfLegendPrivate* priv = grf_legend_get_instance_private(legend);
  priv->title = title;
}
