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
#include <setjmp.h>
#include <cmocka.h>

static void test_displaywindow(void** state){
  (void) state;
  char* filenames[3]     = {"../data/trekkie-nerdbw.png",           // Gray
                            "../data/distance_transform_input.pgm", // Gray
                            "../data/trekkie-nerd.jpg"};            // Color
  GrfArray* image = grf_image_read(filenames[0]);
  GrfDisplayWindow* window = grf_displaywindow_new();
  grf_displaywindow_set_name(window, "teste");
  grf_displaywindow_set_image(window, image,TRUE);
  grf_displaywindow_show(window);
  grf_displaywindow_quit_on_destroy(window, TRUE);
  gtk_main();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  gtk_init(&argc, &argv);
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_displaywindow)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
