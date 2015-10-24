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

void helper_test_image_read(Array* array, uint32_t* correct_sizes, DataType correct_type, uint8_t correct_bitsize, uint8_t correct_dim){
  assert_int_equal(array_get_dim(array), correct_dim);
  uint32_t* sizes = array_get_size(array);
  assert_non_null(sizes);
  assert_int_equal(sizes[0], correct_sizes[0]);
  assert_int_equal(sizes[1], correct_sizes[1]);
  if(correct_dim > 2) assert_int_equal(sizes[2], correct_sizes[2]);
  uint64_t i;
  uint64_t correct_num_elements = 1;
  for(i = 0; i < correct_dim; i++) correct_num_elements *= correct_sizes[i];
  assert_int_equal(array_get_type(array), correct_type);
  assert_int_equal(array_get_num_elements(array), correct_num_elements);
  assert_int_equal(array_get_num_bytes(array), correct_num_elements * correct_bitsize);
  assert_int_equal(array_get_bitsize(array), correct_bitsize);
  uint8_t* data = array_get_data(array);
  assert_non_null(data);
  uint64_t sum = 0;
  for(i = 0; i < array_get_num_elements(array); i++){
      sum += data[i];
  }
  assert_true(sum > 0);
}

static void test_image_read_jpg(void ** state){
  (void)state;
  Array* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = image_read_jpg("../data/chess.jpg");
  helper_test_image_read(image, correct_sizes,GRAFEO_UINT8, 1,3);
  array_free(image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = image_read_jpg("../data/chessgray.jpg");
  helper_test_image_read(image, correct_sizes_gray, GRAFEO_UINT8, 1,3);
  array_free(image);
}

static void test_image_read_png(void ** state){
  (void)state;
  Array* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = image_read_png("../data/chess.png");
  helper_test_image_read(image, correct_sizes,GRAFEO_UINT8, 1,3);
  array_free(image);

  // Gray image
  uint32_t correct_sizes_gray[3] = {8,8,1};
  image = image_read_png("../data/chessgray.png");
  helper_test_image_read(image, correct_sizes_gray, GRAFEO_UINT8, 1,3);
  array_free(image);
}

static void test_image_write_jpg(void ** state){
  (void)state;
  const char* outfile = "imagem.jpg";
  const char* outfile_gray = "imagemgray.jpg";
  // Remove the file if it exists
  remove(outfile);
  remove(outfile_gray);

  // Define the image
  Array* input_image      = image_read_jpg("../data/chess.jpg");
  Array* input_image_gray = image_read_jpg("../data/chessgray.jpg");

  // Save the image
  image_write_jpg(input_image,outfile);
  image_write_jpg(input_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);
  array_free(input_image);
  array_free(input_image_gray);
}

static void test_image_write_png(void ** state){
  (void)state;
  const char* outfile      = "imagem.png";
  const char* outfile_gray = "imagemgray.png";
  const char* outfile_2D   = "test_image_write_png_2D.png";
  // Remove the file if it exists
  remove(outfile);
  remove(outfile_gray);

  // Define the image
  Array* input_image = image_read_png("../data/chess.png");
  Array* input_image_gray = image_read_png("../data/chessgray.png");

  // Save the image
  image_write_png(input_image,outfile);
  image_write_png(input_image_gray,outfile_gray);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);
  assert_int_equal(access(outfile_gray, F_OK), 0);

  // Testing 2D image
  uint8_t data[9] = {0,255,0,255,0,255,0,255,0};
  uint32_t size[2] = {3,3};
  Array* array_2D = array_from_data(data,2,size,GRAFEO_UINT8);
  image_write_png(array_2D, outfile_2D);
  assert_int_equal(access(outfile_2D, F_OK), 0);

  // Memory
  array_free(input_image);
  array_free(input_image_gray);
  array_free(array_2D);
}

static void test_image_read(void** state){
  (void)state;

  const char* image_names[4] = {"../data/chess.png", "../data/chessgray.png", "../data/chess.jpg", "../data/chessgray.jpg"};
  uint32_t correct_channels[4] = {3,1,3,1};
  uint32_t correct_sizes[3] = {8,8,3};
  Array* array;
  int i;
  for(i = 0; i < 4; i++){
    correct_sizes[2] = correct_channels[i];
    array = image_read(image_names[i]);
    helper_test_image_read(array, correct_sizes,GRAFEO_UINT8, 1,3);
    array_free(array);
  }
}

static void test_image_write(void** state){
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

static void test_image_read_pgm(void** state){
  (void)state;
  Array* image;
  // Color image
  uint32_t correct_sizes[2] = {8,8};
  image = image_read_pgm("../data/chess.pgm");
  helper_test_image_read(image, correct_sizes,GRAFEO_UINT8, 1,2);
  array_free(image);
}

static void test_image_write_pgm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  Array* input_image = image_read_pgm("../data/chess.pgm");

  // Save the image
  image_write_pgm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);

  array_free(input_image);
}

static void test_image_read_ppm(void** state){
  (void)state;
  Array* image;
  // Color image
  uint32_t correct_sizes[3] = {8,8,3};
  image = image_read_ppm("../data/chess.ppm");
  helper_test_image_read(image, correct_sizes,GRAFEO_UINT8, 1,3);
  array_free(image);
}

static void test_image_write_ppm(void** state){
  (void)state;
  const char* outfile = "imagem.pgm";
  // Remove the file if it exists
  remove(outfile);

  // Define the image
  Array* input_image = image_read_ppm("../data/chess.ppm");

  // Save the image
  image_write_ppm(input_image,outfile);

  // See if the file exists
  assert_int_equal(access(outfile, F_OK), 0);

  array_free(input_image);
}

static void test_image_cvt_color(void** state){
  (void) state;
  // From grayscale to RGB
  Array* array     = image_read("../data/chess.pgm");
  Array* array_rgb = image_cvt_color(array, GRAFEO_GRAY, GRAFEO_RGB);
  assert_int_equal(array_rgb->dim       , 3);
  assert_int_equal(array_rgb->size[0]   , array->size[0]);
  assert_int_equal(array_rgb->size[1]   , array->size[1]);
  assert_int_equal(array_rgb->size[2]   , 3);
  assert_int_equal(array_rgb->bitsize   , array->bitsize);
  assert_int_equal(array_rgb->contiguous, array->contiguous);
  assert_int_equal(array_rgb->num_elements  , array->num_elements*3);

  for(int i = 0; i < array->num_elements;i++){
    assert_int_equal(array->data_uint8[i],array_rgb->data_uint8[i*3]);
    assert_int_equal(array->data_uint8[i],array_rgb->data_uint8[i*3+1]);
    assert_int_equal(array->data_uint8[i],array_rgb->data_uint8[i*3+2]);
  }

  // From RGB to grayscale
  Array* array_gray = image_cvt_color(array_rgb, GRAFEO_RGB, GRAFEO_GRAY);
  assert_int_equal(array_gray->dim         , 2);
  assert_int_equal(array_gray->size[0]     , array_rgb->size[0]);
  assert_int_equal(array_gray->size[1]     , array_rgb->size[1]);
  assert_int_equal(array_gray->bitsize     , array_rgb->bitsize);
  assert_int_equal(array_gray->contiguous  , array_rgb->contiguous);
  assert_int_equal(array_gray->num_elements, array->num_elements);
  for(int i = 0; i < array_gray->num_elements;i++)
    assert_int_equal(array_gray->data_uint8[i],array->data_uint8[i]);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[11]={
    cmocka_unit_test(test_image_read_jpg),
    cmocka_unit_test(test_image_read_png),
    cmocka_unit_test(test_image_read_pgm),
    cmocka_unit_test(test_image_read_ppm),
    cmocka_unit_test(test_image_read),
    cmocka_unit_test(test_image_write_jpg),
    cmocka_unit_test(test_image_write_png),
    cmocka_unit_test(test_image_write_pgm),
    cmocka_unit_test(test_image_write_ppm),
    cmocka_unit_test(test_image_write),
    cmocka_unit_test(test_image_cvt_color),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
