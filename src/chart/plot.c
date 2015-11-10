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
/**
 * @brief Base class for any plot in a chart
 */
typedef struct _GrfPlotPrivate{
  char     * label;
}GrfPlotPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfPlot, grf_plot, G_TYPE_OBJECT)

static void
grf_plot_class_init(GrfPlotClass *klass){
  klass->get_label = grf_plot_get_label;
  klass->set_label = grf_plot_set_label;
}

static void
grf_plot_init(GrfPlot *self){
  grf_plot_set_label(self, NULL);
}

/*=========================
 *      PUBLIC API
 *=======================*/
char*
grf_plot_get_label(GrfPlot* plot){
  GrfPlotPrivate* priv = grf_plot_get_instance_private(plot);
  return priv->label;
}

void
grf_plot_set_label(GrfPlot* plot, char* label){
  GrfPlotPrivate* priv = grf_plot_get_instance_private(plot);
  priv->label = label;
}
