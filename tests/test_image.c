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
#include <grafeo/image.h>

void helper_test_grf_image_read(GrfArray* array, uint32_t* correct_sizes, GrfDataType correct_type, uint8_t correct_bitsize, uint8_t correct_dim){
  assert_int_equal(grf_array_get_dim(array), correct_dim);
  uint32_t* sizes = grf_array_get_size(array);
  assert_non_null(sizes);
  assert_int_equal(sizes[0], correct_sizes[0]);
  assert_int_equal(sizes[1], correct_sizes[1]);
  if(correct_dim > 2) assert_int_equal(sizes[2], correct_sizes[2]);
  uint64_t i;
  uint64_t correct_num_elements = 1;
  for(i = 0; i < correct_dim; i++) correct_num_elements *= correct_sizes[i];
  assert_int_equal(grf_array_get_type(array), correct_type);
  assert_int_equal(grf_array_get_num_elements(array), correct_num_elements);
  assert_int_equal(grf_array_get_num_bytes(array), correct_num_elements * correct_bitsize);
  assert_int_equal(grf_array_get_bitsize(array), correct_bitsize);
  uint8_t* data = grf_array_get_data(array);
  assert_non_null(data);
  uint64_t sum = 0;
  for(i = 0; i < grf_array_get_num_elements(array); i++){
      sum += data[i];
  }
  assert_true(sum > 0);
}

static void test_grf_image_read_jpg(void ** state){
  (void)state;
  GrfArray* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = grf_image_read_jpg("../data/chess.jpg");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
  grf_array_free(image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = grf_image_read_jpg("../data/chessgray.jpg");
  helper_test_grf_image_read(image, correct_sizes_gray, GRF_UINT8, 1,3);
  grf_array_free(image);
}

static void test_grf_image_read_png(void ** state){
  (void)state;
  GrfArray* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = grf_image_read_png("../data/chess.png");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
  grf_array_free(image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = grf_image_read_png("../data/chessgray.png");
  helper_test_grf_image_read(image, correct_sizes_gray, GRF_UINT8, 1,3);
  grf_array_free(image);
}

static void test_grf_image_write_jpg(void ** state){
  (void)state;
  const char* outfile = "imagem.jpg";
  const char* outfile_gray = "imagemgray.jpg";
  // Remove the file if it exists
  remove(outfile);
  remove(outfile_gray);

  // Define the image
  GrfArray* input_image      = grf_image_read_jpg("../data/chess.jpg");
  GrfArray* input_grf_image_gray = grf_image_read_jpg("../data/chessgray.jpg");

  // Save the image
  grf_image_write_jpg(input_image,outfile);
  grf_image_write_jpg(input_grf_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);
  grf_array_free(input_image);
  grf_array_free(input_grf_image_gray);
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
  GrfArray* input_image = grf_image_read_png("../data/chess.png");
  GrfArray* input_grf_image_gray = grf_image_read_png("../data/chessgray.png");

  // Save the image
  grf_image_write_png(input_image,outfile);
  grf_image_write_png(input_grf_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);

  // Testing 2D image
  uint8_t data[9] = {0,255,0,255,0,255,0,255,0};
  uint32_t size[2] = {3,3};
  GrfArray* grf_array_2D = grf_array_from_data(data,2,size,GRF_UINT8);
  grf_image_write_png(grf_array_2D, outfile_2D);
  assert_int_equal(access(outfile_2D, F_OK), 0);

  // Memory
  grf_array_free(input_image);
  grf_array_free(input_grf_image_gray);
  grf_array_free(grf_array_2D);
}

static void test_grf_image_read(void** state){
  (void)state;

  const char* grf_image_names[4] = {"../data/chess.png", "../data/chessgray.png", "../data/chess.jpg", "../data/chessgray.jpg"};
  uint32_t correct_channels[4] = {3,1,3,1};
  uint32_t correct_sizes[3] = {8,8,3};
  GrfArray* array;
  int i;
  for(i = 0; i < 4; i++){
    correct_sizes[2] = correct_channels[i];
    array = grf_image_read(grf_image_names[i]);
    helper_test_grf_image_read(array, correct_sizes,GRF_UINT8, 1,3);
    grf_array_free(array);
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
  GrfArray* image;
  // Color image
  uint32_t correct_sizes[2] = {8,8};
  image = grf_image_read_pgm("../data/chess.pgm");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,2);
  grf_array_free(image);
}

static void test_grf_image_write_pgm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  GrfArray* input_image = grf_image_read_pgm("../data/chess.pgm");

  // Save the image
  grf_image_write_pgm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);

  grf_array_free(input_image);
}

static void test_grf_image_read_ppm(void** state){
  (void)state;
  GrfArray* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = grf_image_read_ppm("../data/chess.ppm");
  helper_test_grf_image_read(image, correct_sizes,GRF_UINT8, 1,3);
  grf_array_free(image);
}

static void test_grf_image_write_ppm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  GrfArray* input_image = grf_image_read_ppm("../data/chess.ppm");

  // Save the image
  grf_image_write_ppm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);

  grf_array_free(input_image);
}

static void test_grf_image_cvt_color(void** state){
  (void) state;
  // From grayscale to RGB
  GrfArray* array     = grf_image_read("../data/chess.pgm");
  GrfArray* grf_array_rgb = grf_image_cvt_color(array, GRF_GRAY, GRF_RGB);
  assert_int_equal(grf_array_rgb->dim       , 3);
  assert_int_equal(grf_array_rgb->size[0]   , array->size[0]);
  assert_int_equal(grf_array_rgb->size[1]   , array->size[1]);
  assert_int_equal(grf_array_rgb->size[2]   , 3);
  assert_int_equal(grf_array_rgb->bitsize   , array->bitsize);
  assert_int_equal(grf_array_rgb->contiguous, array->contiguous);
  assert_int_equal(grf_array_rgb->num_elements  , array->num_elements*3);
  uint64_t i;
  for(i = 0; i < array->num_elements;i++){
    assert_int_equal(array->data_uint8[i],grf_array_rgb->data_uint8[i*3]);
    assert_int_equal(array->data_uint8[i],grf_array_rgb->data_uint8[i*3+1]);
    assert_int_equal(array->data_uint8[i],grf_array_rgb->data_uint8[i*3+2]);
  }

  // From RGB to grayscale
  GrfArray* grf_array_gray = grf_image_cvt_color(grf_array_rgb, GRF_RGB, GRF_GRAY);
  assert_int_equal(grf_array_gray->dim         , 2);
  assert_int_equal(grf_array_gray->size[0]     , grf_array_rgb->size[0]);
  assert_int_equal(grf_array_gray->size[1]     , grf_array_rgb->size[1]);
  assert_int_equal(grf_array_gray->bitsize     , grf_array_rgb->bitsize);
  assert_int_equal(grf_array_gray->contiguous  , grf_array_rgb->contiguous);
  assert_int_equal(grf_array_gray->num_elements, array->num_elements);
  for(i = 0; i < grf_array_gray->num_elements;i++)
    assert_int_equal(grf_array_gray->data_uint8[i],array->data_uint8[i]);

  // From Grayscale to RGBA
  GrfArray* grf_array_rgba = grf_image_cvt_color(array, GRF_GRAY, GRF_RGBA);
  assert_int_equal(grf_array_rgba->dim       , 3);
  assert_int_equal(grf_array_rgba->size[0]   , array->size[0]);
  assert_int_equal(grf_array_rgba->size[1]   , array->size[1]);
  assert_int_equal(grf_array_rgba->size[2]   , 4);
  assert_int_equal(grf_array_rgba->bitsize   , array->bitsize);
  assert_int_equal(grf_array_rgba->contiguous, array->contiguous);
  assert_int_equal(grf_array_rgba->num_elements  , array->num_elements*4);
  for(i = 0; i < array->num_elements;i++){
    assert_int_equal(array->data_uint8[i],grf_array_rgba->data_uint8[i*4]);
    assert_int_equal(array->data_uint8[i],grf_array_rgba->data_uint8[i*4+1]);
    assert_int_equal(array->data_uint8[i],grf_array_rgba->data_uint8[i*4+2]);
    assert_int_equal(255,grf_array_rgba->data_uint8[i*4+3]);
  }

  // From Grayscale to BGRA
  Array* array_bgra2 = image_cvt_color(array, GRAFEO_GRAY, GRAFEO_BGRA);
  assert_int_equal(array_bgra2->dim       , 3);
  assert_int_equal(array_bgra2->size[0]   , array->size[0]);
  assert_int_equal(array_bgra2->size[1]   , array->size[1]);
  assert_int_equal(array_bgra2->size[2]   , 4);
  assert_int_equal(array_bgra2->bitsize   , array->bitsize);
  assert_int_equal(array_bgra2->contiguous, array->contiguous);
  assert_int_equal(array_bgra2->num_elements  , array->num_elements*4);
  for(i = 0; i < array->num_elements;i++){
    assert_int_equal(array->data_uint8[i],array_rgba->data_uint8[i*4+2]);
    assert_int_equal(array->data_uint8[i],array_rgba->data_uint8[i*4+1]);
    assert_int_equal(array->data_uint8[i],array_rgba->data_uint8[i*4]);
    assert_int_equal(255,array_rgba->data_uint8[i*4+3]);
  }

  // From RGB to RGBA
  GrfArray* grf_array_rgba2 = grf_image_cvt_color(grf_array_rgb, GRF_RGB, GRF_RGBA);
  assert_int_equal(grf_array_rgba2->dim, 3);
  assert_int_equal(grf_array_rgba2->size[0]      , grf_array_rgb->size[0]);
  assert_int_equal(grf_array_rgba2->size[1]      , grf_array_rgb->size[1]);
  assert_int_equal(grf_array_rgba2->size[2]      , 4);
  assert_int_equal(grf_array_rgba2->bitsize       , grf_array_rgb->bitsize);
  assert_int_equal(grf_array_rgba2->contiguous    , grf_array_rgb->contiguous);
  assert_int_equal(grf_array_rgba2->num_elements  , grf_array_rgb->num_elements*4/3);
  for(i = 0; i < array->num_elements;i++){
    assert_int_equal(grf_array_rgba2->data_uint8[i*4]  ,grf_array_rgb->data_uint8[3*i]);
    assert_int_equal(grf_array_rgba2->data_uint8[i*4+1],grf_array_rgb->data_uint8[3*i+1]);
    assert_int_equal(grf_array_rgba2->data_uint8[i*4+2],grf_array_rgb->data_uint8[3*i+2]);
    assert_int_equal(grf_array_rgba2->data_uint8[i*4+3],255);
  }

  // From RGB to BGRA
  GrfArray* grf_array_bgra = grf_image_cvt_color(grf_array_rgb, GRF_RGB, GRF_BGRA);
  assert_int_equal(grf_array_bgra->dim, 3);
  assert_int_equal(grf_array_bgra->size[0]      , grf_array_rgb->size[0]);
  assert_int_equal(grf_array_bgra->size[1]      , grf_array_rgb->size[1]);
  assert_int_equal(grf_array_bgra->size[2]      , 4);
  assert_int_equal(grf_array_bgra->bitsize       , grf_array_rgb->bitsize);
  assert_int_equal(grf_array_bgra->contiguous    , grf_array_rgb->contiguous);
  assert_int_equal(grf_array_bgra->num_elements  , grf_array_rgb->num_elements*4/3);
  for(i = 0; i < array->num_elements;i++){
    assert_int_equal(grf_array_bgra->data_uint8[i*4]  ,grf_array_rgb->data_uint8[3*i+2]);
    assert_int_equal(grf_array_bgra->data_uint8[i*4+1],grf_array_rgb->data_uint8[3*i+1]);
    assert_int_equal(grf_array_bgra->data_uint8[i*4+2],grf_array_rgb->data_uint8[3*i]);
    assert_int_equal(grf_array_bgra->data_uint8[i*4+3],255);
  }

  grf_array_free(grf_array_gray);
  grf_array_free(grf_array_rgb);
  grf_array_free(grf_array_rgba);
  grf_array_free(grf_array_rgba2);
  grf_array_free(grf_array_bgra);
  grf_array_free(array);
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
