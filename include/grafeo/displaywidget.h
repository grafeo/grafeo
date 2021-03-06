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
#ifndef GRAFEO_DISPLAYWIDGET_H
#define GRAFEO_DISPLAYWIDGET_H
#include <grafeo/imagewidget.h>
BEGIN_DECLS

#define GRAFEO_TYPE_DISPLAYWIDGET displaywidget_get_type()
G_DECLARE_DERIVABLE_TYPE(DisplayWidget, displaywidget, GRAFEO, DISPLAYWIDGET, GtkWindow)

/**
  * @brief Class for DisplayWidget static properties and members
  */
typedef struct _DisplayWidgetClass{
  GtkWindowClass parent_class;
}DisplayWidgetClass;

/**
 * @brief Creates a new DisplayWidget
 */
DisplayWidget* displaywidget_new();

END_DECLS
#endif
