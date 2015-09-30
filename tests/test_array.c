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
    uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
    assert_non_null(array);
    assert_int_equal(array_get_num_elements(array), num_elements);
    assert_int_equal(array_get_type(array), type);
    assert_int_equal(array_get_dim(array), dim);
    assert_non_null(array_get_size(array));
    assert_non_null(array_get_step(array));
    assert_int_equal(array_get_bitsize(array), bitsizes[type]);
    assert_int_equal(array_get_num_bytes(array), num_elements * bitsizes[type]);
    uint32_t i;
    uint64_t step = 1;
    for(i = 0; i < dim; step*=sizes[dim-(i++)-1]){
        assert_int_equal(array_get_size(array)[i], sizes[i]);
        assert_int_equal(array_get_step(array)[dim-i-1], step);
    }
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
    assert_int_equal(array_get_bitsize(array), 0);
    assert_int_equal(array_get_num_bytes(array), 0);
    uint32_t* size = array_get_size(array);
    assert_null(size);
    assert_null(array_get_step(array));
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

static void helper_test_array_from_data(Array* array, void* dados, uint8_t dim, uint64_t num_elements, uint64_t num_bytes, uint8_t bitsize, uint64_t* step, uint32_t* size, DataType type){
  assert_non_null(array);
  assert_int_equal(array->dim, dim);
  assert_int_equal(array->num_elements, num_elements);
  assert_int_equal(array->num_bytes, num_bytes);
  assert_int_equal(array->bitsize, bitsize);
  uint8_t i;
  for(i = 0; i < array->dim; i++){
    assert_int_equal(array->step[i], step[i]);
    assert_int_equal(array->size[i], size[i]);
  }

  assert_int_equal(array->type, type);
  for(i = 0; i < array->num_elements; i++)
  {
    switch(type){
      case GRAFEO_UINT8:  assert_int_equal(array->data_uint8[i],  ((uint8_t*)dados)[i]);break;
      case GRAFEO_UINT16: assert_int_equal(array->data_uint16[i], ((uint16_t*)dados)[i]);break;
      case GRAFEO_UINT32: assert_int_equal(array->data_uint32[i], ((uint32_t*)dados)[i]);break;
      case GRAFEO_UINT64: assert_int_equal(array->data_uint64[i], ((uint64_t*)dados)[i]);break;
      case GRAFEO_INT8:   assert_int_equal(array->data_int8[i],   ((int8_t*)dados)[i]);break;
      case GRAFEO_INT16:  assert_int_equal(array->data_int16[i],  ((int16_t*)dados)[i]);break;
      case GRAFEO_INT32:  assert_int_equal(array->data_int32[i],  ((int32_t*)dados)[i]);break;
      case GRAFEO_INT64:  assert_int_equal(array->data_int64[i],  ((int64_t*)dados)[i]);break;
      case GRAFEO_FLOAT:  assert_int_equal(array->data_float[i],  ((float*)dados)[i]);break;
      case GRAFEO_DOUBLE: assert_int_equal(array->data_double[i], ((double*)dados)[i]);break;
    }
  }
}

static void test_array_from_data(void** state){
  (void) state;
  uint32_t **sizes = (uint32_t**) malloc(4*sizeof(uint32_t*));
  uint64_t **steps = (uint64_t**) malloc(4*sizeof(uint64_t*));

  uint32_t size1[1] = {16};
  uint32_t size2[2] = {4,4};
  uint32_t size3[3] = {2,4,2};
  uint32_t size4[4] = {2,2,2,2};

  uint64_t step1[1] = {1};
  uint64_t step2[2] = {4,1};
  uint64_t step3[3] = {8,2,1};
  uint64_t step4[4] = {8,4,2,1};

  sizes[0] = size1;  sizes[1] = size2;  sizes[2] = size3;  sizes[3] = size4;
  steps[0] = step1;  steps[1] = step2;  steps[2] = step3;  steps[3] = step4;

  uint8_t dados[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  Array* array;

  uint8_t i;
  DataType type;
  uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
  for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
    for(i = 0; i < 4; i++){
      array = array_from_data(dados,i+1,sizes[i],type);
      helper_test_array_from_data(array, dados, i+1, 16, 16*bitsizes[type], bitsizes[type], steps[i],sizes[i],type);
      array_free(array);
    }
  }

  free(sizes);
  free(steps);
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
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=2){
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

                        DataType type2 = GRAFEO_DOUBLE-type;
                        Array* array2 = array_zeros_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type2){
                                case GRAFEO_UINT8:  assert_int_equal(array2->data_uint8[n],  0);break;
                                case GRAFEO_UINT16: assert_int_equal(array2->data_uint16[n], 0);break;
                                case GRAFEO_UINT32: assert_int_equal(array2->data_uint32[n], 0);break;
                                case GRAFEO_UINT64: assert_int_equal(array2->data_uint64[n], 0);break;
                                case GRAFEO_INT8:   assert_int_equal(array2->data_int8[n],   0);break;
                                case GRAFEO_INT16:  assert_int_equal(array2->data_int16[n],  0);break;
                                case GRAFEO_INT32:  assert_int_equal(array2->data_int32[n],  0);break;
                                case GRAFEO_INT64:  assert_int_equal(array2->data_int64[n],  0);break;
                                case GRAFEO_FLOAT:  assert_int_equal(array2->data_float[n],  0);break;
                                case GRAFEO_DOUBLE: assert_int_equal(array2->data_double[n], 0);break;
                            }
                        }

                        array_free(array);
                        array_free(array2);
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
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=2){
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
                        DataType type2 = GRAFEO_DOUBLE - type;
                        Array* array2 = array_ones_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type2){
                                case GRAFEO_UINT8:  assert_int_equal(array2->data_uint8[n],  1);break;
                                case GRAFEO_UINT16: assert_int_equal(array2->data_uint16[n], 1);break;
                                case GRAFEO_UINT32: assert_int_equal(array2->data_uint32[n], 1);break;
                                case GRAFEO_UINT64: assert_int_equal(array2->data_uint64[n], 1);break;
                                case GRAFEO_INT8:   assert_int_equal(array2->data_int8[n],   1);break;
                                case GRAFEO_INT16:  assert_int_equal(array2->data_int16[n],  1);break;
                                case GRAFEO_INT32:  assert_int_equal(array2->data_int32[n],  1);break;
                                case GRAFEO_INT64:  assert_int_equal(array2->data_int64[n],  1);break;
                                case GRAFEO_FLOAT:  assert_true(array2->data_float[n] == 1.0);break;
                                case GRAFEO_DOUBLE: assert_true(array2->data_double[n] == 1.0);break;
                            }
                        }
                        array_free(array);
                        array_free(array2);
                    }
                }
            }
        }
    }
}

static void helper_testing_array_sub(Array* array, Array* subarray, uint32_t* subsizes, double value, Range* ranges){
    assert_int_equal(array_get_dim(subarray), 4);
    assert_int_equal(array_get_type(subarray), array_get_type(array));
    uint32_t* sizes = array_get_size(subarray);
    uint64_t* step = array_get_step(subarray);
    assert_non_null(sizes);
    assert_non_null(step);
    uint64_t stepAtual = 1,i;
    for(i = 0; i < array_get_dim(array); stepAtual*= array_get_size(array)[3-(i++)]){
        assert_int_equal(sizes[i], subsizes[i]);
        assert_int_equal(step[3-i], stepAtual);
    }
    array_fill(subarray, value);

    uint32_t indices[4];
    uint32_t faixas_from[4], faixas_to[4];
    for(i = 0; i < 4; i++){
      faixas_from[i] = (ranges[i].from)?ranges[i].from->value:0;
      faixas_to[i]   = (ranges[i].to)?ranges[i].to->value:array->size[i];
    }
    double value1;

    for(indices[0] = faixas_from[0]; indices[0] < faixas_to[0]; indices[0]++){
        for(indices[1] = faixas_from[1]; indices[1] < faixas_to[1]; indices[1]++){
            for(indices[2] = faixas_from[2]; indices[2] < faixas_to[2]; indices[2]++){
                for(indices[3] = faixas_from[3]; indices[3] < faixas_to[3]; indices[3]++){
        switch(array->type){
          case GRAFEO_UINT8: value1 = (double)(*(uint8_t*)array_get_element(array, indices));break;
          case GRAFEO_UINT16: value1 = (double)(*(uint16_t*)array_get_element(array, indices));break;
          case GRAFEO_UINT32: value1 = (double)(*(uint32_t*)array_get_element(array, indices));break;
          case GRAFEO_UINT64: value1 = (double)(*(uint64_t*)array_get_element(array, indices));break;
          case GRAFEO_INT8: value1 = (double)(*(int8_t*)array_get_element(array, indices));break;
          case GRAFEO_INT16: value1 = (double)(*(int16_t*)array_get_element(array, indices));break;
          case GRAFEO_INT32: value1 = (double)(*(int32_t*)array_get_element(array, indices));break;
          case GRAFEO_INT64: value1 = (double)(*(int64_t*)array_get_element(array, indices));break;
          case GRAFEO_FLOAT: value1 = (double)(*(float*)array_get_element(array, indices));break;
          case GRAFEO_DOUBLE: value1 = *(double*)array_get_element(array, indices);break;
        }
        assert_int_equal(value1, value);
    }}}}

}

static void test_array_sub(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {4,4,4,4};
    Array* array = array_new_with_size_type(dim, sizes, GRAFEO_INT32);
    array_fill(array, 10);

    // All ranges defined
    Range ranges[4];
    range_from_to(&ranges[0],1,3);
    range_from_to(&ranges[1],1,3);
    range_from_to(&ranges[2],1,3);
    range_from_to(&ranges[3],1,3);

    Array* subarray = array_sub(array, ranges);
    uint32_t subsizes[4] = {2,2,2,2};
    helper_testing_array_sub(array, subarray, subsizes, 5, ranges);
    array_free(subarray);
    assert_non_null(array);

    // A range with only the starting index
    // TODO: this will replace (we need to free 
    //       old ranges[1] memory)
    uint32_t starting = 1;
    range_from(&ranges[1], starting);
    subarray = array_sub(array, ranges);
    subsizes[1] = 3;
    helper_testing_array_sub(array, subarray, subsizes, 6, ranges);
    array_free(subarray);
    assert_non_null(array);
    
    // A range with only the ending index
    uint32_t ending = 3;
    range_to(&ranges[2],ending);
    subarray = array_sub(array, ranges);
    subsizes[2] = 3;
    helper_testing_array_sub(array, subarray, subsizes, 7, ranges);
    array_free(subarray);
    assert_non_null(array);

    // A range which is the whole range of a dimension
    range_all(&ranges[3]);
    subsizes[3] = 4;
    subarray = array_sub(array, ranges);
    helper_testing_array_sub(array, subarray, subsizes, 8, ranges);
    array_free(subarray);
    assert_non_null(array);


}

static void test_array_reduce(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {10,10,10,10};
    Array* array, *result;
    DataType type;

    for(type = GRAFEO_UINT8; type <= GRAFEO_DOUBLE; type++){
      array  = array_new_with_size_type(dim, sizes, type);
      array_fill(array,10);
      // Agregated sum
      int16_t eixos[2] = {1,2};
      result = array_reduce_sum(array, eixos, 2);
      assert_int_equal(array_get_dim(result), 2);
      array_free(result);
      // Agregated product
      result = array_reduce_mult(array, eixos, 2);
      assert_int_equal(array_get_dim(result), 2);
      array_free(result);
      // Agregated standard deviation
      result = array_reduce_std(array, eixos, 2);
      assert_int_equal(array_get_dim(result), 2);
      array_free(result);
      // Agregated maximum
      result = array_reduce_max(array, eixos, 2);
      assert_int_equal(array_get_dim(result), 2);
      array_free(result);
      // Agregated minimum
      result = array_reduce_min(array, eixos, 2);
      assert_int_equal(array_get_dim(result), 2);
      array_free(result);
    }

}

static void test_array_ops(){
  uint64_t i;
  Array* array = array_new_2D(5,4);
  Array* array2= array_new_2D(5,4);

  array_fill(array, 5);
  array_fill(array2, 7);

  // Adding two arrays to a new one
  Array* result = array_sum(array, array2);
  for(i = 0; i < result->num_elements; i++)
    assert_int_equal(result->data_uint8[i],12);

  // Adding a scalar inside an array
  Array* result1 = array_sum_scalar(array, 7);
  for(i = 0; i < result1->num_elements; i++)
    assert_int_equal(result1->data_uint8[i],12);

  // Subtracting two arrays
  Array* result2 = array_subtract(result, array2);
  for(i = 0; i < result2->num_elements; i++)
    assert_int_equal(result2->data_uint8[i],5);

  // Subtracting a scalar from an array
  Array* result3 = array_subtract_scalar(result, 7);
  for(i = 0; i < result3->num_elements; i++)
    assert_int_equal(result3->data_uint8[i],5);

  // Multiply
  Array* result4 = array_mult(array, array2);
  for(i = 0; i < result4->num_elements; i++)
    assert_int_equal(result4->data_uint8[i],35);

  // Multiply by a scalar
  Array* result5 = array_mult_scalar(array, 7);
  for(i = 0; i < result5->num_elements; i++)
    assert_int_equal(result5->data_uint8[i],35);

  // Divide
  Array* result6 = array_divide(result5, array2);
  for(i = 0; i < result6->num_elements; i++)
    assert_int_equal(result6->data_uint8[i],5);

  // Divide by a scalar
  Array* result7 = array_divide_scalar(result5, 7);
  for(i = 0; i < result7->num_elements; i++)
    assert_int_equal(result7->data_uint8[i],5);

  array_free(result);
  array_free(result1);
  array_free(result2);
  array_free(result3);
  array_free(result4);
  array_free(result5);
  array_free(result6);
  array_free(result7);
}

static void test_array_indices_manip(void** state){
  (void) state;
  // Create an array
  uint32_t sizes[2] = {2,2};
  Array*  array    = array_zeros(2,sizes,GRAFEO_UINT8);

  // Convert an valid ND index to 1D
  int32_t indices1[2] = {0,1};
  int64_t resp = array_index_1D(array, indices1);
  assert_int_equal(resp, 1);

  // Convert an invalid ND index to 1D
  int32_t indices2[2] = {2,0};
  resp = array_index_1D(array, indices2);
  assert_int_equal(resp, 4);

  int32_t indices3[2] = {-2,0};
  resp = array_index_1D(array, indices3);
  assert_int_equal(resp, -4);

  // Convert an valid 1D index to ND
  int32_t* respND = array_index(array, 2);
  assert_int_equal(respND[0], 1);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Convert an invalid 1D index to ND
  array_index(array, 8);
  assert_int_equal(respND[0], 4);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Compare valid index to result of validation
  resp = array_index_1D_is_valid(array, 2);
  assert_int_equal(resp, 1);

  // Compare invalid index to result of validation
  resp = array_index_1D_is_valid(array, -5);
  assert_int_equal(resp, 0);
  resp = array_index_1D_is_valid(array, 8);
  assert_int_equal(resp, 0);

  // ND Indices are in valid range?
  assert_true (array_index_is_valid(array, indices1));
  assert_false(array_index_is_valid(array, indices2));
  assert_false(array_index_is_valid(array, indices3));

  // Modifying value at specified index
  uint32_t indices1u[2] = {(uint32_t)indices1[0],(uint32_t)indices1[1]};
  array_set_element(array, indices1u, 3);
  uint8_t values[4] = {0,3,0,0};
  uint8_t i;
  for(i = 0; i < 4; i++) assert_int_equal(array->data_uint8[i], values[i]);
  array_free(array);
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
    cmocka_unit_test(test_array_from_data),
    cmocka_unit_test(test_array_zeros),
    cmocka_unit_test(test_array_ones),
    cmocka_unit_test(test_array_sub),
    cmocka_unit_test(test_array_reduce),
    cmocka_unit_test(test_array_ops),
    cmocka_unit_test(test_array_indices_manip),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
