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
#include <grafeo/drawing.h>
#include <setjmp.h>
#include <cmocka.h>


GrfArray*image, *image2;
uint8_t pressionado;
int variable1;
void mouse_callback_1(GrfMouseEventType event, int x, int y, GrfMouseEventFlags flags, void* user_data){
  (void) user_data; (void) flags;
  if(event & GRF_MOUSE_EVENT_PRESS) pressionado = 1;
  else if(event & GRF_MOUSE_EVENT_RELEASE) pressionado = 0;
  else if(event & GRF_MOUSE_EVENT_MOVE && pressionado){
    GrfScalar2D center = {x,y};
    GrfScalar4D color = grf_scalar4D_new(0,255,0,255);
    grf_array_draw_circle(image,center,3,&color,-1,GRF_NEIGHBOR_8,0);
    grf_display_named("Figure1");
    grf_display_show(image);
  }
}

void trackbar_changed(int pos){
  printf("%d %d\n",variable1, pos);
}

static void test_display(void** state){
  (void)   state;
  char*    filenames[3]  = {"../data/trekkie-nerdbw.png",           // Gray
                            "../data/distance_transform_input.pgm", // Gray
                            "../data/trekkie-nerd.jpg"};            // Color
  image         = grf_image_read(filenames[2]);
  image2        = grf_image_read(filenames[1]);
  pressionado   = 0;

  // Figure 1
  grf_display_named("Figure1");
  grf_display_connect_mouse_callback("Figure1",mouse_callback_1,NULL);
  grf_display_add_trackbar("Figure1","Variável",&variable1,0,50,trackbar_changed);
  grf_display_add_trackbar("Figure1","Variável2",&variable1,50,100,trackbar_changed);
  grf_display_show(image);

  // Figure 2
  grf_display_named("Figure2");
  grf_display_show(image2);

  // Wait Key
  uint8_t key = 0;
  while(key != 27){
    key = grf_display_waitkey();
    printf("%d\n",key);
  }
  printf("%d\n", key);

  // Show first image in the second window
  grf_display_named("Figure2");
  grf_display_show(image);
  key  = grf_display_waitkey();

  grf_array_free(image);
  grf_array_free(image2);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  grf_display_setup(&argc, &argv);
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_display)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
