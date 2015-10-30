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
#include <grafeo/displaywidget.h>

/*=================================
 * PRIVATE API
 *=================================*/
typedef struct _DisplayWidgetPrivate{
  ImageWidget* imagewidget;
  GtkBox     * box;
}DisplayWidgetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(DisplayWidget, displaywidget, GTK_TYPE_WINDOW)

static void
displaywidget_init(DisplayWidget* self){
  DisplayWidgetPrivate* priv = displaywidget_get_instance_private(self);
  priv->imagewidget          = imagewidget_new();
  priv->box                  = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);

  gtk_box_pack_start(priv->box,priv->imagewidget,TRUE,TRUE,0);
  gtk_container_add(GTK_CONTAINER(self),priv->box);
}

static void
displawidget_class_init(DisplayWidgetClass *klass){

}

/*=================================
 * PUBLIC API
 *=================================*/
DisplayWidget* displaywidget_new(){
  return GRAFEO_DISPLAYWIDGET(gtk_widget_new(GRAFEO_TYPE_DISPLAYWIDGET, NULL));
}

