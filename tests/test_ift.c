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
#include <math.h>

static void assert_array_equal(Array* array1, Array* array2){
  uint64_t i;
  assert_int_equal(array1->dim,          array2->dim);
  assert_int_equal(array1->type,         array2->type);
  assert_int_equal(array1->num_bytes,    array2->num_bytes);
  assert_int_equal(array1->num_elements, array2->num_elements);
  for(i = 0; i < array1->dim; i++)
    assert_int_equal(array1->size[i],      array2->size[i]);
  for(i = 0; i < array1->num_elements; i++)
    assert_int_equal(array1->data_uint8[i], array2->data_uint8[i]);

}

static double weight_diff(Array *array, uint64_t index1, uint64_t index2){
  switch (array->type){
    case GRAFEO_UINT8:  return fabs((double) array->data_uint8[index1]  - (double) array->data_uint8[index2]);break;
    case GRAFEO_UINT16: return fabs((double) array->data_uint16[index1] - (double) array->data_uint16[index2]);break;
    case GRAFEO_UINT32: return fabs((double) array->data_uint32[index1] - (double) array->data_uint32[index2]);break;
    case GRAFEO_UINT64: return fabs((double) array->data_uint64[index1] - (double) array->data_uint64[index2]);break;
    case GRAFEO_INT8:   return fabs((double) array->data_int8[index1]   - (double) array->data_int8[index2]);break;
    case GRAFEO_INT16:  return fabs((double) array->data_int16[index1]  - (double) array->data_int16[index2]);break;
    case GRAFEO_INT32:  return fabs((double) array->data_int32[index1]  - (double) array->data_int32[index2]);break;
    case GRAFEO_INT64:  return fabs((double) array->data_int64[index1]  - (double) array->data_int64[index2]);break;
    case GRAFEO_FLOAT:  return fabs((double) array->data_float[index1]  - (double) array->data_float[index2]);break;
    case GRAFEO_DOUBLE: return fabs((double) array->data_double[index1] - (double) array->data_double[index2]);break;
  }
  return 0;
}

static void helper_test_ift(const char* imagepath, const char* correctpath, PathConnectivityFunc path_connectivity){
  // Load an image, its labels, connectivity and root maps
  // 8x8 image
  uint8_t data[4]        = {000,000,    255,255};
  uint8_t predecessor[4] = {000,000,    003,003};
  uint8_t label[4]       = {000,000,    001,001};
  uint8_t connectivity[4]= {000,000,    000,000};
  uint8_t root[4]        = {000,000,    003,003};

  // Generate the image
  uint32_t size[3] = {2,2};
  Array* image                = array_from_data(data,         2, size, GRAFEO_UINT8);
  Array* correct_predecessor  = array_from_data(predecessor,  2, size, GRAFEO_UINT8);
  Array* correct_label        = array_from_data(label,        2, size, GRAFEO_UINT8);
  Array* correct_connectivity = array_from_data(connectivity, 2, size, GRAFEO_UINT8);
  Array* correct_root         = array_from_data(root,         2, size, GRAFEO_UINT8);

  // Run standard IFT
  IFT*     ift     = ift_apply_array(image, GRAFEO_NEIGHBOR_4, GRAFEO_IFT_MIN, weight_diff, path_connectivity);

  assert_non_null(ift);
  assert_non_null(ift->connectivity);
  assert_non_null(ift->label);
  assert_non_null(ift->original);
  assert_non_null(ift->predecessors);
  assert_non_null(ift->root);

  // Check Values
  assert_array_equal(ift_get_label(ift)       , correct_label);
  assert_array_equal(ift_get_root(ift)        , correct_root);
  assert_array_equal(ift_get_connectivity(ift), correct_connectivity);
  assert_array_equal(ift_get_predecessors(ift) , correct_predecessor);

  ift_free(ift);
  array_free(image);

//  uint8_t data[64] = {000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,255,255,000,000,000,
//                      000,000,000,255,255,000,000,000,
//                      000,000,255,255,255,255,000,000,
//                      000,000,255,255,255,255,000,000,
//                      000,255,255,255,255,255,255,000,
//                      000,000,000,000,000,000,000,000};
//  uint8_t predecessors[64] = {
//                      000,000,001,002,003,004,005,006,
//                      000,008,009,010,011,012,013,014,
//                      008,000,000,255,255,000,000,000,
//                      016,000,000,255,255,000,000,000,
//                      024,000,255,255,255,255,000,000,
//                      032,000,255,255,255,255,000,000,
//                      040,255,255,255,255,255,255,000,
//                      048,000,000,000,000,000,000,000};
//  uint8_t label[64] = {000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,001,001,000,000,000,
//                      000,000,000,001,001,000,000,000,
//                      000,000,001,001,001,001,000,000,
//                      000,000,001,001,001,001,000,000,
//                      000,001,001,001,001,001,001,000,
//                      000,000,000,000,000,000,000,000};
//  uint8_t connectivity[64] = {
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000};
//  uint8_t root[64] = {000,000,000,000,000,000,000,000,
//                      000,000,000,000,000,000,000,000,
//                      000,000,000,044,044,000,000,000,
//                      000,000,000,044,044,000,000,000,
//                      000,000,044,044,044,044,000,000,
//                      000,000,044,044,044,044,000,000,
//                      000,044,044,044,044,044,044,000,
//                      000,000,000,000,000,000,000,000};
//  uint32_t size[3] = {8,8,1};
//  Array* image = array_from_data(data, 3, size);

//  // Apply original IFT with fsum
//  IFT*   ift    = ift_apply_array(image, GRAFEO_NEIGHBOR_4, weight_diff, path_connectivity);

//  // Load correct values (keys: label, root, connectivity, predecessor)
//  HashTable* correct = array_load(correctpath);
//  Array* correct_label        = hashtable_lookup(correct, "label");
//  Array* correct_root         = hashtable_lookup(correct, "root");
//  Array* correct_connectivity = hashtable_lookup(correct, "connectivity");
//  Array* correct_predecessor  = hashtable_lookup(correct, "predecessor");

//  // Check Values
//  assert_array_equal(ift_get_label(ift)       , correct_label);
//  assert_array_equal(ift_get_root(ift)        , correct_root);
//  assert_array_equal(ift_get_connectivity(ift), correct_connectivity);
//  assert_array_equal(ift_get_predecessor(ift) , correct_predecessor);

//  // Free memory
//  hashtable_free(correct);
//  ift_free(ift);
//  array_free(image);
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
