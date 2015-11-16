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
#include <grafeo/medical.h>
#include <setjmp.h>
#include <cmocka.h>

int16_t*  dados, *offset;
grfsize_t size[3];
GrfArray* array = NULL;
GrfArray* output = NULL;
int       slice = 0;
GrfNiftiImage* image;

static void get_data(){
  dados    = (int16_t*)image->data;
  offset   = dados + slice*(256*256);
  if(array)  grf_array_free(array);
  if(output) grf_array_free(output);
  array    = grf_array_from_data(offset,2,size,GRF_INT16);
  output   = grf_array_as_type(array,GRF_UINT8);
}

static void change_event(int pos){
  slice = pos;
  get_data();
  grf_display_named("Figure1");
  grf_display_show(output);
}

static void test_grf_nifti_open(void** state){
  image = grf_nifti_image_read("../data/MRHead.nii.gz",1);

  size[0] = 256;
  size[1] = 256;
  size[2] = 1;
  slice = 0;
  get_data();

  grf_display_setup(NULL, NULL);
  grf_display_named("Figure1");
  grf_display_add_trackbar("Figure1","slice num",&slice,0,129,change_event);
  grf_display_show(output);
  uint8_t key = 0;while(key != 27) key = grf_display_waitkey();
  grf_image_write(output,"teste.png");


  //znzFile file = nifti_image_open("../data/MRHead.nii.gz","w6h",&image);
  grf_nifti_image_free(image);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_nifti_open),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
