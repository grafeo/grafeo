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
typedef struct _GrfChartComponentPrivate{
  char* title; /**< Title of the component */
}GrfChartComponentPrivate;

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GrfChartComponent, grf_chart_component, G_TYPE_OBJECT)

static void
grf_chart_component_init(GrfChartComponent *self){
  GrfChartComponentPrivate* priv = grf_chart_component_get_instance_private(self);
  priv->title = NULL;
}

static void
grf_chart_component_class_init(GrfChartComponentClass *klass){
  klass->get_title = grf_chart_component_get_title;
  klass->set_title = grf_chart_component_set_title;
}

/*=========================
 *      PUBLIC API
 *=======================*/
char*
grf_chart_component_get_title(GrfChartComponent* chart_component){
  GrfChartComponentPrivate* priv = grf_chart_component_get_instance_private(chart_component);
  return priv->title;
}

void
grf_chart_component_set_title(GrfChartComponent* chart_component, char* title){
  GrfChartComponentPrivate* priv = grf_chart_component_get_instance_private(chart_component);
  priv->title = title;
}
