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
// Private members
typedef struct _GrfImageWidgetPrivate{

  gdouble          zoom;
  gdouble          offset_x;
  gdouble          offset_y;
  gboolean         show_cursor;
  GdkPixbuf*       pixbuf;
  GdkInterpType    interp;
  GrfFittingMode   fitting;
  GtkAdjustment*   hadj;
  GtkAdjustment*   vadj;

  GrfArray*        grf_image_original;
  GrfArray*        grf_image_output;
  cairo_surface_t* grf_image_surface;
  uint8_t          flags;
} GrfImageWidgetPrivate;

// Signals sent by ImageWidget
typedef enum{
  GRF_SIGNAL_IMAGEWIDGET_ZOOM_SET,
  GRF_SIGNAL_IMAGEWIDGET_ZOOM_IN,
  GRF_SIGNAL_IMAGEWIDGET_ZOOM_OUT,
  GRF_SIGNAL_IMAGEWIDGET_FITTING_SET,
  GRF_SIGNAL_IMAGEWIDGET_SCROLLED,
  GRF_SIGNAL_IMAGEWIDGET_ZOOM_CHANGED,
  GRF_SIGNAL_IMAGEWIDGET_PIXBUF_CHANGED,
  GRF_SIGNAL_IMAGEWIDGET_NONE,
}GrfImageWidgetSignal;

// Array of signals
static guint grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_NONE] = {0};

// ImageWidget definition
G_DEFINE_TYPE_WITH_PRIVATE(GrfImageWidget, grf_imagewidget, GTK_TYPE_WIDGET)

// Local Forward Declarations
static void
grf_imagewidget_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width);
static void
grf_imagewidget_get_preferred_height(GtkWidget *widget, gint *minimal_height, gint *natural_height);
static void
grf_imagewidget_size_allocate       (GtkWidget *widget, GtkAllocation *allocation);
static gboolean
grf_imagewidget_draw                (GtkWidget* widget, cairo_t* cr);
static void
get_size                            (GrfImageWidget* widget, GtkOrientation direction, int *minimal, int* natural);
static gboolean
grf_imagewidget_button_press_event  (GtkWidget* widget, GdkEventButton *event);
static gboolean
grf_imagewidget_button_release_event(GtkWidget* widget, GdkEventButton *event);
static int
grf_imagewidget_motion_notify_event (GtkWidget* widget, GdkEventMotion *event);
static void
grf_imagewidget_realize             (GtkWidget* widget);
static void
grf_imagewidget_unrealize           (GtkWidget* widget);
static void
grf_imagewidget_set_size            (GtkWidget* widget, int max_width, int max_height);
static void
grf_imagewidget_signals_init        (GrfImageWidgetClass *klass);
static void
grf_imagewidget_scroll              (GrfImageWidget* widget, GtkScrollType scroll_x, GtkScrollType scroll_y);

// Initialize GrfImageWidget
static void
grf_imagewidget_init(GrfImageWidget *imagewidget){
  GtkWidget* widget = GTK_WIDGET(imagewidget);
  gtk_widget_set_can_focus(widget, TRUE);

  //gtk_widget_set_has_window(GTK_WIDGET(imagewidget), FALSE);
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(imagewidget);
  priv->grf_image_original = NULL;
  priv->grf_image_output   = NULL;
  priv->grf_image_surface  = NULL;
  priv->flags              = GRF_WINDOW_NORMAL;

  priv->zoom               = 1.0;
  priv->offset_x           = 0.0;
  priv->offset_y           = 0.0;
  priv->interp             = GDK_INTERP_BILINEAR;
  priv->fitting            = GRF_FITTING_NORMAL;
  priv->pixbuf             = NULL;
  priv->hadj               = GTK_ADJUSTMENT(gtk_adjustment_new(0,1,0,1,1,1));
  priv->vadj               = GTK_ADJUSTMENT(gtk_adjustment_new(0,1,0,1,1,1));
}

// Override Event Functions
static void
grf_imagewidget_class_init(GrfImageWidgetClass *klass){
  grf_imagewidget_signals_init(klass);

  GtkWidgetClass* widget_class      = GTK_WIDGET_CLASS(klass);
  widget_class->button_press_event  = grf_imagewidget_button_press_event;
  widget_class->button_release_event= grf_imagewidget_button_release_event;
  widget_class->motion_notify_event = grf_imagewidget_motion_notify_event;
  widget_class->realize             = grf_imagewidget_realize;
  widget_class->size_allocate       = grf_imagewidget_size_allocate;
  widget_class->unrealize           = grf_imagewidget_unrealize;

//  widget_class->get_preferred_width = grf_imagewidget_get_preferred_width;
//  widget_class->get_preferred_height= grf_imagewidget_get_preferred_height;
//  widget_class->draw                = grf_imagewidget_draw;

  klass->zoom_in        = grf_imagewidget_zoom_in;
  klass->zoom_out       = grf_imagewidget_zoom_out;
  klass->set_zoom       = grf_imagewidget_set_zoom;
  klass->set_fitting    = grf_imagewidget_set_fitting;
  klass->scroll         = grf_imagewidget_scroll;
  klass->pixbuf_changed = NULL;
}

static void
grf_imagewidget_signals_init(GrfImageWidgetClass *klass){
  // Event triggered when Zoom Set
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_ZOOM_SET] =
      g_signal_new("zoom_set",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, set_zoom),
                   NULL,NULL,
                   g_cclosure_marshal_VOID__DOUBLE,
                   G_TYPE_DOUBLE, 1, G_TYPE_DOUBLE);

  // Event triggered when Zoom In
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_ZOOM_IN] =
      g_signal_new("zoom_in",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, zoom_in),
                   NULL,NULL,
                   g_cclosure_marshal_VOID__VOID,
                   G_TYPE_NONE, 0);

  // Event triggered when Zoom Out
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_ZOOM_OUT] =
      g_signal_new("zoom_out",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, zoom_out),
                   NULL,NULL,
                   g_cclosure_marshal_VOID__VOID,
                   G_TYPE_NONE, 0);
  // Event triggered when Fitting mode is set
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_FITTING_SET] =
      g_signal_new("fitting_set",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, set_fitting),
                   NULL,NULL,
                   g_cclosure_marshal_VOID__ENUM,
                   G_TYPE_NONE, 1, G_TYPE_INT);
  // Event triggered when user moved scrollbars
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_SCROLLED] =
      g_signal_new("scrolled",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, scroll),
                   NULL,NULL,
                   grf_marshal_VOID__ENUM_ENUM, G_TYPE_NONE,
                   2, GTK_TYPE_SCROLL_TYPE, GTK_TYPE_SCROLL_TYPE);
  // Event triggered when Zoom Changed (In, Out or Specific)
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_ZOOM_CHANGED] =
      g_signal_new("zoom_changed",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST,
                   0,
                   NULL,NULL,
                   g_cclosure_marshal_VOID__VOID,
                   G_TYPE_NONE, 0);
  // Event triggered when Pixbuf was changed
  grf_imagewidget_signals[GRF_SIGNAL_IMAGEWIDGET_PIXBUF_CHANGED] =
      g_signal_new("pixbuf_changed",
                   G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_LAST,
                   G_STRUCT_OFFSET(GrfImageWidgetClass, pixbuf_changed),
                   NULL,NULL,
                   g_cclosure_marshal_VOID__VOID,
                   G_TYPE_NONE, 0);

}


static gboolean
grf_imagewidget_button_press_event(GtkWidget* widget, GdkEventButton *event){
  printf("pressionando/n");
  return TRUE;
}
static gboolean
grf_imagewidget_button_release_event(GtkWidget* widget, GdkEventButton *event){
  printf("soltando/n");
  return FALSE;
}
static int
grf_imagewidget_motion_notify_event (GtkWidget* widget, GdkEventMotion *event){
  return 0;
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

// Allocate size
static void
grf_imagewidget_size_allocate(GtkWidget *widget, GtkAllocation *allocation){
  gtk_widget_size_allocate(widget, allocation);
  //gtk_widget_set_allocation(widget, allocation);
//  GrfImageWidget* imagewidget = GRF_IMAGEWIDGET(widget);
//  ImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(imagewidget);
//  if((priv->flags & GRF_WINDOW_AUTOSIZE) == 0 && priv->grf_image_original){
//    grf_imagewidget_set_size(widget, allocation->width, allocation->height);
//  }
//  if(gtk_widget_get_realized(widget)){
//    if(priv->grf_image_original && (priv->flags & GRF_WINDOW_AUTOSIZE)){
//      allocation->width = priv->grf_image_output->size[1];
//      allocation->height = priv->grf_image_output->size[0];
//      gtk_widget_set_allocation(widget, allocation);
//      gdk_window_move_resize(gtk_widget_get_window(widget),allocation->x, allocation->y, priv->grf_image_original->size[1],priv->grf_image_original->size[0]);
//    }else{
//      gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x, allocation->y, allocation->width, allocation->height);
//    }
//  }
}

static void
grf_imagewidget_set_size(GtkWidget* widget, int max_width, int max_height){
  GrfImageWidget* imagewidget = GRF_IMAGEWIDGET(widget);
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(imagewidget);
  if(priv->flags & GRF_WINDOW_AUTOSIZE) return;
  if(!priv->grf_image_original) return;
}

// Draw image
static gboolean
grf_imagewidget_draw(GtkWidget *widget, cairo_t *cr){
  GrfImageWidget       * imagewidget = GRF_IMAGEWIDGET(widget);
  GrfImageWidgetPrivate*        priv = grf_imagewidget_get_instance_private(imagewidget);
  cairo_set_source_surface(cr, priv->grf_image_surface, 0, 0);
  cairo_paint(cr);
  cairo_fill(cr);
  return TRUE;
}

// Connect to GdkWindow to receive mouse events
static void
grf_imagewidget_realize(GtkWidget* widget){
  GdkWindowAttr attributes;
  gint attributes_mask;
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  gtk_widget_set_realized(widget, TRUE);
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
}

static void
grf_imagewidget_unrealize(GtkWidget* widget){

}

// Get Preferred Size (Width or Height)
static void
get_size(GrfImageWidget* imagewidget, GtkOrientation direction, int* minimal, int* natural){
  uint8_t             index;
  uint16_t            size_min;
  uint16_t            gdk_size;
  GtkWidget         * widget         = GTK_WIDGET(imagewidget);
  GrfImageWidgetPrivate* priv           = grf_imagewidget_get_instance_private(imagewidget);
  uint32_t          * size_original  = priv->grf_image_original? priv->grf_image_original->size: NULL;
  uint32_t          * size_output    = priv->grf_image_output  ? priv->grf_image_output->size  : NULL;
  GdkWindow         * window         = gtk_widget_get_window(widget);
  switch(direction){
  case GTK_ORIENTATION_HORIZONTAL:
    index = 1; size_min = 320;
    if(window)gdk_size = gdk_window_get_width(window);
    else      gdk_size = size_min;
    break;
  case GTK_ORIENTATION_VERTICAL:
    index = 0; size_min = 240;
    if(window)gdk_size = gdk_window_get_height(window);
    else      gdk_size = size_min;
    break;
  }
  if(priv->grf_image_original != NULL)
    *minimal = (priv->flags & GRF_WINDOW_AUTOSIZE) != GRF_WINDOW_AUTOSIZE ? gdk_size:size_original[index];
  else
    *minimal = size_min;
  if(priv->grf_image_output != NULL)
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

gboolean
grf_imagewidget_get_viewport (GrfImageWidget* widget, GdkRectangle* rect){

}

void
grf_imagewidget_set_fitting(GrfImageWidget *imagewidget, GrfFittingMode fitting){
  int xstep = 0;
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(imagewidget);
  if(scroll_x == GTK_SCROLL_STEP_LEFT)
    xstep = -priv->hadj->step_increment;
}

// Update image
void
grf_imagewidget_set_image(GrfImageWidget* widget, GrfArray* image){
  GrfImageWidgetPrivate* priv = grf_imagewidget_get_instance_private(widget);
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24,image->size[1]);
  priv->grf_image_original   = image;
  if(image->dim == 2 || image->size[2] == 1)
    priv->grf_image_output   = grf_image_cvt_color(image,GRF_GRAY, GRF_BGRA);
  else
    priv->grf_image_output   = grf_image_cvt_color(image,GRF_RGB, GRF_BGRA);
  priv->grf_image_surface    = cairo_image_surface_create_for_data(priv->grf_image_output->data_uint8,CAIRO_FORMAT_RGB24,priv->grf_image_output->size[1],priv->grf_image_output->size[0],stride);
  gtk_widget_queue_draw(GTK_WIDGET(widget));
}
