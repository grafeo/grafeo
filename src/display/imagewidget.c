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
#include <grafeo/display.h>

/*=================================
 * PRIVATE API
 *=================================*/
// Private members
typedef struct _GrfImageWidgetPrivate{
  float            transform[6]; /**< Matrix for affine transformations*/

  // Paramenters for separate transformations
  float            scale;        /**< Uniform scaling*/
  float            translation_x;/**< Affine translation X axis*/
  float            translation_y;/**< Affine translation Y axis*/
  float            angle;        /**< Affine rotation*/

  GrfNDArray*        image_original;
  GrfNDArray*        image_output;
  cairo_surface_t* image_surface;
} GrfImageWidgetPrivate;

static void
grf_i_display_widget_init(GrfIDisplayWidgetInterface *iface){

}

// ImageWidget definition
#define G_DEFINE_TYPE_WITH_CODE_PRIVATE(TN, t_n, T_P, _C_)	    _G_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, 0) {G_ADD_PRIVATE (TN);_C_;} _G_DEFINE_TYPE_EXTENDED_END()
G_DEFINE_TYPE_WITH_CODE_PRIVATE(GrfImageWidget, grf_imagewidget, GTK_TYPE_WIDGET, G_IMPLEMENT_INTERFACE(GRF_TYPE_I_DISPLAY_WIDGET, grf_i_display_widget_init))

// Local Forward Declarations
static void
grf_imagewidget_get_preferred_width (GtkWidget     * widget, gint *minimal_width, gint *natural_width);
static void
grf_imagewidget_get_preferred_height(GtkWidget     * widget, gint *minimal_height, gint *natural_height);
static gboolean
grf_imagewidget_draw                (GtkWidget     * widget, cairo_t* cr);
static void
grf_imagewidget_realize             (GtkWidget     * widget);
static void
get_size                            (GrfImageWidget* imagewidget, GtkOrientation direction, int *minimal, int* natural);

// Initialize GrfImageWidget
static void
grf_imagewidget_init(GrfImageWidget *imagewidget){
  GtkWidget* widget = GTK_WIDGET(imagewidget);
  gtk_widget_set_can_focus(widget, TRUE);

  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(imagewidget);
  priv->image_original = NULL;
  priv->image_output   = NULL;
  priv->image_surface  = NULL;

  priv->scale          = 1.0;
  priv->translation_x  = 0.0;
  priv->translation_y  = 0.0;
  priv->angle          = 0.0;
}

// Override Event Functions
static void
grf_imagewidget_class_init(GrfImageWidgetClass *klass){

  GtkWidgetClass* widget_class      = GTK_WIDGET_CLASS(klass);
  widget_class->realize             = grf_imagewidget_realize;

  widget_class->get_preferred_width = grf_imagewidget_get_preferred_width;
  widget_class->get_preferred_height= grf_imagewidget_get_preferred_height;
  widget_class->draw                = grf_imagewidget_draw;

  klass->set_translation= grf_imagewidget_set_translation;
  klass->set_rotation   = grf_imagewidget_set_rotation;
  klass->set_scale      = grf_imagewidget_set_scale;
  klass->set_transform  = grf_imagewidget_set_transform;

}

// Get Width
static void
grf_imagewidget_get_preferred_width(GtkWidget* widget,
                                gint *minimal_width,
                                gint *natural_width){
  get_size(GRF_IMAGEWIDGET(widget), GTK_ORIENTATION_HORIZONTAL, minimal_width, natural_width);
}

// Get Height
static void
grf_imagewidget_get_preferred_height(GtkWidget* widget,
                                 gint *minimal_height,
                                 gint *natural_height){
  get_size(GRF_IMAGEWIDGET(widget), GTK_ORIENTATION_VERTICAL, minimal_height, natural_height);
}

// Draw image
static gboolean
grf_imagewidget_draw(GtkWidget *widget, cairo_t *cr){
  GrfImageWidget       * imagewidget = GRF_IMAGEWIDGET(widget);
  GrfImageWidgetPrivate*        priv = grf_imagewidget_get_instance_private(imagewidget);

  if(priv->image_surface){
    cairo_save(cr);
    cairo_translate(cr,priv->translation_x,priv->translation_y);
    cairo_scale(cr,priv->scale,priv->scale);
    cairo_set_source_surface(cr,priv->image_surface,0,0);
    cairo_pattern_set_filter(cairo_get_source(cr),CAIRO_FILTER_FAST);
    cairo_paint(cr);
    cairo_fill(cr);
    cairo_restore(cr);
  }

  return TRUE;
}

// Connect to GdkWindow to receive mouse events
static void
grf_imagewidget_realize(GtkWidget* widget){
  GdkWindowAttr attributes;
  gint          attributes_mask;
  GtkAllocation allocation;

  gtk_widget_get_allocation(widget, &allocation);

  attributes.x             = allocation.x;
  attributes.y             = allocation.y;
  attributes.width         = allocation.width;
  attributes.height        = allocation.height;
  attributes.wclass        = GDK_INPUT_OUTPUT;
  attributes.window_type   = GDK_WINDOW_CHILD;
  attributes.visual        = gtk_widget_get_visual (widget);
  attributes.event_mask    = gtk_widget_get_events (widget) |
      GDK_EXPOSURE_MASK       | GDK_BUTTON_PRESS_MASK       |
      GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK;
  attributes_mask = GDK_WA_X  | GDK_WA_Y | GDK_WA_VISUAL;
  GdkWindow* window_parent = gtk_widget_get_parent_window(widget);
  GdkWindow* window        = gdk_window_new(window_parent,&attributes, attributes_mask);
  gtk_widget_set_window(widget,window);
  gtk_widget_register_window(widget, window);
  gtk_widget_set_realized(widget, TRUE);
}

// Get Preferred Size (Width or Height)
static void
get_size(GrfImageWidget* imagewidget, GtkOrientation direction, int* minimal, int* natural){
  uint8_t             index;
  uint16_t            size_min;
  GrfImageWidgetPrivate* priv           = grf_imagewidget_get_instance_private(imagewidget);
  uint32_t          * size_original  = priv->image_original? priv->image_original->size: NULL;
  uint32_t          * size_output    = priv->image_output  ? priv->image_output->size  : NULL;
  switch(direction){
  case GTK_ORIENTATION_HORIZONTAL:
    index = 1; size_min = 320;
    break;
  case GTK_ORIENTATION_VERTICAL:
    index = 0; size_min = 240;
    break;
  }
  if(priv->image_original != NULL)
    *minimal = size_original[index];
  else
    *minimal = size_min;
  if(priv->image_output != NULL)
    *natural = *minimal < size_output[index]?size_output[index]:*minimal;
  else
    *natural = *minimal;
}

/*=================================
 * PUBLIC API
 *=================================*/
// Creates a new GrfImageWidget
GtkWidget*
grf_imagewidget_new(){
  return gtk_widget_new(GRF_TYPE_IMAGEWIDGET, NULL);
}

// Update image
void
grf_imagewidget_set_image(GrfImageWidget* widget, GrfNDArray* image, gboolean invalidate){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  // Updating image variable
  priv->image_original        = image;

  // Converting to appropriate format
  if(image->dim == 2 || image->size[2] == 1)
    priv->image_output   = grf_image_cvt_color(image,GRF_GRAY, GRF_BGRA);
  else
    priv->image_output   = grf_image_cvt_color(image,GRF_RGB, GRF_BGRA);

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,priv->image_output->size[1]);
  priv->image_surface = cairo_image_surface_create_for_data(priv->image_output->data_uint8,
                                                            CAIRO_FORMAT_RGB24,
                                                            priv->image_output->size[1],
                                                            priv->image_output->size[0],
                                                            stride);

  if(invalidate)
    gtk_widget_queue_draw(GTK_WIDGET(widget));
}

void grf_imagewidget_set_scale(GrfImageWidget *widget, float scale, gboolean invalidate){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  priv->scale = scale;
  if(invalidate) gtk_widget_queue_draw(GTK_WIDGET(widget));
}

void grf_imagewidget_set_transform(GrfImageWidget *widget, float *transform, gboolean invalidate){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  memcpy(priv->transform, transform, sizeof(float)*6);
  if(invalidate) gtk_widget_queue_draw(GTK_WIDGET(widget));
}

void grf_imagewidget_set_rotation(GrfImageWidget *widget, float angle, gboolean invalidate){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  priv->angle = angle;
  if(invalidate) gtk_widget_queue_draw(GTK_WIDGET(widget));
}

void grf_imagewidget_set_translation(GrfImageWidget *widget, float x, float y, gboolean invalidate){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  priv->translation_x = x;
  priv->translation_y = y;
  if(invalidate) gtk_widget_queue_draw(GTK_WIDGET(widget));
}

float* grf_imagewidget_get_translation(GrfImageWidget* widget){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  return &priv->translation_x;
}

float  grf_imagewidget_get_scale(GrfImageWidget* widget){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  return priv->scale;
}

float  grf_imagewidget_get_rotation(GrfImageWidget* widget){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  return priv->angle;
}

float* grf_imagewidget_get_transform(GrfImageWidget* widget){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  return priv->transform;
}
GrfNDArray*
grf_imagewidget_get_image(GrfImageWidget *widget){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  return priv->image_original;
}
