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
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    array->num_bytes = 0;
    array->bitsize   = 0;
    array->type = GRAFEO_UINT8;
    return array;
}

Array*    array_new_with_dim(uint16_t dim){
    Array* array        = array_new();
    array->dim          = dim;
    array->size         = malloc(sizeof(uint32_t) * dim);
    return array;
}
Array*    array_new_with_size(uint16_t dim, uint32_t* size){
    return array_new_with_size_type(dim, size, GRAFEO_UINT8);
}

Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type){
    Array* array        = array_new_with_dim(dim);
    uint16_t i;
    array->num_elements = 1;
    for(i = 0; i < dim; i++){
        array->size[i] = size[i];
        array->num_elements *= size[i];
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
void      array_free(Array* array){
    if(array->data) free(array->data);
    if(array->size) free(array->size);
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
void*     array_get_element(Array* array, uint32_t* indices){
    uint8_t* x = array->data_uint8;
    for(int i = 0; i < array->dim; i++){
         x += indices[i] * array->step[i] * array->bitsize;
    }
    return x;
}
Array*    array_reduce_min(Array* array, int16_t* axes, uint16_t size){
    return NULL;
}
Array*    array_reduce_max(Array* array, int16_t* axes, uint16_t size){
    return NULL;
}
Array*    array_reduce_std(Array* array, int16_t* axes, uint16_t size){
    return NULL;
}
Array*    array_reduce_mult(Array* array, int16_t* axes, uint16_t size){
    return NULL;
}
Array*    array_reduce_sum(Array* array, int16_t* axes, uint16_t size){
    return NULL;
}