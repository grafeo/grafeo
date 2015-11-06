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
#ifndef GRF_CHART_COMPONENT_H
#define GRF_CHART_COMPONENT_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GRF_TYPE_CHART_COMPONENT grf_chart_component_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfChartComponent, grf_chart_component, GRF, CHART_COMPONENT, GObject)

/**
 * @brief Base class for supporting multiple charts
 */
typedef struct _GrfChartComponentClass{
  GObjectClass parent_class;

  char* (*get_title)(GrfChartComponent* self);
  void  (*set_title)(GrfChartComponent* self, char* title);
}GrfChartComponentClass;

/**
 * @brief grf_chart_component_get_title
 * @param chart_component
 * @return
 */
char*
grf_chart_component_get_title(GrfChartComponent* chart_component);

/**
 * @brief grf_chart_component_set_title
 * @param chart_component
 * @param title
 */
void
grf_chart_component_set_title(GrfChartComponent* chart_component, char* title);

G_END_DECLS

#endif
