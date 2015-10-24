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
#include <setjmp.h>
#include <cmocka.h>

static void test_imagewidget_show(void**state){
  gtk_init(NULL, NULL);
  GtkWidget* imagewidget = imagewidget_new();
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //GtkWidget* box    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  //gtk_box_pack_start (GTK_BOX(box),imagewidget,TRUE,TRUE,0);
  gtk_container_add  (GTK_CONTAINER(window), imagewidget);
  g_signal_connect   (window, "destroy", G_CALLBACK(gtk_main_quit),NULL);
  gtk_widget_show_all(window);
  gtk_main();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_imagewidget_show)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
