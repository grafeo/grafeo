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
#ifndef GRF_TRACKBAR_H
#define GRF_TRACKBAR_H
#include <grafeo/type.h>
#include <gtk/gtk.h>
BEGIN_DECLS

#define GRF_TYPE_TRACKBAR grf_trackbar_get_type()

G_DECLARE_DERIVABLE_TYPE(GrfTrackbar, grf_trackbar, GRF, TRACKBAR, GtkContainer)

struct _GrfTrackbarClass{
  GtkContainerClass parent_class;

  /* Keybinding signals */
  void (*set_min)        (GrfImageWidget* widget, float scale     , gboolean invalidate);
  void (*set_max)        (GrfImageWidget* widget, float x, float y, gboolean invalidate);
  void (*set_pos)        (GrfImageWidget* widget, float angle     , gboolean invalidate);
  void (*set_name)       (GrfImageWidget* widget, float* transform, gboolean invalidate);
};

typedef void (*GrfTrackbarCallback)(int pos);
typedef void (*GrfTrackbarDataCallback)(int pos, void* user_data);

/**
 * @brief grf_trackbar_new
 * @return
 */
GrfTrackbar*
grf_trackbar_new();
/**
 * @brief grf_trackbar_new_with_name
 * @param name
 * @return
 */
GrfTrackbar*
grf_trackbar_new_with_name(char* name);
/**
 * @brief grf_trackbar_set_max
 * @param trackbar
 * @param max_value
 */
void
grf_trackbar_set_max (GrfTrackbar* trackbar, int max_value);
/**
 * @brief grf_trackbar_set_min
 * @param trackbar
 * @param min_value
 */
void
grf_trackbar_set_min (GrfTrackbar* trackbar, int min_value);
/**
 * @brief grf_trackbar_set_pos
 * @param trackbar
 * @param pos
 */
void
grf_trackbar_set_pos (GrfTrackbar* trackbar, int pos);
/**
 * @brief grf_trackbar_set_name
 * @param trackbar
 * @param name
 */
void
grf_trackbar_set_name(GrfTrackbar* trackbar, char* name);
/**
 * @brief grf_trackbar_get_max
 * @param trackbar
 * @return
 */
int
grf_trackbar_get_max(GrfTrackbar* trackbar);
/**
 * @brief grf_trackbar_get_min
 * @param trackbar
 * @return
 */
int
grf_trackbar_get_min(GrfTrackbar* trackbar);
/**
 * @brief grf_trackbar_get_pos
 * @param trackbar
 * @return
 */
int
grf_trackbar_get_pos(GrfTrackbar* trackbar);
/**
 * @brief grf_trackbar_get_name
 * @param trackbar
 * @return
 */
char*
grf_trackbar_get_name(GrfTrackbar* trackbar);


#endif
