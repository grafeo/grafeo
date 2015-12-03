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
#include <grafeo/imgproc.h>

void helper_test_grf_image_read(GrfNDArray* array, uint32_t* correct_sizes, GrfDataType correct_type, uint8_t correct_bitsize, uint8_t correct_dim){
  assert_int_equal(grf_ndarray_get_dim(array), correct_dim);
  uint32_t* sizes = grf_ndarray_get_size(array);
  assert_non_null(sizes);
  assert_int_equal(sizes[0], correct_sizes[0]);
  assert_int_equal(sizes[1], correct_sizes[1]);
  if(correct_dim > 2) assert_int_equal(sizes[2], correct_sizes[2]);
  uint64_t i;
  uint64_t correct_num_elements = 1;
  for(i = 0; i < correct_dim; i++) correct_num_elements *= correct_sizes[i];
  assert_int_equal(grf_ndarray_get_datatype(array), correct_type);
  assert_int_equal(grf_ndarray_get_num_elements(array), correct_num_elements);
  assert_int_equal(grf_ndarray_get_num_bytes(array), correct_num_elements * correct_bitsize);
  assert_int_equal(grf_ndarray_get_bitsize(array), correct_bitsize);
  uint8_t* data = grf_ndarray_get_data(array);
  assert_non_null(data);
  uint64_t sum = 0;
  for(i = 0; i < grf_ndarray_get_num_elements(array); i++){
      sum += data[i];
  }
  assert_true(sum > 0);
}

static void test_grf_image_read_jpg(void ** state){
  (void)state;
  g_autofree GrfNDArray* image = NULL;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = grf_image_read_jpg("../data/chess.jpg");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
  g_clear_object(&image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = grf_image_read_jpg("../data/chessgray.jpg");
  helper_test_grf_image_read(image, correct_sizes_gray, GRF_UINT8, 1,3);
}

static void test_grf_image_read_png(void ** state){
  (void)state;
  g_autofree GrfNDArray* image = NULL;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = grf_image_read_png("../data/chess.png");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
  g_clear_object(&image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = grf_image_read_png("../data/chessgray.png");
  helper_test_grf_image_read(image, correct_sizes_gray, GRF_UINT8, 1,3);

}

static void test_grf_image_write_jpg(void ** state){
  (void)state;
  const char* outfile = "imagem.jpg";
  const char* outfile_gray = "imagemgray.jpg";
  // Remove the file if it exists
  remove(outfile);
  remove(outfile_gray);

  // Define the image
  g_autofree GrfNDArray* input_image      = grf_image_read_jpg("../data/chess.jpg");
  g_autofree GrfNDArray* input_grf_image_gray = grf_image_read_jpg("../data/chessgray.jpg");

  // Save the image
  grf_image_write_jpg(input_image,outfile);
  grf_image_write_jpg(input_grf_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);
}

static void test_grf_image_write_png(void ** state){
  (void)state;
  const char* outfile      = "imagem.png";
  const char* outfile_gray = "imagemgray.png";
  const char* outfile_2D   = "test_grf_image_write_png_2D.png";
  // Remove the file if it exists
  remove(outfile);
  remove(outfile_gray);

  // Define the image
  g_autofree GrfNDArray* input_image = grf_image_read_png("../data/chess.png");
  g_autofree GrfNDArray* input_grf_image_gray = grf_image_read_png("../data/chessgray.png");

  // Save the image
  grf_image_write_png(input_image,outfile);
  grf_image_write_png(input_grf_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);

  // Testing 2D image
  uint8_t data[9] = {0,255,0,255,0,255,0,255,0};
  uint32_t size[2] = {3,3};
  g_autofree GrfNDArray* grf_ndarray_2D = grf_ndarray_from_data(data,2,size,GRF_UINT8);
  grf_image_write_png(grf_ndarray_2D, outfile_2D);
  assert_int_equal(access(outfile_2D, F_OK), 0);
}

static void test_grf_image_read(void** state){
  (void)state;

  const char* grf_image_names[4] = {"../data/chess.png", "../data/chessgray.png", "../data/chess.jpg", "../data/chessgray.jpg"};
  uint32_t correct_channels[4] = {3,1,3,1};
  uint32_t correct_sizes[3] = {8,8,3};
  GrfNDArray* array;
  int i;
  for(i = 0; i < 4; i++){
    correct_sizes[2] = correct_channels[i];
    array = grf_image_read(grf_image_names[i]);
    helper_test_grf_image_read(array, correct_sizes,GRF_UINT8, 1,3);
    g_clear_object(&array);
  }
}

static void test_grf_image_write(void** state){
  (void)state;
  // Define an array
  // Write that array to JPG
  // Test if the file exists
  // Load the file
  // Test if the file has the same array attributes

  // Write that array to PNG
  // Test if the file exists
  // Load the file
  // Test if the file has the same array attributes
  // Test if the file has the same data as the array
}

static void test_grf_image_read_pgm(void** state){
  (void)state;
  // Color image
  uint32_t correct_sizes[2] = {8,8};
  g_autofree GrfNDArray* image = grf_image_read_pgm("../data/chess.pgm");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,2);
}

static void test_grf_image_write_pgm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  g_autofree GrfNDArray* input_image = grf_image_read_pgm("../data/chess.pgm");

  // Save the image
  grf_image_write_pgm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
}

static void test_grf_image_read_ppm(void** state){
  (void)state;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  g_autofree GrfNDArray* image = grf_image_read_ppm("../data/chess.ppm");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
}

static void test_grf_image_write_ppm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  g_autofree GrfNDArray* input_image = grf_image_read_ppm("../data/chess.ppm");

  // Save the image
  grf_image_write_ppm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);  
}

static void test_grf_image_cvt_color(void** state){
  (void) state;
  g_autofree GrfNDArray* array= grf_image_read("../data/chess.pgm");
  uint8_t  array_bitsize      = grf_ndarray_get_bitsize(array);
  uint32_t*array_size         = grf_ndarray_get_size(array);
  uint64_t array_num_elements = grf_ndarray_get_num_elements(array);
  gboolean array_contiguous   = grf_ndarray_get_contiguous(array);
  uint8_t* array_data_uint8   = (uint8_t*)grf_ndarray_get_data(array);
  // From grayscale to RGB

  g_autofree GrfNDArray* array_rgb = grf_image_cvt_color(array, GRF_GRAY, GRF_RGB);
  uint16_t  rgb_dim          = grf_ndarray_get_dim(array_rgb);
  uint32_t* rgb_size         = grf_ndarray_get_size(array_rgb);
  uint32_t* rgb_bitsize      = grf_ndarray_get_size(array_rgb);
  gboolean  rgb_contiguous   = grf_ndarray_get_contiguous(array_rgb);
  uint64_t  rgb_num_elements = grf_ndarray_get_num_elements(array_rgb);
  uint8_t*  rgb_data_uint8   = (uint8_t*)grf_ndarray_get_data(array_rgb);

  assert_int_equal(rgb_dim, 3);
  assert_int_equal(rgb_size[0], array_size[0]);
  assert_int_equal(rgb_size[1], array_size[1]);
  assert_int_equal(rgb_size[2], 3);
  assert_int_equal(rgb_bitsize, array_bitsize);
  assert_int_equal(rgb_contiguous, array_contiguous);
  assert_int_equal(rgb_num_elements, array_num_elements*3);
  uint64_t i;

  for(i = 0; i < array_num_elements;i++){
    assert_int_equal(array_data_uint8[i],rgb_data_uint8[i*3]);
    assert_int_equal(array_data_uint8[i],rgb_data_uint8[i*3+1]);
    assert_int_equal(array_data_uint8[i],rgb_data_uint8[i*3+2]);
  }

  // From RGB to grayscale
  g_autofree GrfNDArray* array_gray = grf_image_cvt_color(array_rgb, GRF_RGB, GRF_GRAY);
  uint16_t gray_dim          = grf_ndarray_get_dim(array_gray);
  uint8_t  gray_bitsize      = grf_ndarray_get_bitsize(array_gray);
  uint32_t*gray_size         = grf_ndarray_get_size(array_gray);
  gboolean gray_contiguous   = grf_ndarray_get_contiguous(array_gray);
  uint64_t gray_num_elements = grf_ndarray_get_num_elements(array_gray);
  uint8_t* gray_data_uint8 = (uint8_t*)grf_ndarray_get_data(array_gray);
  assert_int_equal(gray_dim         , 2);
  assert_int_equal(gray_size[0]     , rgb_size[0]);
  assert_int_equal(gray_size[1]     , rgb_size[1]);
  assert_int_equal(gray_bitsize     , rgb_bitsize);
  assert_int_equal(gray_contiguous  , rgb_contiguous);
  assert_int_equal(gray_num_elements, array_num_elements);
  for(i = 0; i < gray_num_elements;i++)
    assert_int_equal(gray_data_uint8[i],array_data_uint8[i]);

  // From Grayscale to RGBA
  g_autofree GrfNDArray* array_rgba = grf_image_cvt_color(array, GRF_GRAY, GRF_RGBA);
  uint16_t  rgba_dim          = grf_ndarray_get_dim(array_rgba);
  uint32_t* rgba_size         = grf_ndarray_get_size(array_rgba);
  uint32_t* rgba_bitsize      = grf_ndarray_get_size(array_rgba);
  gboolean  rgba_contiguous   = grf_ndarray_get_contiguous(array_rgba);
  uint64_t  rgba_num_elements = grf_ndarray_get_num_elements(array_rgba);
  uint8_t*  rgba_data_uint8   = (uint8_t*)grf_ndarray_get_data(array_rgba);

  assert_int_equal(rgba_dim           , 3);
  assert_int_equal(rgba_size[0]       , array_size[0]);
  assert_int_equal(rgba_size[1]       , array_size[1]);
  assert_int_equal(rgba_size[2]       , 4);
  assert_int_equal(rgba_bitsize       , array_bitsize);
  assert_int_equal(rgba_contiguous    , array_contiguous);
  assert_int_equal(rgba_num_elements  , array_num_elements*4);
  for(i = 0; i < array_num_elements;i++){
    assert_int_equal(array_data_uint8[i],rgba_data_uint8[i*4]);
    assert_int_equal(array_data_uint8[i],rgba_data_uint8[i*4+1]);
    assert_int_equal(array_data_uint8[i],rgba_data_uint8[i*4+2]);
    assert_int_equal(255,rgba_data_uint8[i*4+3]);
  }

  // From Grayscale to BGRA
  g_autofree GrfNDArray* array_bgra2 = grf_image_cvt_color(array, GRF_GRAY, GRF_BGRA);
  uint16_t  bgra2_dim          = grf_ndarray_get_dim           (array_bgra2);
  uint32_t* bgra2_size         = grf_ndarray_get_size          (array_bgra2);
  uint32_t* bgra2_bitsize      = grf_ndarray_get_size          (array_bgra2);
  gboolean  bgra2_contiguous   = grf_ndarray_get_contiguous    (array_bgra2);
  uint64_t  bgra2_num_elements = grf_ndarray_get_num_elements  (array_bgra2);
  uint8_t*  bgra2_data_uint8   = (uint8_t*)grf_ndarray_get_data(array_bgra2);

  assert_int_equal(bgra2_dim       , 3);
  assert_int_equal(bgra2_size[0]   , array_size[0]);
  assert_int_equal(bgra2_size[1]   , array_size[1]);
  assert_int_equal(bgra2_size[2]   , 4);
  assert_int_equal(bgra2_bitsize   , array_bitsize);
  assert_int_equal(bgra2_contiguous, array_contiguous);
  assert_int_equal(bgra2_num_elements  , array_num_elements*4);
  for(i = 0; i < array_num_elements;i++){
    assert_int_equal(array_data_uint8[i],bgra2_data_uint8[i*4+2]);
    assert_int_equal(array_data_uint8[i],bgra2_data_uint8[i*4+1]);
    assert_int_equal(array_data_uint8[i],bgra2_data_uint8[i*4]);
    assert_int_equal(255,bgra2_data_uint8[i*4+3]);
  }

  // From RGB to RGBA
  g_autofree GrfNDArray* array_rgba2 = grf_image_cvt_color(array_rgb, GRF_RGB, GRF_RGBA);
  uint16_t  rgba2_dim          = grf_ndarray_get_dim           (array_rgba2);
  uint32_t* rgba2_size         = grf_ndarray_get_size          (array_rgba2);
  uint32_t* rgba2_bitsize      = grf_ndarray_get_size          (array_rgba2);
  gboolean  rgba2_contiguous   = grf_ndarray_get_contiguous    (array_rgba2);
  uint64_t  rgba2_num_elements = grf_ndarray_get_num_elements  (array_rgba2);
  uint8_t*  rgba2_data_uint8   = (uint8_t*)grf_ndarray_get_data(array_rgba2);

  assert_int_equal(rgba2_dim, 3);
  assert_int_equal(rgba2_size[0]      , rgb_size[0]);
  assert_int_equal(rgba2_size[1]      , rgb_size[1]);
  assert_int_equal(rgba2_size[2]      , 4);
  assert_int_equal(rgba2_bitsize       , rgb_bitsize);
  assert_int_equal(rgba2_contiguous    , rgb_contiguous);
  assert_int_equal(rgba2_num_elements  , rgb_num_elements*4/3);
  for(i = 0; i < array_num_elements;i++){
    assert_int_equal(rgba2_data_uint8[i*4]  ,rgb_data_uint8[3*i]);
    assert_int_equal(rgba2_data_uint8[i*4+1],rgb_data_uint8[3*i+1]);
    assert_int_equal(rgba2_data_uint8[i*4+2],rgb_data_uint8[3*i+2]);
    assert_int_equal(rgba2_data_uint8[i*4+3],255);
  }

  // From RGB to BGRA
  g_autofree GrfNDArray* array_bgra = grf_image_cvt_color(array_rgb, GRF_RGB, GRF_BGRA);
  uint16_t  bgra_dim          = grf_ndarray_get_dim           (array_bgra);
  uint32_t* bgra_size         = grf_ndarray_get_size          (array_bgra);
  uint32_t* bgra_bitsize      = grf_ndarray_get_size          (array_bgra);
  gboolean  bgra_contiguous   = grf_ndarray_get_contiguous    (array_bgra);
  uint64_t  bgra_num_elements = grf_ndarray_get_num_elements  (array_bgra);
  uint8_t*  bgra_data_uint8   = (uint8_t*)grf_ndarray_get_data(array_bgra);
  assert_int_equal(bgra_dim, 3);
  assert_int_equal(bgra_size[0]      , rgb_size[0]);
  assert_int_equal(bgra_size[1]      , rgb_size[1]);
  assert_int_equal(bgra_size[2]      , 4);
  assert_int_equal(bgra_bitsize       , rgb_bitsize);
  assert_int_equal(bgra_contiguous    , rgb_contiguous);
  assert_int_equal(bgra_num_elements  , rgb_num_elements*4/3);
  for(i = 0; i < array_num_elements;i++){
    assert_int_equal(bgra_data_uint8[i*4]  ,rgb_data_uint8[3*i+2]);
    assert_int_equal(bgra_data_uint8[i*4+1],rgb_data_uint8[3*i+1]);
    assert_int_equal(bgra_data_uint8[i*4+2],rgb_data_uint8[3*i]);
    assert_int_equal(bgra_data_uint8[i*4+3],255);
  }
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[11]={
    cmocka_unit_test(test_grf_image_read_jpg),
    cmocka_unit_test(test_grf_image_read_png),
    cmocka_unit_test(test_grf_image_read_pgm),
    cmocka_unit_test(test_grf_image_read_ppm),
    cmocka_unit_test(test_grf_image_read),
    cmocka_unit_test(test_grf_image_write_jpg),
    cmocka_unit_test(test_grf_image_write_png),
    cmocka_unit_test(test_grf_image_write_pgm),
    cmocka_unit_test(test_grf_image_write_ppm),
    cmocka_unit_test(test_grf_image_write),
    cmocka_unit_test(test_grf_image_cvt_color),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
