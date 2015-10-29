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

static uint8_t pressionado;
static guint context;
static GtkWidget* statusbar;
static GtkWidget* lbl_color;

static void pressionar(GtkWidget* widget, GdkEvent *event, gpointer user_data){
  printf("press\n");
  pressionado = 1;
}
static void soltar(GtkWidget* widget, GdkEvent *event, gpointer user_data){
  printf("soltar\n");
  pressionado = 0;
}
static void mover(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  printf("mover\n");
  GrfArray* imagem = (GrfArray*)user_data;
  double x, y;
  char texto[10];
  char texto2[60];
  gdk_event_get_coords(event, &x, &y);
  uint64_t pixel_index = y*imagem->step[0]+x*imagem->step[1];
  uint8_t* pixel = &imagem->data_uint8[pixel_index];
  sprintf(texto,"%d %d", (int)x,(int)y);
  sprintf(texto2,"(<span color=\"red\">R=%03d</span>, <span color=\"green\">G=%03d</span>, <span color=\"blue\">B=%03d</span>)", pixel[0],pixel[1],pixel[2]);
  gtk_label_set_markup(GTK_LABEL(lbl_color), "<span color=\"red\">small</span>");
  gtk_label_set_label(GTK_LABEL(lbl_color),texto2);

  if(pressionado){
    GrfScalar2D centro = {x,y};
    GrfScalar4D cor = grf_scalar4D_new (255,0,0,255);
    grf_array_draw_circle(imagem,centro,3,&cor,-1,GRF_NEIGHBOR_8,0);
    GrfImageWidget* imagewidget = GRF_IMAGEWIDGET(widget);
    grf_imagewidget_set_image(imagewidget, imagem,TRUE);
    gtk_widget_queue_draw(widget);
  }
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), context, texto);
}

static void window_key_press_event(GtkWidget* widget, GdkEventKey* event){
  printf("evento\n");
  //gtk_main_quit();
}

static void scroll(GtkWidget* widget, GdkEvent *event, gpointer user_data){

}

static void test_grf_imagewidget_show(void**state){
  (void) state;
  gtk_init(NULL, NULL);
  pressionado            = 0;
  char* filenames[3]     = {"../data/trekkie-nerdbw.png",           // Gray
                            "../data/distance_transform_input.pgm", // Gray
                            "../data/trekkie-nerd.jpg"};            // Color
  GrfArray*  array_gray  = grf_image_read(filenames[2]);
  GtkWidget* imagewidget = grf_imagewidget_new();
  GtkWidget* window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget* box         = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  lbl_color   = gtk_label_new("");
  statusbar              = gtk_statusbar_new();
  context = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),"example");

  gtk_widget_set_size_request(statusbar,-1,30);
  gtk_widget_add_events(imagewidget, GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

  GrfScalar2D p0 = {200,450}, p1 = {300,450};
  GrfScalar4D color = grf_scalar4D_new(255,0,0,255);

  grf_array_draw_line(array_gray,p0,p1,&color,1,GRF_NEIGHBOR_8, 0);
  grf_array_draw_circle(array_gray,p0,3,&color,-1,GRF_NEIGHBOR_8,0);
  grf_imagewidget_set_image(GRF_IMAGEWIDGET(imagewidget),array_gray, TRUE);

  gtk_box_pack_start (GTK_BOX(box),imagewidget,TRUE,TRUE,0);
  gtk_box_pack_start (GTK_BOX(box),statusbar,FALSE,TRUE,0);
  gtk_box_pack_end(GTK_BOX(statusbar),lbl_color,FALSE,FALSE,0);
  gtk_container_add  (GTK_CONTAINER(window), box);
  g_signal_connect   (imagewidget, "button-press-event"  , G_CALLBACK(pressionar), NULL);
  g_signal_connect   (imagewidget, "button-release-event", G_CALLBACK(soltar)    , NULL);
  g_signal_connect   (imagewidget, "motion-notify-event" , G_CALLBACK(mover)    , array_gray);
  g_signal_connect   (imagewidget, "scroll-event"        , G_CALLBACK(scroll)    , NULL);
  g_signal_connect   (window     , "destroy"             , G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect   (window     , "key-press-event"     , G_CALLBACK(window_key_press_event), NULL);
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
