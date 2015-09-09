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
#include <grafeo/image.h>

static void test_image_read_jpg(void ** state){
    Array* image = image_read("image.jpg");
    assert_int_equal(array_get_dim(image), 3);
    uint32_t sizes = array_get_size(image);
    assert_non_null(sizes);
    assert_int_equal(sizes[0], 32);
    assert_int_equal(sizes[1], 64);
    assert_int_equal(sizes[2], 3);
    assert_int_equal(array_get_type(image), GRAFEO_UINT8);
    assert_int_equal(array_get_num_elements(image), 32*64*3);
    assert_int_equal(array_get_num_bytes(image), 32*64*3);
    assert_int_equal(array_get_bitsize(image), 1);
    uint8_t* data = array_get_data(image);
    assert_non_null(data);
    uint64_t sum = 0, i;
    for(i = 0; i < array_get_num_elements(image); i++){
        sum += data[i];
    }
    assert_true(sum > 0);
}

static void test_image_read_png(void ** state){
    Array* image = image_read_png("image.png");
    assert_int_equal(array_get_dim(image), 3);
    uint32_t sizes = array_get_size(image);
    assert_non_null(sizes);
    assert_int_equal(sizes[0], 32);
    assert_int_equal(sizes[1], 64);
    assert_int_equal(sizes[2], 3);
    assert_int_equal(array_get_type(image), GRAFEO_UINT8);
    assert_int_equal(array_get_num_elements(image), 32*64*3);
    assert_int_equal(array_get_num_bytes(image), 32*64*3);
    assert_int_equal(array_get_bitsize(image), 1);
    uint8_t* data = array_get_data(image);
    assert_non_null(data);
    uint64_t sum = 0, i;
    for(i = 0; i < array_get_num_elements(image); i++){
        sum += data[i];
    }
    assert_true(sum > 0);
}

static void test_image_write_jpg(void ** state){
    // Remove the file if it exists

    // Define the image
    Array* image = image;

    // Save the image

    // See if the file exists

}

static void test_image_write_png(void ** state){
    // Remove the file if it exists

    // Define the image
    Array* image = image;

    // Save the image

    // See if the file exists
}

static void test_image_read(void** state){
    // Load PNG by image_read
    // See if is a correct image
    // Load JPG by image_read
    // See if it's a correct image
}

static void test_image_write(void** state){
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

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_image_read_jpg),
    cmocka_unit_test(test_image_read_png),
    cmocka_unit_test(test_image_read),
    cmocka_unit_test(test_image_write_jpg),
    cmocka_unit_test(test_image_write_png),
    cmocka_unit_test(test_image_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}