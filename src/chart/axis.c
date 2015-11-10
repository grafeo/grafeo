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


/* =======================
 * PRIVATE API
 * =======================*/


typedef struct _GrfAxisPrivate{
  char*            label;
  GrfRange         range;
  grfbool_t        visible;
  GrfScalar4D      color;
  uint8_t          linewidth;

  float*           tick_positions;
  GrfTickDirection tick_direction;
  uint8_t          tick_linewidth;
  GrfScalar4D      tick_color;

  grfbool_t        grid_visible;
  GrfGridStyle     grid_style;

  GrfAxisLocation  location;
  GrfAxisLimitMode limitmode;
}GrfAxisPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfAxis, grf_axis, G_TYPE_OBJECT)

static void
grf_axis_init(GrfAxis* self){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(self);
  priv->label          = NULL;
  grf_range_all(&priv->range);
  priv->visible        = TRUE;
  grf_scalar4D_fill(priv->color,0,0,0,255);
  priv->linewidth      = 1;

  priv->tick_positions = NULL;
  priv->tick_direction = GRF_TICK_DIR_IN;
  priv->tick_linewidth = 1;
  grf_scalar4D_fill(priv->tick_color,127,127,127,255);

  priv->grid_visible   = TRUE;
  priv->grid_style     = GRF_GRID_STYLE_SOLID;

  priv->location  = GRF_AXIS_LOC_ORIGIN;
  priv->limitmode = GRF_AXIS_LIM_AUTO;
}

static void
grf_axis_class_init(GrfAxisClass *klass){
  // Getters
  klass->get_label           = grf_axis_get_label;
  klass->get_range           = grf_axis_get_range;
  klass->get_visible         = grf_axis_get_visible;
  klass->get_color           = grf_axis_get_color;
  klass->get_linewidth       = grf_axis_get_linewidth;
  klass->get_tick_positions  = grf_axis_get_tick_positions;
  klass->get_tick_direction  = grf_axis_get_tick_direction;
  klass->get_tick_linewidth  = grf_axis_get_tick_linewidth;
  klass->get_tick_color      = grf_axis_get_tick_color;
  klass->get_grid_visible    = grf_axis_get_grid_visible;
  klass->get_grid_style      = grf_axis_get_grid_style;
  klass->get_location        = grf_axis_get_location;
  klass->get_limitmode       = grf_axis_get_limitmode;

  // Setters
  klass->set_label           = grf_axis_set_label;
  klass->set_range           = grf_axis_set_range;
  klass->set_visible         = grf_axis_set_visible;
  klass->set_color           = grf_axis_set_color;
  klass->set_linewidth       = grf_axis_set_linewidth;
  klass->set_tick_positions  = grf_axis_set_tick_positions;
  klass->set_tick_direction  = grf_axis_set_tick_direction;
  klass->set_tick_linewidth  = grf_axis_set_tick_linewidth;
  klass->set_tick_color      = grf_axis_set_tick_color;
  klass->set_grid_visible    = grf_axis_set_grid_visible;
  klass->set_grid_style      = grf_axis_set_grid_style;
  klass->set_location        = grf_axis_set_location;
  klass->set_limitmode       = grf_axis_set_limitmode;
}
/* =======================
 * PUBLIC API: CREATION
 * =======================*/
GrfAxis*
grf_axis_new(){
  return g_object_new(GRF_TYPE_AXIS, NULL);
}

void
grf_axis_free(GrfAxis* axis){
  g_clear_object(&axis);
}

/* =======================
 * PUBLIC API: ACCESSORS
 * =======================*/
char*
grf_axis_get_label(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->label;
}

GrfRange
grf_axis_get_range(GrfAxis *axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->range;
}

grfbool_t
grf_axis_get_visible(GrfAxis *axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->visible;
}

GrfScalar4D
grf_axis_get_color(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->color;
}

uint8_t
grf_axis_get_linewidth(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->linewidth;
}

float*
grf_axis_get_tick_positions(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->tick_positions;
}

GrfTickDirection
grf_axis_get_tick_direction(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->tick_direction;
}

uint8_t
grf_axis_get_tick_linewidth(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->tick_linewidth;
}

GrfScalar4D
grf_axis_get_tick_color(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->tick_color;
}

grfbool_t
grf_axis_get_grid_visible(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->grid_visible;
}

GrfGridStyle
grf_axis_get_grid_style(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->grid_style;
}

GrfAxisLocation
grf_axis_get_location(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->location;
}

GrfAxisLimitMode
grf_axis_get_limitmode(GrfAxis* axis){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  return priv->limitmode;
}

/* =======================
 * PUBLIC API: MUTATORS
 * =======================*/

void
grf_axis_set_label(GrfAxis *axis, char* label){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->label = label;
}

void
grf_axis_set_range(GrfAxis *axis, GrfRange range){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->range = range;
}

void
grf_axis_set_visible(GrfAxis* axis, grfbool_t visible){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->visible = visible;
}

void
grf_axis_set_color(GrfAxis* axis, GrfScalar4D color){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->color = color;
}

void
grf_axis_set_linewidth(GrfAxis* axis, uint8_t linewidth){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->linewidth = linewidth;
}

void
grf_axis_set_tick_positions(GrfAxis* axis, float* tick_positions){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->tick_positions = tick_positions;
}

void
grf_axis_set_tick_direction(GrfAxis* axis, GrfTickDirection tick_direction){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->tick_direction = tick_direction;
}

void
grf_axis_set_tick_linewidth(GrfAxis* axis, uint8_t tick_linewidth){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->tick_linewidth = tick_linewidth;
}

void
grf_axis_set_tick_color(GrfAxis* axis, GrfScalar4D tick_color){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->tick_color = tick_color;
}

void
grf_axis_set_grid_visible(GrfAxis* axis, grfbool_t grid_visible){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->grid_visible = grid_visible;
}

void
grf_axis_set_grid_style(GrfAxis* axis, GrfGridStyle grid_style){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->grid_style = grid_style;
}

void
grf_axis_set_location(GrfAxis* axis, GrfAxisLocation location){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->location = location;
}

void
grf_axis_set_limitmode(GrfAxis* axis, GrfAxisLimitMode limitmode){
  GrfAxisPrivate *priv = grf_axis_get_instance_private(axis);
  priv->limitmode = limitmode;
}
