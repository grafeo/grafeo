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
typedef struct _GrfChartWindowPrivate{
  GrfChartWidget* widget;
  GtkWidget* toolbar;
  GtkWidget* statusbar;
  GtkWidget* window;
  GtkWidget* box;
}GrfChartWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfChartWindow, grf_chart_window, G_TYPE_OBJECT)

static void
grf_chart_window_class_init(GrfChartWindowClass *klass){

}

static void
grf_chart_window_init(GrfChartWindow *self){
  GrfChartWindowPrivate* priv = grf_chart_window_get_instance_private(self);
  priv->window    = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  priv->statusbar = gtk_statusbar_new();
  priv->toolbar   = gtk_toolbar_new();
  priv->box       = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  priv->widget    = grf_chart_widget_new();

  gtk_container_add(GTK_CONTAINER(priv->window),priv->box);
  gtk_box_pack_start(GTK_BOX(priv->box), priv->toolbar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(priv->box), priv->widget,  TRUE,  TRUE,  0);
  gtk_box_pack_end(GTK_BOX(priv->box), priv->statusbar, FALSE, FALSE, 0);
}


/*=========================
 *      PUBLIC API
 *=======================*/
GrfChartWindow*
grf_chart_window_new(){
  return GRF_CHART_WINDOW(g_object_new(GRF_TYPE_CHART_WINDOW, NULL));
}

void
grf_chart_window_show(GrfChartWindow* window){
  GrfChartWindowPrivate* priv = grf_chart_window_get_instance_private(window);
  gtk_widget_show_all(priv->window);
}
