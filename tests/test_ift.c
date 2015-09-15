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

static double weight_diff(Array *array, uint64_t index1, uint64_t index2){
  switch (array->type){
    case GRAFEO_UINT8:  return (double) array->data_uint8[index1]  - (double) array->data_uint8[index2];break;
    case GRAFEO_UINT16: return (double) array->data_uint16[index1] - (double) array->data_uint16[index2];break;
    case GRAFEO_UINT32: return (double) array->data_uint32[index1] - (double) array->data_uint32[index2];break;
    case GRAFEO_UINT64: return (double) array->data_uint64[index1] - (double) array->data_uint64[index2];break;
    case GRAFEO_INT8:   return (double) array->data_int8[index1]   - (double) array->data_int8[index2];break;
    case GRAFEO_INT16:  return (double) array->data_int16[index1]  - (double) array->data_int16[index2];break;
    case GRAFEO_INT32:  return (double) array->data_int32[index1]  - (double) array->data_int32[index2];break;
    case GRAFEO_INT64:  return (double) array->data_int64[index1]  - (double) array->data_int64[index2];break;
    case GRAFEO_FLOAT:  return (double) array->data_float[index1]  - (double) array->data_float[index2];break;
    case GRAFEO_DOUBLE: return (double) array->data_double[index1] - (double) array->data_double[index2];break;
  }
  return 0;
}

static void helper_test_ift(const char* imagepath, const char* correctpath, double path_connectivity(double connectivity_value, double weight_value)){
  // Load an image, its labels, connectivity and root maps
  // 8x8 image
  uint8_t data[64] = {000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,255,255,000,000,000,
                      000,000,000,255,255,000,000,000,
                      000,000,255,255,255,255,000,000,
                      000,000,255,255,255,255,000,000,
                      000,255,255,255,255,255,255,000,
                      000,000,000,000,000,000,000,000};
  uint8_t predecessors[64] = {
                      000,000,001,002,003,004,005,006,
                      000,008,009,010,011,012,013,014,
                      008,000,000,255,255,000,000,000,
                      016,000,000,255,255,000,000,000,
                      024,000,255,255,255,255,000,000,
                      032,000,255,255,255,255,000,000,
                      040,255,255,255,255,255,255,000,
                      048,000,000,000,000,000,000,000};
  uint8_t label[64] = {000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,001,001,000,000,000,
                      000,000,000,001,001,000,000,000,
                      000,000,001,001,001,001,000,000,
                      000,000,001,001,001,001,000,000,
                      000,001,001,001,001,001,001,000,
                      000,000,000,000,000,000,000,000};
  uint8_t connectivity[64] = {
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000};
  uint8_t root[64] = {000,000,000,000,000,000,000,000,
                      000,000,000,000,000,000,000,000,
                      000,000,000,044,044,000,000,000,
                      000,000,000,044,044,000,000,000,
                      000,000,044,044,044,044,000,000,
                      000,000,044,044,044,044,000,000,
                      000,044,044,044,044,044,044,000,
                      000,000,000,000,000,000,000,000};



  uint32_t size[3] = {8,8,1};
  Array* image = array_from_data(data, 3, size);

  // Apply original IFT with fsum
  IFT*   ift    = ift_apply_array(image, GRAFEO_NEIGHBOR_4, weight_diff, path_connectivity);

  // Load correct values (keys: label, root, connectivity, predecessor)
  HashTable* correct = array_load(correctpath);
  Array* correct_label        = hashtable_lookup(correct, "label");
  Array* correct_root         = hashtable_lookup(correct, "root");
  Array* correct_connectivity = hashtable_lookup(correct, "connectivity");
  Array* correct_predecessor  = hashtable_lookup(correct, "predecessor");

  // Check Values
  assert_array_equal(ift_get_label(ift)       , correct_label);
  assert_array_equal(ift_get_root(ift)        , correct_root);
  assert_array_equal(ift_get_connectivity(ift), correct_connectivity);
  assert_array_equal(ift_get_predecessor(ift) , correct_predecessor);

  // Free memory
  hashtable_free(correct);
  ift_free(ift);
  array_free(image);
}

static void test_ift_sum(void** state){
  (void) state;
  helper_test_ift("../data/trianglebw.png","../data/trianglebw_ift_sum.zip", path_connectivity_sum);
}
static void test_ift_max(void** state){
  (void) state;
  helper_test_ift("../data/trianglebw.png","../data/trianglebw_ift_max.zip", path_connectivity_max);
}
static void test_ift_min(void** state){
  (void) state;
  helper_test_ift("../data/trianglebw.png","../data/trianglebw_ift_min.zip", path_connectivity_min);
}
static void test_ift_euc(void** state){
  (void) state;
  helper_test_ift("../data/trianglebw.png","../data/trianglebw_ift_euc.zip", path_connectivity_euc);
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
