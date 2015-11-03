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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <grafeo/dicom.h>
#include <grafeo/display.h>
#include <grafeo/drawing.h>

static uint8_t pressed = 0;
static uint8_t cur_object  = 0;
static void mouse_callback(GrfMouseEventType type, int x, int y, GrfMouseEventFlags flags, void* user_data){
  if(type & GRF_MOUSE_EVENT_PRESS)     pressed = 1;
  else if(type & GRF_MOUSE_EVENT_RELEASE) pressed = 0;
  else if(type & GRF_MOUSE_EVENT_MOVE && pressed) {
    GrfArray* image = (GrfArray*) user_data;
    GrfScalar2D center = {x,y};
    GrfScalar4D color = grf_scalar4D_new(255,0,0,255);
    if(cur_object == 1) color.x = 127;
    grf_array_draw_circle(image,center,3,&color,-1,GRF_NEIGHBOR_8,0);
    grf_display_show(image);
  }
}

static void test_grf_dicom_read(void** state){
  (void) state;
  const char* filename = "../data/MR-MONO2-16-head.dcm";
  GrfDicom* dicom = grf_dicom_read(filename);
  assert_non_null(dicom);
  assert_string_equal(dicom->filename, filename);
  assert_string_equal(dicom->modality, "MR");
  assert_true(dicom->pixel_data_tag_found);
  assert_string_equal(dicom->unit, "mm");
  assert_int_equal(dicom->image->dim, 3);
  assert_int_equal(dicom->image->bitsize, 2);
  assert_int_equal(dicom->image->size[0], 256);
  assert_int_equal(dicom->image->size[1], 256);
  assert_int_equal(dicom->image->size[2], 1);

//  // This is for displaying the image
//  long double max_value = grf_array_reduce_max_num(dicom->image);
//  GrfArray* image2 = grf_array_mult_scalar(dicom->image,255.0/max_value);
//  GrfArray* image3 = grf_array_as_type(image2,GRF_UINT8);
//  grf_display_named("Figure1");
//  grf_display_connect_mouse_callback("Figure1",mouse_callback,image3);
//  grf_display_show(image3);

//  uint8_t key = 0;
//  while(key != 27) {
//    key = grf_display_waitkey();
//    switch(key){
//    case '1':
//      cur_object = 0;
//      break;
//    case '2':
//      cur_object = 1;
//      break;
//    }
//  }

  grf_dicom_free(dicom);
}
static void test_grf_dicom_write(void** state){
  (void) state;
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  grf_display_setup(&argc,&argv);
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_grf_dicom_read),
    cmocka_unit_test(test_grf_dicom_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
