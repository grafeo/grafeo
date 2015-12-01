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
#include <grafeo/core.h>

/*=================================
 * PRIVATE API
 *=================================*/
static GrfDisplayWindow* cur_display;
static GrfQueue        * displays_queue;
uint8_t                  key_pressed;

// search window by name
GrfDisplayWindow* grf_display_get_by_name(const char* name){
  GrfList* current;
  for(current = displays_queue->begin; current; current = grf_list_next(current))
  {
    GrfDisplayWindow   * display = GRF_DISPLAYWINDOW(grf_list_value(current));
    if(!strcmp(grf_displaywindow_get_name(display), name)) return display;
  }
  return NULL;
}
static void
key_callback(GrfKeyEventType event, int key_val, void* user_data){
  (void) user_data;
  if(event == GRF_KEY_EVENT_RELEASE){
    key_pressed = key_val;
    gtk_main_quit();
  }
}

/*=================================
 * PUBLIC API
 *=================================*/
void
grf_display_setup(int *argc, char*** argv){
  gtk_init(argc, argv);
  displays_queue     = grf_queue_new();
  cur_display        = NULL;
}

void
grf_display_named(const char* name){
  GrfDisplayWindow* display = grf_display_get_by_name(name);
  if(!display){
    display     = grf_displaywindow_new_with_name((char*)name);
    grf_queue_append(displays_queue,display);
    grf_displaywindow_connect_key_callback(display, key_callback, NULL);
  }
  cur_display = display;
}

void
grf_display_show(GrfNDArray* array){
  if(!cur_display)
    grf_display_named("Figure 1");
  grf_displaywindow_set_image(cur_display, array, TRUE);
  grf_displaywindow_show(cur_display);
}

uint8_t grf_display_waitkey(){
  gtk_main();
  return key_pressed;
}

void
grf_display_connect_mouse_callback(const char* name,
                                   GrfMouseCallback mouse_callback,
                                   void* user_data){
  GrfDisplayWindow* window = grf_display_get_by_name(name);
  if(window)
    grf_displaywindow_connect_mouse_callback(window, mouse_callback, user_data);
}

void
grf_display_disconnect_mouse_callback(const char* name){
  GrfDisplayWindow* window = grf_display_get_by_name(name);
  if(window)
    grf_displaywindow_disconnect_mouse_callback(window);
}

/*=================================
 * PUBLIC API: Display Trackbar
 *=================================*/
int
grf_display_add_trackbar(const char* display_name,
                         const char* track_name,
                         int* variable,
                         int min_value, int max_value,
                         GrfTrackbarCallback trackbar_changed_event){
  GrfDisplayWindow* display   = grf_display_get_by_name(display_name);
  if(!display) return 0;
  GrfTrackbar* trackbar = grf_displaywindow_get_trackbar_by_name(display, (char*)track_name);
  if(trackbar) return 0;
  else{
    trackbar = grf_trackbar_new_with_name((char*)track_name);
    grf_displaywindow_add_trackbar(display,trackbar);
    grf_trackbar_set_max(trackbar,max_value);
    grf_trackbar_set_min(trackbar,min_value);
    grf_trackbar_connect_change_callback(trackbar, variable, trackbar_changed_event);
  }
  return 1;
}

//int
//grf_display_add_trackbar(const char* display_name, const char* track_name, int* variable, int min_value, int max_value, GrfTrackbarCallback grf_trackbar_changed_event){
//  // Find trackbar
//  GrfDisplay* display   = grf_display_get_by_name(display_name);
//  if(!display) return 0;

//  GrfTrackbar* trackbar = grf_trackbar_get_by_name(display, track_name);
//  // Not found? Create-it
//  if(!trackbar){
//    trackbar = grf_trackbar_new_with_name(display_name);
//    grf_trackbar_set_display(display);

//  }
//}

//int
//grf_display_add_trackbar_with_data(const char* display_name, const char* track_name, int* variable, int min_value, int max_value, GrfTrackbarDataCallback grf_trackbar_changed_event, void* user_data){

//}

//int
//grf_display_get_trackbar_pos(const char* display_name, const char* track_name){
//  GrfDisplay * display  = grf_display_get_by_name(display_name);
//  GrfTrackbar* trackbar = grf_display_get_trackbar_by_name(display, track_name);
//}

//void
//grf_display_get_trackbar_min(const char* display_name, const char* track_name, int pos){

//}

//void
//grf_display_get_trackbar_max(const char* display_name, const char* track_name, int pos){

//}

//void
//grf_trackbar_set_min(const char* display_name, const char* track_name, int min_val){

//}

//void
//grf_trackbar_set_man(const char* display_name, const char* track_name, int max_val){

//}


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



//void    display_show(GrfNDArray* array){
//  if(!display_num_windows()) cur_display = display_new("Figure 1");
//  gtk_widget_show_all(cur_display->window);
//}

//uint8_t display_wait_key(){
//  gtk_main();
//  return cur_display->key_pressed;
//}
