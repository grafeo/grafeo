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
#include <grafeo/core.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
/**
 * @brief GrfNDArray Structure
 *
 * Each array will have its own data
 *
 * Each array will have its own size vector
 *
 * Support multidimensional arrays (dim > 2)
 */
typedef struct _GrfNDArrayPrivate{
    uint16_t  dim;          /**< number of dimensions */
    uint32_t* size;         /**< number of elements in each dimension */
    uint64_t  num_elements; /**< number of elements of the array */
    size_t    num_bytes;    /**< total number of bytes */
    uint8_t   bitsize;      /**< number of bytes of each value*/
    GrfDataType  type;         /**< data type of each element */
    uint64_t* step;         /**< number of elements to increase an axis value */
    uint8_t   contiguous;   /**< it can be iterated using a common loop */
    uint8_t   owns_data;    /**< owner of its data */
    union{
        void*          data;
        unsigned char* data_uint8;
        uint16_t*      data_uint16;
        uint32_t*      data_uint32;
        uint64_t*      data_uint64;
        char*          data_int8;
        int16_t*       data_int16;
        int32_t*       data_int32;
        int64_t*       data_int64;
        float*         data_float;
        double*        data_double;
        unsigned int*  data_int;
    };
}GrfNDArrayPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(GrfNDArray, grf_ndarray, G_TYPE_OBJECT)

static void
grf_ndarray_init(GrfNDArray *self){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(self);
  priv->dim          = 0;
  priv->size         = NULL;
  priv->num_elements = 0;
  priv->num_bytes    = 0;
  priv->bitsize      = 0;
  priv->type         = GRF_UINT8;
  priv->step         = NULL;
  priv->contiguous   = 1;
  priv->owns_data    = 1;
}

static void
grf_ndarray_class_init(GrfNDArrayClass *klass){

}

static size_t grf_ndarray_calculate_bitsize(GrfDataType type){
  switch(type){
      case GRF_UINT8:  return sizeof(uint8_t); break;
      case GRF_UINT16: return sizeof(uint16_t);break;
      case GRF_UINT32: return sizeof(uint32_t);break;
      case GRF_UINT64: return sizeof(uint64_t);break;
      case GRF_INT8:   return sizeof(int8_t);  break;
      case GRF_INT16:  return sizeof(int16_t); break;
      case GRF_INT32:  return sizeof(int32_t); break;
      case GRF_INT64:  return sizeof(int64_t); break;
      case GRF_FLOAT:  return sizeof(float);   break;
      case GRF_DOUBLE: return sizeof(double);  break;
  }
  return 0;
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

/* Array Creation Functions
 *=================================*/

GrfNDArray*    grf_ndarray_new(){
    return g_object_new(GRF_TYPE_NDARRAY, NULL);
}
GrfNDArray*    grf_ndarray_new_with_dim(uint16_t dim){
    GrfNDArray* array        = grf_ndarray_new();
    GrfNDArrayPrivate* priv  = grf_ndarray_get_instance_private(array);
    priv->dim                = dim;
    priv->size               = malloc(sizeof(uint32_t) * dim);
    priv->step               = malloc(sizeof(uint64_t) * dim);
    return array;
}
GrfNDArray*    grf_ndarray_new_with_size(uint16_t dim, uint32_t* size){
    return grf_ndarray_new_with_size_type(dim, size, GRF_UINT8);
}
void      grf_ndarray_fill_header(GrfNDArray* array, uint32_t* size, GrfDataType type){
  GrfNDArrayPrivate* priv  = grf_ndarray_get_instance_private(array);
  uint16_t i;
  priv->num_elements = 1;
  priv->step[array->dim-1] = 1;
  uint64_t step = 1;
  for(i = 0; i < array->dim; step*=size[array->dim-(i++)-1]){
      priv->size[i]        = size[i];
      priv->step[array->dim-i-1]  = step;
      priv->num_elements  *= size[i];
  }
  priv->bitsize      = grf_ndarray_calculate_bitsize(type);
  priv->num_bytes    = array->bitsize * array->num_elements;
  priv->type         = type;
}
GrfNDArray*    grf_ndarray_new_with_size_type(uint16_t dim, uint32_t* size, GrfDataType type){
  GrfNDArray* array        = grf_ndarray_new_with_dim(dim);
  GrfNDArrayPrivate* priv  = grf_ndarray_get_instance_private(array);
  grf_ndarray_fill_header(array, size, type);
  priv->data         = malloc(array->num_bytes);
  return array;
}
GrfNDArray*    grf_ndarray_new_1D(uint32_t size1){
  return grf_ndarray_new_with_size(1, &size1);
}
GrfNDArray*    grf_ndarray_new_2D(uint32_t size1, uint32_t size2){
  uint32_t sizes[2] = {size1, size2};
  return grf_ndarray_new_with_size(2, sizes);
}
GrfNDArray*    grf_ndarray_new_3D(uint32_t size1, uint32_t size2, uint32_t size3){
  uint32_t sizes[3] = {size1, size2, size3};
  return grf_ndarray_new_with_size(3, sizes);
}
GrfNDArray*    grf_ndarray_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4){
  uint32_t sizes[4] = {size1, size2, size3, size4};
  return grf_ndarray_new_with_size(4, sizes);
}
GrfNDArray*    grf_ndarray_new_1D_type(uint32_t size1, GrfDataType type){
  return grf_ndarray_new_with_size_type(1, &size1, type);
}
GrfNDArray*    grf_ndarray_new_2D_type(uint32_t size1, uint32_t size2, GrfDataType type){
  uint32_t sizes[2] = {size1, size2};
  return grf_ndarray_new_with_size_type(2, sizes, type);
}
GrfNDArray*    grf_ndarray_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, GrfDataType type){
  uint32_t sizes[3] = {size1, size2, size3};
  return grf_ndarray_new_with_size_type(3, sizes, type);
}
GrfNDArray*
grf_ndarray_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, GrfDataType type){
  uint32_t sizes[4] = {size1, size2, size3, size4};
  return grf_ndarray_new_with_size_type(4, sizes, type);
}
GrfNDArray*
grf_ndarray_new_like(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return grf_ndarray_new_with_size_type(priv->dim, priv->size, priv->type);
}
GrfNDArray*
grf_ndarray_zeros(uint16_t dim, uint32_t* sizes, GrfDataType type){
  GrfNDArray* array = grf_ndarray_new_with_size_type(dim, sizes, type);
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  memset(priv->data, 0, priv->num_bytes);
  return array;
}
GrfNDArray*    grf_ndarray_zeros_like(GrfNDArray *array){
  return grf_ndarray_zeros_like_type(array,grf_ndarray_get_elemtype(array));
}

GrfNDArray*
grf_ndarray_zeros_like_type(GrfNDArray* array, GrfDataType type){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return grf_ndarray_zeros(priv->dim, priv->size, type);
}
GrfNDArray*
grf_ndarray_ones(uint16_t dim, uint32_t* sizes, GrfDataType type){
  GrfNDArray* array = grf_ndarray_new_with_size_type(dim, sizes, type);
  grf_ndarray_fill(array,1);
  return array;
}
GrfNDArray*    grf_ndarray_ones_like(GrfNDArray *array){
  return grf_ndarray_ones_like_type(array, grf_ndarray_get_elemtype(array));
}

GrfNDArray*
grf_ndarray_ones_like_type(GrfNDArray* array, GrfDataType type){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return grf_ndarray_ones(priv->dim, priv->size, type);
}
/* Operations
 *=================================*/
static long double max_values[10] = {__UINT8_MAX__,__UINT16_MAX__,__UINT32_MAX__,__UINT64_MAX__,__INT8_MAX__,
                                 __INT16_MAX__,__INT32_MAX__ ,__INT64_MAX__ ,__FLT_MAX__   ,__DBL_MAX__};
void
grf_ndarray_fill_max(GrfNDArray *array){
  grf_ndarray_fill(array, max_values[grf_ndarray_get_elemtype(array)]);
}

long double min_values[10] = {0,0,0,0,-__INT8_MAX__-1,-__INT16_MAX__-1,-__INT32_MAX__-1,-__INT64_MAX__-1,__FLT_MIN__,__DBL_MIN__};
void
grf_ndarray_fill_min(GrfNDArray *array){  
  grf_ndarray_fill(array, min_values[grf_ndarray_get_elemtype(array)]);
}
void
grf_ndarray_fill(GrfNDArray* array, long double value){
  uint64_t i, ii, index_1d;
  uint16_t* indices, atual, anterior;
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  if(!priv->contiguous){
    indices = malloc(sizeof(uint16_t) * priv->dim);
    memset(indices, 0, sizeof(uint16_t) * priv->dim);
  }
  for(i = 0; i < priv->num_elements; i++){
    // Get index (contiguous or not)
    if(!priv->contiguous){
      atual    = priv->dim - 2;
      anterior = atual + 1;
      index_1d = 0;
      // Get ND coords
      for(ii = 0; ii < priv->dim-1; ii++, atual--, anterior--){
        indices[atual]   += indices[anterior]/priv->size[anterior];
        indices[anterior] = indices[anterior]%priv->size[anterior];
      }
      // Convert to 1D
      for(ii = 0; ii < priv->dim; ii++)
        index_1d += indices[ii] * priv->step[ii];
    }
    // If it's contiguouos, just get counter
    else index_1d = i;

    // Update
    switch(priv->type){
      case GRF_UINT8:  priv->data_uint8[index_1d] = (uint8_t)value;break;
      case GRF_UINT16: priv->data_uint16[index_1d] = (uint16_t)value;break;
      case GRF_UINT32: priv->data_uint32[index_1d] = (uint32_t)value;break;
      case GRF_UINT64: priv->data_uint64[index_1d] = (uint64_t)value;break;
      case GRF_INT8:   priv->data_int8[index_1d] = (int8_t)value;break;
      case GRF_INT16:  priv->data_int16[index_1d] = (int16_t)value;break;
      case GRF_INT32:  priv->data_int32[index_1d] = (int32_t)value;break;
      case GRF_INT64:  priv->data_int64[index_1d] = (int64_t)value;break;
      case GRF_FLOAT:  priv->data_float[index_1d] = (float)value;break;
      case GRF_DOUBLE: priv->data_double[index_1d] = (double)value;break;
    }
    if(!priv->contiguous) indices[priv->dim-1]++;
  }
  if(!priv->contiguous) free(indices);
}

/* Accessors
 *=================================*/

uint64_t
grf_ndarray_get_num_elements(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->num_elements;
}
GrfDataType
grf_ndarray_get_elemtype(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->type;
}
uint16_t
grf_ndarray_get_dim(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->dim;
}
uint32_t*
grf_ndarray_get_size(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->size;
}
void*
grf_ndarray_get_data(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->data;
}
uint8_t
grf_ndarray_get_bitsize(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->bitsize;
}
uint64_t
grf_ndarray_get_num_bytes(GrfNDArray* array){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return priv->num_bytes;
}

int32_t*
grf_ndarray_index(GrfNDArray* array, int64_t index){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  int32_t* indices = malloc(sizeof(int32_t) * priv->dim);
  uint8_t i;
  div_t division;
  for(i = 0; i < priv->dim; i++){
    division   = div(index, priv->step[i]);
    indices[i] = division.quot;
    index      = division.rem;
  }
  return indices;
}

int64_t
grf_ndarray_index_1D(GrfNDArray* array, int32_t* indices){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  int64_t x = 0, i;
  for(i = 0; i < priv->dim; x += indices[i] * priv->step[i], i++);
  return x;
}

uint8_t
grf_ndarray_index_is_valid(GrfNDArray *array, int32_t *indices){
  uint8_t i;
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  for(i = 0; i < priv->dim; i++)
    if(indices[i] < 0 || indices[i] >= priv->size[i]) return 0;
  return 1;
}

uint8_t
grf_ndarray_index_1D_is_valid(GrfNDArray *array, int64_t index){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
  return index >= 0 && index < priv->num_elements;
}
void*
grf_ndarray_get_element(GrfNDArray* array, uint32_t* indices){
  GrfNDArrayPrivate* priv = grf_ndarray_get_instance_private(array);
    uint8_t* x = priv->data_uint8;
    int i;
    for(i = 0; i < array->dim; i++){
         x += indices[i] * priv->step[i] * array->bitsize;
    }
    return x;
}
void*
grf_ndarray_get_element_1D(GrfNDArray* array, uint64_t index){
  return array->data_uint8 + index * array->bitsize;
}

uint64_t*
grf_ndarray_get_step(GrfNDArray* array){
    return array->step;
}

void
grf_ndarray_set_element(GrfNDArray* array, uint32_t* indices, double value){
  uint64_t x = 0;
  uint8_t i;
  for(i = 0; i < array->dim; i++)
       x += indices[i] * array->step[i];
  grf_ndarray_set_element_1D(array, x, value);
}

void grf_ndarray_set_element_1D(GrfNDArray* array, uint64_t i, double value){
  switch(array->type){
    case GRF_UINT8:  array->data_uint8[i] = (uint8_t)value; break;
    case GRF_UINT16: array->data_uint16[i] = (uint16_t)value; break;
    case GRF_UINT32: array->data_uint32[i] = (uint32_t)value; break;
    case GRF_UINT64: array->data_uint64[i] = (uint64_t)value; break;
    case GRF_INT8:   array->data_int8[i] = (int8_t)value; break;
    case GRF_INT16:  array->data_int16[i] = (int16_t)value; break;
    case GRF_INT32:  array->data_int32[i] = (int32_t)value; break;
    case GRF_INT64:  array->data_int64[i] = (int64_t)value; break;
    case GRF_FLOAT:  array->data_float[i] = (float)value; break;
    case GRF_DOUBLE: array->data_double[i] = (double)value; break;
  }
}

long double grf_ndarray_get_long_double_1D(GrfNDArray* array1, uint64_t i){
  long double value1;
  switch(array1->type){
    case GRF_UINT8: value1 = (long double)array1->data_uint8[i];break;
    case GRF_UINT16: value1 = (long double)array1->data_uint16[i];break;
    case GRF_UINT32: value1 = (long double)array1->data_uint32[i];break;
    case GRF_UINT64: value1 = (long double)array1->data_uint64[i];break;
    case GRF_INT8: value1 = (long double)array1->data_int8[i];break;
    case GRF_INT16: value1 = (long double)array1->data_int16[i];break;
    case GRF_INT32: value1 = (long double)array1->data_int32[i];break;
    case GRF_INT64: value1 = (long double)array1->data_int64[i];break;
    case GRF_FLOAT: value1 = (long double)array1->data_float[i];break;
    case GRF_DOUBLE: value1 = (long double)array1->data_double[i];break;
  }
  return value1;
}

void
grf_ndarray_free(GrfNDArray* array){
    if(array->data && array->owns_data) free(array->data);
    if(array->size) free(array->size);
    if(array->step) free(array->step);
    free(array);
}

GrfNDArray*
grf_ndarray_sub(GrfNDArray* array, GrfRange* ranges){
    GrfNDArray* subarray = grf_ndarray_new_with_dim(grf_ndarray_get_dim(array));
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

GrfNDArray*
grf_ndarray_from_data(void* data, uint16_t dim, uint32_t* size, GrfDataType type){
  GrfNDArray* array = grf_ndarray_new_with_dim(dim);
  grf_ndarray_fill_header(array, size, type);
  array->owns_data = 0;
  array->data      = data;
  return array;
}
GrfNDArray*
grf_ndarray_as_type(GrfNDArray* array, GrfDataType type){
  if(type != array->type){
    GrfNDArray* array2 = grf_ndarray_new_with_size_type(array->dim, array->size, type);
    uint64_t i;
    for(i = 0; i < array->num_elements; i++)
      grf_ndarray_set_element_1D(array2, i, grf_ndarray_get_long_double_1D(array,i));
    return array2;
  }
  return array;
}
GrfNDArray*    grf_ndarray_circular_indices(uint16_t dim, float radius){
  int32_t dz, dy, dx, r2, i;
  uint32_t n;

  // Discover number of pixels
  n  = 0;
  r2 = (int32_t)(radius*radius + 0.5);
  for(dy=-radius;dy<=radius;dy++)
    for(dx=-radius;dx<=radius;dx++)
      if(dim == 2){
        if(((dx*dx)+(dy*dy)) <= r2) n++;
      }
      else if(dim == 3){
        for(dz=-radius;dz<=radius;dz++)
          if(((dx*dx)+(dy*dy)+(dz*dz)) <= r2) n++;
      }
  // Create the array
  uint32_t size[2] = {n-1,dim};
  GrfNDArray* indices = grf_ndarray_new_with_size_type(2,size,GRF_INT64);

  // Fill the array
  i = 0;
  for(dy=-radius;dy<=radius;dy++){
    for(dx=-radius;dx<=radius;dx++){
      if(dim == 2){
        if(((dx*dx)+(dy*dy)) <= r2){
          if ((dx != 0)||(dy != 0)){
            indices->data_int64[i++] = dy;
            indices->data_int64[i++] = dx;
          }
        }
      }
      else if(dim == 3){
        for(dz=-radius;dz<=radius;dz++){
          if(((dx*dx)+(dy*dy)+(dz*dz)) <= r2){
            if ((dx != 0)||(dy != 0)||(dz != 0)){
              indices->data_int64[i++] = dz;
              indices->data_int64[i++] = dy;
              indices->data_int64[i++] = dx;
            }
          }
        }
      }
    }
  }


  return indices;
}

GrfNDArray*
grf_ndarray_copy(GrfNDArray* array){
  GrfNDArray* array2 = grf_ndarray_new_like(array);
  memcpy(array2->data,array->data,array->num_bytes);
  return array2;
}

GrfNDArray*
grf_ndarray_reduce_min(GrfNDArray* array, int16_t* axes, uint16_t size){
  return grf_ndarray_reduce(array, axes, size, GRF_MIN);
}
GrfNDArray*
grf_ndarray_reduce_max(GrfNDArray* array, int16_t* axes, uint16_t size){
  return grf_ndarray_reduce(array, axes, size, GRF_MAX);
}
GrfNDArray*
grf_ndarray_reduce_std(GrfNDArray* array, int16_t* axes, uint16_t size){
  return grf_ndarray_reduce(array, axes, size, GRF_STD);
}
GrfNDArray*
grf_ndarray_reduce_mult(GrfNDArray* array, int16_t* axes, uint16_t size){
  return grf_ndarray_reduce(array, axes, size, GRF_MULT);
}
GrfNDArray*
grf_ndarray_reduce_sum(GrfNDArray* array, int16_t* axes, uint16_t size){
  return grf_ndarray_reduce(array, axes, size, GRF_SUM);
}

GrfNDArray*
grf_ndarray_reduce(GrfNDArray* array, int16_t* axes, uint16_t size, GrfNDArrayOperation operation){
  // Calculate size of reduced array
  uint16_t  original_dim  = grf_ndarray_get_dim(array);
  uint32_t* original_size = grf_ndarray_get_size(array);
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
  GrfNDArray *reduced;
  switch(operation){
    case GRF_SUM:
    case GRF_STD:
    case GRF_MEAN: reduced = grf_ndarray_zeros(reduced_dim,reduced_size,grf_ndarray_get_elemtype(array));break;
    case GRF_MULT: reduced = grf_ndarray_ones(reduced_dim,reduced_size,grf_ndarray_get_elemtype(array));break;
    case GRF_MAX:  reduced = grf_ndarray_new_with_size_type(reduced_dim, reduced_size, grf_ndarray_get_elemtype(array));
                      grf_ndarray_fill_min(reduced); break;
    case GRF_MIN:  reduced = grf_ndarray_new_with_size_type(reduced_dim, reduced_size, grf_ndarray_get_elemtype(array));
                      grf_ndarray_fill_max(reduced); break;
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
    case GRF_SUM:
    case GRF_MEAN:
        switch(array->type){
          case GRF_UINT8:  reduced->data_uint8[reduced_1d]  += array->data_uint8[original_1d] ;break;
          case GRF_UINT16: reduced->data_uint16[reduced_1d] += array->data_uint16[original_1d];break;
          case GRF_UINT32: reduced->data_uint32[reduced_1d] += array->data_uint32[original_1d];break;
          case GRF_UINT64: reduced->data_uint64[reduced_1d] += array->data_uint64[original_1d];break;
          case GRF_INT8:   reduced->data_int8[reduced_1d]   += array->data_int8[original_1d]  ;break;
          case GRF_INT16:  reduced->data_int16[reduced_1d]  += array->data_int16[original_1d] ;break;
          case GRF_INT32:  reduced->data_int32[reduced_1d]  += array->data_int32[original_1d] ;break;
          case GRF_INT64:  reduced->data_int64[reduced_1d]  += array->data_int64[original_1d] ;break;
          case GRF_FLOAT:  reduced->data_float[reduced_1d]  += array->data_float[original_1d] ;break;
          case GRF_DOUBLE: reduced->data_double[reduced_1d] += array->data_double[original_1d];break;
        }
        break;
    case GRF_MIN:
        switch(array->type){
          case GRF_UINT8:  reduced->data_uint8[reduced_1d]  = min(reduced->data_uint8[reduced_1d],array->data_uint8[original_1d]) ;break;
          case GRF_UINT16: reduced->data_uint16[reduced_1d] = min(reduced->data_uint16[reduced_1d],array->data_uint16[original_1d]);break;
          case GRF_UINT32: reduced->data_uint32[reduced_1d] = min(reduced->data_uint32[reduced_1d],array->data_uint32[original_1d]);break;
          case GRF_UINT64: reduced->data_uint64[reduced_1d] = min(reduced->data_uint64[reduced_1d],array->data_uint64[original_1d]);break;
          case GRF_INT8:   reduced->data_int8[reduced_1d]   = min(reduced->data_int8[reduced_1d],array->data_int8[original_1d])  ;break;
          case GRF_INT16:  reduced->data_int16[reduced_1d]  = min(reduced->data_int16[reduced_1d],array->data_int16[original_1d]) ;break;
          case GRF_INT32:  reduced->data_int32[reduced_1d]  = min(reduced->data_int32[reduced_1d],array->data_int32[original_1d]) ;break;
          case GRF_INT64:  reduced->data_int64[reduced_1d]  = min(reduced->data_int64[reduced_1d],array->data_int64[original_1d]) ;break;
          case GRF_FLOAT:  reduced->data_float[reduced_1d]  = min(reduced->data_float[reduced_1d],array->data_float[original_1d]) ;break;
          case GRF_DOUBLE: reduced->data_double[reduced_1d] = min(reduced->data_double[reduced_1d],array->data_double[original_1d]);break;
        }
        break;
    case GRF_MAX:
      switch(array->type){
        case GRF_UINT8:  reduced->data_uint8[reduced_1d]  = max(reduced->data_uint8[reduced_1d],array->data_uint8[original_1d]) ;break;
        case GRF_UINT16: reduced->data_uint16[reduced_1d] = max(reduced->data_uint16[reduced_1d],array->data_uint16[original_1d]);break;
        case GRF_UINT32: reduced->data_uint32[reduced_1d] = max(reduced->data_uint32[reduced_1d],array->data_uint32[original_1d]);break;
        case GRF_UINT64: reduced->data_uint64[reduced_1d] = max(reduced->data_uint64[reduced_1d],array->data_uint64[original_1d]);break;
        case GRF_INT8:   reduced->data_int8[reduced_1d]   = max(reduced->data_int8[reduced_1d],array->data_int8[original_1d])  ;break;
        case GRF_INT16:  reduced->data_int16[reduced_1d]  = max(reduced->data_int16[reduced_1d],array->data_int16[original_1d]) ;break;
        case GRF_INT32:  reduced->data_int32[reduced_1d]  = max(reduced->data_int32[reduced_1d],array->data_int32[original_1d]) ;break;
        case GRF_INT64:  reduced->data_int64[reduced_1d]  = max(reduced->data_int64[reduced_1d],array->data_int64[original_1d]) ;break;
        case GRF_FLOAT:  reduced->data_float[reduced_1d]  = max(reduced->data_float[reduced_1d],array->data_float[original_1d]) ;break;
        case GRF_DOUBLE: reduced->data_double[reduced_1d] = max(reduced->data_double[reduced_1d],array->data_double[original_1d]);break;
      }
      break;
    case GRF_MULT:
      switch(array->type){
        case GRF_UINT8:  reduced->data_uint8[reduced_1d]  *= array->data_uint8[original_1d] ;break;
        case GRF_UINT16: reduced->data_uint16[reduced_1d] *= array->data_uint16[original_1d];break;
        case GRF_UINT32: reduced->data_uint32[reduced_1d] *= array->data_uint32[original_1d];break;
        case GRF_UINT64: reduced->data_uint64[reduced_1d] *= array->data_uint64[original_1d];break;
        case GRF_INT8:   reduced->data_int8[reduced_1d]   *= array->data_int8[original_1d]  ;break;
        case GRF_INT16:  reduced->data_int16[reduced_1d]  *= array->data_int16[original_1d] ;break;
        case GRF_INT32:  reduced->data_int32[reduced_1d]  *= array->data_int32[original_1d] ;break;
        case GRF_INT64:  reduced->data_int64[reduced_1d]  *= array->data_int64[original_1d] ;break;
        case GRF_FLOAT:  reduced->data_float[reduced_1d]  *= array->data_float[original_1d] ;break;
        case GRF_DOUBLE: reduced->data_double[reduced_1d] *= array->data_double[original_1d];break;
      }
      break;
    case GRF_STD:
      break;
    }
  }

  if(operation == GRF_MEAN){
    grf_ndarray_divide_scalar(reduced, num_reduced);
  }

  free(reduced_size);
  free(reduced_ind);
  free(original_indices);

  //
  return reduced;
}

void grf_ndarray_squeeze(GrfNDArray* array){
  uint16_t current_pos = 0,i;
  for(i = current_pos; i < array->dim; i++){
    if(array->size[i] != 1) array->size[current_pos++] = array->size[i];
  }
  array->dim = current_pos;
  array->size = realloc(array->size, current_pos * sizeof(uint32_t));
}

void grf_ndarray_squeeze_axis(GrfNDArray* array, uint8_t num_axis, uint16_t* axis){
  uint16_t current_pos = axis[0], current_axis_pos = 0,i;
  for(i = current_pos; i < array->dim; i++){
    // Copy whether non single-dimension or not indicated by axis
    if(current_axis_pos       == num_axis || // Traversed all indicated axis?
       array->size[i]         != 1 ||        // Non single-dimension?
       axis[current_axis_pos] != i)          // Is not an indicated axis?
      array->size[current_pos++] = array->size[i];
    // If indicated, don't copy, just see next indicated axis
    else if(i == axis[current_axis_pos])
      current_axis_pos++;
  }
  array->dim = current_pos;
  array->size = realloc(array->size, current_pos * sizeof(uint32_t));
}

GrfNDArray* grf_ndarray_sum_scalar(GrfNDArray *array, double value){
  GrfNDArray* new_array = grf_ndarray_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  + value; break;
      case GRF_UINT16: new_array->data_uint16[i] = array->data_uint16[i] + value; break;
      case GRF_UINT32: new_array->data_uint32[i] = array->data_uint32[i] + value; break;
      case GRF_UINT64: new_array->data_uint64[i] = array->data_uint64[i] + value; break;
      case GRF_INT8:   new_array->data_int8[i]   = array->data_int8[i]   + value; break;
      case GRF_INT16:  new_array->data_int16[i]  = array->data_int16[i]  + value; break;
      case GRF_INT32:  new_array->data_int32[i]  = array->data_int32[i]  + value; break;
      case GRF_INT64:  new_array->data_int64[i]  = array->data_int64[i]  + value; break;
      case GRF_FLOAT:  new_array->data_float[i]  = array->data_float[i]  + value; break;
      case GRF_DOUBLE: new_array->data_double[i] = array->data_double[i] + value; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_subtract_scalar(GrfNDArray *array, double value){
  GrfNDArray* new_array = grf_ndarray_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  - value; break;
      case GRF_UINT16: new_array->data_uint16[i] = array->data_uint16[i] - value; break;
      case GRF_UINT32: new_array->data_uint32[i] = array->data_uint32[i] - value; break;
      case GRF_UINT64: new_array->data_uint64[i] = array->data_uint64[i] - value; break;
      case GRF_INT8:   new_array->data_int8[i]   = array->data_int8[i]   - value; break;
      case GRF_INT16:  new_array->data_int16[i]  = array->data_int16[i]  - value; break;
      case GRF_INT32:  new_array->data_int32[i]  = array->data_int32[i]  - value; break;
      case GRF_INT64:  new_array->data_int64[i]  = array->data_int64[i]  - value; break;
      case GRF_FLOAT:  new_array->data_float[i]  = array->data_float[i]  - value; break;
      case GRF_DOUBLE: new_array->data_double[i] = array->data_double[i] - value; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_mult_scalar(GrfNDArray *array, double value){
  GrfNDArray* new_array = grf_ndarray_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  * value; break;
      case GRF_UINT16: new_array->data_uint16[i] = array->data_uint16[i] * value; break;
      case GRF_UINT32: new_array->data_uint32[i] = array->data_uint32[i] * value; break;
      case GRF_UINT64: new_array->data_uint64[i] = array->data_uint64[i] * value; break;
      case GRF_INT8:   new_array->data_int8[i]   = array->data_int8[i]   * value; break;
      case GRF_INT16:  new_array->data_int16[i]  = array->data_int16[i]  * value; break;
      case GRF_INT32:  new_array->data_int32[i]  = array->data_int32[i]  * value; break;
      case GRF_INT64:  new_array->data_int64[i]  = array->data_int64[i]  * value; break;
      case GRF_FLOAT:  new_array->data_float[i]  = array->data_float[i]  * value; break;
      case GRF_DOUBLE: new_array->data_double[i] = array->data_double[i] * value; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_divide_scalar(GrfNDArray *array, double value){
  GrfNDArray* new_array = grf_ndarray_new_like(array);
  uint64_t i;
  for(i = 0; i < array->num_elements; i++){
    switch(array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array->data_uint8[i]  / value; break;
      case GRF_UINT16: new_array->data_uint16[i] = array->data_uint16[i] / value; break;
      case GRF_UINT32: new_array->data_uint32[i] = array->data_uint32[i] / value; break;
      case GRF_UINT64: new_array->data_uint64[i] = array->data_uint64[i] / value; break;
      case GRF_INT8:   new_array->data_int8[i]   = array->data_int8[i]   / value; break;
      case GRF_INT16:  new_array->data_int16[i]  = array->data_int16[i]  / value; break;
      case GRF_INT32:  new_array->data_int32[i]  = array->data_int32[i]  / value; break;
      case GRF_INT64:  new_array->data_int64[i]  = array->data_int64[i]  / value; break;
      case GRF_FLOAT:  new_array->data_float[i]  = array->data_float[i]  / value; break;
      case GRF_DOUBLE: new_array->data_double[i] = array->data_double[i] / value; break;
    }
  }
  return new_array;
}

GrfNDArray* grf_ndarray_sum(GrfNDArray *array1, GrfNDArray *array2){
  return grf_ndarray_sum_to(array1, array2, NULL);
}
GrfNDArray* grf_ndarray_subtract(GrfNDArray *array1, GrfNDArray *array2){
  return grf_ndarray_subtract_to(array1, array2, NULL);
}
GrfNDArray* grf_ndarray_mult(GrfNDArray *array1, GrfNDArray *array2){
  return grf_ndarray_mult_to(array1, array2, NULL);
}
GrfNDArray* grf_ndarray_divide(GrfNDArray *array1, GrfNDArray *array2){
  return grf_ndarray_divide_to(array1, array2, NULL);
}
long double grf_ndarray_reduce_sum_num(GrfNDArray* array){
  long double sum = 0;
  uint64_t i;
  for(i = 0; i < array->num_elements; i++)
    sum += grf_ndarray_get_long_double_1D(array,i);
  return sum;
}
long double grf_ndarray_reduce_max_num(GrfNDArray* array){
  long double num_max = __LDBL_MIN__;
  uint64_t i;
  for(i = 0; i < array->num_elements; i++)
    num_max = max(num_max,grf_ndarray_get_long_double_1D(array,i));
  return num_max;
}
long double grf_ndarray_reduce_min_num(GrfNDArray* array){
  long double num_min = __LDBL_MAX__;
  uint64_t i;
  for(i = 0; i < array->num_elements; i++)
    num_min = min(num_min,grf_ndarray_get_long_double_1D(array,i));
  return num_min;
}

GrfNDArray* grf_ndarray_sum_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array){
  if(!new_array) new_array = grf_ndarray_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  + array2->data_uint8[i];  break;
      case GRF_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] + array2->data_uint16[i]; break;
      case GRF_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] + array2->data_uint32[i]; break;
      case GRF_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] + array2->data_uint64[i]; break;
      case GRF_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   + array2->data_int8[i];   break;
      case GRF_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  + array2->data_int16[i];  break;
      case GRF_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  + array2->data_int32[i];  break;
      case GRF_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  + array2->data_int64[i];  break;
      case GRF_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  + array2->data_float[i];  break;
      case GRF_DOUBLE: new_array->data_double[i] = array1->data_double[i] + array2->data_double[i]; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_subtract_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array){
  if(!new_array) new_array = grf_ndarray_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  - array2->data_uint8[i];  break;
      case GRF_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] - array2->data_uint16[i]; break;
      case GRF_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] - array2->data_uint32[i]; break;
      case GRF_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] - array2->data_uint64[i]; break;
      case GRF_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   - array2->data_int8[i];   break;
      case GRF_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  - array2->data_int16[i];  break;
      case GRF_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  - array2->data_int32[i];  break;
      case GRF_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  - array2->data_int64[i];  break;
      case GRF_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  - array2->data_float[i];  break;
      case GRF_DOUBLE: new_array->data_double[i] = array1->data_double[i] - array2->data_double[i]; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_mult_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array){
  if(!new_array) new_array = grf_ndarray_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  * array2->data_uint8[i];  break;
      case GRF_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] * array2->data_uint16[i]; break;
      case GRF_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] * array2->data_uint32[i]; break;
      case GRF_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] * array2->data_uint64[i]; break;
      case GRF_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   * array2->data_int8[i];   break;
      case GRF_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  * array2->data_int16[i];  break;
      case GRF_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  * array2->data_int32[i];  break;
      case GRF_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  * array2->data_int64[i];  break;
      case GRF_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  * array2->data_float[i];  break;
      case GRF_DOUBLE: new_array->data_double[i] = array1->data_double[i] * array2->data_double[i]; break;
    }
  }
  return new_array;
}
GrfNDArray* grf_ndarray_divide_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array){
  if(!new_array) new_array = grf_ndarray_new_like(array1);
  uint64_t i;
  for(i = 0; i < new_array->num_elements; i++){
    switch(new_array->type){
      case GRF_UINT8:  new_array->data_uint8[i]  = array1->data_uint8[i]  / array2->data_uint8[i];  break;
      case GRF_UINT16: new_array->data_uint16[i] = array1->data_uint16[i] / array2->data_uint16[i]; break;
      case GRF_UINT32: new_array->data_uint32[i] = array1->data_uint32[i] / array2->data_uint32[i]; break;
      case GRF_UINT64: new_array->data_uint64[i] = array1->data_uint64[i] / array2->data_uint64[i]; break;
      case GRF_INT8:   new_array->data_int8[i]   = array1->data_int8[i]   / array2->data_int8[i];   break;
      case GRF_INT16:  new_array->data_int16[i]  = array1->data_int16[i]  / array2->data_int16[i];  break;
      case GRF_INT32:  new_array->data_int32[i]  = array1->data_int32[i]  / array2->data_int32[i];  break;
      case GRF_INT64:  new_array->data_int64[i]  = array1->data_int64[i]  / array2->data_int64[i];  break;
      case GRF_FLOAT:  new_array->data_float[i]  = array1->data_float[i]  / array2->data_float[i];  break;
      case GRF_DOUBLE: new_array->data_double[i] = array1->data_double[i] / array2->data_double[i]; break;
    }
  }
  return new_array;
}

double grf_ndarray_norm_difference(GrfNDArray* array1, GrfNDArray* array2, GrfNormType norm_type){
  long double value1;
  long double value2;
  long double value;
  double accum = 0;
  uint64_t i;
  for(i = 0; i < array1->num_elements;i++){
    value1  = grf_ndarray_get_long_double_1D(array1,i);
    value2  = grf_ndarray_get_long_double_1D(array2,i);
    value   = fabs(value2-value1);
    switch(norm_type){
      case GRF_NORM_L1:    accum += value;break;
      case GRF_NORM_L2:
      case GRF_NORM_L2SQR: accum += value*value;break;
      case GRF_NORM_INF:   accum  = max(accum, value); break;
    default: break;
    }
  }
  if(norm_type == GRF_NORM_L2) accum = sqrt(accum);
  return accum;
}


/*-----------------------------------
 *       ARRAY IO FUNCTIONS
 *-----------------------------------*/
static GrfNDArray* grf_ndarray_read_csv_type_minimum(const char* filename, GrfDataType type, uint8_t automatic){
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
  uint32_t size[2]     = {0,0};// GrfNDArray size
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
    // Get type of GrfNDArray
    if(is_integer){
      if(is_unsigned){
        if(maximum <= __UINT64_MAX__){
          type = GRF_UINT64;
          if(maximum <= __UINT32_MAX__){
            type = GRF_UINT32;
            if(maximum <= __UINT16_MAX__){
              type = GRF_UINT16;
              if(maximum <= __UINT8_MAX__){
                type = GRF_UINT8;
              }
            }
          }
        }
      }
      else{
        if(maximum <= __INT64_MAX__){
          type = GRF_INT64;
          if(maximum <= __INT32_MAX__){
            type = GRF_INT32;
            if(maximum <= __INT16_MAX__){
              type = GRF_INT16;
              if(maximum <= __INT8_MAX__){
                type = GRF_INT8;
              }
            }
          }
        }
      }
    }
    else{
      if(maximum <= __FLT_MAX__)
        type = GRF_FLOAT;
      else
        type = GRF_DOUBLE;
    }
  }

  // Create the array
  GrfNDArray* array = grf_ndarray_new_with_size_type(dim, size, type);
  for(i = 0; i < array->num_elements; i++){
    switch(type){
      case GRF_UINT8:  array->data_uint8[i]  = (uint8_t)  data[i];break;
      case GRF_UINT16: array->data_uint16[i] = (uint16_t) data[i];break;
      case GRF_UINT32: array->data_uint32[i] = (uint32_t) data[i];break;
      case GRF_UINT64: array->data_uint64[i] = (uint64_t) data[i];break;
      case GRF_INT8:   array->data_int8[i]   = (int8_t)   data[i];break;
      case GRF_INT16:  array->data_int16[i]  = (int16_t)  data[i];break;
      case GRF_INT32:  array->data_int32[i]  = (int32_t)  data[i];break;
      case GRF_INT64:  array->data_int64[i]  = (int64_t)  data[i];break;
      case GRF_FLOAT:  array->data_float[i]  = (float)    data[i];break;
      case GRF_DOUBLE: array->data_double[i] = (double)   data[i];break;
    }
  }

  free(data);
  fclose(file);
  return array;
}

GrfNDArray* grf_ndarray_read_csv(const char* filename){
  return grf_ndarray_read_csv_type_minimum(filename, GRF_UINT32, 1);
}
GrfNDArray* grf_ndarray_read_csv_type(const char* filename, GrfDataType type){
  return grf_ndarray_read_csv_type_minimum(filename, type, 0);
}

void grf_ndarray_write_csv(GrfNDArray* array, const char* filename){
  FILE* file = fopen(filename, "w");
  char* line;
  if(array->dim == 1) line = grf_ndarray_join(array, ";");
  else                line = grf_ndarray_join(array,"\n;");
  fprintf(file,line);
  free(line);
  fclose(file);
}

char* grf_ndarray_join(GrfNDArray* array, const char* delimiters){
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
      case GRF_UINT8:  cur_length += sprintf(cur_text,"%hhu%c",array->data_uint8[i] , delimiters[d]);break;
      case GRF_UINT16: cur_length += sprintf(cur_text,"%hu%c" ,array->data_uint16[i], delimiters[d]);break;
      case GRF_UINT32: cur_length += sprintf(cur_text,"%u%c"  ,array->data_uint32[i], delimiters[d]);break;
      case GRF_UINT64: cur_length += sprintf(cur_text,"%lu%c" ,array->data_uint64[i], delimiters[d]);break;
      case GRF_INT8:   cur_length += sprintf(cur_text,"%hhd%c",array->data_int8[i]  , delimiters[d]);break;
      case GRF_INT16:  cur_length += sprintf(cur_text,"%hd%c" ,array->data_int16[i] , delimiters[d]);break;
      case GRF_INT32:  cur_length += sprintf(cur_text,"%d%c"  ,array->data_int32[i] , delimiters[d]);break;
      case GRF_INT64:  cur_length += sprintf(cur_text,"%ld%c" ,array->data_int64[i] , delimiters[d]);break;
      case GRF_FLOAT:  cur_length += sprintf(cur_text,"%f%c"  ,array->data_float[i] , delimiters[d]);break;
      case GRF_DOUBLE: cur_length += sprintf(cur_text,"%lf%c" ,array->data_double[i], delimiters[d]);break;
    }
    cur_text    = text + cur_length;
  }
  // Remove last delimiter
  cur_text--;cur_length--;
  *cur_text = '\0';
  text = realloc(text,cur_length);
  return text;
}

void
grf_ndarray_filter(GrfNDArray*          array,
                 GrfBoolDataFunc comparison_function,
                 GrfNDArray**         result_indices,
                 GrfNDArray**         result_values,
                 void*              user_data){
  uint64_t total   = 1024;
  void* values  = malloc(total * array->bitsize);
  uint64_t* indices = malloc(total * sizeof(uint64_t));
  uint64_t j,i;
  // Get array of values and indices of filtered pixels
  for(i = 0, j = 0; i < array->num_elements; i++)
    if(comparison_function(grf_ndarray_get_element_1D(array,i),user_data)){
      switch(array->type){
        case GRF_UINT8:  ((uint8_t*) values)[j] = array->data_uint8[i];break;
        case GRF_UINT16: ((uint16_t*)values)[j] = array->data_uint16[i];break;
        case GRF_UINT32: ((uint32_t*)values)[j] = array->data_uint32[i];break;
        case GRF_UINT64: ((uint64_t*)values)[j] = array->data_uint64[i];break;
        case GRF_INT8:   ((int8_t*)  values)[j] = array->data_int8[i];break;
        case GRF_INT16:  ((int16_t*) values)[j] = array->data_int16[i];break;
        case GRF_INT32:  ((int32_t*) values)[j] = array->data_int32[i];break;
        case GRF_INT64:  ((int64_t*) values)[j] = array->data_int64[i];break;
        case GRF_FLOAT:  ((float*)   values)[j] = array->data_float[i];break;
        case GRF_DOUBLE: ((double*)  values)[j] = array->data_double[i];break;
      }
      indices[j] = i;
      j++;
      if(j == total){
        total <<= 1;
        values  = realloc(values,total * array->bitsize);
        indices = realloc(values,total * sizeof(uint64_t));
      }
    }

  // Remove empty space caused by duplication of memory
  values  = realloc(values,j * array->bitsize);
  indices = realloc(indices,j * sizeof(uint64_t));

  // Create GrfNDArray for values and indices
  *result_indices = grf_ndarray_new_with_dim(1);
  (*result_indices)->data_uint64 = indices;
  (*result_indices)->type    = GRF_UINT64;
  (*result_indices)->size[0] = j;
  (*result_indices)->bitsize = sizeof(uint64_t);
  (*result_indices)->contiguous = 1;
  (*result_indices)->num_elements = j;
  (*result_indices)->num_bytes = j * sizeof(uint64_t);
  (*result_indices)->owns_data = 1;
  (*result_indices)->step[0] = 1;


  *result_values  = grf_ndarray_new_with_dim(1);
  (*result_values)->data = values;
  (*result_values)->type    = array->type;
  (*result_values)->size[0] = j;
  (*result_values)->bitsize = array->bitsize;
  (*result_values)->contiguous = 1;
  (*result_values)->num_elements = j;
  (*result_values)->num_bytes = j * array->bitsize;
  (*result_values)->owns_data = 1;
  (*result_values)->step[0] = 1;
}
/*-----------------------------------
 *     GEOMETRIC TRANSFORMATIONS
 *-----------------------------------*/
GrfNDArray*
grf_ndarray_eye(uint32_t size, GrfDataType type){
  uint32_t* sizes = (uint32_t*)malloc(2*sizeof(uint32_t));
  sizes[0] = size;sizes[1] = size;
  GrfNDArray* array = grf_ndarray_zeros(2,sizes,type);
  uint32_t i;
  for(i = 0; i < size; i++) grf_ndarray_set_element_1D(array,i*size+i,1.0f);
  free(sizes);
  return array;
}

GrfNDArray*
grf_ndarray_translate(GrfNDArray* array, GrfNDArray* offset){
  GrfNDArray* trans = grf_ndarray_copy(array);
  uint32_t size = array->size[0]-1;
  uint32_t index,i;
  long double sum;
  for(i = 0; i < size; i++) {
    index = (i+1)*(size+1)-1;
    sum   = grf_ndarray_get_long_double_1D(array,index)+grf_ndarray_get_long_double_1D(offset,i);
    grf_ndarray_set_element_1D(trans,index,sum);
  }
  return trans;
}

GrfNDArray*
grf_ndarray_translate_4f(GrfNDArray* array, float x, float y, float z){
  GrfNDArray *trans;
  if(array) trans = grf_ndarray_copy(array);
  else      trans = grf_ndarray_eye(4,GRF_FLOAT);
  trans->data_float[ 3] += x;
  trans->data_float[ 7] += y;
  trans->data_float[11] += z;
  return trans;
}

void
grf_ndarray_transpose(GrfNDArray* array){
  long double tmp;
  uint64_t size = array->size[0];
  uint64_t index2 = size;
  uint64_t index1 = 1, y, x;

  for(y = 1, index2=size; y < size; y++){
    for(x = 0, index1=y,index2=y*size; x < y;  x++, index1 += size, index2++){
      tmp = grf_ndarray_get_long_double_1D(array,index1);
      grf_ndarray_set_element_1D(array,index1,grf_ndarray_get_long_double_1D(array,index2));
      grf_ndarray_set_element_1D(array,index2,tmp);
    }
  }
}

GrfNDArray*
grf_ndarray_scale(GrfNDArray* array, GrfNDArray* scale){
  GrfNDArray* scaled = grf_ndarray_copy(array);
  uint32_t size = array->size[0]-1;
  uint32_t i, index = 0;
  long double value = 0;
  for(i = 0; i < size; i++,index += size+1){
    value = grf_ndarray_get_long_double_1D(array,index)*grf_ndarray_get_long_double_1D(scale,i);
    grf_ndarray_set_element_1D(scaled,index,value);
  }
  return scaled;
}

GrfNDArray*
grf_ndarray_scale_4f(GrfNDArray* array, float x, float y, float z){
  GrfNDArray *scaled;
  if(array) scaled = grf_ndarray_copy(array);
  else      scaled = grf_ndarray_eye(4,GRF_FLOAT);
  scaled->data_float[ 0] *= x;
  scaled->data_float[ 5] *= y;
  scaled->data_float[10] *= z;
  return scaled;
}

GrfNDArray*
grf_ndarray_ortho(float left,float right,float bottom,float top,float near,float far){
  GrfNDArray* ortho = grf_ndarray_eye(4,GRF_FLOAT);
  ortho->data_float[0]  =  2.0f/(right-left);
  ortho->data_float[5]  =  2.0f/(top-bottom);
  ortho->data_float[10] = -2.0f/(far-near);
  ortho->data_float[3]  = -(right+left)/(right-left);
  ortho->data_float[7]  = -(top+bottom)/(top-bottom);
  ortho->data_float[11] = -(far+near  )/(far-near);
  return ortho;
}

GrfNDArray*
grf_ndarray_perspective(float fov, float aspect, float near, float far){
  float tanHalfFovy = tan(fov/2.0f);
  uint32_t  size[2]     = {4,4};
  GrfNDArray* perspective = grf_ndarray_zeros(2,size,GRF_FLOAT);
  perspective->data_float[0 ] = 1.0f/(aspect*tanHalfFovy);
  perspective->data_float[5 ] = 1.0f/(tanHalfFovy);
  perspective->data_float[10] = -(far+near)/(far-near);
  perspective->data_float[14] = -1.0f;
  perspective->data_float[11] = -(2.0f*far*near)/(far-near);
  return perspective;
}

GrfNDArray*
grf_ndarray_dot(GrfNDArray* array, GrfNDArray* array2){
  GrfNDArray* result = grf_ndarray_zeros_like(array);
  float* data1  =  array->data_float;
  float* data2  = array2->data_float;
  float* datar  = result->data_float;
  uint32_t size = array->size[0];
  uint64_t index = 0,index1 = 0, index2;
  uint32_t i, x, y;
  for(y = 0; y < size; y++){
    for(x = 0; x < size; x++, index++){
      index1 = y*size;
      index2 = x;
      for(i = 0; i < size; i++){
        datar[index] += data1[index1] * data2[index2];
        index1++;
        index2 += size;
      }
    }
  }
  return result;
}
GrfNDArray*
grf_ndarray_normalize(GrfNDArray* array){
  GrfNDArray* normalized = grf_ndarray_copy(array);

  float* d = array->data_float;
  float* n = normalized->data_float;
  // Get norm
  float norm = sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
  // Divide normalized by norm
  n[0] /= norm;
  n[1] /= norm;
  n[2] /= norm;
  return normalized;
}

GrfNDArray*
grf_ndarray_rotate_4f(GrfNDArray* array, float angle_rad, float x,float y,float z){
  float a            = angle_rad;
  float c            = cos(a);
  float cc           = 1-c;
  float s            = sin(a);

  uint32_t  size     = 3;
  uint32_t  sizes[2] = {4,4};
  float     ve[3]    = {x,y,z};
  GrfNDArray* axisn    = grf_ndarray_from_data(ve,1,&size,GRF_FLOAT);
  GrfNDArray* axis     = grf_ndarray_normalize(axisn);

  float u  = axis->data_float[0];
  float v  = axis->data_float[1];
  float w  = axis->data_float[2];
  float u2 = u*u;
  float v2 = v*v;
  float w2 = w*w;
  float uv = u*v;
  float uw = u*w;
  float vw = v*w;

  GrfNDArray* rotation = grf_ndarray_zeros(2,sizes,GRF_FLOAT);

  rotation->data_float[0]  = u2+(1-u2)*c;
  rotation->data_float[1]  = uv*cc - w*s;
  rotation->data_float[2]  = uw*cc + v*s;

  rotation->data_float[4]  = uv*cc + w*s;
  rotation->data_float[5]  = v2+(1-v2)*c;
  rotation->data_float[6]  = vw*cc - u*s;

  rotation->data_float[8]  = uw*cc - v*s;
  rotation->data_float[9]  = vw*cc + u*s;
  rotation->data_float[10] = w2+(1-w2)*c;

  rotation->data_float[15] = 1;

  if(!array) return rotation;
  GrfNDArray* result = grf_ndarray_dot(array,rotation);
  grf_ndarray_free(rotation);
  return result;
}

GrfNDArray*
grf_ndarray_cross(GrfNDArray* v1, GrfNDArray* v2){
  float* f1 = v1->data_float;
  float* f2 = v2->data_float;
  GrfNDArray* result = grf_ndarray_zeros_like(v1);
  float* d = result->data_float;
  d[0] = f1[1] * f2[2] - f2[1] * f1[2];
  d[1] = f1[2] * f2[0] - f2[2] * f1[0];
  d[2] = f1[0] * f2[1] - f2[0] * f1[1];
  return result;
}

GrfNDArray*
grf_ndarray_lookat_4f(float px,float py,float pz,
                 float tx,float ty,float tz,
                 float ux,float uy,float uz){
  GrfNDArray* result = grf_ndarray_eye(4,GRF_FLOAT);
  uint32_t size   = 3;

  float up[3]     = {ux   ,uy   ,uz   };
  GrfNDArray* upa    = grf_ndarray_from_data(up,1,&size,GRF_FLOAT);
  float cme[3]    = {tx-px,ty-py,tz-pz};

  GrfNDArray* fn = grf_ndarray_from_data(cme,1,&size,GRF_FLOAT);
  GrfNDArray* f  = grf_ndarray_normalize(fn);
  float*    ff = f->data_float;

  GrfNDArray* sn = grf_ndarray_cross(f,upa);
  GrfNDArray* s  = grf_ndarray_normalize(sn);
  float*    sf = s->data_float;

  GrfNDArray* un = grf_ndarray_cross(s,f);
  GrfNDArray* u  = grf_ndarray_normalize(un);
  float*    uf = u->data_float;

  float* d = result->data_float;
  d[0 ] = sf[0];
  d[1 ] = sf[1];
  d[2 ] = sf[2];

  d[4 ] = uf[0];
  d[5 ] = uf[1];
  d[6 ] = uf[2];

  d[8 ] = -ff[0];
  d[9 ] = -ff[1];
  d[10] = -ff[2];

  d[3]  = -(sf[0]*px + sf[1]*py + sf[2]*pz);
  d[7]  = -(uf[0]*px + uf[1]*py + uf[2]*pz);
  d[11] =  (ff[0]*px + ff[1]*py + ff[2]*pz);

  grf_ndarray_free(fn);
  grf_ndarray_free(sn);
  grf_ndarray_free(un);
  grf_ndarray_free(f);
  grf_ndarray_free(s);
  grf_ndarray_free(u);
  grf_ndarray_free(upa);

  return result;
}
