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
 * PRIVATE API: Structure
 *=================================*/

typedef struct _GrfDisplayWindowPrivate{
  GtkWidget  *     imagewidget;
  GtkWidget  *     window;
  GtkWidget  *     toolbar;
  GtkWidget  *     btn_save;
  GtkWidget  *     btn_zoom_in;
  GtkWidget  *     btn_zoom_out;
  GtkWidget  *     btn_pan_left;
  GtkWidget  *     btn_pan_right;
  GtkWidget  *     btn_pan_top;
  GtkWidget  *     btn_pan_bottom;
  GtkWidget  *     btn_zoom_ori;
  GtkWidget  *     btn_zoom_fit;
  GtkWidget  *     btn_about;
  GtkWidget  *     box;
  GtkWidget  *     statusbar;
  GtkWidget  *     lbl_color;
  GrfQueue   *     trackbars;
  char       *     name;
  guint            context;
  uint8_t          key_pressed;
  gulong           destroy_id;
  GrfMouseCallback mouse_callback;
  gulong           mouse_press_id;
  gulong           mouse_release_id;
  gulong           mouse_move_id;
  void       *     mouse_user_data;
  GrfKeyCallback   key_callback;
  gulong           key_press_id;
  gulong           key_release_id;
  void       *     key_user_data;

}GrfDisplayWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfDisplayWindow, grf_displaywindow, G_TYPE_OBJECT)

/*=================================
 * PRIVATE API: Buttons Events
 *=================================*/
static void
grf_displaywindow_btn_save_clicked(GtkWidget* widget, gpointer user_data){
  (void) widget;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);

  // Build a dialog
  GtkWidget* dialog = gtk_file_chooser_dialog_new("Save image",
                                                  GTK_WINDOW(priv->window),
                                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                                  _("_Cancel"),GTK_RESPONSE_CANCEL,
                                                  _("_Save"),  GTK_RESPONSE_ACCEPT,
                                                  NULL);
  GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

  // Run a dialog
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));

  // Did the user have chosen to save the file?
  if(res == GTK_RESPONSE_ACCEPT){
     char* filename = gtk_file_chooser_get_filename(chooser);
     grf_image_write(grf_displaywindow_get_image(display),filename);
  }
  gtk_widget_destroy(dialog);
}
static void
grf_displaywindow_btn_zoom_in_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);

  float cur_scale = grf_imagewidget_get_scale(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), cur_scale * 1.5, TRUE);
}
static void
grf_displaywindow_btn_zoom_out_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);

  float cur_scale = grf_imagewidget_get_scale(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), cur_scale / 1.5, TRUE);
}
static void
grf_displaywindow_btn_zoom_ori_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);

  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), 1, TRUE);
}
static void
grf_displaywindow_btn_zoom_fit_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget; (void) user_data;
  //GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  //GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  //gtk_widget_get_preferred_height(priv->imagewidget, &min_height, &nat_height);

  //grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), 1, TRUE);
}
static void
grf_displaywindow_btn_pan_left_clicked(GtkWidget * widget, GdkEvent *event, gpointer user_data){
  (void) widget; (void) event;
  gtk_main_quit();
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  float* translations = grf_imagewidget_get_translation(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_translation(GRF_IMAGEWIDGET(priv->imagewidget),translations[0]-5,translations[1],TRUE);
}
static void
grf_displaywindow_btn_pan_right_clicked(GtkWidget * widget, GdkEvent *event, gpointer user_data){
  (void) widget; (void) event;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  float* translations = grf_imagewidget_get_translation(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_translation(GRF_IMAGEWIDGET(priv->imagewidget),translations[0]+5,translations[1],TRUE);
}
static void
grf_displaywindow_btn_pan_top_clicked(GtkWidget * widget, GdkEvent *event, gpointer user_data){
  (void) widget; (void) event;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  float* translations = grf_imagewidget_get_translation(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_translation(GRF_IMAGEWIDGET(priv->imagewidget),translations[0],translations[1]-5,TRUE);
}
static void
grf_displaywindow_btn_pan_bottom_clicked(GtkWidget * widget, GdkEvent *event, gpointer user_data){
  (void) widget; (void) event;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  float* translations = grf_imagewidget_get_translation(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_translation(GRF_IMAGEWIDGET(priv->imagewidget),translations[0],translations[1]+5,TRUE);
}
static void
grf_displaywindow_btn_about_clicked(GtkWidget* widget, gpointer user_data){
  (void) widget;
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  g_auto(GStrv) authors   = g_strsplit("Anderson Tavares:Paulo Miranda", ":", -1);
  g_auto(GStrv) comments  = g_strsplit("Grafeo Library:Grafeo", ":", -1);
  gtk_show_about_dialog(GTK_WINDOW(priv->window),
                        "program-name","Grafeo",
                        "title",_("About Grafeo"),
                        "authors",authors,
                        "website",grf_config_get_website(),
                        "website-label",grf_config_get_website(),
                        "version","0.0.6",
                        "copyright","All rights reserved (C) 2015-2016",
                        "comments","Library for Graph/Computer Vision Problems",
                        "license",grf_config_get_license(),
                        NULL);

}

static gboolean
grf_displaywindow_imagewidget_motion_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  double x, y;
  char text_position[13];
  char text_color[64];
  GrfDisplayWindow* display     = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  GrfArray* image               = grf_displaywindow_get_image(display);
  if(image){
    gdk_event_get_coords(event, &x, &y);
    float scale = grf_imagewidget_get_scale(GRF_IMAGEWIDGET(priv->imagewidget));
    x /= scale;
    y /= scale;

    if(x <= image->size[1] && y <= image->size[0]){
      // Position in Statusbar
      sprintf(text_position,"Pos: %d %d", (int)x,(int)y);
      gtk_statusbar_push(GTK_STATUSBAR(priv->statusbar), priv->context, text_position);

      // Color in Statusbar
      uint64_t    pixel_index = ((int)y)*image->step[0]+((int)x)*image->step[1];
      uint8_t  *  pixel       = &image->data_uint8[pixel_index];
      if(image->dim == 2 || image->size[2] == 1)
        sprintf(text_color,"(G = %03d)", pixel[0]);
      else
        sprintf(text_color,"(<span color=\"red\">R=%03d</span>, <span color=\"green\">G=%03d</span>, <span color=\"blue\">B=%03d</span>)", pixel[2],pixel[1],pixel[0]);
      gtk_label_set_label(GTK_LABEL(priv->lbl_color),text_color);
    }
  }
  return FALSE;
}

/*=================================
 * PRIVATE API: Init Functions
 *=================================*/
static void
grf_displaywindow_init(GrfDisplayWindow* self){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(self);
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

  gtk_widget_add_events(priv->window, GDK_BUTTON_PRESS_MASK);
  gtk_widget_add_events(priv->imagewidget,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
  gtk_widget_add_events(priv->imagewidget, GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);

  g_signal_connect(priv->btn_save,       "clicked", G_CALLBACK(grf_displaywindow_btn_save_clicked),self);
  g_signal_connect(priv->btn_zoom_in,    "clicked", G_CALLBACK(grf_displaywindow_btn_zoom_in_clicked),self);
  g_signal_connect(priv->btn_zoom_out,   "clicked", G_CALLBACK(grf_displaywindow_btn_zoom_out_clicked),self);
  g_signal_connect(priv->btn_zoom_ori,   "clicked", G_CALLBACK(grf_displaywindow_btn_zoom_ori_clicked),self);
  g_signal_connect(priv->btn_zoom_fit,   "clicked", G_CALLBACK(grf_displaywindow_btn_zoom_fit_clicked),self);
  g_signal_connect(priv->btn_pan_left,   "clicked", G_CALLBACK(grf_displaywindow_btn_pan_left_clicked),self);
  g_signal_connect(priv->btn_pan_right,  "clicked", G_CALLBACK(grf_displaywindow_btn_pan_right_clicked),self);
  g_signal_connect(priv->btn_pan_bottom, "clicked", G_CALLBACK(grf_displaywindow_btn_pan_bottom_clicked),self);
  g_signal_connect(priv->btn_pan_top,    "clicked", G_CALLBACK(grf_displaywindow_btn_pan_top_clicked),self);
  g_signal_connect(priv->btn_about,      "clicked", G_CALLBACK(grf_displaywindow_btn_about_clicked),self);

  // Status bar
  priv->context = gtk_statusbar_get_context_id(GTK_STATUSBAR(priv->statusbar),"display_pixel_coord");
  g_signal_connect(priv->imagewidget, "motion-notify-event", G_CALLBACK(grf_displaywindow_imagewidget_motion_event),self);
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
  gtk_box_pack_end(GTK_BOX(priv->box),priv->statusbar  ,FALSE,FALSE,0);

  gtk_container_add (GTK_CONTAINER(priv->window) ,priv->box);
}

static void
grf_displaywindow_class_init(GrfDisplayWindowClass *klass){
  (void) klass;
}

static guint
grf_displaywindow_generate_key_flags(GdkEventButton* event_button){
  GdkModifierType modifiers = gtk_accelerator_get_default_mod_mask();
  guint           flag      = 0;
  guint           flag_keys = event_button->state & modifiers;
  if(flag_keys & GDK_CONTROL_MASK) flag |= GRF_MOUSE_FLAG_CTRLKEY;
  if(flag_keys & GDK_SHIFT_MASK)   flag |= GRF_MOUSE_FLAG_CTRLKEY;
  if(flag_keys & GDK_MOD1_MASK)    flag |= GRF_MOUSE_FLAG_ALTKEY;
  return flag;
}

static gboolean
grf_displaywindow_call_mouse_callback(GdkEvent* event, gpointer user_data, GrfMouseEventType event_type){
  GrfDisplayWindow*        display      = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv         = grf_displaywindow_get_instance_private(display);
  GdkEventButton*          event_button = (GdkEventButton*) event;
  gdouble x,y;
  gdk_event_get_coords(event,&x,&y);
  int flag = (event_button->button-1) | grf_displaywindow_generate_key_flags(event_button);
  priv->mouse_callback(event_type, (int)x, (int)y, flag, priv->mouse_user_data);
  return FALSE;
}

static gboolean
grf_displaywindow_button_press_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  return grf_displaywindow_call_mouse_callback(event,user_data,GRF_MOUSE_EVENT_PRESS);
}
static gboolean
grf_displaywindow_button_release_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  return grf_displaywindow_call_mouse_callback(event,user_data,GRF_MOUSE_EVENT_RELEASE);
}
static gboolean
grf_displaywindow_call_key_callback(GdkEvent* event, gpointer user_data, GrfKeyEventType event_type){
  GrfDisplayWindow*        display      = GRF_DISPLAYWINDOW(user_data);
  GrfDisplayWindowPrivate* priv         = grf_displaywindow_get_instance_private(display);
  GdkEventKey* event_key = (GdkEventKey*) event;
  priv->key_callback(event_type,event_key->keyval, priv->key_user_data);
  return FALSE;
}

static gboolean
grf_displaywindow_button_move_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  return grf_displaywindow_call_mouse_callback(event,user_data,GRF_MOUSE_EVENT_MOVE);
}
static gboolean
grf_displaywindow_key_press_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  return grf_displaywindow_call_key_callback(event, user_data, GRF_KEY_EVENT_PRESS);
}
static gboolean
grf_displaywindow_key_release_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  return grf_displaywindow_call_key_callback(event, user_data, GRF_KEY_EVENT_RELEASE);
}

/*=================================
 * PUBLIC API
 *=================================*/
GrfDisplayWindow*
grf_displaywindow_new(){
  return GRF_DISPLAYWINDOW(g_object_new(GRF_TYPE_DISPLAYWINDOW, NULL));
}

GrfDisplayWindow*
grf_displaywindow_new_with_name(char* name){
  GrfDisplayWindow* display = grf_displaywindow_new();
  grf_displaywindow_set_name(display, name);
  return display;
}

void
grf_displaywindow_set_image(GrfDisplayWindow* display, GrfArray* image, gboolean invalidate){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  grf_imagewidget_set_image(GRF_IMAGEWIDGET(priv->imagewidget), image, invalidate);
}

GrfArray*
grf_displaywindow_get_image(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  return grf_imagewidget_get_image(GRF_IMAGEWIDGET(priv->imagewidget));
}

void
grf_displaywindow_show(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  gtk_widget_show_all(priv->window);
}

void
grf_displaywindow_quit_on_destroy(GrfDisplayWindow* display, gboolean value){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  if(value)
    priv->destroy_id = g_signal_connect(priv->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  else
    g_signal_handler_disconnect(priv->window, priv->destroy_id);
}

void
grf_displaywindow_hide(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  gtk_widget_hide(priv->window);
}

void
grf_displaywindow_connect_mouse_callback(GrfDisplayWindow* display, GrfMouseCallback mouse_callback, void* user_data){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  priv->mouse_callback   = mouse_callback;
  priv->mouse_user_data  = user_data;
  priv->mouse_press_id   = g_signal_connect(priv->imagewidget,"button-press-event"  , G_CALLBACK(grf_displaywindow_button_press_event),   display);
  priv->mouse_release_id = g_signal_connect(priv->imagewidget,"button-release-event", G_CALLBACK(grf_displaywindow_button_release_event), display);
  priv->mouse_move_id    = g_signal_connect(priv->imagewidget,"motion-notify-event" , G_CALLBACK(grf_displaywindow_button_move_event),    display);
}

void
grf_displaywindow_disconnect_mouse_callback(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  g_signal_handler_disconnect(priv->window,priv->mouse_press_id);
  g_signal_handler_disconnect(priv->window,priv->mouse_release_id);
  g_signal_handler_disconnect(priv->window,priv->mouse_move_id);
}

void
grf_displaywindow_connect_key_callback(GrfDisplayWindow* display, GrfKeyCallback key_callback, void* user_data){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  priv->key_callback     = key_callback;
  priv->key_user_data    = user_data;
  priv->key_press_id     = g_signal_connect(priv->window,"key-press-event"  , G_CALLBACK(grf_displaywindow_key_press_event),   display);
  priv->key_release_id   = g_signal_connect(priv->window,"key-release-event", G_CALLBACK(grf_displaywindow_key_release_event),   display);
}
void
grf_displaywindow_disconnect_key_callback(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  g_signal_handler_disconnect(priv->window,priv->key_press_id);
  g_signal_handler_disconnect(priv->window,priv->key_release_id);
}


void
grf_displaywindow_set_name(GrfDisplayWindow* display, char* name){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  priv->name = name;
}

char*
grf_displaywindow_get_name(GrfDisplayWindow* display){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  return priv->name;
}

void
grf_displaywindow_add_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  gtk_box_pack_start(GTK_BOX(priv->box),GTK_WIDGET(trackbar),FALSE,FALSE,0);
  grf_queue_append(priv->trackbars,trackbar);
}

void
grf_displaywindow_remove_trackbar(GrfDisplayWindow* display, GrfTrackbar* trackbar){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  gtk_container_remove(GTK_CONTAINER(priv->box),GTK_WIDGET(trackbar));
  grf_queue_remove(priv->trackbars,trackbar);
}

GrfTrackbar*
grf_displaywindow_get_trackbar_by_name(GrfDisplayWindow* display, char* name){
  GrfDisplayWindowPrivate* priv = grf_displaywindow_get_instance_private(display);
  GrfList* item;
  for(item = priv->trackbars->begin; item; item = grf_list_next(item)){
    GrfTrackbar* trackbar = GRF_TRACKBAR(grf_list_value(item));
    if(strcmp(grf_trackbar_get_name(trackbar),name) == 0)
      return trackbar;
  }
  return NULL;
}
