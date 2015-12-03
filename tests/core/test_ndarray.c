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
#include <grafeo/core.h>

static void helper_testing_array(GrfNDArray* array, uint64_t num_elements, GrfDataType type, uint16_t dim, uint32_t* sizes){
    uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
    assert_non_null(array);
    assert_int_equal(grf_ndarray_get_num_elements(array), num_elements);
    assert_int_equal(grf_ndarray_get_datatype(array), type);
    assert_int_equal(grf_ndarray_get_dim(array), dim);
    assert_non_null(grf_ndarray_get_size(array));
    assert_non_null(grf_ndarray_get_step(array));
    assert_int_equal(grf_ndarray_get_bitsize(array), bitsizes[type]);
    assert_int_equal(grf_ndarray_get_num_bytes(array), num_elements * bitsizes[type]);
    uint32_t i;
    uint64_t step = 1;
    for(i = 0; i < dim; step*=sizes[dim-(i++)-1]){
        assert_int_equal(grf_ndarray_get_size(array)[i], sizes[i]);
        assert_int_equal(grf_ndarray_get_step(array)[dim-i-1], step);
    }
    if(num_elements)
        assert_non_null(grf_ndarray_get_data(array));
}

/**
 * Testing new arrays
 * TODO: testing also free
 */
static void test_grf_ndarray_new(void** state){
    (void) state;
    g_autoptr(GrfNDArray) array = grf_ndarray_new();
    assert_non_null(array);
    assert_int_equal(grf_ndarray_get_num_elements(array),0);
    assert_int_equal(grf_ndarray_get_datatype(array),GRF_UINT8);
    assert_int_equal(grf_ndarray_get_dim(array), 0);
    assert_int_equal(grf_ndarray_get_bitsize(array), 0);
    assert_int_equal(grf_ndarray_get_num_bytes(array), 0);
    uint32_t* size = grf_ndarray_get_size(array);
    assert_null(size);
    assert_null(grf_ndarray_get_step(array));
    void* data = grf_ndarray_get_data(array);
    assert_null(data);
}

/**
 * Testing 1D array creation
 */
static void test_grf_ndarray_new_1D(void** state){
    (void) state;
    uint32_t i;
    uint32_t i_max = 100;
    // Several sizes
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i++){
        array = grf_ndarray_new_1D(i);
        helper_testing_array(array, i, GRF_UINT8, 1, &i);
        g_clear_object(&array);
    }
}

static void test_grf_ndarray_new_2D(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
   GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            array = grf_ndarray_new_2D(i, j);
            helper_testing_array(array, i*j, GRF_UINT8, 2, sizes);
            g_clear_object(&array);
        }
    }
}

static void test_grf_ndarray_new_3D(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50,
             k, k_max = 50;
    uint32_t sizes[3];
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                array = grf_ndarray_new_3D(i, j, k);
                helper_testing_array(array, i*j*k, GRF_UINT8, 3, sizes);
                g_clear_object(&array);
            }
        }
    }
}

static void test_grf_ndarray_new_4D(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint32_t sizes[4];
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            for(k = 0; k < k_max; k++){
                sizes[2] = k;
                for(l = 0; l < l_max; l++){
                    sizes[3] = l;
                    array = grf_ndarray_new_4D(i, j, k, l);
                    helper_testing_array(array, i*j*k*l, GRF_UINT8, 4, sizes);
                    g_clear_object(&array);
                }
            }
        }
    }
}

static void test_grf_ndarray_new_1D_type(void ** state){
    (void) state;
    GrfDataType type;
    uint32_t i, i_max=100;
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i+=2){
        for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
            array = grf_ndarray_new_1D_type(i, type);
            helper_testing_array(array, i, type, 1, &i);
            g_clear_object(&array);
        }
    }
}

static void test_grf_ndarray_new_2D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
    GrfDataType type;
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                array = grf_ndarray_new_2D_type(i, j, type);
                helper_testing_array(array, i*j, type, 2, sizes);
                g_clear_object(&array);
            }
        }
    }
}

static void test_grf_ndarray_new_3D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50,
             k, k_max = 50;
    uint32_t sizes[3];
    GrfDataType type;
    GrfNDArray* array  = NULL;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                    array = grf_ndarray_new_3D_type(i, j, k, type);
                    helper_testing_array(array, i*j*k, type, 3, sizes);
                    g_clear_object(&array);
                }
            }
        }
    }
}

static void test_grf_ndarray_new_4D_type(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint32_t sizes[4];
    GrfDataType type;
    GrfNDArray* array = NULL;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=2){
                    sizes[3] = l;
                    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                        array = grf_ndarray_new_4D_type(i, j, k, l, type);
                        helper_testing_array(array, i*j*k*l, type, 4, sizes);
                        g_clear_object(&array);
                    }
                }
            }
        }
    }
}

static void helper_test_grf_ndarray_from_data(GrfNDArray* array, void* data, uint8_t dim, uint64_t num_elements, uint64_t num_bytes, uint8_t bitsize, uint64_t* step, uint32_t* size, GrfDataType type){
  assert_non_null(array);
  uint16_t array_dim = grf_ndarray_get_dim(array);
  uint64_t array_num_elements = grf_ndarray_get_num_elements(array);
  assert_int_equal(array_dim, dim);
  assert_int_equal(array_num_elements, num_elements);
  assert_int_equal(grf_ndarray_get_num_bytes(array), num_bytes);
  assert_int_equal(grf_ndarray_get_bitsize(array), bitsize);
  uint8_t i;
  for(i = 0; i < array_dim; i++){
    assert_int_equal(grf_ndarray_get_step(array)[i], step[i]);
    assert_int_equal(grf_ndarray_get_size(array)[i], size[i]);
  }

  assert_int_equal(grf_ndarray_get_datatype(array), type);


  for(i = 0; i < array_num_elements; i++)
  {
    long double array_value = grf_ndarray_get_long_double_1D(array,i);
    long double data_value;
    switch(type){
      case GRF_UINT8:  data_value = (long double)((uint8_t*)data)[i];break;
      case GRF_UINT16: data_value = (long double)((uint16_t*)data)[i];break;
      case GRF_UINT32: data_value = (long double)((uint32_t*)data)[i];break;
      case GRF_UINT64: data_value = (long double)((uint64_t*)data)[i];break;
      case GRF_INT8:   data_value = (long double)((int8_t*)data)[i];break;
      case GRF_INT16:  data_value = (long double)((int16_t*)data)[i];break;
      case GRF_INT32:  data_value = (long double)((int32_t*)data)[i];break;
      case GRF_INT64:  data_value = (long double)((int64_t*)data)[i];break;
      case GRF_FLOAT:  data_value = (long double)((float*)data)[i];break;
      case GRF_DOUBLE: data_value = (long double)((double*)data)[i];break;
    }
    assert_true(array_value == data_value);
  }
}

static void test_grf_ndarray_from_data(void** state){
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
  GrfNDArray* array = NULL;

  uint8_t i;
  GrfDataType type;
  uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
  for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
    for(i = 0; i < 4; i++){
      array = grf_ndarray_from_data(dados,i+1,sizes[i],type);
      helper_test_grf_ndarray_from_data(array, dados, i+1, 16, 16*bitsizes[type], bitsizes[type], steps[i],sizes[i],type);
      g_clear_object(&array);
    }
  }

  free(sizes);
  free(steps);
}

#define TEST_ARRAY(type2, array, value, valuef)\
array##_data = grf_ndarray_get_data(array);\
for(n = num_elements-1; n < num_elements; n++){\
    switch(type2){\
        case GRF_UINT8:  assert_int_equal(((uint8_t *)array##_data)[n], value);break;\
        case GRF_UINT16: assert_int_equal(((uint16_t*)array##_data)[n], value);break;\
        case GRF_UINT32: assert_int_equal(((uint32_t*)array##_data)[n], value);break;\
        case GRF_UINT64: assert_int_equal(((uint64_t*)array##_data)[n], value);break;\
        case GRF_INT8:   assert_int_equal(((  int8_t*)array##_data)[n], value);break;\
        case GRF_INT16:  assert_int_equal((( int16_t*)array##_data)[n], value);break;\
        case GRF_INT32:  assert_int_equal((( int32_t*)array##_data)[n], value);break;\
        case GRF_INT64:  assert_int_equal((( int64_t*)array##_data)[n], value);break;\
        case GRF_FLOAT:  assert_true(((  float *)array##_data)[n]== valuef);break;\
        case GRF_DOUBLE: assert_true(((  double*)array##_data)[n]== valuef);break;\
    }\
}
static void test_grf_ndarray_zeros(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    GrfDataType type;
    GrfNDArray* array = NULL;
    GrfNDArray* array_like = NULL;
    GrfNDArray* array2 = NULL;
    void* array_data = NULL;
    void* array2_data = NULL;
    void* array_like_data = NULL;
    for(i = 0; i < i_max; i+=4){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=4){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=4){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=4){
                    sizes[3] = l;
                    num_elements = i*j*k*l;
                    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                        array = grf_ndarray_zeros(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        TEST_ARRAY(type,array,0,0)

                        array_like = grf_ndarray_zeros_like(array);
                        TEST_ARRAY(type,array_like,0,0)
                        helper_testing_array(array_like, num_elements, type, 4, sizes);
                        g_clear_object(&array_like);

                        GrfDataType type2 = GRF_DOUBLE-type;
                        array2 = grf_ndarray_zeros_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        TEST_ARRAY(type2,array2,0,0)

                        g_clear_object(&array);
                        g_clear_object(&array2);
                    }
                }
            }
        }
    }
}


static void test_grf_ndarray_ones(void** state){
    (void) state;
    uint32_t i, i_max = 15,
             j, j_max = 15,
             k, k_max = 15,
             l, l_max = 15;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    GrfDataType type;
    GrfNDArray* array = NULL;
    GrfNDArray* array_like = NULL;
    GrfNDArray* array2 = NULL;

    void* array_data = NULL;
    void* array_like_data = NULL;
    void* array2_data = NULL;

    for(i = 0; i < i_max; i+=4){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=4){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=4){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=4){
                    sizes[3] = l;
                    num_elements = i*j*k*l;
                    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                        array = grf_ndarray_ones(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        TEST_ARRAY(type,array,1,1.0)

                        array_like = grf_ndarray_ones_like(array);
                        helper_testing_array(array_like, num_elements, type, 4, sizes);
                        TEST_ARRAY(type,array_like,1,1.0)


                        GrfDataType type2 = GRF_DOUBLE - type;
                        array2 = grf_ndarray_ones_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        TEST_ARRAY(type2,array2,1,1.0)

                        g_clear_object(&array);
                        g_clear_object(&array_like);
                        g_clear_object(&array2);
                    }
                }
            }
        }
    }
}

static void helper_testing_grf_ndarray_sub(GrfNDArray* array, GrfNDArray* subarray, uint32_t* subsizes, double value, GrfRange* ranges){
    assert_int_equal(grf_ndarray_get_dim(subarray), 4);
    assert_int_equal(grf_ndarray_get_datatype(subarray), grf_ndarray_get_datatype(array));
    uint32_t* sizes = grf_ndarray_get_size(subarray);
    uint64_t* step = grf_ndarray_get_step(subarray);
    assert_non_null(sizes);
    assert_non_null(step);
    uint64_t stepAtual = 1,i;
    for(i = 0; i < grf_ndarray_get_dim(array); stepAtual*= grf_ndarray_get_size(array)[3-(i++)]){
        assert_int_equal(sizes[i], subsizes[i]);
        assert_int_equal(step[3-i], stepAtual);
    }
    grf_ndarray_fill(subarray, value);

    uint32_t indices[4];
    uint32_t faixas_from[4], faixas_to[4];
    uint32_t* array_size = grf_ndarray_get_size(array);
    for(i = 0; i < 4; i++){
      faixas_from[i] = (ranges[i].from)?ranges[i].from->value:0;
      faixas_to[i]   = (ranges[i].to)?ranges[i].to->value:array_size[i];
    }
    double value1;
    GrfDataType array_type = grf_ndarray_get_datatype(array);
    for(indices[0] = faixas_from[0]; indices[0] < faixas_to[0]; indices[0]++){
        for(indices[1] = faixas_from[1]; indices[1] < faixas_to[1]; indices[1]++){
            for(indices[2] = faixas_from[2]; indices[2] < faixas_to[2]; indices[2]++){
                for(indices[3] = faixas_from[3]; indices[3] < faixas_to[3]; indices[3]++){
        switch(array_type){
          case GRF_UINT8: value1 = (double)(*(uint8_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_UINT16: value1 = (double)(*(uint16_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_UINT32: value1 = (double)(*(uint32_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_UINT64: value1 = (double)(*(uint64_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_INT8: value1 = (double)(*(int8_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_INT16: value1 = (double)(*(int16_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_INT32: value1 = (double)(*(int32_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_INT64: value1 = (double)(*(int64_t*)grf_ndarray_get_element(array, indices));break;
          case GRF_FLOAT: value1 = (double)(*(float*)grf_ndarray_get_element(array, indices));break;
          case GRF_DOUBLE: value1 = *(double*)grf_ndarray_get_element(array, indices);break;
        }
        assert_int_equal(value1, value);
    }}}}

}

static void test_grf_ndarray_sub(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {4,4,4,4};
    GrfNDArray* array = grf_ndarray_new_with_size_type(dim, sizes, GRF_INT32);
    grf_ndarray_fill(array, 10);

    // All ranges defined
    GrfRange ranges[4];
    grf_range_from_to(&ranges[0],1,3);
    grf_range_from_to(&ranges[1],1,3);
    grf_range_from_to(&ranges[2],1,3);
    grf_range_from_to(&ranges[3],1,3);

    GrfNDArray* subarray = grf_ndarray_sub(array, ranges);
    uint32_t subsizes[4] = {2,2,2,2};
    helper_testing_grf_ndarray_sub(array, subarray, subsizes, 5, ranges);
    g_clear_object(&subarray);
    assert_non_null(array);

    // A range with only the starting index
    // TODO: this will replace (we need to free 
    //       old ranges[1] memory)
    uint32_t starting = 1;
    grf_range_from(&ranges[1], starting);
    subarray = grf_ndarray_sub(array, ranges);
    subsizes[1] = 3;
    helper_testing_grf_ndarray_sub(array, subarray, subsizes, 6, ranges);
    g_clear_object(&subarray);
    assert_non_null(array);
    
    // A range with only the ending index
    uint32_t ending = 3;
    grf_range_to(&ranges[2],ending);
    subarray = grf_ndarray_sub(array, ranges);
    subsizes[2] = 3;
    helper_testing_grf_ndarray_sub(array, subarray, subsizes, 7, ranges);
    g_clear_object(&subarray);
    assert_non_null(array);

    // A range which is the whole range of a dimension
    grf_range_all(&ranges[3]);
    subsizes[3] = 4;
    subarray = grf_ndarray_sub(array, ranges);
    helper_testing_grf_ndarray_sub(array, subarray, subsizes, 8, ranges);
    g_clear_object(&subarray);
    assert_non_null(array);


}

static void test_grf_ndarray_reduce(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {10,10,10,10};
    GrfNDArray* array, *result;
    long double result_1D;
    GrfDataType type;

    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
      array  = grf_ndarray_new_with_size_type(dim, sizes, type);
      grf_ndarray_fill(array,10);
      // Agregated sum
      int16_t eixos[2] = {1,2};
      result = grf_ndarray_reduce_sum(array, eixos, 2);
      assert_int_equal(grf_ndarray_get_dim(result), 2);
      result_1D = grf_ndarray_reduce_sum_num(array);
      assert_int_equal(result_1D, 100000);
      g_clear_object(&result);
      // Agregated product
      result = grf_ndarray_reduce_mult(array, eixos, 2);
      assert_int_equal(grf_ndarray_get_dim(result), 2);
      g_clear_object(&result);
      // Agregated standard deviation
      result = grf_ndarray_reduce_std(array, eixos, 2);
      assert_int_equal(grf_ndarray_get_dim(result), 2);
      g_clear_object(&result);
      // Agregated maximum
      result = grf_ndarray_reduce_max(array, eixos, 2);
      assert_int_equal(grf_ndarray_get_dim(result), 2);
      result_1D = grf_ndarray_reduce_max_num(array);
      assert_int_equal(result_1D, 10);
      g_clear_object(&result);
      // Agregated minimum
      result = grf_ndarray_reduce_min(array, eixos, 2);
      assert_int_equal(grf_ndarray_get_dim(result), 2);
      result_1D = grf_ndarray_reduce_min_num(array);
      assert_int_equal(result_1D, 10);
      g_clear_object(&result);
    }

}

static void test_grf_ndarray_ops(){
  uint64_t i;
  GrfNDArray* array = grf_ndarray_new_2D(5,4);
  GrfNDArray* array2= grf_ndarray_new_2D(5,4);

  grf_ndarray_fill(array, 5);
  grf_ndarray_fill(array2, 7);

  // Adding two arrays to a new one
  g_autoptr(GrfNDArray) result = grf_ndarray_sum(array, array2);
  uint64_t result_num_elements = grf_ndarray_get_num_elements(result);
  uint8_t* result_data_uint8 = (uint8_t*)grf_ndarray_get_data(result);
  for(i = 0; i < result_num_elements; i++)
    assert_int_equal(result_data_uint8[i],12);

  // Adding a scalar inside an array
  g_autoptr(GrfNDArray) result1 = grf_ndarray_sum_scalar(array, 7);
  uint64_t result1_num_elements = grf_ndarray_get_num_elements(result1);
  uint8_t* result1_data_uint8 = (uint8_t*)grf_ndarray_get_data(result1);
  for(i = 0; i < result1_num_elements; i++)
    assert_int_equal(result1_data_uint8[i],12);

  // Subtracting two arrays
  g_autoptr(GrfNDArray) result2 = grf_ndarray_subtract(result, array2);
  uint64_t result2_num_elements = grf_ndarray_get_num_elements(result2);
  uint8_t* result2_data_uint8 = (uint8_t*)grf_ndarray_get_data(result2);
  for(i = 0; i < result2_num_elements; i++)
    assert_int_equal(result2_data_uint8[i],5);

  // Subtracting a scalar from an array
  g_autoptr(GrfNDArray) result3 = grf_ndarray_subtract_scalar(result, 7);
  uint64_t result3_num_elements = grf_ndarray_get_num_elements(result3);
  uint8_t* result3_data_uint8 = (uint8_t*)grf_ndarray_get_data(result3);
  for(i = 0; i < result3_num_elements; i++)
    assert_int_equal(result3_data_uint8[i],5);

  // Multiply
  g_autoptr(GrfNDArray) result4 = grf_ndarray_mult(array, array2);
  uint64_t result4_num_elements = grf_ndarray_get_num_elements(result4);
  uint8_t* result4_data_uint8 = (uint8_t*)grf_ndarray_get_data(result4);
  for(i = 0; i < result4_num_elements; i++)
    assert_int_equal(result4_data_uint8[i],35);

  // Multiply by a scalar
  g_autoptr(GrfNDArray) result5 = grf_ndarray_mult_scalar(array, 7);
  uint64_t result5_num_elements = grf_ndarray_get_num_elements(result5);
  uint8_t* result5_data_uint8 = (uint8_t*)grf_ndarray_get_data(result5);
  for(i = 0; i < result5_num_elements; i++)
    assert_int_equal(result5_data_uint8[i],35);

  // Divide
  g_autoptr(GrfNDArray) result6 = grf_ndarray_divide(result5, array2);
  uint64_t result6_num_elements = grf_ndarray_get_num_elements(result6);
  uint8_t* result6_data_uint8 = (uint8_t*)grf_ndarray_get_data(result6);
  for(i = 0; i < result6_num_elements; i++)
    assert_int_equal(result6_data_uint8[i],5);

  // Divide by a scalar
  g_autoptr(GrfNDArray) result7 = grf_ndarray_divide_scalar(result5, 7);
  uint64_t result7_num_elements = grf_ndarray_get_num_elements(result7);
  uint8_t* result7_data_uint8 = (uint8_t*)grf_ndarray_get_data(result7);
  for(i = 0; i < result7_num_elements; i++)
    assert_int_equal(result7_data_uint8[i],5);
}

static void test_grf_ndarray_indices_manip(void** state){
  (void) state;
  // Create an array
  uint32_t sizes[2] = {2,2};
  g_autoptr(GrfNDArray)  array    = grf_ndarray_zeros(2,sizes,GRF_UINT8);

  // Convert an valid ND index to 1D
  int32_t indices1[2] = {0,1};
  int64_t resp = grf_ndarray_index_1D(array, indices1);
  assert_int_equal(resp, 1);

  // Convert an invalid ND index to 1D
  int32_t indices2[2] = {2,0};
  resp = grf_ndarray_index_1D(array, indices2);
  assert_int_equal(resp, 4);

  int32_t indices3[2] = {-2,0};
  resp = grf_ndarray_index_1D(array, indices3);
  assert_int_equal(resp, -4);

  // Convert an valid 1D index to ND
  int32_t* respND = grf_ndarray_index(array, 2);
  assert_int_equal(respND[0], 1);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Convert an invalid 1D index to ND
  grf_ndarray_index(array, 8);
  assert_int_equal(respND[0], 4);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Compare valid index to result of validation
  resp = grf_ndarray_index_1D_is_valid(array, 2);
  assert_int_equal(resp, 1);

  // Compare invalid index to result of validation
  resp = grf_ndarray_index_1D_is_valid(array, -5);
  assert_int_equal(resp, 0);
  resp = grf_ndarray_index_1D_is_valid(array, 8);
  assert_int_equal(resp, 0);

  // ND Indices are in valid range?
  assert_true (grf_ndarray_index_is_valid(array, indices1));
  assert_false(grf_ndarray_index_is_valid(array, indices2));
  assert_false(grf_ndarray_index_is_valid(array, indices3));

  // Modifying value at specified index
  uint32_t indices1u[2] = {(uint32_t)indices1[0],(uint32_t)indices1[1]};
  grf_ndarray_set_element(array, indices1u, 3);
  uint8_t values[4] = {0,3,0,0};
  uint8_t i;
  uint8_t* array_data_uint8 = (uint8_t*)grf_ndarray_get_data(array);
  for(i = 0; i < 4; i++) assert_int_equal(array_data_uint8[i], values[i]);
}

static void assert_grf_ndarray_equal(GrfNDArray* array1, GrfNDArray* array2){
  uint64_t i;
  GrfDataType array1_type = grf_ndarray_get_datatype(array1);
  GrfDataType array2_type = grf_ndarray_get_datatype(array2);
  uint16_t    array1_dim  = grf_ndarray_get_dim(array1);
  uint16_t    array2_dim  = grf_ndarray_get_dim(array2);
  uint64_t    array1_num_elements = grf_ndarray_get_num_elements(array1);
  uint64_t    array2_num_elements = grf_ndarray_get_num_elements(array2);
  uint32_t*   array1_size = grf_ndarray_get_size(array1);
  uint32_t*   array2_size = grf_ndarray_get_size(array2);
  assert_non_null(array1);
  assert_non_null(array2);
  assert_int_equal(array1_type, array2_type);
  assert_int_equal(array1_dim, array2_dim);
  assert_int_equal(array1_num_elements, array2_num_elements);
  for(i = 0; i < array1_dim; i++)
    assert_int_equal(array1_size[i],array2_size[i]);
  for(i = 0; i < array1_num_elements; i++)
    assert_int_equal(grf_ndarray_get_long_double_1D(array1,i),
                     grf_ndarray_get_long_double_1D(array2,i));
}

static void test_grf_ndarray_io_csv(void** state){
  (void) state;
  uint64_t i,j;

  // Loading CSV
  // UINT8
  // 1D (just one line)
  uint8_t ground_data1[8] = {1,2,3,4,5,6,7,8};
  g_autoptr(GrfNDArray) array = grf_ndarray_read_csv("../data/test_array_read_csv_uint8_1.csv");
  GrfDataType array_type         = grf_ndarray_get_datatype    (array);
  uint16_t    array_dim          = grf_ndarray_get_dim         (array);
  uint32_t*   array_size         = grf_ndarray_get_size        (array);
  uint64_t    array_num_elements = grf_ndarray_get_num_elements(array);
  uint8_t*    array_data_uint8   = grf_ndarray_get_data        (array);
  assert_non_null(array);
  assert_int_equal(array_type, GRF_UINT8);
  assert_int_equal(array_dim, 1);
  assert_int_equal(array_size[0],8);
  assert_int_equal(array_num_elements, 8);
  for(i = 0; i < array_num_elements;i++)
    assert_int_equal(array_data_uint8[i], ground_data1[i]);

  // 2D (several lines)
  uint8_t ground_data2[10][8];
  for(i = 0; i < 10; i++)
    for(j = 0; j < 8; j++)
      ground_data2[i][j] = i*8+j;
  g_autoptr(GrfNDArray) array2 = grf_ndarray_read_csv("../data/test_array_read_csv_uint8_2.csv");
  GrfDataType array2_type         = grf_ndarray_get_datatype    (array2);
  uint16_t    array2_dim          = grf_ndarray_get_dim         (array2);
  uint32_t*   array2_size         = grf_ndarray_get_size        (array2);
  uint64_t    array2_num_elements = grf_ndarray_get_num_elements(array2);
  uint8_t*    array2_data_uint8   = grf_ndarray_get_data        (array2);
  assert_non_null(array2);
  assert_int_equal(array2_type, GRF_UINT8);
  assert_int_equal(array2_dim, 2);
  assert_int_equal(array2_size[0],10);
  assert_int_equal(array2_size[1],8);
  assert_int_equal(array2_num_elements, 80);
  for(i = 0; i < 10; i++)
    for(j = 0; j < 8; j++)
      assert_int_equal(array2_data_uint8[i*8+j], ground_data2[i][j]);

  // UINT16
  // 1D (just one line)
  uint16_t ground_data3[15] = {1,2,3,4,5,6,7,8,23,64,128,256,512,1,6};
  g_autoptr(GrfNDArray) array3 = grf_ndarray_read_csv("../data/test_array_read_csv_uint16_1.csv");
  GrfDataType array3_type         = grf_ndarray_get_datatype    (array3);
  uint16_t    array3_dim          = grf_ndarray_get_dim         (array3);
  uint32_t*   array3_size         = grf_ndarray_get_size        (array3);
  uint64_t    array3_num_elements = grf_ndarray_get_num_elements(array3);
  uint16_t*   array3_data_uint16  = grf_ndarray_get_data        (array3);
  assert_non_null(array3);
  assert_int_equal(array3_type, GRF_UINT16);
  assert_int_equal(array3_dim, 1);
  assert_int_equal(array3_size[0],15);
  assert_int_equal(array3_num_elements, 15);
  for(i = 0; i < array3_num_elements;i++)
    assert_int_equal(array3_data_uint16[i], ground_data3[i]);

  // 2D (several lines)
  uint16_t ground_data4[50][8];
  for(i = 0; i < 50; i++)
    for(j = 0; j < 8; j++)
      ground_data4[i][j] = i*8+j;
  g_autoptr(GrfNDArray) array4 = grf_ndarray_read_csv("../data/test_array_read_csv_uint16_2.csv");
  GrfDataType array4_type         = grf_ndarray_get_datatype    (array4);
  uint16_t    array4_dim          = grf_ndarray_get_dim         (array4);
  uint32_t*   array4_size         = grf_ndarray_get_size        (array4);
  uint64_t    array4_num_elements = grf_ndarray_get_num_elements(array4);
  uint16_t*   array4_data_uint16  = grf_ndarray_get_data        (array4);
  assert_non_null(array4);
  assert_int_equal(array4_type, GRF_UINT16);
  assert_int_equal(array4_dim, 2);
  assert_int_equal(array4_size[0],50);
  assert_int_equal(array4_size[1],8);
  assert_int_equal(array4_num_elements, 400);
  for(i = 0; i < 50; i++)
    for(j = 0; j < 8; j++)
      assert_int_equal(array4_data_uint16[i*8+j], ground_data4[i][j]);

  // Float
  float ground_data5[8] = {3.2     , 231.123,453.234,10234.999,
                            34.00001, 1.111,  0.0001, 999.99999};
  g_autoptr(GrfNDArray) array5 = grf_ndarray_read_csv_type("../data/test_array_read_csv_float_1.csv", GRF_FLOAT);
  GrfDataType array5_type         = grf_ndarray_get_datatype    (array5);
  uint16_t    array5_dim          = grf_ndarray_get_dim         (array5);
  uint32_t*   array5_size         = grf_ndarray_get_size        (array5);
  uint64_t    array5_num_elements = grf_ndarray_get_num_elements(array5);
  float*    array5_data_float     = grf_ndarray_get_data        (array5);
  assert_non_null(array5);
  assert_int_equal(array5_type, GRF_FLOAT);
  assert_int_equal(array5_dim, 1);
  assert_int_equal(array5_size[0],8);
  assert_int_equal(array5_num_elements, 8);
  for(i = 0; i < array5_num_elements;i++)
    assert_int_equal(array5_data_float[i], ground_data5[i]);

  // Write CSV
  // UINT8
  // 1D (just one line)
  grf_ndarray_write_csv(array, "test_array_write_csv_uint8_1.csv");
  g_autoptr(GrfNDArray) arrayw = grf_ndarray_read_csv("test_array_write_csv_uint8_1.csv");
  assert_grf_ndarray_equal(arrayw, array);

  // 2D (several lines)
  grf_ndarray_write_csv(array2, "test_array_write_csv_uint8_2.csv");
  g_autoptr(GrfNDArray) arrayw2 = grf_ndarray_read_csv("test_array_write_csv_uint8_2.csv");
  assert_grf_ndarray_equal(arrayw2, array2);

  // UINT16
  // 1D (just one line)
  grf_ndarray_write_csv(array3, "test_array_write_csv_uint16_1.csv");
  g_autoptr(GrfNDArray) arrayw3 = grf_ndarray_read_csv("test_array_write_csv_uint16_1.csv");
  assert_grf_ndarray_equal(arrayw3, array3);

  // 2D (several lines)
  grf_ndarray_write_csv(array4, "test_array_write_csv_uint16_2.csv");
  g_autoptr(GrfNDArray) arrayw4 = grf_ndarray_read_csv("test_array_write_csv_uint16_2.csv");
  assert_grf_ndarray_equal(arrayw4, array4);
}

static void test_grf_ndarray_get_long_double(void** state){
  (void) state;

  // Setup
  uint32_t sizes[3] = {2,4,8};
  g_autoptr(GrfNDArray) array = grf_ndarray_zeros(3,sizes,GRF_UINT8);
  uint32_t indices[3] = {1,3,5};
  grf_ndarray_set_element(array,indices,45);

  // Test
  long double value = grf_ndarray_get_long_double_1D(array, 0);
  assert_int_equal(value, 0);
  value = grf_ndarray_get_long_double_1D(array, 5 + 3*8 + 1*32);
  assert_int_equal(value, 45);
}

static void test_grf_ndarray_conversion(void** state){
  (void) state;
  uint8_t data[8] = {1,2,3,4,5,6,7,8};
  uint32_t size   = 8;
  uint64_t i;
  g_autoptr(GrfNDArray) array    = grf_ndarray_from_data(data,1,&size,GRF_UINT8);

  // To UINT16
  g_autoptr(GrfNDArray) array2   = grf_ndarray_as_type(array, GRF_UINT16);
  uint64_t array2_num_elements = grf_ndarray_get_num_elements(array2);
  assert_non_null(array2);
  assert_int_equal(grf_ndarray_get_datatype(array2), GRF_UINT16);
  for(i = 0; i < array2_num_elements; i++){
    assert_int_equal(grf_ndarray_get_long_double_1D(array2,i),data[i]);
  }
  // To FLOAT
  g_autoptr(GrfNDArray) array3   = grf_ndarray_as_type(array, GRF_FLOAT);
  uint64_t array3_num_elements = grf_ndarray_get_num_elements(array2);
  assert_non_null(array3);
  assert_int_equal(grf_ndarray_get_datatype(array3), GRF_FLOAT);
  for(i = 0; i < array3_num_elements; i++){
    assert_int_equal(grf_ndarray_get_long_double_1D(array3,i),data[i]);
  }
}

static void test_grf_ndarray_squeeze(void** state){
  (void) state;
  uint32_t N1=10,N2=20;
  uint16_t array_dim;
  uint32_t* array_size;
  // Remove all single-dimensional axis
  uint32_t size[4] = {1,N1,N2,1};
  GrfNDArray* array = grf_ndarray_new_with_size(4,size);
  grf_ndarray_squeeze(array);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_int_equal(array_dim, 2);
  assert_int_equal(array_size[0], N1);
  assert_int_equal(array_size[1], N2);
  g_clear_object(&array);

  // Remove specific single-dimensional axis
  array = grf_ndarray_new_with_size(4,size);
  uint16_t axis[1] = {0};
  grf_ndarray_squeeze_axis(array, 1, axis);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_int_equal(array_dim, 3);
  assert_int_equal(array_size[0], N1);
  assert_int_equal(array_size[1], N2);
  assert_int_equal(array_size[2], 1);
  g_clear_object(&array);

  array = grf_ndarray_new_with_size(4,size);
  axis[0] = 3;
  grf_ndarray_squeeze_axis(array, 1, axis);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_int_equal(array_dim, 3);
  assert_int_equal(array_size[0], 1);
  assert_int_equal(array_size[1], N1);
  assert_int_equal(array_size[2], N2);
  g_clear_object(&array);
}

static void test_grf_ndarray_circular_indices(void** state){
  (void)state;
  uint16_t dim;
  float radius;

  // 2D
  dim = 2; radius = 1.0;
  GrfNDArray* array = grf_ndarray_circular_indices(dim, radius);
  uint16_t array_dim  = grf_ndarray_get_dim(array);
  uint32_t* array_size = grf_ndarray_get_size(array);
  assert_non_null(array);
  assert_int_equal(array_dim, 2); // (# Neighbors, dim)
  assert_int_equal(array_size[0], 4);// 4 neighbors
  assert_int_equal(array_size[1], 2);// 2D each neighbor
  g_clear_object(&array);
  radius = 1.5;
  array = grf_ndarray_circular_indices(dim, radius);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_non_null(array);
  assert_int_equal(array_dim, 2); // (# Neighbors, dim)
  assert_int_equal(array_size[0], 8);// 4 neighbors
  assert_int_equal(array_size[1], 2);// 2D each neighbor
  g_clear_object(&array);

  // 3D
  dim = 3; radius = 1.0;
  array = grf_ndarray_circular_indices(dim, radius);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_non_null(array);
  assert_int_equal(array_dim, 2); // (# Neighbors, dim)
  assert_int_equal(array_size[0], 6);// 4 neighbors
  assert_int_equal(array_size[1], 3);// 2D each neighbor
  g_clear_object(&array);
  radius = 1.5;
  array = grf_ndarray_circular_indices(dim, radius);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_non_null(array);
  assert_int_equal(array_dim, 2); // (# Neighbors, dim)
  assert_int_equal(array_size[0], 18);// 18 neighbors
  assert_int_equal(array_size[1], 3);// 2D each neighbor
  g_clear_object(&array);
  radius = 1.8;
  array = grf_ndarray_circular_indices(dim, radius);
  array_dim  = grf_ndarray_get_dim(array);
  array_size = grf_ndarray_get_size(array);
  assert_non_null(array);
  assert_int_equal(array_dim, 2); // (# Neighbors, dim)
  assert_int_equal(array_size[0], 26);// 26 neighbors
  assert_int_equal(array_size[1], 3);// 2D each neighbor
  g_clear_object(&array);
}

static void test_grf_ndarray_norm(void** state){
  (void) state;
  uint32_t size[2] = {10,10};
  g_autoptr(GrfNDArray) array1 = grf_ndarray_ones(2,size,GRF_UINT8);
  g_autoptr(GrfNDArray) array2 = grf_ndarray_zeros(2,size,GRF_UINT8);
  // L1-Norm
  double result = grf_ndarray_norm_difference(array1, array2, GRF_NORM_L1);
  assert_int_equal(result, 100);

  // L2-Norm
  result = grf_ndarray_norm_difference(array1, array2, GRF_NORM_L2);
  assert_int_equal(result, 10);

  // L2-squared-norm
  result = grf_ndarray_norm_difference(array1, array2, GRF_NORM_L2SQR);
  assert_int_equal(result, 100);

  // Inf-Norm
  result = grf_ndarray_norm_difference(array1, array2, GRF_NORM_INF);
  assert_int_equal(result, 1);

}

static void test_grf_ndarray_copy(void** state){
  (void) state;
  uint32_t data[8] = {0,1,2,3,4,5,6,7};
  uint32_t size[3] = {2,2,2};
  g_autoptr(GrfNDArray) array     = grf_ndarray_from_data(data,3,size,GRF_UINT32);
  g_autoptr(GrfNDArray) array2    = grf_ndarray_copy(array);
  assert_grf_ndarray_equal(array, array2);
}

static uint8_t more_than_3(void* data, void* user_data){
  (void) user_data;
  uint32_t* data_32 = (uint32_t*) data;
  return *data_32 > 3;
}

static void test_grf_ndarray_filter(void** state){
  (void) state;
  uint32_t data[8] = {0,1,2,3,4,5,6,7};
  uint32_t size[3] = {2,2,2};
  g_autoptr(GrfNDArray) array     = grf_ndarray_from_data(data,3,size,GRF_UINT32);
  g_autoptr(GrfNDArray) indices;
  g_autoptr(GrfNDArray) values;

  grf_ndarray_filter(array,more_than_3,&indices, &values,NULL);
  uint8_t     indices_bitsize      = grf_ndarray_get_bitsize     (indices);
  gboolean    indices_contiguous   = grf_ndarray_get_contiguous  (indices);
  uint64_t*   indices_data_uint64  = grf_ndarray_get_data        (indices);
  uint16_t    indices_dim          = grf_ndarray_get_dim         (indices);
  uint64_t    indices_num_bytes    = grf_ndarray_get_num_bytes   (indices);
  uint64_t    indices_num_elements = grf_ndarray_get_num_elements(indices);
  gboolean    indices_owns_data    = grf_ndarray_get_owns_data   (indices);
  uint32_t*   indices_size         = grf_ndarray_get_size        (indices);
  uint64_t*   indices_step         = grf_ndarray_get_step        (indices);
  GrfDataType indices_type         = grf_ndarray_get_datatype    (indices);

  uint8_t     values_bitsize      = grf_ndarray_get_bitsize     (values);
  gboolean    values_contiguous   = grf_ndarray_get_contiguous  (values);
  uint32_t*   values_data_uint32  = grf_ndarray_get_data        (values);
  uint16_t    values_dim          = grf_ndarray_get_dim         (values);
  uint64_t    values_num_bytes    = grf_ndarray_get_num_bytes   (values);
  uint64_t    values_num_elements = grf_ndarray_get_num_elements(values);
  gboolean    values_owns_data    = grf_ndarray_get_owns_data   (values);
  uint32_t*   values_size         = grf_ndarray_get_size        (values);
  uint64_t*   values_step         = grf_ndarray_get_step        (values);
  GrfDataType values_type         = grf_ndarray_get_datatype    (values);

  assert_int_equal(indices_bitsize, sizeof(uint64_t));
  assert_int_equal(indices_contiguous, 1);
  assert_int_equal(indices_data_uint64[0], 4);
  assert_int_equal(indices_data_uint64[1], 5);
  assert_int_equal(indices_data_uint64[2], 6);
  assert_int_equal(indices_data_uint64[3], 7);
  assert_int_equal(indices_dim, 1);
  assert_int_equal(indices_num_bytes, 4*sizeof(uint64_t));
  assert_int_equal(indices_num_elements, 4);
  assert_int_equal(indices_owns_data, 1);
  assert_int_equal(indices_size[0], 4);
  assert_int_equal(indices_step[0], 1);
  assert_int_equal(indices_type, GRF_UINT64);

  assert_int_equal(values_bitsize, sizeof(uint32_t));
  assert_int_equal(values_contiguous, 1);
  assert_int_equal(values_data_uint32[0], 4);
  assert_int_equal(values_data_uint32[1], 5);
  assert_int_equal(values_data_uint32[2], 6);
  assert_int_equal(values_data_uint32[3], 7);
  assert_int_equal(values_dim, 1);
  assert_int_equal(values_num_bytes, 4*sizeof(uint32_t));
  assert_int_equal(values_num_elements, 4);
  assert_int_equal(values_owns_data, 1);
  assert_int_equal(values_size[0], 4);
  assert_int_equal(values_step[0], 1);
  assert_int_equal(values_type, GRF_UINT32);

}



int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[24]={
    cmocka_unit_test(test_grf_ndarray_new),
    cmocka_unit_test(test_grf_ndarray_new_1D),
    cmocka_unit_test(test_grf_ndarray_new_2D),
    cmocka_unit_test(test_grf_ndarray_new_3D),
    cmocka_unit_test(test_grf_ndarray_new_4D),
    cmocka_unit_test(test_grf_ndarray_new_1D_type),
    cmocka_unit_test(test_grf_ndarray_new_2D_type),
    cmocka_unit_test(test_grf_ndarray_new_3D_type),
    cmocka_unit_test(test_grf_ndarray_new_4D_type),
    cmocka_unit_test(test_grf_ndarray_copy),
    cmocka_unit_test(test_grf_ndarray_from_data),
    cmocka_unit_test(test_grf_ndarray_zeros),
    cmocka_unit_test(test_grf_ndarray_ones),
    cmocka_unit_test(test_grf_ndarray_sub),
    cmocka_unit_test(test_grf_ndarray_reduce),
    cmocka_unit_test(test_grf_ndarray_ops),
    cmocka_unit_test(test_grf_ndarray_indices_manip),
    cmocka_unit_test(test_grf_ndarray_get_long_double),
    cmocka_unit_test(test_grf_ndarray_io_csv),
    cmocka_unit_test(test_grf_ndarray_conversion),
    cmocka_unit_test(test_grf_ndarray_squeeze),
    cmocka_unit_test(test_grf_ndarray_circular_indices),
    cmocka_unit_test(test_grf_ndarray_norm),
    cmocka_unit_test(test_grf_ndarray_filter),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
