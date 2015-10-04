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
  assert_non_null(array1);
  assert_non_null(array2);
  assert_int_equal(array1->dim,          array2->dim);
  assert_int_equal(array1->num_elements, array2->num_elements);
  for(i = 0; i < array1->dim; i++)
    assert_int_equal(array1->size[i],      array2->size[i]);
  for(i = 0; i < array1->num_elements; i++)
    assert_true(array_get_long_double_1D(array1,i) == array_get_long_double_1D(array2,i));
}

static void helper_test_ift_img(const char* imagepath,
                                const char* labels_filename,
                                const char* seeds_indices_filename,
                                const char* seeds_labels_filename,
                                PathConnectivityFunc path_connectivity, IFTOptimization ift_optimization){
  // Load images and seeds
  Array* image        = image_read(imagepath);
  Array* labels       = image_read(labels_filename);

  array_squeeze(image);
  array_squeeze(labels);

  Array* seeds_labels         = array_read_csv_type(seeds_labels_filename,  GRAFEO_UINT16);
  Array* seeds_indices        = array_read_csv_type(seeds_indices_filename, GRAFEO_UINT64);

  IFT* ift = ift_apply_array(image, GRAFEO_NEIGHBOR_4, ift_optimization, weight_diff, path_connectivity, seeds_indices, seeds_labels);

  Array* label  = array_as_type(ift->label, GRAFEO_UINT8);
  Array* output = array_mult_scalar(label, 255);
  image_write(output,"label_output.png");
  array_free(label);
  array_free(output);

  assert_non_null(ift);

  assert_array_equal(ift_get_label(ift)        , labels);
}

static void helper_test_ift(PathConnectivityFunc path_connectivity, IFTOptimization ift_optimization){
  // Load an image, its labels, connectivity and root maps
  // 8x8 image
  uint8_t data[4]            = {000,000,    255,255};
  uint8_t predecessor[4]     = {000,000,    003,003};
  uint8_t label[4]           = {000,000,    001,001};
  uint8_t connectivity[4]    = {000,000,    000,000};
  uint8_t root[4]            = {000,000,    003,003};

  uint8_t predecessor_min[4] = {000,003,    000,003};
  uint8_t label_min[4]       = {000,001,    000,001};
  int64_t connectivity_min[4]= {__INT64_MAX__,255,    255,__INT64_MAX__};
  uint8_t root_min[4]        = {000,003,    000,003};

  uint8_t predecessor_euc[4] = {000,000,    000,003};
  uint8_t label_euc[4]       = {000,000,    000,001};
  uint8_t connectivity_euc[4]= {000,001,    001,000};
  uint8_t root_euc[4]        = {000,000,    000,003};

  // Define seeds
  Array* seeds_labels           = array_new_1D_type(2,GRAFEO_UINT16);
  Array* seeds_indices          = array_new_1D_type(2,GRAFEO_UINT64);

  seeds_labels->data_uint16[0]  = 0;
  seeds_labels->data_uint16[1]  = 1;
  seeds_indices->data_uint64[0] = 0;
  seeds_indices->data_uint64[1] = 3;

  // Generate the image
  uint32_t size[2] = {2,2};
  Array*   image   = array_from_data(data, 2, size, GRAFEO_UINT8);

  // Run standard IFT
  IFT*     ift     = ift_apply_array(image, GRAFEO_NEIGHBOR_4, ift_optimization, weight_diff, path_connectivity, seeds_indices, seeds_labels);

  assert_non_null(ift);
  assert_non_null(ift->connectivity);
  assert_non_null(ift->label);
  assert_non_null(ift->original);
  assert_non_null(ift->predecessors);
  assert_non_null(ift->root);

  // Check Values
  if(ift_optimization == GRAFEO_IFT_MIN){
    if(path_connectivity == path_connectivity_euc){
      Array* correct_predecessor  = array_from_data(predecessor_euc,  2, size, GRAFEO_UINT8);
      Array* correct_label        = array_from_data(label_euc,        2, size, GRAFEO_UINT8);
      Array* correct_connectivity = array_from_data(connectivity_euc, 2, size, GRAFEO_UINT8);
      Array* correct_root         = array_from_data(root_euc,         2, size, GRAFEO_UINT8);
      assert_array_equal(ift_get_label(ift)        , correct_label);
      assert_array_equal(ift_get_root(ift)         , correct_root);
      assert_array_equal(ift_get_connectivity(ift) , correct_connectivity);
      assert_array_equal(ift_get_predecessors(ift) , correct_predecessor);
    }
    else{
      Array* correct_predecessor  = array_from_data(predecessor,  2, size, GRAFEO_UINT8);
      Array* correct_label        = array_from_data(label,        2, size, GRAFEO_UINT8);
      Array* correct_connectivity = array_from_data(connectivity, 2, size, GRAFEO_UINT8);
      Array* correct_root         = array_from_data(root,         2, size, GRAFEO_UINT8);
      assert_array_equal(ift_get_label(ift)        , correct_label);
      assert_array_equal(ift_get_root(ift)         , correct_root);
      assert_array_equal(ift_get_connectivity(ift) , correct_connectivity);
      assert_array_equal(ift_get_predecessors(ift) , correct_predecessor);
    }
  }else{
    Array* correct_predecessor  = array_from_data(predecessor_min,  2, size, GRAFEO_UINT8);
    Array* correct_label        = array_from_data(label_min,        2, size, GRAFEO_UINT8);
    Array* correct_connectivity = array_from_data(connectivity_min, 2, size, GRAFEO_INT64);
    Array* correct_root         = array_from_data(root_min,         2, size, GRAFEO_UINT8);
    assert_array_equal(ift_get_label(ift)        , correct_label);
    assert_array_equal(ift_get_root(ift)         , correct_root);
    assert_array_equal(ift_get_connectivity(ift) , correct_connectivity);
    assert_array_equal(ift_get_predecessors(ift) , correct_predecessor);
  }


  ift_free(ift);
  array_free(image);
}

static void helper_test_ift_rgb(PathConnectivityFunc connectivity_function, IFTOptimization optimization){
  uint32_t size[3]              = {2,2,3}; // 2x2 RGB
  uint32_t seeds_size[1]        = {2};
  uint8_t  data[12]             = {  0,  0, 0,  20, 12, 23,
                                   128,200,50, 230,130,124};
  uint64_t seeds_indices_data[2]= {};
  uint16_t seeds_labels_data[2] = {};
  uint8_t  labels_data[4]       = {  0,        0,
                                     1,        1};
  Array*   image                = array_from_data(data              , 3, size      , GRAFEO_UINT8);
  Array*   correct_labels       = array_from_data(labels_data       , 2, size      , GRAFEO_UINT16);
  Array*   seeds_indices        = array_from_data(seeds_indices_data, 1, seeds_size, GRAFEO_UINT64);
  Array*   seeds_labels         = array_from_data(seeds_labels_data , 1, seeds_size, GRAFEO_UINT16);
  IFT* ift = ift_apply_array(image,GRAFEO_NEIGHBOR_4,optimization, weight_diff_3, connectivity_function, seeds_indices, seeds_labels);
  assert_array_equal(ift_get_label(ift), correct_labels);
}

static void test_ift_sum(void** state){
  (void) state;
  helper_test_ift(path_connectivity_sum, GRAFEO_IFT_MIN);
  helper_test_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      path_connectivity_sum, GRAFEO_IFT_MIN);
}
static void test_ift_max(void** state){
  (void) state;
  helper_test_ift(path_connectivity_max, GRAFEO_IFT_MIN);
  helper_test_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      path_connectivity_max, GRAFEO_IFT_MIN);
}
static void test_ift_min(void** state){
  (void) state;
  helper_test_ift(path_connectivity_min, GRAFEO_IFT_MAX);
  helper_test_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      path_connectivity_min, GRAFEO_IFT_MAX);
}
static void test_ift_euc(void** state){
  (void) state;
  helper_test_ift(path_connectivity_euc, GRAFEO_IFT_MIN);
  helper_test_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      path_connectivity_euc, GRAFEO_IFT_MIN);
}

static void test_ift_max_rgb(void** state){
  (void) state;
  helper_test_ift_rgb(path_connectivity_max, GRAFEO_IFT_MIN);
  helper_test_ift_img("../data/starrgb.png",
                      "../data/starrgb_labels.png",
                      "../data/starrgb_seeds_indices.csv",
                      "../data/starrgb_seeds_labels.csv",
                      path_connectivity_euc, GRAFEO_IFT_MIN);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_ift_sum),
    cmocka_unit_test(test_ift_max),
    cmocka_unit_test(test_ift_max_rgb),
    //cmocka_unit_test(test_ift_min),
    //cmocka_unit_test(test_ift_euc),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
