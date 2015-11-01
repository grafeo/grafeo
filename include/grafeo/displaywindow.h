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
#ifndef GRF_DISPLAYWINDOW_H
#define GRF_DISPLAYWINDOW_H
#include <grafeo/array.h>
#include <grafeo/queue.h>
#include <grafeo/imagewidget.h>
#include <grafeo/trackbar.h>
BEGIN_DECLS

#define GRF_TYPE_DISPLAYWINDOW grf_displaywindow_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfDisplayWindow, grf_displaywindow, GRF, DISPLAYWINDOW, GObject)

typedef struct _GrfDisplayWindowClass{
  GObjectClass parent_class;
}GrfDisplayWindowClass;

/**
 * @brief grf_displaywindow_new
 * @return
 */
GrfDisplayWindow*
grf_displaywindow_new();
/**
 * @brief grf_displaywindow_set_image
 * @param display
 * @param image
 */
void
grf_displaywindow_set_image(GrfDisplayWindow* display, GrfArray* image);
/**
 * @brief grf_displaywindow_get_image
 * @param display
 * @return
 */
GrfArray*
grf_displaywindow_get_image(GrfDisplayWindow* display);
/**
 * @brief grf_displaywindow_show
 * @param display
 */
void
grf_displaywindow_show(GrfDisplayWindow* display);
/**
 * @brief grf_displaywindow_hide
 * @param display
 */
void
grf_displaywindow_hide(GrfDisplayWindow* display);
/**
 * @brief grf_displaywindow_set_name
 * @param display
 * @param name
 */
void
grf_displaywindow_set_name(GrfDisplayWindow* display, char* name);
/**
 * @brief grf_displaywindow_get_name
 * @param display
 * @return
 */
char*
grf_displaywindow_get_name(GrfDisplayWindow* display);
/**
 * @brief grf_displaywindow_add_trackbar
 * @param display
 * @param trackbar
 */
void
grf_displaywindow_add_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar);
/**
 * @brief grf_displaywindow_remove_trackbar
 * @param display
 * @param trackbar
 */
void
grf_displaywindow_remove_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar);
/**
 * @brief grf_displaywindow_get_trackbar_by_name
 * @param display
 * @param name
 */
void
grf_displaywindow_get_trackbar_by_name(GrfDisplayWindow* display, char* name);

END_DECLS
#endif
