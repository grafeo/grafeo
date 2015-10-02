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

size_t calculate_bitsize(DataType type){
  switch(type){
      case GRAFEO_UINT8:  return sizeof(uint8_t); break;
      case GRAFEO_UINT16: return sizeof(uint16_t);break;
      case GRAFEO_UINT32: return sizeof(uint32_t);break;
      case GRAFEO_UINT64: return sizeof(uint64_t);break;
      case GRAFEO_INT8:   return sizeof(int8_t);  break;
      case GRAFEO_INT16:  return sizeof(int16_t); break;
      case GRAFEO_INT32:  return sizeof(int32_t); break;
      case GRAFEO_INT64:  return sizeof(int64_t); break;
      case GRAFEO_FLOAT:  return sizeof(float);   break;
      case GRAFEO_DOUBLE: return sizeof(double);  break;
  }
  return 0;
}

/*-----------------------------------
 *     ARRAY CREATION FUNCTIONS
 *-----------------------------------*/
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    array->num_bytes = 0;
    array->bitsize   = 0;
    array->type = GRAFEO_UINT8;
    array->step = NULL;
    array->contiguous = 1;
    array->owns_data  = 1;
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
void      array_fill_header(Array* array, uint32_t* size, DataType type){
  uint16_t i;
  array->num_elements = 1;
  array->step[array->dim-1] = 1;
  uint64_t step = 1;
  for(i = 0; i < array->dim; step*=size[array->dim-(i++)-1]){
      array->size[i]        = size[i];
      array->step[array->dim-i-1]  = step;
      array->num_elements  *= size[i];
  }
  array->bitsize      = calculate_bitsize(type);
  array->num_bytes    = array->bitsize * array->num_elements;
  array->type         = type;
}
Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type){
    Array* array        = array_new_with_dim(dim);
    array_fill_header(array, size, type);
    array->data         = malloc(array->num_bytes);
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
Array*
array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, DataType type){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    return array_new_with_size_type(4, sizes, type);
}
Array*
array_new_like(Array* array){
  return array_new_with_size_type(array->dim, array->size, array->type);
}
Array*
array_zeros(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    memset(array->data, 0, array->num_bytes);
    return array;
}
Array*    array_zeros_like(Array *array){
  return array_zeros_like_type(array,array->type);
}

Array*
array_zeros_like_type(Array* array, DataType type){
  return array_zeros(array->dim, array->size, type);
}
Array*
array_ones(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    array_fill(array,1);
    return array;
}
Array*    array_ones_like(Array *array){
  return array_ones_like_type(array, array->type);
}

Array*
array_ones_like_type(Array* array, DataType type){
  return array_ones(array->dim, array->size, type);
}
/*-----------------------------------
 *   ARRAY OPERATIONS FUNCTIONS
 *-----------------------------------*/
void
array_fill_max(Array *array){
  long double values[10] = {__UINT8_MAX__,__UINT16_MAX__,__UINT32_MAX__,__UINT64_MAX__,__INT8_MAX__,__INT16_MAX__,__INT32_MAX__,__INT64_MAX__,__FLT_MAX__,__DBL_MAX__};
  array_fill(array, values[array->type]);
}
void
array_fill_min(Array *array){
  long double values[10] = {0,0,0,0,-__INT8_MAX__-1,-__INT16_MAX__-1,-__INT32_MAX__-1,-__INT64_MAX__-1,__FLT_MIN__,__DBL_MIN__};
  array_fill(array, values[array->type]);
}
void
array_fill(Array* array, long double value){
    uint64_t i, ii, index_1d;
    uint16_t* indices, atual, anterior;
    if(!array->contiguous){
      indices = malloc(sizeof(uint16_t) * array->dim);
      memset(indices, 0, sizeof(uint16_t) * array->dim);
    }
    for(i = 0; i < array->num_elements; i++){
      // Get index (contiguous or not)
      if(!array->contiguous){
        atual    = array->dim - 2;
        anterior = atual + 1;
        index_1d = 0;
        // Get ND coords
        for(ii = 0; ii < array->dim-1; ii++, atual--, anterior--){
          indices[atual]   += indices[anterior]/array->size[anterior];
          indices[anterior] = indices[anterior]%array->size[anterior];
        }
        // Convert to 1D
        for(ii = 0; ii < array->dim; ii++)
          index_1d += indices[ii] * array->step[ii];
      }
      // If it's contiguouos, just get counter
      else index_1d = i;

      // Update
      switch(array->type){
        case GRAFEO_UINT8:  array->data_uint8[index_1d] = (uint8_t)value;break;
        case GRAFEO_UINT16: array->data_uint16[index_1d] = (uint16_t)value;break;
        case GRAFEO_UINT32: array->data_uint32[index_1d] = (uint32_t)value;break;
        case GRAFEO_UINT64: array->data_uint64[index_1d] = (uint64_t)value;break;
        case GRAFEO_INT8:   array->data_int8[index_1d] = (int8_t)value;break;
        case GRAFEO_INT16:  array->data_int16[index_1d] = (int16_t)value;break;
        case GRAFEO_INT32:  array->data_int32[index_1d] = (int32_t)value;break;
        case GRAFEO_INT64:  array->data_int64[index_1d] = (int64_t)value;break;
        case GRAFEO_FLOAT:  array->data_float[index_1d] = (float)value;break;
        case GRAFEO_DOUBLE: array->data_double[index_1d] = (double)value;break;
      }
      if(!array->contiguous) indices[array->dim-1]++;
    }
    if(!array->contiguous) free(indices);
}
/*-----------------------------------
 *   ARRAY ACCESSOR FUNCTIONS
 *-----------------------------------*/
uint64_t
array_get_num_elements(Array* array){
    return array->num_elements;
}
DataType
array_get_type(Array* array){
    return array->type;
}
uint16_t
array_get_dim(Array* array){
    return array->dim;
}
uint32_t*
array_get_size(Array* array){
    return array->size;
}
void*
array_get_data(Array* array){
    return array->data;
}
uint8_t
array_get_bitsize(Array* array){
    return array->bitsize;
}
uint64_t
array_get_num_bytes(Array* array){
    return array->num_bytes;
}

int32_t*
array_index(Array* array, int64_t index){
  int32_t* indices = malloc(sizeof(int32_t) * array->dim);
  uint8_t i;
  div_t division;
  for(i = 0; i < array->dim; i++){
    division   = div(index, array->step[i]);
    indices[i] = division.quot;
    index      = division.rem;
  }
  return indices;
}

int64_t
array_index_1D(Array* array, int32_t* indices){
  int64_t x = 0, i;
  for(i = 0; i < array->dim; x += indices[i] * array->step[i], i++);
  return x;
}

uint8_t
array_index_is_valid(Array *array, int32_t *indices){
  uint8_t i;
  for(i = 0; i < array->dim; i++)
    if(indices[i] < 0 || indices[i] >= array->size[i]) return 0;
  return 1;
}

uint8_t
array_index_1D_is_valid(Array *array, int64_t index){
  return index >= 0 && index < array->num_elements;
}
void*
array_get_element(Array* array, uint32_t* indices){
    uint8_t* x = array->data_uint8;
    int i;
    for(i = 0; i < array->dim; i++){
         x += indices[i] * array->step[i] * array->bitsize;
    }
    return x;
}
void*
array_get_element_1D(Array* array, uint64_t index){
  return array->data_uint8 + index * array->bitsize;
}

uint64_t*
array_get_step(Array* array){
    return array->step;
}

void
array_set_element(Array* array, uint32_t* indices, double value){
  uint64_t x = 0;
  uint8_t i;
  for(i = 0; i < array->dim; i++)
       x += indices[i] * array->step[i];
  switch(array->type){
    case GRAFEO_UINT8:  array->data_uint8[x] = (uint8_t)value; break;
    case GRAFEO_UINT16: array->data_uint16[x] = (uint16_t)value; break;
    case GRAFEO_UINT32: array->data_uint32[x] = (uint32_t)value; break;
    case GRAFEO_UINT64: array->data_uint64[x] = (uint64_t)value; break;
    case GRAFEO_INT8:   array->data_int8[x] = (int8_t)value; break;
    case GRAFEO_INT16:  array->data_int16[x] = (int16_t)value; break;
    case GRAFEO_INT32:  array->data_int32[x] = (int32_t)value; break;
    case GRAFEO_INT64:  array->data_int64[x] = (int64_t)value; break;
    case GRAFEO_FLOAT:  array->data_float[x] = (float)value; break;
    case GRAFEO_DOUBLE: array->data_double[x] = (double)value; break;
  }

}

long double array_get_long_double_1D(Array* array1, uint64_t i){
  long double value1;
  switch(array1->type){
    case GRAFEO_UINT8: value1 = (long double)array1->data_uint8[i];break;
    case GRAFEO_UINT16: value1 = (long double)array1->data_uint16[i];break;
    case GRAFEO_UINT32: value1 = (long double)array1->data_uint32[i];break;
    case GRAFEO_UINT64: value1 = (long double)array1->data_uint64[i];break;
    case GRAFEO_INT8: value1 = (long double)array1->data_int8[i];break;
    case GRAFEO_INT16: value1 = (long double)array1->data_int16[i];break;
    case GRAFEO_INT32: value1 = (long double)array1->data_int32[i];break;
    case GRAFEO_INT64: value1 = (long double)array1->data_int64[i];break;
    case GRAFEO_FLOAT: value1 = (long double)array1->data_float[i];break;
    case GRAFEO_DOUBLE: value1 = (long double)array1->data_double[i];break;
  }
  return value1;
}

void
array_free(Array* array){
    if(array->data && array->owns_data) free(array->data);
    if(array->size) free(array->size);
    if(array->step) free(array->step);
    free(array);
}

Array*
array_sub(Array* array, Range* ranges){
    Array* subarray = array_new_with_dim(array_get_dim(array));
    subarray->type = array->type;
    subarray->bitsize = array->bitsize;
    subarray->data = array->data;
    subarray->owns_data = 0;

    // Define the beginning
    uint16_t i;
    uint64_t beginning = 0;
    subarray->num_elements = 1;
    uint64_t faixa_from, faixa_to;
    for(i = 0; i < subarray->dim; i++){
        faixa_from = ((ranges[i].from==NULL)?0             :ranges[i].from->value);
        faixa_to   = ((ranges[i].to  ==NULL)?array->size[i]:ranges[i].to->value);
        beginning += faixa_from * array->step[i];
        subarray->step[i] = array->step[i];
        subarray->size[i] = faixa_to-faixa_from;
        if(subarray->size[i] != array->size[i]) subarray->contiguous = 0;
        subarray->num_elements *= subarray->size[i];
    }
    subarray->num_bytes = subarray->bitsize * subarray->num_elements;
    subarray->data      = array->data + beginning * array->bitsize;

    return subarray;
}

Array*
array_from_data(void* data, uint16_t dim, uint32_t* size, DataType type){
  Array* array = array_new_with_dim(dim);
  array_fill_header(array, size, type);
  array->owns_data = 0;
  array->data      = data;
  return array;
}

Array*
array_reduce_min(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MIN);
}
Array*
array_reduce_max(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MAX);
}
Array*
array_reduce_std(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_STD);
}
Array*
array_reduce_mult(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_MULT);
}
Array*
array_reduce_sum(Array* array, int16_t* axes, uint16_t size){
  return array_reduce(array, axes, size, GRAFEO_SUM);
}

Array*
array_reduce(Array* array, int16_t* axes, uint16_t size, ArrayOperation operation){
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

Array* array_sum_scalar(Array *array, double value){
  Array* new_array = array_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  + value; break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array->data_uint16[i] + value; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array->data_uint32[i] + value; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array->data_uint64[i] + value; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array->data_int8[i]   + value; break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array->data_int16[i]  + value; break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array->data_int32[i]  + value; break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array->data_int64[i]  + value; break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array->data_float[i]  + value; break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array->data_double[i] + value; break;
    }
  }
  return new_array;
}
Array* array_subtract_scalar(Array *array, double value){
  Array* new_array = array_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  - value; break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array->data_uint16[i] - value; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array->data_uint32[i] - value; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array->data_uint64[i] - value; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array->data_int8[i]   - value; break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array->data_int16[i]  - value; break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array->data_int32[i]  - value; break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array->data_int64[i]  - value; break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array->data_float[i]  - value; break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array->data_double[i] - value; break;
    }
  }
  return new_array;
}
Array* array_mult_scalar(Array *array, double value){
  Array* new_array = array_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  * value; break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array->data_uint16[i] * value; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array->data_uint32[i] * value; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array->data_uint64[i] * value; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array->data_int8[i]   * value; break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array->data_int16[i]  * value; break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array->data_int32[i]  * value; break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array->data_int64[i]  * value; break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array->data_float[i]  * value; break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array->data_double[i] * value; break;
    }
  }
  return new_array;
}
Array* array_divide_scalar(Array *array, double value){
  Array* new_array = array_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  / value; break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array->data_uint16[i] / value; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array->data_uint32[i] / value; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array->data_uint64[i] / value; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array->data_int8[i]   / value; break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array->data_int16[i]  / value; break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array->data_int32[i]  / value; break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array->data_int64[i]  / value; break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array->data_float[i]  / value; break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array->data_double[i] / value; break;
    }
  }
  return new_array;
}

Array* array_sum(Array *array1, Array *array2){
  return array_sum_to(array1, array2, NULL);
}
Array* array_subtract(Array *array1, Array *array2){
  return array_subtract_to(array1, array2, NULL);
}
Array* array_mult(Array *array1, Array *array2){
  return array_mult_to(array1, array2, NULL);
}
Array* array_divide(Array *array1, Array *array2){
  return array_divide_to(array1, array2, NULL);
}
Array* array_sum_to(Array* array1, Array* array2, Array* new_array){
  if(!new_array) new_array = array_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  + array2->data_uint8[i];  break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] + array2->data_uint16[i]; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] + array2->data_uint32[i]; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] + array2->data_uint64[i]; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   + array2->data_int8[i];   break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  + array2->data_int16[i];  break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  + array2->data_int32[i];  break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  + array2->data_int64[i];  break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  + array2->data_float[i];  break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array1->data_double[i] + array2->data_double[i]; break;
    }
  }
  return new_array;
}
Array* array_subtract_to(Array* array1, Array* array2, Array* new_array){
  if(!new_array) new_array = array_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  - array2->data_uint8[i];  break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] - array2->data_uint16[i]; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] - array2->data_uint32[i]; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] - array2->data_uint64[i]; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   - array2->data_int8[i];   break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  - array2->data_int16[i];  break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  - array2->data_int32[i];  break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  - array2->data_int64[i];  break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  - array2->data_float[i];  break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array1->data_double[i] - array2->data_double[i]; break;
    }
  }
  return new_array;
}
Array* array_mult_to(Array* array1, Array* array2, Array* new_array){
  if(!new_array) new_array = array_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  * array2->data_uint8[i];  break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] * array2->data_uint16[i]; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] * array2->data_uint32[i]; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] * array2->data_uint64[i]; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   * array2->data_int8[i];   break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  * array2->data_int16[i];  break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  * array2->data_int32[i];  break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  * array2->data_int64[i];  break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  * array2->data_float[i];  break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array1->data_double[i] * array2->data_double[i]; break;
    }
  }
  return new_array;
}
Array* array_divide_to(Array* array1, Array* array2, Array* new_array){
  if(!new_array) new_array = array_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRAFEO_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  / array2->data_uint8[i];  break;
      case GRAFEO_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] / array2->data_uint16[i]; break;
      case GRAFEO_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] / array2->data_uint32[i]; break;
      case GRAFEO_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] / array2->data_uint64[i]; break;
      case GRAFEO_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   / array2->data_int8[i];   break;
      case GRAFEO_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  / array2->data_int16[i];  break;
      case GRAFEO_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  / array2->data_int32[i];  break;
      case GRAFEO_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  / array2->data_int64[i];  break;
      case GRAFEO_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  / array2->data_float[i];  break;
      case GRAFEO_DOUBLE: new_array->data_double[i] = array1->data_double[i] / array2->data_double[i]; break;
    }
  }
  return new_array;
}

long double array_euclidian_distance(Array* array1, Array* array2){
  return sqrt(array_square_euclidian_distance(array1, array2));
}

long double array_square_euclidian_distance(Array* array1, Array* array2){
  long double sum = 0, value1, value2, value;
  uint64_t i;
  for(i = 0; i < array1->num_elements; i++){
    value1 = array_get_long_double_1D(array1,i);
    value2 = array_get_long_double_1D(array2,i);
    value  = value1-value2;
    sum   += value * value;
  }
  return sum;
}


/*-----------------------------------
 *       ARRAY IO FUNCTIONS
 *-----------------------------------*/
static Array* array_read_csv_type_minimum(const char* filename, DataType type, uint8_t automatic){
  FILE* file = fopen(filename, "r");
  char* record, *line;

  // Temporary buffers
  char         buffer[1024];
  uint16_t     N       = 1024;
  long double* data    = malloc(N*sizeof(long double));

  // Some informations to be extracted while traversing file
  uint64_t i           = 0; // Current number of elements
  uint32_t height      = 0; // Number of lines
  uint32_t width       = 0; // Number of columns
  uint8_t  first_line  = 1; // To calculate width once
  uint8_t  dim         = 1; // if height=1, the array is 1D
  uint32_t size[2]     = {0,0};// Array size
  uint8_t  is_integer  = 1; // If all numbers are integers, the
                            // array type reduces from float to int
  uint8_t  is_unsigned = 1; // If all numbers are positive, the
                            // array type is unsigned (to save
                            // memory)
  long double maximum  = __DBL_MIN__; // Maximum value

  // For each line
  while((line=fgets(buffer, sizeof(buffer),file))!=NULL){
    // For each entry/token on line
    record = strtok(line,";");
    while(record != NULL){
      // Define array width
      if(first_line) width++;
      // Convert string to number
      data[i] = strtod(record, NULL);
      // Is unsigned?
      if(data[i] < 0) is_unsigned = 0;
      // Is maximum?
      if(maximum < data[i]) maximum = data[i];
      // Is integer?
      if(rintl(data[i]) != data[i]) is_integer = 0;
      // Does it need expansion?
      i++;
      if(i >= N){
        N <<= 1;
        // duplicate array size
        data = realloc(data,N*sizeof(long double));
      }
      // Get next entry/token
      record = strtok(NULL,";");
    }
    // Define array height
    height++;
    // Finish defining width
    first_line = 0;
  }
  // Convert to 1D
  if(height == 1){
    dim     = 1;
    size[0] = width;
  }else{
    dim     = 2;
    size[0] = height;
    size[1] = width;
  }
  if(automatic){
    // Get type of Array
    if(is_integer){
      if(is_unsigned){
        if(maximum <= __UINT64_MAX__){
          type = GRAFEO_UINT64;
          if(maximum <= __UINT32_MAX__){
            type = GRAFEO_UINT32;
            if(maximum <= __UINT16_MAX__){
              type = GRAFEO_UINT16;
              if(maximum <= __UINT8_MAX__){
                type = GRAFEO_UINT8;
              }
            }
          }
        }
      }
      else{
        if(maximum <= __INT64_MAX__){
          type = GRAFEO_INT64;
          if(maximum <= __INT32_MAX__){
            type = GRAFEO_INT32;
            if(maximum <= __INT16_MAX__){
              type = GRAFEO_INT16;
              if(maximum <= __INT8_MAX__){
                type = GRAFEO_INT8;
              }
            }
          }
        }
      }
    }
    else{
      if(maximum <= __FLT_MAX__)
        type = GRAFEO_FLOAT;
      else
        type = GRAFEO_DOUBLE;
    }
  }

  // Create the array
  Array* array = array_new_with_size_type(dim, size, type);
  for(i = 0; i < array->num_elements; i++){
    switch(type){
      case GRAFEO_UINT8:  array->data_uint8[i]  = (uint8_t)  data[i];break;
      case GRAFEO_UINT16: array->data_uint16[i] = (uint16_t) data[i];break;
      case GRAFEO_UINT32: array->data_uint32[i] = (uint32_t) data[i];break;
      case GRAFEO_UINT64: array->data_uint64[i] = (uint64_t) data[i];break;
      case GRAFEO_INT8:   array->data_int8[i]   = (int8_t)   data[i];break;
      case GRAFEO_INT16:  array->data_int16[i]  = (int16_t)  data[i];break;
      case GRAFEO_INT32:  array->data_int32[i]  = (int32_t)  data[i];break;
      case GRAFEO_INT64:  array->data_int64[i]  = (int64_t)  data[i];break;
      case GRAFEO_FLOAT:  array->data_float[i]  = (float)    data[i];break;
      case GRAFEO_DOUBLE: array->data_double[i] = (double)   data[i];break;
    }
  }

  free(data);
  fclose(file);
  return array;
}

Array* array_read_csv(const char* filename){
  return array_read_csv_type_minimum(filename, GRAFEO_UINT32, 1);
}
Array* array_read_csv_type(const char* filename, DataType type){
  return array_read_csv_type_minimum(filename, type, 0);
}

void array_write_csv(Array* array, const char* filename){
  FILE* file = fopen(filename, "w");
  char* line;
  if(array->dim == 1) line = array_join(array, ";");
  else                line = array_join(array,"\n;");
  fprintf(file,line);
  free(line);
  fclose(file);
}

char* array_join(Array* array, const char* delimiters){
  char* text, *cur_text;
  uint8_t d = 0;
  uint64_t i;

  // Initial allocation of text
  uint64_t N = 1024;
  uint64_t N2 = N >> 1;
  uint64_t cur_length = 0;
  text = malloc(N);
  cur_text = text;

  for(i = 0; i < array->num_elements; i++){
    // Resize data if needed
    if(cur_length > N2){
      N2 = N;
      N <<= 1;
      text = realloc(text, N);
    }

    // If last line item
    if(array->dim > 1){
      if(!((i+1) % array->size[1])) d = 0;
      else                          d = 1;
    }
    else if(!((i+1) % array->size[0])) d = 1;
    else d = 0;
    // Print data and delimiter
    switch(array->type){
      case GRAFEO_UINT8:  cur_length += sprintf(cur_text,"%hhu%c",array->data_uint8[i] , delimiters[d]);break;
      case GRAFEO_UINT16: cur_length += sprintf(cur_text,"%hu%c" ,array->data_uint16[i], delimiters[d]);break;
      case GRAFEO_UINT32: cur_length += sprintf(cur_text,"%u%c"  ,array->data_uint32[i], delimiters[d]);break;
      case GRAFEO_UINT64: cur_length += sprintf(cur_text,"%lu%c" ,array->data_uint64[i], delimiters[d]);break;
      case GRAFEO_INT8:   cur_length += sprintf(cur_text,"%hhd%c",array->data_int8[i]  , delimiters[d]);break;
      case GRAFEO_INT16:  cur_length += sprintf(cur_text,"%hd%c" ,array->data_int16[i] , delimiters[d]);break;
      case GRAFEO_INT32:  cur_length += sprintf(cur_text,"%d%c"  ,array->data_int32[i] , delimiters[d]);break;
      case GRAFEO_INT64:  cur_length += sprintf(cur_text,"%ld%c" ,array->data_int64[i] , delimiters[d]);break;
      case GRAFEO_FLOAT:  cur_length += sprintf(cur_text,"%f%c"  ,array->data_float[i] , delimiters[d]);break;
      case GRAFEO_DOUBLE: cur_length += sprintf(cur_text,"%lf%c" ,array->data_double[i], delimiters[d]);break;
    }
    cur_text    = text + cur_length;
  }
  // Remove last delimiter
  cur_text--;cur_length--;
  *cur_text = '\0';
  text = realloc(text,cur_length);
  return text;
}
