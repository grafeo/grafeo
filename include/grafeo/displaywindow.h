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
#include <grafeo/config.h>

typedef enum{
  GRF_MOUSE_EVENT_MOVE    = 1,
  GRF_MOUSE_EVENT_PRESS   = 2,
  GRF_MOUSE_EVENT_RELEASE = 4,
  GRF_MOUSE_EVENT_DBLCLK  = 8,
  GRF_MOUSE_EVENT_WHEEL   = 16
} GrfMouseEventType;
#define GRF_MOUSE_FLAG_MASK     3
typedef enum{
  // Mouse Buttons
  GRF_MOUSE_FLAG_LBUTTON  = 0,
  GRF_MOUSE_FLAG_RBUTTON  = 1,
  GRF_MOUSE_FLAG_MBUTTON  = 2,

  // Modifiers
  GRF_MOUSE_FLAG_CTRLKEY  = 4,
  GRF_MOUSE_FLAG_SHIFTKEY = 8,
  GRF_MOUSE_FLAG_ALTKEY   = 16,
} GrfMouseEventFlags;

typedef enum{
  GRF_KEY_EVENT_PRESS,
  GRF_KEY_EVENT_RELEASE,
} GrfKeyEventType;

typedef void (*GrfMouseCallback)(GrfMouseEventType event, int x, int y, GrfMouseEventFlags flags, void* user_data);
typedef void (*GrfKeyCallback)(GrfKeyEventType event, int key_val, void* user_data);

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
 * @brief grf_displaywindow_new_with_name
 * @param name
 * @return
 */
GrfDisplayWindow*
grf_displaywindow_new_with_name(char* name);
/**
 * @brief grf_displaywindow_set_image
 * @param display
 * @param image
 */
void
grf_displaywindow_set_image(GrfDisplayWindow* display, GrfArray* image, gboolean invalidate);
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
 * @brief grf_displaywindow_connect_mouse_callback
 * @param window
 * @param mouse_callback
 * @param user_data
 */
void
grf_displaywindow_connect_mouse_callback(GrfDisplayWindow* window, GrfMouseCallback mouse_callback, void* user_data);
/**
 * @brief grf_displaywindow_disconnect_mouse_callback
 * @param window
 */
void
grf_displaywindow_disconnect_mouse_callback(GrfDisplayWindow* window);
/**
 * @brief grf_displaywindow_connect_key_callback
 * @param display
 * @param key_callback
 * @param user_data
 */
void
grf_displaywindow_connect_key_callback(GrfDisplayWindow* display, GrfKeyCallback key_callback, void* user_data);
/**
 * @brief grf_displaywindow_disconnect_key_callback
 * @param display
 */
void
grf_displaywindow_disconnect_key_callback(GrfDisplayWindow* display);
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
 * @return
 */
GrfTrackbar*
grf_displaywindow_get_trackbar_by_name(GrfDisplayWindow* display, char* name);
/**
 * @brief grf_displaywindow_quit_on_destroy
 * @param display
 * @param value
 */
void
grf_displaywindow_quit_on_destroy(GrfDisplayWindow* display, gboolean value);

END_DECLS
#endif
