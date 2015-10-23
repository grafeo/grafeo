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

// Private information
typedef struct {
  Array* image_original;
  Array* image_output;
} ImageWidgetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ImageWidget, imagewidget, GTK_TYPE_WIDGET)

static void imagewidget_init(ImageWidget *imagewidget){
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
  priv->image_original = NULL;
  priv->image_output   = NULL;
}

static void imagewidget_get_preferred_width(GtkWidget* widget, gint *minimal_width, gint *natural_width){
  ImageWidget* imagewidget = GRAFEO_IMAGEWIDGET(widget);
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
  if(priv->image_original != NULL){
    *minimal_width = priv->image_original->size[1];
  }
  else{
    *minimal_width = 320;
  }
  if(priv->image_output != NULL){
    *natural_width = *minimal_width < priv->image_output->size[1] ? priv->image_output->size[1] : *minimal_width;
  }
  else{
    *natural_width = *minimal_width;
  }
}
static void imagewidget_get_preferred_height(GtkWidget* widget, gint *minimal_height, gint *natural_height){
  ImageWidget* imagewidget = GRAFEO_IMAGEWIDGET(widget);
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(imagewidget);
  if(priv->image_original != NULL){
    *minimal_height = priv->image_original->size[1];
  }
  else{
    *minimal_height = 240;
  }
  if(priv->image_output != NULL){
    *natural_height = *minimal_height < priv->image_output->size[1] ? priv->image_output->size[2]:*minimal_height;
  }
  else{
    *natural_height = *minimal_height;
  }
}

static void imagewidget_realize(GtkWidget* widget){
  GdkWindowAttr attributes;
  gint          attributes_mask;
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  gtk_widget_set_realized(widget, TRUE);
  attributes.x      = allocation.x;
  attributes.y      = allocation.y;
  attributes.width  = allocation.width;
  attributes.height = allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask  = gtk_widget_get_events(widget) |
                           GDK_EXPOSURE_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK;
  attributes.visual = gtk_widget_get_visual(widget);
  attributes_mask = GDA_WA_X | GDA_WA_Y | GDA_WA_VISUAL;
  gtk_widget_set_window(widget,gdk_window_new(gtk_widget_get_parent_window(widget),&attributes,attributes_mask));
  gtk_widget_set_style(widget,gtk_style_attach(gtk_widget_get_style(widget),gtk_widget_get_window(widget)));
  gdk_window_set_user_data(gtk_widget_get_window(widget),widget);
  gtk_style_set_background(gtk_widgetget_style(widget),gtk_widget_get_window(widget),GTK_STATE_ACTIVE);
}

static void imagewidget_size_allocate(GtkWidget* widget, GtkAllocation* allocation){

}

static void imagewidget_destroy(GtkWidget* object){
  ImageWidgetPrivate* priv = imagewidget_get_instance_private(GRAFEO_IMAGEWIDGET(object));

  array_free(priv->image_original);
  array_free(priv->image_output);
}

static void imagewidget_class_init(ImageWidgetClass* klass){
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  widget_class->destroy              = imagewidget_destroy;
  widget_class->get_preferred_width  = imagewidget_get_preferred_width;
  widget_class->get_preferred_height = imagewidget_get_preferred_height;
  widget_class->realize              = imagewidget_realize;
  widget_class->size_allocate        = imagewidget_size_allocate;
  widget_class->button_press_event   = NULL;
  widget_class->button_release_event = NULL;
  widget_class->motion_notify_event  = NULL;
}

GtkWidget* imagewidget_new(){
  return gtk_widget_new(GRAFEO_TYPE_IMAGEWIDGET, NULL);
}



