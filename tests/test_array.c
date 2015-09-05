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

static void helper_testing_array(Array* array, uint64_t num_elements, DataType type, uint16_t dim, uint32_t* sizes){
    assert_non_null(array);
    assert_int_equal(array_get_num_elements(array), num_elements);
    assert_int_equal(array_get_type(array), type);
    assert_int_equal(array_get_dim(array), dim);
    assert_non_null(array_get_size(array));
    uint32_t i;
    for(i = 0; i < dim; i++)
        assert_int_equal(array_get_size(array)[i], sizes[i]);
    if(num_elements)
        assert_non_null(array_get_data(array));
}

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
    uint32_t i;
    uint32_t i_max = 100;
    // Several sizes
    for(i = 0; i < i_max; i++){
        Array* array = array_new_1D(i);
        helper_testing_array(array, i, GRAFEO_UINT8, 1, &i);
        array_free(array);
    }
}

static void test_array_new_2D(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            Array* array = array_new_2D(i, j);
            helper_testing_array(array, i*j, GRAFEO_UINT8, 2, sizes);
            array_free(array);
        }
    }
}

static void test_array_new_3D(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50,
             k, k_max = 50;
    uint32_t sizes[3];
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                Array* array = array_new_3D(i, j, k);
                helper_testing_array(array, i*j*k, GRAFEO_UINT8, 3, sizes);
                array_free(array);
            }
        }
    }
}

static void test_array_new_4D(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint32_t sizes[4];
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(l = 0; l < l_max; l++){
                    sizes[3] = l;
                    Array* array = array_new_4D(i, j, k, l);
                    helper_testing_array(array, i*j*k*l, GRAFEO_UINT8, 4, sizes);
                    array_free(array);
                }
            }
        }
    }
}

static void test_array_new_1D_type(void ** state){
    (void) state;
    DataType type;
    uint32_t i, i_max=100;
    for(i = 0; i < i_max; i++){
        for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
            Array* array = array_new_1D_type(i, type);
            helper_testing_array(array, i, type, 1, &i);
            array_free(array);
        }
    }
}

static void test_array_new_2D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
    DataType type;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
                Array* array = array_new_2D_type(i, j, type);
                helper_testing_array(array, i*j, type, 2, sizes);
                array_free(array);
            }
        }
    }
}

static void test_array_new_3D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50,
             k, k_max = 50;
    uint32_t sizes[3];
    DataType type;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
                    Array* array = array_new_3D_type(i, j, k, type);
                    helper_testing_array(array, i*j*k, type, 3, sizes);
                    array_free(array);
                }
            }
        }
    }
}

static void test_array_new_4D_type(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint32_t sizes[4];
    DataType type;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(l = 0; l < l_max; l++){
                    sizes[3] = l;
                    for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
                        Array* array = array_new_4D_type(i, j, k, l, type);
                        helper_testing_array(array, i*j*k*l, type, 4, sizes);
                        array_free(array);
                    }
                }
            }
        }
    }
}

static void test_array_zeros(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    DataType type;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(l = 0; l < l_max; l++){
                    sizes[3] = l;
                    num_elements = i*j*k*l;
                    for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
                        Array* array = array_zeros(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRAFEO_UINT8:  assert_int_equal(array->data_uint8[n],  0);break;
                                case GRAFEO_UINT16: assert_int_equal(array->data_uint16[n], 0);break;
                                case GRAFEO_UINT32: assert_int_equal(array->data_uint32[n], 0);break;
                                case GRAFEO_UINT64: assert_int_equal(array->data_uint64[n], 0);break;
                                case GRAFEO_INT8:   assert_int_equal(array->data_int8[n],   0);break;
                                case GRAFEO_INT16:  assert_int_equal(array->data_int16[n],  0);break;
                                case GRAFEO_INT32:  assert_int_equal(array->data_int32[n],  0);break;
                                case GRAFEO_INT64:  assert_int_equal(array->data_int64[n],  0);break;
                                case GRAFEO_FLOAT:  assert_int_equal(array->data_float[n],  0);break;
                                case GRAFEO_DOUBLE: assert_int_equal(array->data_double[n], 0);break;
                            }
                        }
                        array_free(array);
                    }
                }
            }
        }
    }
}

static void test_array_ones(void** state){
    (void) state;
    uint32_t i, i_max = 15,
             j, j_max = 15,
             k, k_max = 15,
             l, l_max = 15;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    DataType type;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(l = 0; l < l_max; l++){
                    sizes[3] = l;
                    num_elements = i*j*k*l;
                    for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
                        Array* array = array_ones(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRAFEO_UINT8:  assert_int_equal(array->data_uint8[n],  1);break;
                                case GRAFEO_UINT16: assert_int_equal(array->data_uint16[n], 1);break;
                                case GRAFEO_UINT32: assert_int_equal(array->data_uint32[n], 1);break;
                                case GRAFEO_UINT64: assert_int_equal(array->data_uint64[n], 1);break;
                                case GRAFEO_INT8:   assert_int_equal(array->data_int8[n],   1);break;
                                case GRAFEO_INT16:  assert_int_equal(array->data_int16[n],  1);break;
                                case GRAFEO_INT32:  assert_int_equal(array->data_int32[n],  1);break;
                                case GRAFEO_INT64:  assert_int_equal(array->data_int64[n],  1);break;
                                case GRAFEO_FLOAT:  assert_true(array->data_float[n] == 1.0);break;
                                case GRAFEO_DOUBLE: assert_true(array->data_double[n] == 1.0);break;
                            }
                        }
                        array_free(array);
                    }
                }
            }
        }
    }
}

static void test_array_sub(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {10,10,10,10};
    Array* array = array_new_with_size_type(dim, sizes, type);

    // All ranges defined
    Range*  ranges[4];
    ranges[0] = range_from_to(3,6);
    ranges[1] = range_from_to(3,6);
    ranges[2] = range_from_to(3,6);
    ranges[3] = range_from_to(3,6);

    Array* subarray = array_sub(array, ranges);
    assert_int_equal(array_get_dim(subarray), 4);
    assert_int_equal(array_get_type(subarray), array_get_type(array));
    uint32_t* sizes = array_get_size(subarray);
    assert_non_null(sizes);
    assert_int_equal(sizes[0], 3);
    assert_int_equal(sizes[1], 3);
    assert_int_equal(sizes[2], 3);
    assert_int_equal(sizes[3], 3);
    array_assign_scalar(subarray, 5);
    array_get_element(array, );



    array_free(subarray);

    // A range with only the starting index
    // TODO: this will replace (we need to free 
    //       old ranges[1] memory)
    ranges[1] = range_from(starting);
    Array* subarray = array_sub(array, ranges);
    array_free(subarray);
    
    // A range with only the ending index
    ranges[2] = range_to(ending);
    Array* subarray = array_sub(array, ranges);
    array_free(subarray);

    // A range which is the whole range of a dimension
    ranges[3] = RANGE_ALL;
    Array* subarray = array_sub(array, ranges);
    array_free(subarray);


}

static void test_array_reduce(void** state){
    Array* array  = array_new_with_type();
    // Agregated sum
    Array* result = array_reduce_sum();
    // Agregated product
    Array* result = array_reduce_mult();
    // Agregated standard deviation
    Array* result = array_reduce_std();
    // Agregated maximum
    Array* result = array_reduce_max();
    // Agregated minimum
    Array* result = array_reduce_min();
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_array_new),
    cmocka_unit_test(test_array_new_1D),
    cmocka_unit_test(test_array_new_2D),
    cmocka_unit_test(test_array_new_3D),
    cmocka_unit_test(test_array_new_4D),
    cmocka_unit_test(test_array_new_1D_type),
    cmocka_unit_test(test_array_new_2D_type),
    cmocka_unit_test(test_array_new_3D_type),
    cmocka_unit_test(test_array_new_4D_type),
    cmocka_unit_test(test_array_zeros),
    cmocka_unit_test(test_array_ones),
    cmocka_unit_test(test_array_sub),
    cmocka_unit_test
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
