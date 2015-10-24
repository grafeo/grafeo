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

typedef struct _DisplayPrivate{
  GtkWidget  * imagewidget;
  GtkWidget  * window;
  GtkWidget  * toolbar;
  GtkWidget  * box;
  char       * name;
}DisplayPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(Display, display, G_TYPE_OBJECT)

static void
display_init(Display* self){
  DisplayPrivate* priv = display_get_instance_private(self);
  priv->imagewidget = imagewidget_new();
  priv->toolbar     = gtk_toolbar_new();
  priv->window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  priv->box         = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

  gtk_box_pack_start(GTK_BOX(priv->box),priv->toolbar    ,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(priv->box),priv->imagewidget,TRUE,TRUE,0);
  gtk_container_add(GTK_CONTAINER(priv->window) ,priv->box);


}


static void
display_class_init(DisplayClass *klass){

}

void display_setup(){
  gtk_init(0, NULL);
//  displays     = queue_new();
//  cur_display  = NULL;
}

static Display* display_new(){
  return GRAFEO_DISPLAY(g_object_new(GRAFEO_TYPE_DISPLAY,NULL));
}

void    display_show(Array* array){
  Display       * display = display_new();
  DisplayPrivate* priv    = display_get_instance_private(display);
  imagewidget_set_image(priv->imagewidget,array);
  gtk_widget_show_all(priv->window);
}

uint8_t display_wait_key(){
  gtk_main();
  return 0;
}


//// Collection of displays
//static Queue  * displays;
//static Display* cur_display;

//// search window by name
//Display* display_get_by_name(const char* name){
//  List* current;
//  for(current = displays->begin; current; current = list_next(current))
//  {
//    Display* window = (Display*)list_value(current);
//    if(!strcmp(window->name, name)) return window;
//  }
//  return NULL;
//}
//Display* display_get_by_window(GtkWidget* window){
//  List* current;
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
//    display->imagewidget = imagewidget_new();

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



//void    display_show(Array* array){
//  if(!display_num_windows()) cur_display = display_new("Figure 1");
//  gtk_widget_show_all(cur_display->window);
//}

//uint8_t display_wait_key(){
//  gtk_main();
//  return cur_display->key_pressed;
//}
