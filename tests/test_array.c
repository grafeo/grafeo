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

static void helper_testing_array(GrfArray* array, uint64_t num_elements, GrfDataType type, uint16_t dim, uint32_t* sizes){
    uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
    assert_non_null(array);
    assert_int_equal(grf_array_get_num_elements(array), num_elements);
    assert_int_equal(grf_array_get_type(array), type);
    assert_int_equal(grf_array_get_dim(array), dim);
    assert_non_null(grf_array_get_size(array));
    assert_non_null(grf_array_get_step(array));
    assert_int_equal(grf_array_get_bitsize(array), bitsizes[type]);
    assert_int_equal(grf_array_get_num_bytes(array), num_elements * bitsizes[type]);
    uint32_t i;
    uint64_t step = 1;
    for(i = 0; i < dim; step*=sizes[dim-(i++)-1]){
        assert_int_equal(grf_array_get_size(array)[i], sizes[i]);
        assert_int_equal(grf_array_get_step(array)[dim-i-1], step);
    }
    if(num_elements)
        assert_non_null(grf_array_get_data(array));
}

/**
 * Testing new arrays
 * TODO: testing also free
 */
static void test_grf_array_new(void** state){
    (void) state;
    GrfArray* array = grf_array_new();
    assert_non_null(array);
    assert_int_equal(grf_array_get_num_elements(array),0);
    assert_int_equal(grf_array_get_type(array),GRF_UINT8);
    assert_int_equal(grf_array_get_dim(array), 0);
    assert_int_equal(grf_array_get_bitsize(array), 0);
    assert_int_equal(grf_array_get_num_bytes(array), 0);
    uint32_t* size = grf_array_get_size(array);
    assert_null(size);
    assert_null(grf_array_get_step(array));
    void* data = grf_array_get_data(array);
    assert_null(data);
    grf_array_free(array);
}

/**
 * Testing 1D array creation
 */
static void test_grf_array_new_1D(void** state){
    (void) state;
    uint32_t i;
    uint32_t i_max = 100;
    // Several sizes
    for(i = 0; i < i_max; i++){
        GrfArray* array = grf_array_new_1D(i);
        helper_testing_array(array, i, GRF_UINT8, 1, &i);
        grf_array_free(array);
    }
}

static void test_grf_array_new_2D(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
    for(i = 0; i < i_max; i++){
        sizes[0] = i;
        for(j = 0; j < j_max; j++){
            sizes[1] = j;
            GrfArray* array = grf_array_new_2D(i, j);
            helper_testing_array(array, i*j, GRF_UINT8, 2, sizes);
            grf_array_free(array);
        }
    }
}

static void test_grf_array_new_3D(void** state){
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
                GrfArray* array = grf_array_new_3D(i, j, k);
                helper_testing_array(array, i*j*k, GRF_UINT8, 3, sizes);
                grf_array_free(array);
            }
        }
    }
}

static void test_grf_array_new_4D(void** state){
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
                    GrfArray* array = grf_array_new_4D(i, j, k, l);
                    helper_testing_array(array, i*j*k*l, GRF_UINT8, 4, sizes);
                    grf_array_free(array);
                }
            }
        }
    }
}

static void test_grf_array_new_1D_type(void ** state){
    (void) state;
    GrfDataType type;
    uint32_t i, i_max=100;
    for(i = 0; i < i_max; i+=2){
        for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
            GrfArray* array = grf_array_new_1D_type(i, type);
            helper_testing_array(array, i, type, 1, &i);
            grf_array_free(array);
        }
    }
}

static void test_grf_array_new_2D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50;
    uint32_t sizes[2];
    GrfDataType type;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                GrfArray* array = grf_array_new_2D_type(i, j, type);
                helper_testing_array(array, i*j, type, 2, sizes);
                grf_array_free(array);
            }
        }
    }
}

static void test_grf_array_new_3D_type(void** state){
    (void) state;
    uint32_t i, i_max = 50,
             j, j_max = 50,
             k, k_max = 50;
    uint32_t sizes[3];
    GrfDataType type;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                    GrfArray* array = grf_array_new_3D_type(i, j, k, type);
                    helper_testing_array(array, i*j*k, type, 3, sizes);
                    grf_array_free(array);
                }
            }
        }
    }
}

static void test_grf_array_new_4D_type(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint32_t sizes[4];
    GrfDataType type;
    for(i = 0; i < i_max; i+=2){
        sizes[0] = i;
        for(j = 0; j < j_max; j+=2){
            sizes[1] = j;
            for(k = 0; k < k_max; k+=2){
                sizes[2] = k;
                for(l = 0; l < l_max; l+=2){
                    sizes[3] = l;
                    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
                        GrfArray* array = grf_array_new_4D_type(i, j, k, l, type);
                        helper_testing_array(array, i*j*k*l, type, 4, sizes);
                        grf_array_free(array);
                    }
                }
            }
        }
    }
}

static void helper_test_grf_array_from_data(GrfArray* array, void* dados, uint8_t dim, uint64_t num_elements, uint64_t num_bytes, uint8_t bitsize, uint64_t* step, uint32_t* size, GrfDataType type){
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
      case GRF_UINT8:  assert_int_equal(array->data_uint8[i],  ((uint8_t*)dados)[i]);break;
      case GRF_UINT16: assert_int_equal(array->data_uint16[i], ((uint16_t*)dados)[i]);break;
      case GRF_UINT32: assert_int_equal(array->data_uint32[i], ((uint32_t*)dados)[i]);break;
      case GRF_UINT64: assert_int_equal(array->data_uint64[i], ((uint64_t*)dados)[i]);break;
      case GRF_INT8:   assert_int_equal(array->data_int8[i],   ((int8_t*)dados)[i]);break;
      case GRF_INT16:  assert_int_equal(array->data_int16[i],  ((int16_t*)dados)[i]);break;
      case GRF_INT32:  assert_int_equal(array->data_int32[i],  ((int32_t*)dados)[i]);break;
      case GRF_INT64:  assert_int_equal(array->data_int64[i],  ((int64_t*)dados)[i]);break;
      case GRF_FLOAT:  assert_int_equal(array->data_float[i],  ((float*)dados)[i]);break;
      case GRF_DOUBLE: assert_int_equal(array->data_double[i], ((double*)dados)[i]);break;
    }
  }
}

static void test_grf_array_from_data(void** state){
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
  GrfArray* array;

  uint8_t i;
  GrfDataType type;
  uint8_t bitsizes[10] = {1,2,4,8,1,2,4,8,4,8};
  for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
    for(i = 0; i < 4; i++){
      array = grf_array_from_data(dados,i+1,sizes[i],type);
      helper_test_grf_array_from_data(array, dados, i+1, 16, 16*bitsizes[type], bitsizes[type], steps[i],sizes[i],type);
      grf_array_free(array);
    }
  }

  free(sizes);
  free(steps);
}

static void test_grf_array_zeros(void** state){
    (void) state;
    uint32_t i, i_max = 25,
             j, j_max = 25,
             k, k_max = 25,
             l, l_max = 25;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    GrfDataType type;
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
                        GrfArray* array = grf_array_zeros(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRF_UINT8:  assert_int_equal(array->data_uint8[n],  0);break;
                                case GRF_UINT16: assert_int_equal(array->data_uint16[n], 0);break;
                                case GRF_UINT32: assert_int_equal(array->data_uint32[n], 0);break;
                                case GRF_UINT64: assert_int_equal(array->data_uint64[n], 0);break;
                                case GRF_INT8:   assert_int_equal(array->data_int8[n],   0);break;
                                case GRF_INT16:  assert_int_equal(array->data_int16[n],  0);break;
                                case GRF_INT32:  assert_int_equal(array->data_int32[n],  0);break;
                                case GRF_INT64:  assert_int_equal(array->data_int64[n],  0);break;
                                case GRF_FLOAT:  assert_int_equal(array->data_float[n],  0);break;
                                case GRF_DOUBLE: assert_int_equal(array->data_double[n], 0);break;
                            }
                        }

                        GrfArray* grf_array_like = grf_array_zeros_like(array);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRF_UINT8:  assert_int_equal(grf_array_like->data_uint8[n],  0);break;
                                case GRF_UINT16: assert_int_equal(grf_array_like->data_uint16[n], 0);break;
                                case GRF_UINT32: assert_int_equal(grf_array_like->data_uint32[n], 0);break;
                                case GRF_UINT64: assert_int_equal(grf_array_like->data_uint64[n], 0);break;
                                case GRF_INT8:   assert_int_equal(grf_array_like->data_int8[n],   0);break;
                                case GRF_INT16:  assert_int_equal(grf_array_like->data_int16[n],  0);break;
                                case GRF_INT32:  assert_int_equal(grf_array_like->data_int32[n],  0);break;
                                case GRF_INT64:  assert_int_equal(grf_array_like->data_int64[n],  0);break;
                                case GRF_FLOAT:  assert_int_equal(grf_array_like->data_float[n],  0);break;
                                case GRF_DOUBLE: assert_int_equal(grf_array_like->data_double[n], 0);break;
                            }
                        }
                        helper_testing_array(grf_array_like, num_elements, type, 4, sizes);

                        grf_array_free(grf_array_like);
                        GrfDataType type2 = GRF_DOUBLE-type;
                        GrfArray* array2 = grf_array_zeros_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type2){
                                case GRF_UINT8:  assert_int_equal(array2->data_uint8[n],  0);break;
                                case GRF_UINT16: assert_int_equal(array2->data_uint16[n], 0);break;
                                case GRF_UINT32: assert_int_equal(array2->data_uint32[n], 0);break;
                                case GRF_UINT64: assert_int_equal(array2->data_uint64[n], 0);break;
                                case GRF_INT8:   assert_int_equal(array2->data_int8[n],   0);break;
                                case GRF_INT16:  assert_int_equal(array2->data_int16[n],  0);break;
                                case GRF_INT32:  assert_int_equal(array2->data_int32[n],  0);break;
                                case GRF_INT64:  assert_int_equal(array2->data_int64[n],  0);break;
                                case GRF_FLOAT:  assert_int_equal(array2->data_float[n],  0);break;
                                case GRF_DOUBLE: assert_int_equal(array2->data_double[n], 0);break;
                            }
                        }

                        grf_array_free(array);
                        grf_array_free(array2);
                    }
                }
            }
        }
    }
}

static void test_grf_array_ones(void** state){
    (void) state;
    uint32_t i, i_max = 15,
             j, j_max = 15,
             k, k_max = 15,
             l, l_max = 15;
    uint64_t  n;
    uint64_t num_elements;
    uint32_t sizes[4];
    GrfDataType type;
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
                        GrfArray* array = grf_array_ones(4, sizes, type);
                        helper_testing_array(array, num_elements, type, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRF_UINT8:  assert_int_equal(array->data_uint8[n],  1);break;
                                case GRF_UINT16: assert_int_equal(array->data_uint16[n], 1);break;
                                case GRF_UINT32: assert_int_equal(array->data_uint32[n], 1);break;
                                case GRF_UINT64: assert_int_equal(array->data_uint64[n], 1);break;
                                case GRF_INT8:   assert_int_equal(array->data_int8[n],   1);break;
                                case GRF_INT16:  assert_int_equal(array->data_int16[n],  1);break;
                                case GRF_INT32:  assert_int_equal(array->data_int32[n],  1);break;
                                case GRF_INT64:  assert_int_equal(array->data_int64[n],  1);break;
                                case GRF_FLOAT:  assert_true(array->data_float[n] == 1.0);break;
                                case GRF_DOUBLE: assert_true(array->data_double[n] == 1.0);break;
                            }
                        }

                        GrfArray* grf_array_like = grf_array_ones_like(array);
                        helper_testing_array(grf_array_like, num_elements, type, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type){
                                case GRF_UINT8:  assert_int_equal(grf_array_like->data_uint8[n],  1);break;
                                case GRF_UINT16: assert_int_equal(grf_array_like->data_uint16[n], 1);break;
                                case GRF_UINT32: assert_int_equal(grf_array_like->data_uint32[n], 1);break;
                                case GRF_UINT64: assert_int_equal(grf_array_like->data_uint64[n], 1);break;
                                case GRF_INT8:   assert_int_equal(grf_array_like->data_int8[n],   1);break;
                                case GRF_INT16:  assert_int_equal(grf_array_like->data_int16[n],  1);break;
                                case GRF_INT32:  assert_int_equal(grf_array_like->data_int32[n],  1);break;
                                case GRF_INT64:  assert_int_equal(grf_array_like->data_int64[n],  1);break;
                                case GRF_FLOAT:  assert_true(grf_array_like->data_float[n] == 1.0);break;
                                case GRF_DOUBLE: assert_true(grf_array_like->data_double[n] == 1.0);break;
                            }
                        }

                        grf_array_free(grf_array_like);
                        GrfDataType type2 = GRF_DOUBLE - type;
                        GrfArray* array2 = grf_array_ones_like_type(array, type2);
                        helper_testing_array(array2, num_elements, type2, 4, sizes);
                        for(n = num_elements-1; n < num_elements; n++){
                            switch(type2){
                                case GRF_UINT8:  assert_int_equal(array2->data_uint8[n],  1);break;
                                case GRF_UINT16: assert_int_equal(array2->data_uint16[n], 1);break;
                                case GRF_UINT32: assert_int_equal(array2->data_uint32[n], 1);break;
                                case GRF_UINT64: assert_int_equal(array2->data_uint64[n], 1);break;
                                case GRF_INT8:   assert_int_equal(array2->data_int8[n],   1);break;
                                case GRF_INT16:  assert_int_equal(array2->data_int16[n],  1);break;
                                case GRF_INT32:  assert_int_equal(array2->data_int32[n],  1);break;
                                case GRF_INT64:  assert_int_equal(array2->data_int64[n],  1);break;
                                case GRF_FLOAT:  assert_true(array2->data_float[n] == 1.0);break;
                                case GRF_DOUBLE: assert_true(array2->data_double[n] == 1.0);break;
                            }
                        }
                        grf_array_free(array);
                        grf_array_free(array2);
                    }
                }
            }
        }
    }
}

static void helper_testing_grf_array_sub(GrfArray* array, GrfArray* subarray, uint32_t* subsizes, double value, GrfRange* ranges){
    assert_int_equal(grf_array_get_dim(subarray), 4);
    assert_int_equal(grf_array_get_type(subarray), grf_array_get_type(array));
    uint32_t* sizes = grf_array_get_size(subarray);
    uint64_t* step = grf_array_get_step(subarray);
    assert_non_null(sizes);
    assert_non_null(step);
    uint64_t stepAtual = 1,i;
    for(i = 0; i < grf_array_get_dim(array); stepAtual*= grf_array_get_size(array)[3-(i++)]){
        assert_int_equal(sizes[i], subsizes[i]);
        assert_int_equal(step[3-i], stepAtual);
    }
    grf_array_fill(subarray, value);

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
          case GRF_UINT8: value1 = (double)(*(uint8_t*)grf_array_get_element(array, indices));break;
          case GRF_UINT16: value1 = (double)(*(uint16_t*)grf_array_get_element(array, indices));break;
          case GRF_UINT32: value1 = (double)(*(uint32_t*)grf_array_get_element(array, indices));break;
          case GRF_UINT64: value1 = (double)(*(uint64_t*)grf_array_get_element(array, indices));break;
          case GRF_INT8: value1 = (double)(*(int8_t*)grf_array_get_element(array, indices));break;
          case GRF_INT16: value1 = (double)(*(int16_t*)grf_array_get_element(array, indices));break;
          case GRF_INT32: value1 = (double)(*(int32_t*)grf_array_get_element(array, indices));break;
          case GRF_INT64: value1 = (double)(*(int64_t*)grf_array_get_element(array, indices));break;
          case GRF_FLOAT: value1 = (double)(*(float*)grf_array_get_element(array, indices));break;
          case GRF_DOUBLE: value1 = *(double*)grf_array_get_element(array, indices);break;
        }
        assert_int_equal(value1, value);
    }}}}

}

static void test_grf_array_sub(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {4,4,4,4};
    GrfArray* array = grf_array_new_with_size_type(dim, sizes, GRF_INT32);
    grf_array_fill(array, 10);

    // All ranges defined
    GrfRange ranges[4];
    grf_range_from_to(&ranges[0],1,3);
    grf_range_from_to(&ranges[1],1,3);
    grf_range_from_to(&ranges[2],1,3);
    grf_range_from_to(&ranges[3],1,3);

    GrfArray* subarray = grf_array_sub(array, ranges);
    uint32_t subsizes[4] = {2,2,2,2};
    helper_testing_grf_array_sub(array, subarray, subsizes, 5, ranges);
    grf_array_free(subarray);
    assert_non_null(array);

    // A range with only the starting index
    // TODO: this will replace (we need to free 
    //       old ranges[1] memory)
    uint32_t starting = 1;
    grf_range_from(&ranges[1], starting);
    subarray = grf_array_sub(array, ranges);
    subsizes[1] = 3;
    helper_testing_grf_array_sub(array, subarray, subsizes, 6, ranges);
    grf_array_free(subarray);
    assert_non_null(array);
    
    // A range with only the ending index
    uint32_t ending = 3;
    grf_range_to(&ranges[2],ending);
    subarray = grf_array_sub(array, ranges);
    subsizes[2] = 3;
    helper_testing_grf_array_sub(array, subarray, subsizes, 7, ranges);
    grf_array_free(subarray);
    assert_non_null(array);

    // A range which is the whole range of a dimension
    grf_range_all(&ranges[3]);
    subsizes[3] = 4;
    subarray = grf_array_sub(array, ranges);
    helper_testing_grf_array_sub(array, subarray, subsizes, 8, ranges);
    grf_array_free(subarray);
    assert_non_null(array);


}

static void test_grf_array_reduce(void** state){
    (void) state;
    uint16_t dim = 4;
    uint32_t sizes[4] = {10,10,10,10};
    GrfArray* array, *result;
    long double result_1D;
    GrfDataType type;

    for(type = GRF_UINT8; type <= GRF_DOUBLE; type++){
      array  = grf_array_new_with_size_type(dim, sizes, type);
      grf_array_fill(array,10);
      // Agregated sum
      int16_t eixos[2] = {1,2};
      result = grf_array_reduce_sum(array, eixos, 2);
      assert_int_equal(grf_array_get_dim(result), 2);
      result_1D = grf_array_reduce_sum_num(array);
      assert_int_equal(result_1D, 100000);
      grf_array_free(result);
      // Agregated product
      result = grf_array_reduce_mult(array, eixos, 2);
      assert_int_equal(grf_array_get_dim(result), 2);
      grf_array_free(result);
      // Agregated standard deviation
      result = grf_array_reduce_std(array, eixos, 2);
      assert_int_equal(grf_array_get_dim(result), 2);
      grf_array_free(result);
      // Agregated maximum
      result = grf_array_reduce_max(array, eixos, 2);
      assert_int_equal(grf_array_get_dim(result), 2);
      result_1D = grf_array_reduce_max_num(array);
      assert_int_equal(result_1D, 10);
      grf_array_free(result);
      // Agregated minimum
      result = grf_array_reduce_min(array, eixos, 2);
      assert_int_equal(grf_array_get_dim(result), 2);
      result_1D = grf_array_reduce_min_num(array);
      assert_int_equal(result_1D, 10);
      grf_array_free(result);
    }

}

static void test_grf_array_ops(){
  uint64_t i;
  GrfArray* array = grf_array_new_2D(5,4);
  GrfArray* array2= grf_array_new_2D(5,4);

  grf_array_fill(array, 5);
  grf_array_fill(array2, 7);

  // Adding two arrays to a new one
  GrfArray* result = grf_array_sum(array, array2);
  for(i = 0; i < result->num_elements; i++)
    assert_int_equal(result->data_uint8[i],12);

  // Adding a scalar inside an array
  GrfArray* result1 = grf_array_sum_scalar(array, 7);
  for(i = 0; i < result1->num_elements; i++)
    assert_int_equal(result1->data_uint8[i],12);

  // Subtracting two arrays
  GrfArray* result2 = grf_array_subtract(result, array2);
  for(i = 0; i < result2->num_elements; i++)
    assert_int_equal(result2->data_uint8[i],5);

  // Subtracting a scalar from an array
  GrfArray* result3 = grf_array_subtract_scalar(result, 7);
  for(i = 0; i < result3->num_elements; i++)
    assert_int_equal(result3->data_uint8[i],5);

  // Multiply
  GrfArray* result4 = grf_array_mult(array, array2);
  for(i = 0; i < result4->num_elements; i++)
    assert_int_equal(result4->data_uint8[i],35);

  // Multiply by a scalar
  GrfArray* result5 = grf_array_mult_scalar(array, 7);
  for(i = 0; i < result5->num_elements; i++)
    assert_int_equal(result5->data_uint8[i],35);

  // Divide
  GrfArray* result6 = grf_array_divide(result5, array2);
  for(i = 0; i < result6->num_elements; i++)
    assert_int_equal(result6->data_uint8[i],5);

  // Divide by a scalar
  GrfArray* result7 = grf_array_divide_scalar(result5, 7);
  for(i = 0; i < result7->num_elements; i++)
    assert_int_equal(result7->data_uint8[i],5);

  grf_array_free(result);
  grf_array_free(result1);
  grf_array_free(result2);
  grf_array_free(result3);
  grf_array_free(result4);
  grf_array_free(result5);
  grf_array_free(result6);
  grf_array_free(result7);
}

static void test_grf_array_indices_manip(void** state){
  (void) state;
  // Create an array
  uint32_t sizes[2] = {2,2};
  GrfArray*  array    = grf_array_zeros(2,sizes,GRF_UINT8);

  // Convert an valid ND index to 1D
  int32_t indices1[2] = {0,1};
  int64_t resp = grf_array_index_1D(array, indices1);
  assert_int_equal(resp, 1);

  // Convert an invalid ND index to 1D
  int32_t indices2[2] = {2,0};
  resp = grf_array_index_1D(array, indices2);
  assert_int_equal(resp, 4);

  int32_t indices3[2] = {-2,0};
  resp = grf_array_index_1D(array, indices3);
  assert_int_equal(resp, -4);

  // Convert an valid 1D index to ND
  int32_t* respND = grf_array_index(array, 2);
  assert_int_equal(respND[0], 1);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Convert an invalid 1D index to ND
  grf_array_index(array, 8);
  assert_int_equal(respND[0], 4);
  assert_int_equal(respND[1], 0);
  free(respND);

  // Compare valid index to result of validation
  resp = grf_array_index_1D_is_valid(array, 2);
  assert_int_equal(resp, 1);

  // Compare invalid index to result of validation
  resp = grf_array_index_1D_is_valid(array, -5);
  assert_int_equal(resp, 0);
  resp = grf_array_index_1D_is_valid(array, 8);
  assert_int_equal(resp, 0);

  // ND Indices are in valid range?
  assert_true (grf_array_index_is_valid(array, indices1));
  assert_false(grf_array_index_is_valid(array, indices2));
  assert_false(grf_array_index_is_valid(array, indices3));

  // Modifying value at specified index
  uint32_t indices1u[2] = {(uint32_t)indices1[0],(uint32_t)indices1[1]};
  grf_array_set_element(array, indices1u, 3);
  uint8_t values[4] = {0,3,0,0};
  uint8_t i;
  for(i = 0; i < 4; i++) assert_int_equal(array->data_uint8[i], values[i]);
  grf_array_free(array);
}

static void assert_grf_array_equal(GrfArray* array1, GrfArray* array2){
  uint64_t i;
  assert_non_null(array1);
  assert_non_null(array2);
  assert_int_equal(array1->type, array2->type);
  assert_int_equal(array1->dim, array2->dim);
  assert_int_equal(array1->num_elements, array1->num_elements);
  for(i = 0; i < array1->dim; i++)
    assert_int_equal(array1->size[i],array2->size[i]);
  for(i = 0; i < array1->num_elements; i++)
    assert_int_equal(grf_array_get_long_double_1D(array1,i),
                     grf_array_get_long_double_1D(array2,i));
}

static void test_grf_array_io_csv(void** state){
  (void) state;
  uint64_t i,j;

  // Loading CSV
  // UINT8
  // 1D (just one line)
  uint8_t ground_data1[8] = {1,2,3,4,5,6,7,8};
  GrfArray* array = grf_array_read_csv("../data/test_array_read_csv_uint8_1.csv");
  assert_non_null(array);
  assert_int_equal(array->type, GRF_UINT8);
  assert_int_equal(array->dim, 1);
  assert_int_equal(array->size[0],8);
  assert_int_equal(array->num_elements, 8);
  for(i = 0; i < array->num_elements;i++)
    assert_int_equal(array->data_uint8[i], ground_data1[i]);

  // 2D (several lines)
  uint8_t ground_data2[10][8];
  for(i = 0; i < 10; i++)
    for(j = 0; j < 8; j++)
      ground_data2[i][j] = i*8+j;
  GrfArray* array2 = grf_array_read_csv("../data/test_array_read_csv_uint8_2.csv");
  assert_non_null(array2);
  assert_int_equal(array2->type, GRF_UINT8);
  assert_int_equal(array2->dim, 2);
  assert_int_equal(array2->size[0],10);
  assert_int_equal(array2->size[1],8);
  assert_int_equal(array2->num_elements, 80);
  for(i = 0; i < 10; i++)
    for(j = 0; j < 8; j++)
      assert_int_equal(array2->data_uint8[i*8+j], ground_data2[i][j]);

  // UINT16
  // 1D (just one line)
  uint16_t ground_data3[15] = {1,2,3,4,5,6,7,8,23,64,128,256,512,1,6};
  GrfArray* array3 = grf_array_read_csv("../data/test_array_read_csv_uint16_1.csv");
  assert_non_null(array3);
  assert_int_equal(array3->type, GRF_UINT16);
  assert_int_equal(array3->dim, 1);
  assert_int_equal(array3->size[0],15);
  assert_int_equal(array3->num_elements, 15);
  for(i = 0; i < array3->num_elements;i++)
    assert_int_equal(array3->data_uint16[i], ground_data3[i]);

  // 2D (several lines)
  uint16_t ground_data4[50][8];
  for(i = 0; i < 50; i++)
    for(j = 0; j < 8; j++)
      ground_data4[i][j] = i*8+j;
  GrfArray* array4 = grf_array_read_csv("../data/test_array_read_csv_uint16_2.csv");
  assert_non_null(array4);
  assert_int_equal(array4->type, GRF_UINT16);
  assert_int_equal(array4->dim, 2);
  assert_int_equal(array4->size[0],50);
  assert_int_equal(array4->size[1],8);
  assert_int_equal(array4->num_elements, 400);
  for(i = 0; i < 50; i++)
    for(j = 0; j < 8; j++)
      assert_int_equal(array4->data_uint16[i*8+j], ground_data4[i][j]);

  // Float
  float ground_data5[8] = {3.2     , 231.123,453.234,10234.999,
                            34.00001, 1.111,  0.0001, 999.99999};
  GrfArray* array5 = grf_array_read_csv_type("../data/test_array_read_csv_float_1.csv", GRF_FLOAT);
  assert_non_null(array5);
  assert_int_equal(array5->type, GRF_FLOAT);
  assert_int_equal(array5->dim, 1);
  assert_int_equal(array5->size[0],8);
  assert_int_equal(array5->num_elements, 8);
  for(i = 0; i < array5->num_elements;i++)
    assert_int_equal(array5->data_float[i], ground_data5[i]);

  // Write CSV
  // UINT8
  // 1D (just one line)
  grf_array_write_csv(array, "test_array_write_csv_uint8_1.csv");
  GrfArray* arrayw = grf_array_read_csv("test_array_write_csv_uint8_1.csv");
  assert_grf_array_equal(arrayw, array);

  // 2D (several lines)
  grf_array_write_csv(array2, "test_array_write_csv_uint8_2.csv");
  GrfArray* arrayw2 = grf_array_read_csv("test_array_write_csv_uint8_2.csv");
  assert_grf_array_equal(arrayw2, array2);

  // UINT16
  // 1D (just one line)
  grf_array_write_csv(array3, "test_array_write_csv_uint16_1.csv");
  GrfArray* arrayw3 = grf_array_read_csv("test_array_write_csv_uint16_1.csv");
  assert_grf_array_equal(arrayw3, array3);

  // 2D (several lines)
  grf_array_write_csv(array4, "test_array_write_csv_uint16_2.csv");
  GrfArray* arrayw4 = grf_array_read_csv("test_array_write_csv_uint16_2.csv");
  assert_grf_array_equal(arrayw4, array4);

  grf_array_free(array);
  grf_array_free(array2);
  grf_array_free(array3);
  grf_array_free(array4);
  grf_array_free(array5);
  grf_array_free(arrayw);
  grf_array_free(arrayw2);
  grf_array_free(arrayw3);
  grf_array_free(arrayw4);
}

static void test_grf_array_get_long_double(void** state){
  (void) state;

  // Setup
  uint32_t sizes[3] = {2,4,8};
  GrfArray* array = grf_array_zeros(3,sizes,GRF_UINT8);
  uint32_t indices[3] = {1,3,5};
  grf_array_set_element(array,indices,45);

  // Test
  long double value = grf_array_get_long_double_1D(array, 0);
  assert_int_equal(value, 0);
  value = grf_array_get_long_double_1D(array, 5 + 3*8 + 1*32);
  assert_int_equal(value, 45);

  grf_array_free(array);
}

static void test_grf_array_conversion(void** state){
  (void) state;
  uint8_t data[8] = {1,2,3,4,5,6,7,8};
  uint32_t size   = 8;
  uint64_t i;
  GrfArray* array    = grf_array_from_data(data,1,&size,GRF_UINT8);

  // To UINT16
  GrfArray* array2   = grf_array_as_type(array, GRF_UINT16);
  assert_non_null(array2);
  assert_int_equal(array2->type, GRF_UINT16);
  for(i = 0; i < array2->num_elements; i++){
    assert_int_equal(grf_array_get_long_double_1D(array2,i),data[i]);
  }
  // To FLOAT
  GrfArray* array3   = grf_array_as_type(array, GRF_FLOAT);
  assert_non_null(array3);
  assert_int_equal(array3->type, GRF_FLOAT);
  for(i = 0; i < array2->num_elements; i++){
    assert_int_equal(grf_array_get_long_double_1D(array3,i),data[i]);
  }
}

static void test_grf_array_squeeze(void** state){
  (void) state;
  uint32_t N1=10,N2=20;
  // Remove all single-dimensional axis
  uint32_t size[4] = {1,N1,N2,1};
  GrfArray* array = grf_array_new_with_size(4,size);
  grf_array_squeeze(array);
  assert_int_equal(array->dim, 2);
  assert_int_equal(array->size[0], N1);
  assert_int_equal(array->size[1], N2);
  grf_array_free(array);

  // Remove specific single-dimensional axis
  array = grf_array_new_with_size(4,size);
  uint16_t axis[1] = {0};
  grf_array_squeeze_axis(array, 1, axis);
  assert_int_equal(array->dim, 3);
  assert_int_equal(array->size[0], N1);
  assert_int_equal(array->size[1], N2);
  assert_int_equal(array->size[2], 1);
  grf_array_free(array);

  array = grf_array_new_with_size(4,size);
  axis[0] = 3;
  grf_array_squeeze_axis(array, 1, axis);
  assert_int_equal(array->dim, 3);
  assert_int_equal(array->size[0], 1);
  assert_int_equal(array->size[1], N1);
  assert_int_equal(array->size[2], N2);
  grf_array_free(array);
}

static void test_grf_array_circular_indices(void** state){
  (void)state;
  uint16_t dim;
  float radius;

  // 2D
  dim = 2; radius = 1.0;
  GrfArray* array = grf_array_circular_indices(dim, radius);
  assert_non_null(array);
  assert_int_equal(array->dim, 2); // (# Neighbors, dim)
  assert_int_equal(array->size[0], 4);// 4 neighbors
  assert_int_equal(array->size[1], 2);// 2D each neighbor
  grf_array_free(array);
  radius = 1.5;
  array = grf_array_circular_indices(dim, radius);
  assert_non_null(array);
  assert_int_equal(array->dim, 2); // (# Neighbors, dim)
  assert_int_equal(array->size[0], 8);// 4 neighbors
  assert_int_equal(array->size[1], 2);// 2D each neighbor
  grf_array_free(array);

  // 3D
  dim = 3; radius = 1.0;
  array = grf_array_circular_indices(dim, radius);
  assert_non_null(array);
  assert_int_equal(array->dim, 2); // (# Neighbors, dim)
  assert_int_equal(array->size[0], 6);// 4 neighbors
  assert_int_equal(array->size[1], 3);// 2D each neighbor
  grf_array_free(array);
  radius = 1.5;
  array = grf_array_circular_indices(dim, radius);
  assert_non_null(array);
  assert_int_equal(array->dim, 2); // (# Neighbors, dim)
  assert_int_equal(array->size[0], 18);// 18 neighbors
  assert_int_equal(array->size[1], 3);// 2D each neighbor
  grf_array_free(array);
  radius = 1.8;
  array = grf_array_circular_indices(dim, radius);
  assert_non_null(array);
  assert_int_equal(array->dim, 2); // (# Neighbors, dim)
  assert_int_equal(array->size[0], 26);// 26 neighbors
  assert_int_equal(array->size[1], 3);// 2D each neighbor
  grf_array_free(array);
}

static void test_grf_array_norm(void** state){
  (void) state;
  uint32_t size[2] = {10,10};
  GrfArray* array1 = grf_array_ones(2,size,GRF_UINT8);
  GrfArray* array2 = grf_array_zeros(2,size,GRF_UINT8);
  // L1-Norm
  double result = grf_array_norm_difference(array1, array2, GRF_NORM_L1);
  assert_int_equal(result, 100);

  // L2-Norm
  result = grf_array_norm_difference(array1, array2, GRF_NORM_L2);
  assert_int_equal(result, 10);

  // L2-squared-norm
  result = grf_array_norm_difference(array1, array2, GRF_NORM_L2SQR);
  assert_int_equal(result, 100);

  // Inf-Norm
  result = grf_array_norm_difference(array1, array2, GRF_NORM_INF);
  assert_int_equal(result, 1);

  grf_array_free(array1);
  grf_array_free(array2);
}

static void test_grf_array_copy(void** state){
  (void) state;
  uint32_t data[8] = {0,1,2,3,4,5,6,7};
  uint32_t size[3] = {2,2,2};
  GrfArray* array     = grf_array_from_data(data,3,size,GRF_UINT32);
  GrfArray* array2    = grf_array_copy(array);
  assert_grf_array_equal(array, array2);
  grf_array_free(array);
  grf_array_free(array2);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[22]={
    cmocka_unit_test(test_grf_array_new),
    cmocka_unit_test(test_grf_array_new_1D),
    cmocka_unit_test(test_grf_array_new_2D),
    cmocka_unit_test(test_grf_array_new_3D),
    cmocka_unit_test(test_grf_array_new_4D),
    cmocka_unit_test(test_grf_array_new_1D_type),
    cmocka_unit_test(test_grf_array_new_2D_type),
    cmocka_unit_test(test_grf_array_new_3D_type),
    cmocka_unit_test(test_grf_array_new_4D_type),
    cmocka_unit_test(test_grf_array_copy),
    cmocka_unit_test(test_grf_array_from_data),
    cmocka_unit_test(test_grf_array_zeros),
    cmocka_unit_test(test_grf_array_ones),
    cmocka_unit_test(test_grf_array_sub),
    cmocka_unit_test(test_grf_array_reduce),
    cmocka_unit_test(test_grf_array_ops),
    cmocka_unit_test(test_grf_array_indices_manip),
    cmocka_unit_test(test_grf_array_get_long_double),
    cmocka_unit_test(test_grf_array_io_csv),
    cmocka_unit_test(test_grf_array_conversion),
    cmocka_unit_test(test_grf_array_squeeze),
    cmocka_unit_test(test_grf_array_circular_indices),
    cmocka_unit_test(test_grf_array_norm),    
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
