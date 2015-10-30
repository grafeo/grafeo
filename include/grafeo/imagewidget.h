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
#include <gtk/gtk.h>
BEGIN_DECLS

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
  void (*set_scale)      (GrfImageWidget* widget, float scale     , gboolean invalidate);
  void (*set_translation)(GrfImageWidget* widget, float x, float y, gboolean invalidate);
  void (*set_rotation)   (GrfImageWidget* widget, float angle     , gboolean invalidate);
  void (*set_transform)  (GrfImageWidget* widget, float* transform, gboolean invalidate);
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
 * @brief grf_imagewidget_get_translation
 * @param widget
 * @return
 */
float* grf_imagewidget_get_translation(GrfImageWidget* widget);
/**
 * @brief grf_imagewidget_get_scale
 * @param widget
 * @return
 */
float  grf_imagewidget_get_scale(GrfImageWidget* widget);
/**
 * @brief grf_imagewidget_get_rotation
 * @param widget
 * @return
 */
float  grf_imagewidget_get_rotation(GrfImageWidget* widget);
/**
 * @brief grf_imagewidget_get_transform
 * @param widget
 * @return
 */
float* grf_imagewidget_get_transform(GrfImageWidget* widget);

/*====================*
       MUTATORS
 *====================*/
/**
 * @brief grf_imagewidget_set_translation
 * @param widget
 * @param x
 * @param y
 * @param invalidate
 */
void
grf_imagewidget_set_translation(GrfImageWidget* widget, float x, float y, gboolean invalidate);
/**
 * @brief grf_imagewidget_set_rotation
 * @param widget
 * @param angle
 * @param invalidate
 */
void
grf_imagewidget_set_rotation(GrfImageWidget* widget, float angle, gboolean invalidate);
/**
 * @brief grf_imagewidget_set_scale
 * @param widget
 * @param scale
 * @param invalidate
 */
void
grf_imagewidget_set_scale(GrfImageWidget* widget, float scale, gboolean invalidate);
/**
 * @brief grf_imagewidget_set_transform
 * @param widget
 * @param transform
 * @param invalidate
 */
void
grf_imagewidget_set_transform(GrfImageWidget* widget, float* transform, gboolean invalidate);
/**
 * @brief grf_imagewidget_set_image
 * @param widget
 * @param image
 * @param invalidate
 */
void
grf_imagewidget_set_image(GrfImageWidget* widget, GrfArray* image, gboolean invalidate);
/*======================*
       OTHER METHODS
 *======================*/
void
grf_imagewidget_reset_transformations(GrfImageWidget* widget, gboolean invalidate);


END_DECLS
#endif
