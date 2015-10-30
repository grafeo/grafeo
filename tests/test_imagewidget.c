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
#include <grafeo/image.h>
#include <grafeo/drawing.h>
#include <setjmp.h>
#include <cmocka.h>

static uint8_t    pressed;
static double     curx,cury;
static guint      context;
static GtkWidget* statusbar;
static GtkWidget* lbl_color;
static uint8_t    ctrl_pressed;

static void helper_test_imagewidget_press_event(GtkWidget* widget, GdkEvent *event, gpointer user_data){
  (void) widget;  (void) user_data;
  printf("press\n");
  pressed = 1;
  gdk_event_get_coords(event, &curx, &cury);
}
static void helper_test_imagewidget_release_event(GtkWidget* widget, GdkEvent *event, gpointer user_data){
  (void) widget;  (void) event; (void) user_data;

  printf("soltar\n");
  pressed = 0;
}
static void helper_test_imagewidget_move_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  double x, y;
  char text_position[10];
  char text_color[60];

  GrfImageWidget* imagewidget         = GRF_IMAGEWIDGET(widget);
  float         * current_translation = grf_imagewidget_get_translation(imagewidget);
  GrfArray      * image              = (GrfArray*)user_data;

  gdk_event_get_coords(event, &x, &y);

  // Translation or brush
  if(pressed){
    if(ctrl_pressed)
      grf_imagewidget_set_translation(imagewidget,current_translation[0]+(x-curx),current_translation[1]+(y-cury),TRUE);
    else{
      GrfScalar2D centro      = {x,y};
      GrfScalar4D cor         = grf_scalar4D_new (255,0,0,255);
      grf_array_draw_circle(image,centro,3,&cor,-1,GRF_NEIGHBOR_8,0);
      grf_imagewidget_set_image(imagewidget, image,TRUE);
    }
  }

  // Get current pixel color
  uint64_t    pixel_index = ((int)y)*image->step[0]+((int)x)*image->step[1];
  uint8_t  *  pixel       = &image->data_uint8[pixel_index];
  if(image->dim == 2 || image->size[2] == 1)
    sprintf(text_color,"(G = %03d)", pixel[0]);
  else
    sprintf(text_color,"(<span color=\"red\">R=%03d</span>, <span color=\"green\">G=%03d</span>, <span color=\"blue\">B=%03d</span>)", pixel[2],pixel[1],pixel[0]);
  gtk_label_set_markup(GTK_LABEL(lbl_color), "");
  gtk_label_set_label(GTK_LABEL(lbl_color),text_color);

  // Get current position
  sprintf(text_position,"%d %d", (int)x,(int)y);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), context, text_position);

  curx = x;
  cury = y;
}

static gboolean window_key_press_event(GtkWidget* widget, GdkEventKey* event, gpointer user_data){
  (void) widget;  (void) user_data;
  if(event->keyval == GDK_KEY_Control_L)
    ctrl_pressed = TRUE;
  return TRUE;
}
static gboolean window_key_release_event(GtkWidget* widget, GdkEventKey* event, gpointer user_data){
  (void) widget;  (void) user_data;
  if(event->keyval == GDK_KEY_Control_L)
    ctrl_pressed = FALSE;
  return TRUE;
}

static void helper_test_imagewidget_scroll_event(GtkWidget* widget, GdkEvent *event, gpointer user_data){
  (void) user_data;
  printf("scrolling\n");
  GdkScrollDirection direction;
  GrfImageWidget* imagewidget = GRF_IMAGEWIDGET(widget);
  gdk_event_get_scroll_direction(event,&direction);
  if(direction == GDK_SCROLL_UP)
    grf_imagewidget_set_scale(imagewidget,grf_imagewidget_get_scale(imagewidget)*1.5,TRUE);
  else
    grf_imagewidget_set_scale(imagewidget,grf_imagewidget_get_scale(imagewidget)/1.5,TRUE);
}

static void test_grf_imagewidget_show(void**state){
  (void) state;
  gtk_init(NULL, NULL);
  pressed            = 0;
  char* filenames[3]     = {"../data/trekkie-nerdbw.png",           // Gray
                            "../data/distance_transform_input.pgm", // Gray
                            "../data/trekkie-nerd.jpg"};            // Color
  GrfArray*  array_gray  = grf_image_read(filenames[0]);
  GtkWidget* imagewidget = grf_imagewidget_new();
  GtkWidget* window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* box         = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  lbl_color              = gtk_label_new("");
  statusbar              = gtk_statusbar_new();
  context = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),"example");

  gtk_widget_set_size_request(statusbar,-1,30);
  gtk_widget_add_events(imagewidget, GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
  gtk_widget_add_events(imagewidget,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

  grf_imagewidget_set_image(GRF_IMAGEWIDGET(imagewidget),array_gray, TRUE);

  gtk_box_pack_start (GTK_BOX(box),imagewidget,TRUE,TRUE,0);
  gtk_box_pack_start (GTK_BOX(box),statusbar,FALSE,TRUE,0);
  gtk_box_pack_end(GTK_BOX(statusbar),lbl_color,FALSE,FALSE,0);
  gtk_container_add  (GTK_CONTAINER(window), box);
  // Mouse events
  g_signal_connect   (imagewidget, "button-press-event"  , G_CALLBACK(helper_test_imagewidget_press_event), NULL);
  g_signal_connect   (imagewidget, "button-release-event", G_CALLBACK(helper_test_imagewidget_release_event)    , NULL);
  g_signal_connect   (imagewidget, "motion-notify-event" , G_CALLBACK(helper_test_imagewidget_move_event)    , array_gray);
  g_signal_connect   (imagewidget, "scroll-event"        , G_CALLBACK(helper_test_imagewidget_scroll_event)    , NULL);
  // Keyboard events
  g_signal_connect   (imagewidget, "key-press-event"     , G_CALLBACK(window_key_press_event), NULL);
  g_signal_connect   (imagewidget, "key-release-event"   , G_CALLBACK(window_key_release_event), NULL);
  // Other events
  g_signal_connect   (window     , "destroy"             , G_CALLBACK(gtk_main_quit),NULL);

  // Show window
  gtk_widget_show_all(window);
  gtk_main();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_imagewidget_show)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
