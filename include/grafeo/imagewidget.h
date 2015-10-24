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
#ifndef GRAFEO_IMAGEWIDGET_H
#define GRAFEO_IMAGEWIDGET_H
#include <grafeo/array.h>
#include <grafeo/image.h>

BEGIN_DECLS
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define GRAFEO_TYPE_IMAGEWIDGET imagewidget_get_type()
G_DECLARE_DERIVABLE_TYPE(ImageWidget, imagewidget, GRAFEO, IMAGEWIDGET, GtkWidget)

struct _ImageWidgetClass{
  GtkWidgetClass parent_class;
};

typedef enum{
  // Indices of list of window properties
  GRAFEO_PROP_FULLSCREEN   = 0,
  GRAFEO_PROP_AUTOSIZE     = 1,
  GRAFEO_PROP_KEEPRATIO    = 2,
  GRAFEO_PROP_OPENGL       = 3,

  // Window flags
  GRAFEO_WINDOW_NORMAL     = 0,
  GRAFEO_WINDOW_AUTOSIZE   = 1,
  GRAFEO_WINDOW_FULLSCREEN = 4,
  GRAFEO_WINDOW_OPENGL     = 8,
  GRAFEO_WINDOW_KEEPRATIO  = 16,
  GRAFEO_WINDOW_FREERATIO  = 0,
} WindowFlag;

GtkWidget  * imagewidget_new();
void         imagewidget_set_image(ImageWidget* widget, Array* image);

END_DECLS
#endif
