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
#include <grafeo/array.h>
/**
 * Testing new arrays
 * TODO: testing also free
 */
static void test_array_new(void** state){
    (void) state;
    Array* array = array_new();
    assert_non_null(array);
    assert_int_equal(array_get_num_elements(array),0);
    assert_int_equal(array_get_type(array),GRAFEO_UINT8);
    assert_int_equal(array_get_dim(array), 0);
    uint32_t* size = array_get_size(array);
    assert_null(size);
    void* data = array_get_data(array);
    assert_null(data);
    array_free(array);
}

/**
 * Testing 1D array creation
 */
static void test_array_new_1D(void** state){
    (void) state;
    uint64_t i;
    uint64_t i_max = 100;
    // Several sizes
    for(i = 0; i < i_max; i++){
        Array* array = array_new_1D(i);
        //print_message("i = %" PRIu64 "\n", i);
        assert_non_null(array);
        assert_int_equal(array_get_num_elements(array),i);
        assert_int_equal(array_get_type(array), GRAFEO_UINT8);
        assert_int_equal(array_get_dim(array),1);
        assert_non_null(array_get_size(array));
        assert_int_equal(array_get_size(array)[0], i);
        if(i != 0)assert_non_null(array_get_data(array));
        array_free(array);
    }
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_array_new),
    cmocka_unit_test(test_array_new_1D),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
