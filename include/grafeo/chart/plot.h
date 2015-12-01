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
#ifndef GRF_PLOT_H
#define GRF_PLOT_H
#include <grafeo/core.h>
#include <grafeo/chart.h>
G_BEGIN_DECLS

#define GRF_TYPE_PLOT grf_plot_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfPlot, grf_plot, GRF, PLOT, GObject)

typedef struct _GrfPlotClass{
  GObjectClass parent_class;
  void  (*set_label) (GrfPlot* plot, char* label);
  char* (*get_label) (GrfPlot* plot);
}GrfPlotClass;

/**
 * @brief grf_plot_get_label
 * @param plot
 * @return
 */
char*
grf_plot_get_label(GrfPlot* plot);

/**
 * @brief grf_plot_set_label
 * @param plot
 * @param label
 */
void
grf_plot_set_label(GrfPlot* plot, char* label);


G_END_DECLS
#endif
