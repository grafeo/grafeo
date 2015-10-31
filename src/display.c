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
#define GETTEXT_PACKAGE "gtk30"
#include <glib/gi18n-lib.h>

/*=================================
 * PRIVATE API
 *=================================*/
typedef struct _DisplayPrivate{
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
  char       * name;
  guint        context;
  uint8_t      key_pressed;
}GrfDisplayPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfDisplay, grf_display, G_TYPE_OBJECT)

static GrfDisplay* cur_display;
static GrfQueue*   displays_queue;
uint8_t key_pressed;

static GrfArray*
grf_display_get_image(GrfDisplay* display){
  GrfDisplayPrivate* priv       = grf_display_get_instance_private(display);
  return grf_imagewidget_get_image(GRF_IMAGEWIDGET(priv->imagewidget));
}

/*=================================
 * BUTTONS EVENTS
 *=================================*/
static void
grf_display_btn_save_clicked(GtkWidget* widget, gpointer user_data){
  (void) widget;
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);

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
     grf_image_write(grf_display_get_image(display),filename);
  }
  gtk_widget_destroy(dialog);
}
static void
grf_display_btn_zoom_in_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);

  float cur_scale = grf_imagewidget_get_scale(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), cur_scale * 1.5, TRUE);
}
static void
grf_display_btn_zoom_out_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);

  float cur_scale = grf_imagewidget_get_scale(GRF_IMAGEWIDGET(priv->imagewidget));
  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), cur_scale / 1.5, TRUE);
}
static void
grf_display_btn_zoom_ori_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);

  grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), 1, TRUE);
}
static void
grf_display_btn_zoom_fit_clicked(GtkWidget * widget, gpointer user_data){
  (void) widget;
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);

  //grf_imagewidget_set_scale(GRF_IMAGEWIDGET(priv->imagewidget), 1, TRUE);
}
static void
grf_display_btn_pan_left_clicked(GtkWidget * widget, gpointer user_data){

}
static void
grf_display_btn_pan_right_clicked(GtkWidget * widget, gpointer user_data){

}
static void
grf_display_btn_pan_top_clicked(GtkWidget * widget, gpointer user_data){

}
static void
grf_display_btn_pan_bottom_clicked(GtkWidget * widget, gpointer user_data){

}
static void
grf_display_btn_about_clicked(GtkWidget* widget, gpointer user_data){
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);
  g_auto(GStrv) authors   = g_strsplit("Anderson Tavares:Paulo Miranda", ":", -1);
  g_auto(GStrv) comments  = g_strsplit("Grafeo Library:Grafeo", ":", -1);
  gtk_show_about_dialog(GTK_WINDOW(priv->window),
                        "program-name","Grafeo",
                        "title",_("About Grafeo"),
                        "authors",authors,
                        "website","http://grafeo.github.io",
                        "website-label","http://grafeo.github.io",
                        "version","0.0.6",
                        "copyright","All rights reserved (C) 2015-2016",
                        "comments","Library for Graph/Computer Vision Problems",
                        "license",grf_library_get_license(),
                        NULL);

}

static void
grf_display_imagewidget_motion_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  (void) widget;
  double x, y;
  char text_position[13];
  char text_color[64];
  GrfDisplay* display     = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);
  GrfArray* image         = grf_display_get_image(display);

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

static void
grf_display_key_press_event(GtkWidget* window, GdkEventKey* event, gpointer user_data){
  (void) window;
  GrfDisplay*        display = GRF_DISPLAY(user_data);
  GrfDisplayPrivate* priv    = grf_display_get_instance_private(display);
  priv->key_pressed          = event->keyval;
  key_pressed                = priv->key_pressed;
  gtk_main_quit();
}

static void
grf_display_init(GrfDisplay* self){
  GrfDisplayPrivate* priv = grf_display_get_instance_private(self);
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

static GrfDisplay*
grf_display_new(char* name){
  GrfDisplay* display     = GRF_DISPLAY(g_object_new(GRF_TYPE_DISPLAY,NULL));
  GrfDisplayPrivate* priv = grf_display_get_instance_private(display);
  priv->name              = name;
  return display;
}

static void
grf_display_destroy(GtkWidget* widget, gpointer user_data){
  (void) widget; (void) user_data;
  grf_queue_remove(displays_queue,user_data);
  if(grf_queue_is_empty(displays_queue)) {
    cur_display = 0;
    gtk_main_quit();
  }
  g_object_unref(user_data);
}

// search window by name
GrfDisplay* grf_display_get_by_name(const char* name){
  GrfList* current;
  for(current = displays_queue->begin; current; current = grf_list_next(current))
  {
    GrfDisplay       * display = (GrfDisplay*)grf_list_value(current);
    GrfDisplayPrivate* priv    = grf_display_get_instance_private(display);
    if(!strcmp(priv->name, name)) return display;
  }
  return NULL;
}
/*=================================
 * PUBLIC API
 *=================================*/
void grf_display_setup(){
  gtk_init(0, NULL);  
  displays_queue     = grf_queue_new();
  cur_display        = NULL;
}

void
grf_display_named(const char* name){
  GrfDisplay* display = grf_display_get_by_name(name);
  GrfDisplayPrivate* priv;
  if(!display){
    display     = grf_display_new((char*)name);
    grf_queue_append(displays_queue,display);
    priv        = grf_display_get_instance_private(display);
    g_signal_connect(priv->window, "destroy"        , G_CALLBACK(grf_display_destroy), display);
    g_signal_connect(priv->window, "key-press-event", G_CALLBACK(grf_display_key_press_event), display);
  }
  cur_display = display;
}

void
grf_display_show(GrfArray* array){
  GrfDisplayPrivate* priv;
  if(!cur_display)
    grf_display_named("Figure 1");
  else
    priv        = grf_display_get_instance_private(cur_display);
  grf_imagewidget_set_image(GRF_IMAGEWIDGET(priv->imagewidget),array,TRUE);
  gtk_widget_show_all(priv->window);
}

uint8_t grf_display_waitkey(){
  gtk_main();
  return key_pressed;
}


//// Collection of displays
//static GrfQueue  * displays;
//static Display* cur_display;

//// search window by name
//Display* display_get_by_name(const char* name){
//  GrfList* current;
//  for(current = displays->begin; current; current = list_next(current))
//  {
//    Display* window = (Display*)list_value(current);
//    if(!strcmp(window->name, name)) return window;
//  }
//  return NULL;
//}
//Display* display_get_by_window(GtkWidget* window){
//  GrfList* current;
//  for(current = displays->begin; current; current = list_next(current))
//  {
//    Display* display = (Display*) list_value(current);
//    if(display->window == window) return display;
//  }
//  return NULL;
//}

//Display* window_new(){
//  Display* window = malloc(sizeof(Display));
//  return window;
//}

//static void window_delete_event(GtkWidget* window, gpointer data){
//  gtk_main_quit();
//}
//static void window_key_press_event(GtkWidget* window, GdkEventKey* event, gpointer user_data){
//  Display* display = display_get_by_window(window);
//  display->key_pressed = event->keyval;
//  gtk_main_quit();
//}

//// Called when window needs drawing
//static gboolean display_image_draw(GtkWidget* widget, cairo_t *cr, gpointer data){
//  return FALSE;
//}

//// existing window if name exists
//// new window for new name
//Display* display_new(char* name){
//  Display* display = display_get_by_name(name);
//  if(!display){
//    // Creation
//    display = window_new();
//    display->name        = name;
//    display->window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    display->box         = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//    display->imagewidget = grf_imagewidget_new();

//    // Linking them
//    gtk_box_pack_end(GTK_BOX(display->box), display->imagewidget, TRUE, TRUE, 0);
//    gtk_widget_show(display->imagewidget);
//    gtk_container_add(GTK_CONTAINER(display->window), display->box);
//    gtk_widget_show(display->box);

//    // Some properties and signals
//    gtk_window_set_title(GTK_WINDOW(display->window), name);
//    gtk_widget_hide_on_delete(display->window);
//    gtk_window_set_decorated(GTK_WINDOW(display->window),FALSE);
//    gtk_window_set_default_size(GTK_WINDOW(display->window), 200, 200);

//    g_signal_connect(display->window,       "delete-event",    G_CALLBACK(window_delete_event), NULL);
//    g_signal_connect(display->window,       "key_press_event", G_CALLBACK(window_key_press_event),NULL);
//    //g_signal_connect(display->imagewidget,  "draw",            G_CALLBACK(display_image_draw), display);

//    // Append to the list of displays
//    queue_append(displays, display);
//  }
//  return display;
//}

//uint8_t display_num_windows(){
//  return queue_length(displays);
//}



//void    display_show(GrfArray* array){
//  if(!display_num_windows()) cur_display = display_new("Figure 1");
//  gtk_widget_show_all(cur_display->window);
//}

//uint8_t display_wait_key(){
//  gtk_main();
//  return cur_display->key_pressed;
//}
