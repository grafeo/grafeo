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
#include <grafeo/displaywindow.h>

/*=================================
 * PRIVATE API
 *=================================*/

typedef struct _DisplayWindowPrivate{
  GtkWidget  * imagewidget;
  GtkWidget  * window;
  GtkWidget  * toolbar;
  GtkWidget  * btn_save;
  GtkWidget  * btn_zoom_in;
  GtkWidget  * btn_zoom_out;
  GtkWidget  * btn_pan_left;
  GtkWidget  * btn_pan_right;
  GtkWidget  * btn_pan_top;
  GtkWidget  * btn_pan_bottom;
  GtkWidget  * btn_zoom_ori;
  GtkWidget  * btn_zoom_fit;
  GtkWidget  * btn_about;
  GtkWidget  * box;
  GtkWidget  * statusbar;
  GtkWidget  * lbl_color;
  GrfQueue   * trackbars;
  char       * name;
  guint        context;
  uint8_t      key_pressed;
}GrfDisplayWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfDisplayWindow, grf_displaywindow, G_TYPE_OBJECT)

static void
grf_display_init(GrfDisplay* self){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(self);
  priv->imagewidget   = grf_imagewidget_new();
  priv->toolbar       = gtk_toolbar_new();
  priv->btn_save      = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoom_in   = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoom_out  = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoom_ori  = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoom_fit  = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_pan_left  = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_pan_right = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_pan_top   = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_pan_bottom= GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_about     = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->lbl_color     = gtk_label_new("");
  priv->window        = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  priv->box           = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  priv->statusbar     = gtk_statusbar_new();
  priv->name          = NULL;
  priv->trackbars     = grf_queue_new();

  // Buttons
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_save)      ,"document-save");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoom_in)   ,"zoom-in");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoom_out)  ,"zoom-out");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoom_ori)  ,"zoom-original");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoom_fit)  ,"zoom-fit-best");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_pan_left)  ,"go-previous");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_pan_right) ,"go-next");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_pan_top)   ,"go-up");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_pan_bottom),"go-down");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_about),"help-about");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_save),       "Save image");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoom_in),    "Zoom In");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoom_out),   "Zoom Out");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoom_ori),   "Zoom Original");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoom_fit),   "Fit to Screen");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_pan_left),   "Pan Left");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_pan_right),  "Pan Right");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_pan_bottom), "Pan Bottom");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_pan_top),    "Pan Top");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_about),    "About");

  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_save)      ,"Save image");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoom_in)   , "Zoom In");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoom_out)  ,"Zoom Out");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoom_ori)  ,"Zoom Original");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoom_fit)  ,"Fit to Screen");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_pan_left)  ,"Pan Left");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_pan_right) ,"Pan Right");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_pan_bottom),"Pan Bottom");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_pan_top)   ,"Pan Top");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_about)   ,"About");

  g_signal_connect(priv->btn_save, "clicked", G_CALLBACK(grf_display_btn_save_clicked),self);
  g_signal_connect(priv->btn_zoom_in, "clicked", G_CALLBACK(grf_display_btn_zoom_in_clicked),self);
  g_signal_connect(priv->btn_zoom_out, "clicked", G_CALLBACK(grf_display_btn_zoom_out_clicked),self);
  g_signal_connect(priv->btn_zoom_ori, "clicked", G_CALLBACK(grf_display_btn_zoom_ori_clicked),self);
  g_signal_connect(priv->btn_zoom_fit, "clicked", G_CALLBACK(grf_display_btn_zoom_fit_clicked),self);
  g_signal_connect(priv->btn_pan_left, "clicked", G_CALLBACK(grf_display_btn_pan_left_clicked),self);
  g_signal_connect(priv->btn_pan_right, "clicked", G_CALLBACK(grf_display_btn_pan_right_clicked),self);
  g_signal_connect(priv->btn_pan_bottom, "clicked", G_CALLBACK(grf_display_btn_pan_bottom_clicked),self);
  g_signal_connect(priv->btn_pan_top, "clicked", G_CALLBACK(grf_display_btn_pan_top_clicked),self);
  g_signal_connect(priv->btn_about, "clicked", G_CALLBACK(grf_display_btn_about_clicked),self);

  // Status bar
  priv->context = gtk_statusbar_get_context_id(GTK_STATUSBAR(priv->statusbar),"display_pixel_coord");
  g_signal_connect(priv->imagewidget, "motion-notify-event", G_CALLBACK(grf_display_imagewidget_motion_event),self);
  gtk_label_set_markup(GTK_LABEL(priv->lbl_color), "");

  // Joining everything together
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(priv->toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_save),0);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoom_in),1);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoom_out),2);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoom_ori),3);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoom_fit),4);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_pan_left),5);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_pan_right),6);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_pan_top),7);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_pan_bottom),8);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_about),9);
  gtk_box_pack_end(GTK_BOX(priv->statusbar),priv->lbl_color,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->box),priv->toolbar    ,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->box),priv->imagewidget,TRUE ,TRUE ,0);
  gtk_box_pack_start(GTK_BOX(priv->box),priv->statusbar  ,FALSE,FALSE,0);

  gtk_container_add (GTK_CONTAINER(priv->window) ,priv->box);
}

static void
grf_display_class_init(GrfDisplayClass *klass){
  (void) klass;
}

/*=================================
 * PUBLIC API
 *=================================*/
GrfDisplayWindow*
grf_displaywindow_new(){
  return gtk_widget_new(GRF_TYPE_DISPLAYWINDOW, NULL);
}

void
grf_displaywindow_set_image(GrfDisplayWindow* display, GrfArray* image, gboolean invalidate){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  grf_imagewidget_set_image(priv->imagewidget, image, invalidate);
}

GrfArray*
grf_displaywindow_get_image(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  return grf_imagewidget_get_image(priv->imagewidget);
}

void
grf_displaywindow_show(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  gtk_widget_show_all(priv->window);
}

void
grf_displaywindow_hide(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  gtk_widget_hide(priv->window);
}

void
grf_displaywindow_set_name(GrfDisplayWindow* display, char* name){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  priv->name = name;
}

char*
grf_displaywindow_get_name(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  return priv->name;
}

void
grf_displaywindow_add_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  grf_queue_append(priv->trackbars,trackbar);
}

void
grf_displaywindow_remove_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  grf_queue_remove(priv->trackbars,trackbar);
}

void
grf_displaywindow_get_trackbar_by_name(GrfDisplayWindow* display, char* name){
  GrfDisplayWindowPrivate* priv = grf_display_get_instance_private(display);
  List* item;
  for(item = priv->trackbars; item; item = grf_list_next(item)){
    GrfTrackbar* trackbar = GRF_TRACKBAR(grf_list_value(item));
    if(strcmp(grf_trackbar_get_name(trackbar),name) == 0)
      return trackbar;
  }
  return NULL;
}
