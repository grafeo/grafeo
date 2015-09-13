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
#include <unistd.h>
#include <errno.h>
#include <grafeo/array.h>
#include <grafeo/image.h>
#include <grafeo/ift.h>

static void test_ift_sum(void** state){
  (void) state;
  // Load an image
  Array* image  = image_read("../data/trianglebw.png");

  // Apply original IFT with fsum
  IFT*   ift    = ift_apply_array(image, GRAFEO_NEIGHBOR_4, path_connectivity_sum);

  // Compare it with groundtruth
  Array* ground = image_read("../data/trianglebw_iftground.png");
  uint64_t i;
  for(i = 0; i < ground->num_bytes; i++)
    assert_int_equal(ground->data_uint8[i], ift_get_label(ift)->data_uint8[i]);

  array_free(image);
  array_free(ground);
  ift_free(ift);
}

static void test_ift_max(void** state){

}
static void test_ift_min(void** state){

}
static void test_ift_euc(void** state){

}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_ift_sum),
    cmocka_unit_test(test_ift_max),
    cmocka_unit_test(test_ift_min),
    cmocka_unit_test(test_ift_euc),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
