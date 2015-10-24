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
#include <grafeo/imagewidget.h>

/*=================================
 * PRIVATE API
 *=================================*/
// Private information
typedef struct _ImageWidgetPrivate{
  Array*           image_original;
  Array*           image_output;
  cairo_surface_t* image_surface;
  uint8_t          flags;
} ImageWidgetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ImageWidget, imagewidget, GTK_TYPE_WIDGET)

// Local Forward Declarations
static void
imagewidget_get_preferred_width(GtkWidget *widget, gint *minimal_width, gint *natural_width);
static void
imagewidget_get_preferred_height(GtkWidget *widget, gint *minimal_height, gint *natural_height);
static void
imagewidget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean
imagewidget_draw(GtkWidget* widget, cairo_t* cr);
static void
get_size(ImageWidget* widget, GtkOrientation direction, int *minimal, int* natural);

// Initialize ImageWidget
static void
imagewidget_init(ImageWidget *imagewidget){
  gtk_widget_set_has_window(GTK_WIDGET(imagewidget), FALSE);
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
  priv->image_original = NULL;
  priv->image_output   = NULL;
  priv->image_surface  = NULL;
  priv->flags          = GRAFEO_WINDOW_NORMAL;
}

// Override Event Functions
static void
imagewidget_class_init(ImageWidgetClass *klass){
  GtkWidgetClass* widget_class      = GTK_WIDGET_CLASS(klass);
  widget_class->get_preferred_width = imagewidget_get_preferred_width;
  widget_class->get_preferred_height= imagewidget_get_preferred_height;
  widget_class->size_allocate       = imagewidget_size_allocate;
  widget_class->draw                = imagewidget_draw;
}

// Get Width
static void
imagewidget_get_preferred_width(GtkWidget* widget,
                                gint *minimal_width,
                                gint *natural_width){
  get_size(GRAFEO_IMAGEWIDGET(widget), GTK_ORIENTATION_HORIZONTAL, minimal_width, natural_width);
}

// Get Height
static void
imagewidget_get_preferred_height(GtkWidget* widget,
                                 gint *minimal_height,
                                 gint *natural_height){
  get_size(GRAFEO_IMAGEWIDGET(widget), GTK_ORIENTATION_VERTICAL, minimal_height, natural_height);
}

// Allocate size
static void
imagewidget_size_allocate(GtkWidget *widget, GtkAllocation *allocation){
  gtk_widget_set_allocation(widget, allocation);
}

// Draw image
static gboolean
imagewidget_draw(GtkWidget *widget, cairo_t *cr){
  ImageWidget *imagewidget = GRAFEO_IMAGEWIDGET(widget);
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
  cairo_set_source_surface(cr, priv->image_surface, 0, 0);
  cairo_paint(cr);
  cairo_fill(cr);
  return FALSE;
}

// Get Preferred Size (Width or Height)
static void
get_size(ImageWidget* imagewidget, GtkOrientation direction, int* minimal, int* natural){
  uint8_t             index;
  uint16_t            size_min;
  uint16_t            gdk_size;
  GtkWidget         * widget         = GTK_WIDGET(imagewidget);
  ImageWidgetPrivate* priv           = imagewidget_get_instance_private(imagewidget);
  uint32_t          * size_original  = priv->image_original? priv->image_original->size: NULL;
  uint32_t          * size_output    = priv->image_output  ? priv->image_output->size  : NULL;
  switch(direction){
  case GTK_ORIENTATION_HORIZONTAL:
    index = 1; size_min = 320; gdk_size = 320;//gdk_window_get_width(gtk_widget_get_window(widget));
    break;
  case GTK_ORIENTATION_VERTICAL:
    index = 0; size_min = 240; gdk_size = 240;//gdk_window_get_height(gtk_widget_get_window(widget));
    break;
  }
  if(priv->image_original != NULL)
    *minimal = (priv->flags & GRAFEO_WINDOW_AUTOSIZE) != GRAFEO_WINDOW_AUTOSIZE ? gdk_size:size_original[index];
  else
    *minimal = size_min;
  if(priv->image_output != NULL)
    *natural = *minimal < size_output[index]?size_output[index]:*minimal;
  else
    *natural = *minimal;
}


//static void imagewidget_realize(GtkWidget* widget){
//  GdkWindowAttr attributes;
//  gint          attributes_mask;
//  GtkAllocation allocation;
//  gtk_widget_get_allocation(widget, &allocation);
//  gtk_widget_set_realized(widget, TRUE);
//  attributes.x      = allocation.x;
//  attributes.y      = allocation.y;
//  attributes.width  = allocation.width;
//  attributes.height = allocation.height;
//  attributes.wclass = GDK_INPUT_OUTPUT;
//  attributes.window_type = GDK_WINDOW_CHILD;
//  attributes.event_mask  = gtk_widget_get_events(widget) |
//                           GDK_EXPOSURE_MASK |
//                           GDK_BUTTON_PRESS_MASK |
//                           GDK_BUTTON_RELEASE_MASK |
//                           GDK_POINTER_MOTION_MASK;
//  attributes.visual = gtk_widget_get_visual(widget);
//  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
//  gtk_widget_set_window(widget,gdk_window_new(gtk_widget_get_parent_window(widget),&attributes,attributes_mask));
//  //gtk_widget_set_style(widget,gtk_style_attach(gtk_widget_get_style(widget),gtk_widget_get_window(widget)));
//  gdk_window_set_user_data(gtk_widget_get_window(widget),widget);
//  //gtk_render_background(context, style_context_set_background(gtk_widget_get_style_context(widget),gtk_widget_get_window(widget));
//  //gtk_style_set_background(gtk_widgetget_style(widget),gtk_widget_get_window(widget),GTK_STATE_ACTIVE);
//}

//static void imagewidget_size_allocate(GtkWidget* widget, GtkAllocation* allocation){
//  gtk_widget_set_allocation(widget, allocation);
//  ImageWidget* imagewidget = GRAFEO_IMAGEWIDGET(widget);
//  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
//  if(priv->image_original){

//  }
//}

//static void imagewidget_destroy(GtkWidget* object){
//  ImageWidgetPrivate* priv = imagewidget_get_instance_private(GRAFEO_IMAGEWIDGET(object));

//  array_free(priv->image_original);
//  array_free(priv->image_output);
//}

//static void imagewidget_class_init(ImageWidgetClass* klass){
//  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
//  widget_class->destroy              = imagewidget_destroy;
//  widget_class->get_preferred_width  = imagewidget_get_preferred_width;
//  widget_class->get_preferred_height = imagewidget_get_preferred_height;
//  widget_class->realize              = imagewidget_realize;
//  widget_class->size_allocate        = imagewidget_size_allocate;
//  widget_class->button_press_event   = NULL;
//  widget_class->button_release_event = NULL;
//  widget_class->motion_notify_event  = NULL;
//}

/*=================================
 * PUBLIC API
 *=================================*/
// Creates a new ImageWidget
GtkWidget*
imagewidget_new(){
  return gtk_widget_new(GRAFEO_TYPE_IMAGEWIDGET, NULL);
}

// Update image
void
imagewidget_set_image(ImageWidget* widget, Array* image){
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(widget);
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,image->size[1]);
  priv->image_original = image;
  if(image->dim == 2 || image->size[2] == 1)
    priv->image_output   = image_cvt_color(image,GRAFEO_GRAY, GRAFEO_RGBA);
  else
    priv->image_output   = image_cvt_color(image,GRAFEO_RGB, GRAFEO_RGBA);
  priv->image_surface  = cairo_image_surface_create_for_data(priv->image_output->data_uint8,CAIRO_FORMAT_RGB24,priv->image_output->size[1],priv->image_output->size[0],stride);
}
