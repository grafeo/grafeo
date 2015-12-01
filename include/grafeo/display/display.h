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
#include <grafeo/core.h>
#include <grafeo/display.h>
BEGIN_DECLS

typedef enum{
  // Indices of list of window properties
  GRF_PROP_FULLSCREEN   = 0,
  GRF_PROP_AUTOSIZE     = 1,
  GRF_PROP_KEEPRATIO    = 2,
  GRF_PROP_OPENGL       = 3,

  // Window flags
  GRF_WINDOW_NORMAL     = 0,
  GRF_WINDOW_AUTOSIZE   = 1,
  GRF_WINDOW_FULLSCREEN = 4,
  GRF_WINDOW_OPENGL     = 8,
  GRF_WINDOW_KEEPRATIO  = 16,
  GRF_WINDOW_FREERATIO  = 0,
} GrfDisplayWindowFlag;

/*=================================
 * DISPLAY API
 *=================================*/
/**
 * @brief Init some display information
 * @param argc
 * @param argv
 */
void
grf_display_setup(int* argc, char*** argv);
/**
 * @brief Show a window to display an array
 * @param array the array representing the image to be displayed
 */
void
grf_display_show(GrfNDArray* array);
/**
 * @brief Lock the flow until a user presses the key
 * @return
 */
uint8_t
grf_display_waitkey();
/**
 * @brief grf_display_named
 * @param name
 */
void
grf_display_named(const char* name);
/**
 * @brief grf_display_connect_mouse_callback
 * @param name
 * @param mouse_callback
 * @param user_data
 */
void
grf_display_connect_mouse_callback(const char* name,
                                   GrfMouseCallback mouse_callback,
                                   void* user_data);
/**
 * @brief grf_display_disconnect_mouse_callback
 * @param name
 */
void
grf_display_disconnect_mouse_callback(const char* name);

/*=================================
 * DISPLAY TRACKBAR API
 *=================================*/
/**
 * @brief grf_display_add_trackbar
 * @param display_name
 * @param track_name
 * @param variable
 * @param min_value
 * @param max_value
 * @param grf_trackbar_changed_event
 * @return
 */
int
grf_display_add_trackbar(const char* display_name,
                         const char* track_name,
                         int* variable,
                         int min_value, int max_value,
                         GrfTrackbarCallback grf_trackbar_changed_event);
/**
 * @brief grf_display_add_trackbar_with_data
 * @param display_name
 * @param track_name
 * @param variable
 * @param min_value
 * @param max_value
 * @param grf_trackbar_changed_event
 * @param user_data
 * @return
 */
int
grf_display_add_trackbar_with_data(const char* display_name,
                                   const char* track_name,
                                   int* variable,
                                   int min_value,
                                   int max_value,
                                   GrfTrackbarDataCallback grf_trackbar_changed_event,
                                   void* user_data);
/**
 * @brief grf_display_get_trackbar_pos
 * @param display_name
 * @param track_name
 * @return
 */
int
grf_display_get_trackbar_pos(const char* display_name, const char* track_name);
/**
 * @brief grf_display_get_trackbar_min
 * @param display_name
 * @param track_name
 * @return
 */
int
grf_display_get_trackbar_min(const char* display_name, const char* track_name);
/**
 * @brief grf_display_get_trackbar_max
 * @param display_name
 * @param track_name
 * @return
 */
int
grf_display_get_trackbar_max(const char* display_name, const char* track_name);
/**
 * @brief grf_display_set_trackbar_pos
 * @param display_name
 * @param track_name
 * @param pos
 */
void
grf_display_set_trackbar_pos(const char* display_name, const char* track_name, int pos);
/**
 * @brief grf_display_set_trackbar_min
 * @param display_name
 * @param track_name
 * @param min_value
 */
void
grf_display_set_trackbar_min(const char* display_name, const char* track_name, int min_value);
/**
 * @brief grf_display_set_trackbar_max
 * @param display_name
 * @param track_name
 * @param max_value
 */
void
grf_display_set_trackbar_max(const char* display_name, const char* track_name, int max_value);
/**
 * @brief grf_display_set_trackbar_name
 * @param display_name
 * @param track_name
 * @param new_track_name
 */
void
grf_display_set_trackbar_name(const char* display_name, const char* track_name, char* new_track_name);

/*=================================
 * DISPLAY OPTIONS API
 *=================================*/
//void
//grf_display_add_options(const char* display_name, const char* options_name, int* variable, char** options, GrfOptionsCallback grf_option_changed_event);

/*=================================
 * DISPLAY CHECK API
 *=================================*/
//void
//grf_display_add_check(const char* display_name, const char* check_name, uint8_t* variable, GrfCheckCallback grf_check_changed_event);

END_DECLS
#endif
