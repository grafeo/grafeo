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
#ifndef GRF_IMAGEWIDGET_H
#define GRF_IMAGEWIDGET_H
#include <grafeo/array.h>
#include <grafeo/image.h>

BEGIN_DECLS
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <grafeo/marshal.h>

/*====================*
    CLASS DEFINITION
 *====================*/
#define GRF_TYPE_IMAGEWIDGET grf_imagewidget_get_type()

/**
 * @brief Visual Class for showing images
 *
 * Based on GtkWidget, just draws an image, but with several functionalities:
 *
 * - zoom in
 * - zoom out
 * - zoom with a specific value
 * - offset (see part of the image)
 * - fitting mode
 * - scroll
 */
G_DECLARE_DERIVABLE_TYPE(GrfImageWidget, grf_imagewidget, GRF, IMAGEWIDGET, GtkWidget)

struct _GrfImageWidgetClass{
  GtkWidgetClass parent_class;

  /* Keybinding signals */
  void (*set_zoom)       (GrfImageWidget *imagewidget, gdouble zoom);
  void (*zoom_in)        (GrfImageWidget *imagewidget);
  void (*zoom_out)       (GrfImageWidget *imagewidget);
  void (*set_fitting)    (GrfImageWidget *imagewidget, GrfFittingMode fitting);
  void (*scroll)         (GrfImageWidget *imagewidget, GtkScrollType scroll_x, GtkScrollType scroll_y);
  void (*pixbuf_changed) (GrfImageWidget* imagewidget);
};

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
} WindowFlag;

/*====================*
      CONSTRUCTORS
 *====================*/
GtkWidget  *   grf_imagewidget_new();

/*====================*
       ACCESSORS
 *====================*/
/**
 * @brief grf_imagewidget_get_viewport
 * @param widget
 * @param rect
 * @return
 */
gboolean
grf_imagewidget_get_viewport (GrfImageWidget* widget, GdkRectangle* rect);
/**
 * @brief grf_imagewidget_get_draw_rect
 * @param widget
 * @param rect
 * @return
 */
gboolean
grf_imagewidget_get_draw_rect(GrfImageWidget* widget, GdkRectangle* rect);
/**
 * @brief Get fitting mode
 * @param widget
 * @return
 */
GrfFittingMode
grf_imagewidget_get_fitting  (GrfImageWidget* widget);
/**
 * @brief Get pixbuf related to current image
 * @param widget
 * @return
 */
GdkPixbuf*
grf_imagewidget_get_pixbuf   (GrfImageWidget* widget);
/**
 * @brief Get current zoom value
 * @param widget
 * @return
 */
gdouble
grf_imagewidget_get_zoom     (GrfImageWidget* widget);

/*====================*
       MUTATORS
 *====================*/
/**
 * @brief grf_imagewidget_set_offset
 * @param widget
 * @param x
 * @param y
 * @param invalidate
 */
void           grf_imagewidget_set_offset   (GrfImageWidget* widget, gdouble x, gdouble y, gboolean invalidate);
/**
 * @brief grf_imagewidget_set_zoom
 * @param widget
 * @param zoom
 */
void           grf_imagewidget_set_zoom     (GrfImageWidget* widget, double zoom);
/**
 * @brief grf_imagewidget_set_pixbuf
 * @param widget
 * @param pixbuf
 * @param reset_fit
 */
void           grf_imagewidget_set_pixbuf   (GrfImageWidget* widget, GdkPixbuf* pixbuf, gboolean reset_fit);
/**
 * @brief grf_imagewidget_set_fitting
 * @param widget
 * @param fitting
 */
void           grf_imagewidget_set_fitting  (GrfImageWidget* widget, GrfFittingMode fitting);

/*======================*
       OTHER METHODS
 *======================*/
/**
 * @brief grf_imagewidget_zoom_in
 * @param widget
 */
void           grf_imagewidget_zoom_in      (GrfImageWidget* widget);
/**
 * @brief grf_imagewidget_zoom_out
 * @param widget
 */
void           grf_imagewidget_zoom_out     (GrfImageWidget* widget);
/**
 * @brief grf_imagewidget_set_image
 * @param widget
 * @param image
 */
void           grf_imagewidget_set_image(GrfImageWidget* widget, GrfArray* image);

END_DECLS
#endif
