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
#include <grafeo/array.h>
#include <limits.h>
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    array->num_bytes = 0;
    array->bitsize   = 0;
    array->type = GRAFEO_UINT8;
    array->step = NULL;
    return array;
}

Array*    array_new_with_dim(uint16_t dim){
    Array* array        = array_new();
    array->dim          = dim;
    array->size         = malloc(sizeof(uint32_t) * dim);
    array->step         = malloc(sizeof(uint64_t) * dim);
    return array;
}
Array*    array_new_with_size(uint16_t dim, uint32_t* size){
    return array_new_with_size_type(dim, size, GRAFEO_UINT8);
}

Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type){
    Array* array        = array_new_with_dim(dim);
    uint16_t i;
    array->num_elements = 1;
    array->step[dim-1] = 1;
    uint64_t step = 1;
    for(i = 0; i < dim; step*=size[dim-(i++)-1]){
        array->size[i]        = size[i];
        array->step[dim-i-1]  = step;
        array->num_elements  *= size[i];
    }
    switch(type){
        case GRAFEO_UINT8:  array->bitsize = sizeof(uint8_t); break;
        case GRAFEO_UINT16: array->bitsize = sizeof(uint16_t);break;
        case GRAFEO_UINT32: array->bitsize = sizeof(uint32_t);break;
        case GRAFEO_UINT64: array->bitsize = sizeof(uint64_t);break;
        case GRAFEO_INT8:   array->bitsize = sizeof(int8_t);  break;
        case GRAFEO_INT16:  array->bitsize = sizeof(int16_t); break;
        case GRAFEO_INT32:  array->bitsize = sizeof(int32_t); break;
        case GRAFEO_INT64:  array->bitsize = sizeof(int64_t); break;
        case GRAFEO_FLOAT:  array->bitsize = sizeof(float);   break;
        case GRAFEO_DOUBLE: array->bitsize = sizeof(double);  break;
    }
    array->num_bytes    = array->bitsize * array->num_elements;
    array->data         = malloc(array->num_bytes);
    array->type         = type;
    return array;
}

Array*    array_new_1D(uint32_t size1){
    Array* array        = array_new_with_size(1, &size1);
    return array;
}
Array*    array_new_2D(uint32_t size1, uint32_t size2){
    uint32_t sizes[2]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    Array* array        = array_new_with_size(2, sizes);
    return array;
}
Array*    array_new_3D(uint32_t size1, uint32_t size2, uint32_t size3){
    uint32_t sizes[3]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    Array* array        = array_new_with_size(3, sizes);
    return array;
}
Array*    array_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    Array* array        = array_new_with_size(4, sizes);
    return array;
}

Array*    array_new_1D_type(uint32_t size1, DataType type){
    return array_new_with_size_type(1, &size1, type);
}
Array*    array_new_2D_type(uint32_t size1, uint32_t size2, DataType type){
    uint32_t sizes[2]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    return array_new_with_size_type(2, sizes, type);

}
Array*    array_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, DataType type){
    uint32_t sizes[3]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    return array_new_with_size_type(3, sizes, type);
}
Array*    array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, DataType type){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    return array_new_with_size_type(4, sizes, type);
}
Array*    array_zeros(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    memset(array->data, 0, array->num_bytes);
    return array;
}
Array*    array_ones(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    array_fill(array,1);
    return array;
}
void array_fill_max(Array *array){
  double values[10] = {__UINT8_MAX__,__UINT16_MAX__,__UINT32_MAX__,__UINT64_MAX__,__INT8_MAX__,__INT16_MAX__,__INT32_MAX__,__INT64_MAX__,__FLT_MAX__,__DBL_MAX__};
  array_fill(array, values[array->type]);
}
void array_fill_min(Array *array){
  double values[10] = {0,0,0,0,-__INT8_MAX__-1,-__INT16_MAX__-1,-__INT32_MAX__-1,-__INT64_MAX__-1,__FLT_MIN__,__DBL_MIN__};
  array_fill(array, values[array->type]);
}

void array_fill(Array* array, double value){
    uint64_t i;
    for(i = 0; i < array->num_elements; i++){
        switch(array->type){
            case GRAFEO_UINT8:  array->data_uint8[i] = (uint8_t)value;break;
            case GRAFEO_UINT16: array->data_uint16[i] = (uint16_t)value;break;
            case GRAFEO_UINT32: array->data_uint32[i] = (uint32_t)value;break;
            case GRAFEO_UINT64: array->data_uint64[i] = (uint64_t)value;break;
            case GRAFEO_INT8:   array->data_int8[i] = (int8_t)value;break;
            case GRAFEO_INT16:  array->data_int16[i] = (int16_t)value;break;
            case GRAFEO_INT32:  array->data_int32[i] = (int32_t)value;break;
            case GRAFEO_INT64:  array->data_int64[i] = (int64_t)value;break;
            case GRAFEO_FLOAT:  array->data_float[i] = (float)value;break;
            case GRAFEO_DOUBLE: array->data_double[i] = (double)value;break;
        }
    }
}

uint64_t array_get_num_elements(Array* array){
    return array->num_elements;
}
DataType  array_get_type(Array* array){
    return array->type;
}
uint16_t array_get_dim(Array* array){
    return array->dim;
}
uint32_t*array_get_size(Array* array){
    return array->size;
}
void* array_get_data(Array* array){
    return array->data;
}
uint8_t array_get_bitsize(Array* array){
    return array->bitsize;
}
uint64_t  array_get_num_bytes(Array* array){
    return array->num_bytes;
}
void*     array_get_element(Array* array, uint32_t* indices){
    uint8_t* x = array->data_uint8;
    for(int i = 0; i < array->dim; i++){
         x += indices[i] * array->step[i] * array->bitsize;
    }
    return x;
}
uint64_t* array_get_step(Array* array){
    return array->step;
}
void      array_free(Array* array){
    if(array->data) free(array->data);
    if(array->size) free(array->size);
    if(array->step) free(array->step);
    free(array);
}

Array*    array_sub(Array* array, Range* ranges){
    Array* subarray = array_new_with_dim(array_get_dim(array));
    subarray->type = array->type;
    subarray->bitsize = array->bitsize;
    // Define the beginningn
    uint16_t i;
    uint64_t beginning = 0;
    for(i = 0; i < subarray->dim; i++){
        beginning += ranges[i].from->value * array->step[i] * array->bitsize;
        subarray->step[i] = array->step[i];
    }
    subarray->data = &array->data_uint8[beginning];

    return subarray;
}

Array*    array_reduce_min(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MIN);
}
Array*    array_reduce_max(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MAX);
}
Array*    array_reduce_std(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_STD);
}
Array*    array_reduce_mult(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MULT);
}
Array*    array_reduce_sum(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_SUM);
}

Array* array_reduce(Array* array, int16_t* axes, uint16_t size, ArrayOperation operation){
  // Calculate size of reduced array
  uint16_t  original_dim  = array_get_dim(array);
  uint32_t* original_size = array_get_size(array);
  uint16_t  reduced_dim   = original_dim-size;
  uint32_t* reduced_size  = malloc(sizeof(uint32_t)*reduced_dim);
  uint16_t* reduced_ind   = malloc(sizeof(uint16_t)*reduced_dim);
  uint16_t  i = 0, j = 0, k = 0;
  uint64_t  original_1d = 0;
  uint16_t  original_di;
  for(i = 0; i < original_dim; i++){
    if(i != axes[k]){ reduced_size[j] = original_size[i]; reduced_ind[j++] = i;}
    else k++;
  }
  uint64_t num_reduced = 1;
  for(i = 0; i < reduced_dim; i++) num_reduced *= original_size[axes[i]];

  // Allocate a new array
  Array *reduced;
  switch(operation){
    case GRAFEO_SUM:
    case GRAFEO_STD:
    case GRAFEO_MEAN: reduced = array_zeros(reduced_dim,reduced_size,array_get_type(array));break;
    case GRAFEO_MULT: reduced = array_ones(reduced_dim,reduced_size,array_get_type(array));break;
    case GRAFEO_MAX:  reduced = array_new_with_size_type(reduced_dim, reduced_size, array_get_type(array));
                      array_fill_min(reduced); break;
    case GRAFEO_MIN:  reduced = array_new_with_size_type(reduced_dim, reduced_size, array_get_type(array));
                      array_fill_max(reduced); break;
  }

  // ND indices of original array
  uint32_t* original_indices = malloc(sizeof(uint32_t) * original_dim);
  memset(original_indices, 0, sizeof(uint32_t) * original_dim);


  // iterate
  for(original_1d = 0;
      original_1d < array->num_elements;
      original_1d++, original_indices[original_dim-1]++){

    // convert to ND
    uint16_t atual    = original_dim-2;
    uint16_t anterior = atual + 1;
    for(original_di = 0; original_di < original_dim-1; original_di++, atual--, anterior--){
      original_indices[atual]    += original_indices[anterior] / original_size[anterior];
      original_indices[anterior]  = original_indices[anterior] % original_size[anterior];
    }
    // extract indices and convert to 1D
    uint64_t reduced_1d = 0;
    for(original_di = 0; original_di < reduced_dim; original_di++){
      reduced_1d += original_indices[reduced_ind[original_di]] * reduced->step[original_di];
    }

    // reduce
    switch(operation){
    case GRAFEO_SUM:
    case GRAFEO_MEAN:
        switch(array->type){
          case GRAFEO_UINT8:  reduced->data_uint8[reduced_1d]  += array->data_uint8[original_1d] ;break;
          case GRAFEO_UINT16: reduced->data_uint16[reduced_1d] += array->data_uint16[original_1d];break;
          case GRAFEO_UINT32: reduced->data_uint32[reduced_1d] += array->data_uint32[original_1d];break;
          case GRAFEO_UINT64: reduced->data_uint64[reduced_1d] += array->data_uint64[original_1d];break;
          case GRAFEO_INT8:   reduced->data_int8[reduced_1d]   += array->data_int8[original_1d]  ;break;
          case GRAFEO_INT16:  reduced->data_int16[reduced_1d]  += array->data_int16[original_1d] ;break;
          case GRAFEO_INT32:  reduced->data_int32[reduced_1d]  += array->data_int32[original_1d] ;break;
          case GRAFEO_INT64:  reduced->data_int64[reduced_1d]  += array->data_int64[original_1d] ;break;
          case GRAFEO_FLOAT:  reduced->data_float[reduced_1d]  += array->data_float[original_1d] ;break;
          case GRAFEO_DOUBLE: reduced->data_double[reduced_1d] += array->data_double[original_1d];break;
        }
        break;
    case GRAFEO_MIN:
        switch(array->type){
          case GRAFEO_UINT8:  reduced->data_uint8[reduced_1d]  = min(reduced->data_uint8[reduced_1d],array->data_uint8[original_1d]) ;break;
          case GRAFEO_UINT16: reduced->data_uint16[reduced_1d] = min(reduced->data_uint16[reduced_1d],array->data_uint16[original_1d]);break;
          case GRAFEO_UINT32: reduced->data_uint32[reduced_1d] = min(reduced->data_uint32[reduced_1d],array->data_uint32[original_1d]);break;
          case GRAFEO_UINT64: reduced->data_uint64[reduced_1d] = min(reduced->data_uint64[reduced_1d],array->data_uint64[original_1d]);break;
          case GRAFEO_INT8:   reduced->data_int8[reduced_1d]   = min(reduced->data_int8[reduced_1d],array->data_int8[original_1d])  ;break;
          case GRAFEO_INT16:  reduced->data_int16[reduced_1d]  = min(reduced->data_int16[reduced_1d],array->data_int16[original_1d]) ;break;
          case GRAFEO_INT32:  reduced->data_int32[reduced_1d]  = min(reduced->data_int32[reduced_1d],array->data_int32[original_1d]) ;break;
          case GRAFEO_INT64:  reduced->data_int64[reduced_1d]  = min(reduced->data_int64[reduced_1d],array->data_int64[original_1d]) ;break;
          case GRAFEO_FLOAT:  reduced->data_float[reduced_1d]  = min(reduced->data_float[reduced_1d],array->data_float[original_1d]) ;break;
          case GRAFEO_DOUBLE: reduced->data_double[reduced_1d] = min(reduced->data_double[reduced_1d],array->data_double[original_1d]);break;
        }
        break;
    case GRAFEO_MAX:
      switch(array->type){
        case GRAFEO_UINT8:  reduced->data_uint8[reduced_1d]  = max(reduced->data_uint8[reduced_1d],array->data_uint8[original_1d]) ;break;
        case GRAFEO_UINT16: reduced->data_uint16[reduced_1d] = max(reduced->data_uint16[reduced_1d],array->data_uint16[original_1d]);break;
        case GRAFEO_UINT32: reduced->data_uint32[reduced_1d] = max(reduced->data_uint32[reduced_1d],array->data_uint32[original_1d]);break;
        case GRAFEO_UINT64: reduced->data_uint64[reduced_1d] = max(reduced->data_uint64[reduced_1d],array->data_uint64[original_1d]);break;
        case GRAFEO_INT8:   reduced->data_int8[reduced_1d]   = max(reduced->data_int8[reduced_1d],array->data_int8[original_1d])  ;break;
        case GRAFEO_INT16:  reduced->data_int16[reduced_1d]  = max(reduced->data_int16[reduced_1d],array->data_int16[original_1d]) ;break;
        case GRAFEO_INT32:  reduced->data_int32[reduced_1d]  = max(reduced->data_int32[reduced_1d],array->data_int32[original_1d]) ;break;
        case GRAFEO_INT64:  reduced->data_int64[reduced_1d]  = max(reduced->data_int64[reduced_1d],array->data_int64[original_1d]) ;break;
        case GRAFEO_FLOAT:  reduced->data_float[reduced_1d]  = max(reduced->data_float[reduced_1d],array->data_float[original_1d]) ;break;
        case GRAFEO_DOUBLE: reduced->data_double[reduced_1d] = max(reduced->data_double[reduced_1d],array->data_double[original_1d]);break;
      }
      break;
    case GRAFEO_MULT:
      switch(array->type){
        case GRAFEO_UINT8:  reduced->data_uint8[reduced_1d]  *= array->data_uint8[original_1d] ;break;
        case GRAFEO_UINT16: reduced->data_uint16[reduced_1d] *= array->data_uint16[original_1d];break;
        case GRAFEO_UINT32: reduced->data_uint32[reduced_1d] *= array->data_uint32[original_1d];break;
        case GRAFEO_UINT64: reduced->data_uint64[reduced_1d] *= array->data_uint64[original_1d];break;
        case GRAFEO_INT8:   reduced->data_int8[reduced_1d]   *= array->data_int8[original_1d]  ;break;
        case GRAFEO_INT16:  reduced->data_int16[reduced_1d]  *= array->data_int16[original_1d] ;break;
        case GRAFEO_INT32:  reduced->data_int32[reduced_1d]  *= array->data_int32[original_1d] ;break;
        case GRAFEO_INT64:  reduced->data_int64[reduced_1d]  *= array->data_int64[original_1d] ;break;
        case GRAFEO_FLOAT:  reduced->data_float[reduced_1d]  *= array->data_float[original_1d] ;break;
        case GRAFEO_DOUBLE: reduced->data_double[reduced_1d] *= array->data_double[original_1d];break;
      }
      break;
    case GRAFEO_STD:
      break;
    }
  }

  if(operation == GRAFEO_MEAN){
    array_divide_scalar(reduced, num_reduced);
  }

  free(reduced_size);
  free(reduced_ind);
  free(original_indices);

  //
  return reduced;
}

void array_divide_scalar(Array *array, double value){
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRAFEO_UINT8: array->data_uint8[i] /= value; break;
      case GRAFEO_UINT16: array->data_uint16[i] /= value; break;
      case GRAFEO_UINT32: array->data_uint32[i] /= value; break;
      case GRAFEO_UINT64: array->data_uint64[i] /= value; break;
      case GRAFEO_INT8: array->data_int8[i] /= value; break;
      case GRAFEO_INT16: array->data_int16[i] /= value; break;
      case GRAFEO_INT32: array->data_int32[i] /= value; break;
      case GRAFEO_INT64: array->data_int64[i] /= value; break;
      case GRAFEO_FLOAT: array->data_float[i] /= value; break;
      case GRAFEO_DOUBLE: array->data_double[i] /= value; break;
    }
  }
}
