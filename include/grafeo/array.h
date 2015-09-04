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
#ifndef GRAFEO_ARRAY_H
#define GRAFEO_ARRAY_H

#include <grafeo/type.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
_Array{
    uint16_t  dim;
    uint32_t* size;
    uint64_t  num_elements;
    size_t    num_bytes;
    DataType  type;
    union{
        void*          data;
        unsigned char* data_uint8;
        uint16_t*     data_uint16;
        uint32_t*     data_uint32;
        uint64_t*     data_uint64;
        char*          data_int8;
        int16_t*       data_int16;
        int32_t*       data_int32;
        int64_t*       data_int64;
        float*         data_float;
        double*        data_double;
        unsigned int*  data_int;
    };
}Array;

Array*    array_new();
Array*    array_new_with_dim(uint16_t dim);
Array*    array_new_with_size(uint16_t dim, uint32_t* size);
Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type);
Array*    array_new_1D(uint32_t size1);
Array*    array_new_2D(uint32_t size1, uint32_t size2);
Array*    array_new_3D(uint32_t size1, uint32_t size2, uint32_t size3);
Array*    array_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4);
Array*    array_new_1D_type(uint32_t size1, DataType type);
Array*    array_new_2D_type(uint32_t size1, uint32_t size2, DataType type);
Array*    array_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, DataType type);
Array*    array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, DataType type);
Array*    array_zeros(uint16_t dim, uint32_t* sizes, DataType type);
Array*    array_ones(uint16_t dim, uint32_t* sizes, DataType type);
void      array_fill(Array* array, double value);
uint64_t  array_get_num_elements(Array* array);
DataType  array_get_type(Array* array);
uint16_t  array_get_dim(Array* array);
uint32_t* array_get_size(Array* array);
void      array_free(Array* array);
void*     array_get_data(Array* array);

#endif