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
    Array* array = array_new();
    assert_non_null(array);
    assert_int_equal(array_get_num_elements(array),0);
    assert_int_equal(array_get_type(array),GRAFEO_UINT8);
    assert_int_equal(array_get_dim(array), 0);
    array_free(array);
}

int main(int argc, char** argv){
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_array_new),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
