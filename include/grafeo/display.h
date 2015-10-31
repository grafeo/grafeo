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
#ifndef GRF_DISPLAY_H
#define GRF_DISPLAY_H
#include <grafeo/array.h>
#include <grafeo/queue.h>
#include <grafeo/imagewidget.h>
BEGIN_DECLS

#define GRF_TYPE_DISPLAY grf_display_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfDisplay, grf_display, GRF, DISPLAY, GObject)

typedef struct _GrfDisplayClass{
  GObjectClass parent_class;
}GrfDisplayClass;

/**
 * @brief Show a window to display an array
 * @param array the array representing the image to be displayed
 */
void    grf_display_show(GrfArray* array);
/**
 * @brief Lock the flow until a user presses the key
 * @return
 */
uint8_t grf_display_waitkey();
/**
 * @brief Init some display information
 */
void    grf_display_setup();
/**
 * @brief grf_display_named
 * @param name
 */
void
grf_display_named(const char* name);
END_DECLS
#endif
