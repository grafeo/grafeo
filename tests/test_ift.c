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
#include <grafeo/core.h>
#include <grafeo/imgproc.h>
#include <grafeo/ift.h>
#include <math.h>

static void assert_grf_ndarray_equal(GrfNDArray* array1, GrfNDArray* array2){
  uint64_t i;
  assert_non_null(array1);
  assert_non_null(array2);
  assert_int_equal(array1->dim,          array2->dim);
  assert_int_equal(array1->num_elements, array2->num_elements);
  for(i = 0; i < array1->dim; i++)
    assert_int_equal(array1->size[i],      array2->size[i]);
  for(i = 0; i < array1->num_elements; i++)
    assert_true(grf_ndarray_get_long_double_1D(array1,i) == grf_ndarray_get_long_double_1D(array2,i));
}

static void helper_test_grf_ift_img(const char*          imagepath,
                                const char*          labels_filename,
                                const char*          seeds_indices_filename,
                                const char*          seeds_labels_filename,
                                uint16_t             map_dimension,
                                GrfPathConnectivityFunc path_connectivity,
                                GrfWeightFunc           weight_function,
                                GrfOptimizationType      grf_ift_optimization){
  // Load images and seeds
  GrfNDArray* image        = grf_image_read(imagepath);
  GrfNDArray* labels       = grf_image_read(labels_filename);

  grf_ndarray_squeeze(image);
  grf_ndarray_squeeze(labels);

  GrfNDArray* seeds_labels         = grf_ndarray_read_csv_type(seeds_labels_filename,  GRF_UINT16);
  GrfNDArray* seeds_indices        = grf_ndarray_read_csv_type(seeds_indices_filename, GRF_UINT64);

  GrfIFT* ift = grf_ift_apply_array(image, map_dimension, GRF_NEIGHBOR_4, grf_ift_optimization, weight_function, path_connectivity, seeds_indices, seeds_labels);

  GrfNDArray* label  = grf_ndarray_as_type(ift->label, GRF_UINT8);
  GrfNDArray* output = grf_ndarray_mult_scalar(label, 255);
  grf_image_write(output,"label_output.png");
  grf_ndarray_free(label);
  grf_ndarray_free(output);

  assert_non_null(ift);

  assert_grf_ndarray_equal(grf_ift_get_label(ift)        , labels);
}

static void helper_test_ift(GrfPathConnectivityFunc path_connectivity, GrfOptimizationType grf_ift_optimization){
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
  GrfNDArray* seeds_labels           = grf_ndarray_new_1D_type(2,GRF_UINT16);
  GrfNDArray* seeds_indices          = grf_ndarray_new_1D_type(2,GRF_UINT64);

  seeds_labels->data_uint16[0]  = 0;
  seeds_labels->data_uint16[1]  = 1;
  seeds_indices->data_uint64[0] = 0;
  seeds_indices->data_uint64[1] = 3;

  // Generate the image
  uint32_t size[2] = {2,2};
  GrfNDArray*   image   = grf_ndarray_from_data(data, 2, size, GRF_UINT8);

  // Run standard IFT
  GrfIFT*     ift     = grf_ift_apply_array(image, image->dim, GRF_NEIGHBOR_4, grf_ift_optimization, grf_weight_diff, path_connectivity, seeds_indices, seeds_labels);

  assert_non_null(ift);
  assert_non_null(ift->connectivity);
  assert_non_null(ift->label);
  assert_non_null(ift->original);
  assert_non_null(ift->predecessors);
  assert_non_null(ift->root);

  // Check Values
  if(grf_ift_optimization == GRF_MINIMIZATION){
    if(path_connectivity == grf_path_connectivity_norm_l2){
      GrfNDArray* correct_predecessor  = grf_ndarray_from_data(predecessor_euc,  2, size, GRF_UINT8);
      GrfNDArray* correct_label        = grf_ndarray_from_data(label_euc,        2, size, GRF_UINT8);
      GrfNDArray* correct_connectivity = grf_ndarray_from_data(connectivity_euc, 2, size, GRF_UINT8);
      GrfNDArray* correct_root         = grf_ndarray_from_data(root_euc,         2, size, GRF_UINT8);
      assert_grf_ndarray_equal(grf_ift_get_label(ift)        , correct_label);
      assert_grf_ndarray_equal(grf_ift_get_root(ift)         , correct_root);
      assert_grf_ndarray_equal(grf_ift_get_connectivity(ift) , correct_connectivity);
      assert_grf_ndarray_equal(grf_ift_get_predecessors(ift) , correct_predecessor);
    }
    else{
      GrfNDArray* correct_predecessor  = grf_ndarray_from_data(predecessor,  2, size, GRF_UINT8);
      GrfNDArray* correct_label        = grf_ndarray_from_data(label,        2, size, GRF_UINT8);
      GrfNDArray* correct_connectivity = grf_ndarray_from_data(connectivity, 2, size, GRF_UINT8);
      GrfNDArray* correct_root         = grf_ndarray_from_data(root,         2, size, GRF_UINT8);
      assert_grf_ndarray_equal(grf_ift_get_label(ift)        , correct_label);
      assert_grf_ndarray_equal(grf_ift_get_root(ift)         , correct_root);
      assert_grf_ndarray_equal(grf_ift_get_connectivity(ift) , correct_connectivity);
      assert_grf_ndarray_equal(grf_ift_get_predecessors(ift) , correct_predecessor);
    }
  }else{
    GrfNDArray* correct_predecessor  = grf_ndarray_from_data(predecessor_min,  2, size, GRF_UINT8);
    GrfNDArray* correct_label        = grf_ndarray_from_data(label_min,        2, size, GRF_UINT8);
    GrfNDArray* correct_connectivity = grf_ndarray_from_data(connectivity_min, 2, size, GRF_INT64);
    GrfNDArray* correct_root         = grf_ndarray_from_data(root_min,         2, size, GRF_UINT8);
    assert_grf_ndarray_equal(grf_ift_get_label(ift)        , correct_label);
    assert_grf_ndarray_equal(grf_ift_get_root(ift)         , correct_root);
    assert_grf_ndarray_equal(grf_ift_get_connectivity(ift) , correct_connectivity);
    assert_grf_ndarray_equal(grf_ift_get_predecessors(ift) , correct_predecessor);
  }


  grf_ift_free(ift);
  grf_ndarray_free(image);
}

static void helper_test_grf_ift_4D(GrfPathConnectivityFunc connectivity_function, GrfOptimizationType optimization){
  uint8_t grf_image_dim          = 3;
  uint32_t size[4]           = {2,2,2,3}; // 2x2x2 RGB
  uint8_t  data[24]          = {  0,   0,   0, 20, 12, 23,  190, 200, 179, 230, 178, 190,
                                250, 180, 216, 30,  10, 5,  175, 190, 201,   5,  31,  29};
  uint64_t seeds_indices_data[2]= {0,6};
  uint16_t seeds_labels_data[2] = {0,1};
  uint32_t seeds_size[1]        = {2};
  uint16_t labels_data[8]       = {0,0,1,1,1,0,1,0};
  GrfNDArray*   image                = grf_ndarray_from_data(data                 , grf_image_dim+1 , size      , GRF_UINT8);
  GrfNDArray*   correct_labels       = grf_ndarray_from_data(labels_data          , grf_image_dim   , size      , GRF_UINT16);
  GrfNDArray*   seeds_indices        = grf_ndarray_from_data(seeds_indices_data   , 1           , seeds_size, GRF_UINT64);
  GrfNDArray*   seeds_labels         = grf_ndarray_from_data(seeds_labels_data    , 1           , seeds_size, GRF_UINT16);
  GrfIFT* ift = grf_ift_apply_array(image,                 // GrfNDArray
                             grf_image_dim,             // Dimension of IFT Maps
                             GRF_NEIGHBOR_6,     // Adjacency
                             optimization,          // Maximization or Minimization
                             grf_weight_diff_3,         // Formula for edge weights
                             connectivity_function, // Path connectivity functions
                             seeds_indices,         // Indices for seeds
                             seeds_labels);         // Labels of seeds
  assert_grf_ndarray_equal(grf_ift_get_label(ift), correct_labels);
  grf_ndarray_free(image);
  grf_ndarray_free(correct_labels);
  grf_ndarray_free(seeds_indices);
  grf_ndarray_free(seeds_labels);
  grf_ift_free(ift);
}

static void helper_test_grf_ift_3D(GrfPathConnectivityFunc connectivity_function, GrfOptimizationType optimization){
  uint8_t grf_image_dim             = 2;
  uint32_t size[3]              = {2,2,3}; // 2x2 RGB

  uint32_t seeds_size[1]        = {2};
  uint8_t  data[12]             = {  0,  0, 0,  20, 12, 23,
                                   128,200,150, 230,130,124};
  uint64_t seeds_indices_data[2]= {0,3};
  uint64_t seeds_indices_data_3D[2]= {0,10};
  uint16_t seeds_labels_data[2] = {0,1};

  uint16_t  labels_data[4]      = {  0,0,1,1};
  uint16_t  labels_data_3D[12]  = {  0,0,0, 0,0,0, 1,1,1, 1,1,1};
  GrfNDArray*   image                = grf_ndarray_from_data(data                 , grf_image_dim+1 , size      , GRF_UINT8);
  GrfNDArray*   correct_labels       = grf_ndarray_from_data(labels_data          , grf_image_dim   , size      , GRF_UINT16);
  GrfNDArray*   correct_labels_3D    = grf_ndarray_from_data(labels_data_3D       , grf_image_dim+1 , size      , GRF_UINT16);
  GrfNDArray*   seeds_indices        = grf_ndarray_from_data(seeds_indices_data   , 1           , seeds_size, GRF_UINT64);
  GrfNDArray*   seeds_indices_3D     = grf_ndarray_from_data(seeds_indices_data_3D, 1           , seeds_size, GRF_UINT64);
  GrfNDArray*   seeds_labels         = grf_ndarray_from_data(seeds_labels_data    , 1           , seeds_size, GRF_UINT16);
  GrfIFT* ift = grf_ift_apply_array(image,                 // GrfNDArray
                             grf_image_dim,             // Dimension of IFT Maps
                             GRF_NEIGHBOR_4,     // Adjacency
                             optimization,          // Maximization or Minimization
                             grf_weight_diff_3,         // Formula for edge weights
                             connectivity_function, // Path connectivity functions
                             seeds_indices,         // Indices for seeds
                             seeds_labels);         // Labels of seeds
  GrfIFT* grf_ift_3D = grf_ift_apply_array(image,                 // GrfNDArray
                                grf_image_dim+1,           // Dimension of IFT Maps
                                GRF_NEIGHBOR_6,     // Adjacency
                                optimization,          // Maximization or Minimization
                                grf_weight_diff,           // Formula for edge weights
                                connectivity_function, // Path connectivity functions
                                seeds_indices_3D,      // Indices for seeds
                                seeds_labels);         // Labels of seeds
  assert_grf_ndarray_equal(grf_ift_get_label(ift), correct_labels);
  assert_grf_ndarray_equal(grf_ift_get_label(grf_ift_3D), correct_labels_3D);

  grf_ndarray_free(image);
  grf_ndarray_free(correct_labels);
  grf_ndarray_free(correct_labels_3D);
  grf_ndarray_free(seeds_indices);
  grf_ndarray_free(seeds_indices_3D);
  grf_ndarray_free(seeds_labels);
  grf_ift_free(ift);
  grf_ift_free(grf_ift_3D);
}

static void test_grf_ift_sum(void** state){
  (void) state;
  helper_test_ift(grf_path_connectivity_sum, GRF_MINIMIZATION);
  helper_test_grf_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      2,
                      grf_path_connectivity_sum,
                      grf_weight_diff,
                      GRF_MINIMIZATION);
}
static void test_grf_ift_max(void** state){
  (void) state;
  helper_test_ift    (grf_path_connectivity_max, GRF_MINIMIZATION); // 2D Grayscale
  helper_test_grf_ift_3D (grf_path_connectivity_max, GRF_MINIMIZATION); // 2D RGB and 3D Grayscale
  helper_test_grf_ift_4D (grf_path_connectivity_max, GRF_MINIMIZATION); // 3D RGB
  helper_test_grf_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      2,
                      grf_path_connectivity_max,
                      grf_weight_diff,
                      GRF_MINIMIZATION);
  helper_test_grf_ift_img("../data/target.png",
                      "../data/target_labels.png",
                      "../data/target_seeds_indices.csv",
                      "../data/target_seeds_labels.csv",
                      2,
                      grf_path_connectivity_max,
                      grf_weight_diff_3,
                      GRF_MINIMIZATION);
}
static void test_grf_ift_min(void** state){
  (void) state;
  helper_test_ift(grf_path_connectivity_min, GRF_MAXIMIZATION);
  helper_test_grf_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      2,
                      grf_path_connectivity_min,
                      grf_weight_diff,
                      GRF_MAXIMIZATION);
}
static void test_grf_ift_euc(void** state){
  (void) state;
  helper_test_ift(grf_path_connectivity_norm_l2, GRF_MINIMIZATION);
  helper_test_grf_ift_img("../data/starbw.png",
                      "../data/starbw_labels.png",
                      "../data/starbw_seeds_indices.csv",
                      "../data/starbw_seeds_labels.csv",
                      2,
                      grf_path_connectivity_norm_l2,
                      grf_weight_diff,
                      GRF_MINIMIZATION);
}

static void test_grf_ift_distance_transform(void** state){
  (void) state;
  GrfNDArray* image = grf_image_read_pgm("../data/distance_transform_input.pgm");

  GrfNDArray* result = grf_ift_distance_transform(image, GRF_NORM_L2);
  grf_image_write_png(result,"distance_transform_output.png");
  grf_ndarray_free(result);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[3]={
    cmocka_unit_test(test_grf_ift_sum),
    cmocka_unit_test(test_grf_ift_max),
    cmocka_unit_test(test_grf_ift_distance_transform),
    //cmocka_unit_test(test_grf_ift_min),
    //cmocka_unit_test(test_grf_ift_euc),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
