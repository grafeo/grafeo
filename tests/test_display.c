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
#include <grafeo/array.h>
#include <setjmp.h>
#include <cmocka.h>
static void test_display(void** state){
  (void)   state;
  char*    filenames[3]  = {"../data/trekkie-nerdbw.png",           // Gray
                            "../data/distance_transform_input.pgm", // Gray
                            "../data/trekkie-nerd.jpg"};            // Color
  GrfArray*image         = grf_image_read(filenames[2]);
  GrfArray*image2        = grf_image_read(filenames[1]);
  grf_display_setup();
  grf_display_named("Figure1");
  grf_display_show(image);
  grf_display_named("Figure2");
  grf_display_show(image2);
  uint8_t key = 0;
  while(key != 27)
    key = grf_display_waitkey();
  printf("%d\n", key);

  grf_display_show(image);
  key  = grf_display_waitkey();

  grf_array_free(image);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_display)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
