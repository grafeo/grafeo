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
#ifndef GRF_AXIS_H
#define GRF_AXIS_H
#include <grafeo/chart.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef enum{
  GRF_TICK_DIR_IN,
  GRF_TICK_DIR_OUT,
  GRF_TICK_DIR_FULL
}GrfTickDirection;

typedef enum{
  GRF_AXIS_LOC_BOTTOM,
  GRF_AXIS_LOC_LEFT,
  GRF_AXIS_LOC_RIGHT,
  GRF_AXIS_LOC_TOP,
  GRF_AXIS_LOC_ORIGIN,
}GrfAxisLocation;

typedef enum{
  GRF_AXIS_SCALE_LINEAR,
  GRF_AXIS_SCALE_LOG
}GrfAxisScale;

typedef enum{
  GRF_AXIS_LIM_AUTO,
  GRF_AXIS_LIM_MANUAL
}GrfAxisLimitMode;

typedef enum{
  GRF_GRID_STYLE_SOLID,
  GRF_GRID_STYLE_DASHED,
  GRF_GRID_STYLE_DOTTED,
  GRF_GRID_STYLE_DASH_DOTTED,
  GRF_GRID_STYLE_NONE,
}GrfGridStyle;

#define GRF_TYPE_AXIS grf_axis_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfAxis, grf_axis, GRF, AXIS, GObject)

/**
 * @brief Base class for supporting multiple charts
 */
typedef struct _GrfAxisClass{
  GObjectClass parent_class;

  // Getters
  char*            (*get_label)           (GrfAxis* self);
  GrfRange         (*get_range)           (GrfAxis* self);
  grfbool_t        (*get_visible)         (GrfAxis* self);
  GrfScalar4D      (*get_color)           (GrfAxis* self);
  uint8_t          (*get_linewidth)       (GrfAxis* self);
  float*           (*get_tick_positions)  (GrfAxis* self);
  GrfTickDirection (*get_tick_direction)  (GrfAxis* self);
  uint8_t          (*get_tick_linewidth)  (GrfAxis* self);
  GrfScalar4D      (*get_tick_color)      (GrfAxis* self);
  grfbool_t        (*get_grid_visible)    (GrfAxis* self);
  GrfGridStyle     (*get_grid_style)      (GrfAxis* self);
  GrfAxisLocation  (*get_location)        (GrfAxis* self);
  GrfAxisLimitMode (*get_limitmode)       (GrfAxis* self);

  // Setters
  void (*set_label)         (GrfAxis* self, char* label);
  void (*set_range)         (GrfAxis* self, GrfRange range);
  void (*set_visible)       (GrfAxis* self, grfbool_t visible);
  void (*set_color)         (GrfAxis* self, GrfScalar4D color);
  void (*set_linewidth)     (GrfAxis* self, uint8_t linewidth);
  void (*set_tick_positions)(GrfAxis* self, float* tick_positions);
  void (*set_tick_direction)(GrfAxis* self, GrfTickDirection tick_direction);
  void (*set_tick_linewidth)(GrfAxis* self, uint8_t tick_linewidth);
  void (*set_tick_color)    (GrfAxis* self, GrfScalar4D tick_color);
  void (*set_grid_visible)  (GrfAxis* self, grfbool_t visible);
  void (*set_grid_style)    (GrfAxis* self, GrfGridStyle grid_style);
  void (*set_location)      (GrfAxis* self, GrfAxisLocation location);
  void (*set_limitmode)     (GrfAxis* self, GrfAxisLimitMode limitmode);

}GrfAxisClass;

/* =======================
 * CREATION
 * =======================*/
GrfAxis*
grf_axis_new();
void
grf_axis_free(GrfAxis* axis);

/* =======================
 * ACCESSORS
 * =======================*/
/**
 * @brief grf_axis_get_label
 * @param axis
 * @return
 */
char*
grf_axis_get_label(GrfAxis* axis);
/**
 * @brief grf_axis_get_range
 * @param axis
 * @return
 */
GrfRange
grf_axis_get_range(GrfAxis *axis);
/**
 * @brief grf_axis_get_visible
 * @param axis
 * @return
 */
grfbool_t
grf_axis_get_visible(GrfAxis *axis);
/**
 * @brief grf_axis_get_color
 * @param axis
 * @return
 */
GrfScalar4D
grf_axis_get_color(GrfAxis* axis);
/**
 * @brief grf_axis_get_linewidth
 * @param axis
 * @return
 */
uint8_t
grf_axis_get_linewidth(GrfAxis* axis);
/**
 * @brief grf_axis_get_tick_positions
 * @param axis
 * @return
 */
float*
grf_axis_get_tick_positions(GrfAxis* axis);
/**
 * @brief grf_axis_get_tick_direction
 * @param axis
 * @return
 */
GrfTickDirection
grf_axis_get_tick_direction(GrfAxis* axis);
/**
 * @brief grf_axis_get_tick_linewidth
 * @param axis
 * @return
 */
uint8_t
grf_axis_get_tick_linewidth(GrfAxis* axis);
/**
 * @brief grf_axis_get_tick_color
 * @param axis
 * @return
 */
GrfScalar4D
grf_axis_get_tick_color(GrfAxis* axis);
/**
 * @brief grf_axis_get_grid_visible
 * @param axis
 * @return
 */
grfbool_t
grf_axis_get_grid_visible(GrfAxis* axis);
/**
 * @brief grf_axis_get_grid_style
 * @param axis
 * @return
 */
GrfGridStyle
grf_axis_get_grid_style(GrfAxis* axis);
/**
 * @brief grf_axis_get_location
 * @param axis
 * @return
 */
GrfAxisLocation
grf_axis_get_location(GrfAxis* axis);
/**
 * @brief grf_axis_get_limitmode
 * @param axis
 * @return
 */
GrfAxisLimitMode
grf_axis_get_limitmode(GrfAxis* axis);

/* =======================
 * PUBLIC API: MUTATORS
 * =======================*/
/**
 * @brief grf_axis_set_label
 * @param axis
 * @param label
 */
void
grf_axis_set_label(GrfAxis *axis, char* label);
/**
 * @brief grf_axis_set_range
 * @param axis
 * @param range
 */
void
grf_axis_set_range(GrfAxis *axis, GrfRange range);
/**
 * @brief grf_axis_set_visible
 * @param axis
 * @param visible
 */
void
grf_axis_set_visible(GrfAxis* axis, grfbool_t visible);
/**
 * @brief grf_axis_set_color
 * @param axis
 * @param color
 */
void
grf_axis_set_color(GrfAxis* axis, GrfScalar4D color);
/**
 * @brief grf_axis_set_linewidth
 * @param axis
 * @param linewidth
 */
void
grf_axis_set_linewidth(GrfAxis* axis, uint8_t linewidth);
/**
 * @brief grf_axis_set_tick_positions
 * @param axis
 * @param tick_positions
 */
void
grf_axis_set_tick_positions(GrfAxis* axis, float* tick_positions);
/**
 * @brief grf_axis_set_tick_direction
 * @param axis
 * @param tick_direction
 */
void
grf_axis_set_tick_direction(GrfAxis* axis, GrfTickDirection tick_direction);
/**
 * @brief grf_axis_set_tick_linewidth
 * @param axis
 * @param tick_linewidth
 */
void
grf_axis_set_tick_linewidth(GrfAxis* axis, uint8_t tick_linewidth);
/**
 * @brief grf_axis_set_tick_color
 * @param axis
 * @param tick_color
 */
void
grf_axis_set_tick_color(GrfAxis* axis, GrfScalar4D tick_color);
/**
 * @brief grf_axis_set_grid_visible
 * @param axis
 * @param grid_visible
 */
void
grf_axis_set_grid_visible(GrfAxis* axis, grfbool_t grid_visible);
/**
 * @brief grf_axis_set_grid_style
 * @param axis
 * @param grid_style
 */
void
grf_axis_set_grid_style(GrfAxis* axis, GrfGridStyle grid_style);
/**
 * @brief grf_axis_set_location
 * @param axis
 * @param location
 */
void
grf_axis_set_location(GrfAxis* axis, GrfAxisLocation location);
/**
 * @brief grf_axis_set_limitmode
 * @param axis
 * @param limitmode
 */
void
grf_axis_set_limitmode(GrfAxis* axis, GrfAxisLimitMode limitmode);


G_END_DECLS

#endif
