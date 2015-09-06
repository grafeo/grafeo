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

/**
 * Array Structure:
 * Each array will have its own data
 * Each array will have its own size vector
 * Support multidimensional arrays (dim > 2)
 * TODO 0.2: make different arrays have same data
 */
typedef struct 
_Array{
    uint16_t  dim;          /**< number of dimensions */
    uint32_t* size;         /**< number of elements in each dimension */
    uint64_t  num_elements; /**< number of elements of the array */
    size_t    num_bytes;    /**< total number of bytes */
    DataType  type;         /**< data type of each element */ 
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
/*-----------------------------------
 *     ARRAY CREATION FUNCTIONS
 *-----------------------------------*/
/**
 * @brief Create a new array: just the structure, without
 * allocating data and size
 * @memberof Array
 * $$\\sum_{i=1}^{N}\\frac{1}{i}$$
 */
Array*    array_new();
/**
 * @brief Create a new array: structure and space for size, without
 * allocating data and defining sizes
 * @memberof Array
 */
Array*    array_new_with_dim(uint16_t dim);
/**
 * @brief Create a new array with size (`sizeof(char)` bytes) and allocated 
 * data, without value.
 * @memberof Array
 */
Array*    array_new_with_size(uint16_t dim, uint32_t* size);
/**
 * @brief Create a new array with size and allocated 
 * data, without value.
 * @memberof Array
 */
Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type);
/**
 * @brief Create a 1D array
 * @memberof Array
 */
Array*    array_new_1D(uint32_t size1);
/**
 * @brief Create a 2D array
 * @memberof Array
 */
Array*    array_new_2D(uint32_t size1, uint32_t size2);
/**
 * @brief Create a 3D array
 * @memberof Array
 */
Array*    array_new_3D(uint32_t size1, uint32_t size2, uint32_t size3);
/**
 * @brief Create a 4D array
 * @memberof Array
 */
Array*    array_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4);
/**
 * @brief Create a 1D array (each elem: sizeof(elementsize) bytes)
 * @memberof Array
 */
Array*    array_new_1D_type(uint32_t size1, DataType type);
/**
 * @brief Create a 2D array (each elem: sizeof(elementsize) bytes)
 * @memberof Array
 */
Array*    array_new_2D_type(uint32_t size1, uint32_t size2, DataType type);
/**
 * @brief Create a 3D array (each elem: sizeof(elementsize) bytes)
 * @memberof Array
 */
Array*    array_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, DataType type);
/**
 * @brief Create a 4D array (each elem: sizeof(elementsize) bytes)
 * @memberof Array
 */
Array*    array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, DataType type);
/**
 * @brief Create a new array filled with zeros (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return Array*
 */
Array*    array_zeros(uint16_t dim, uint32_t* sizes, DataType type);
/**
 * @brief Create a new array filled with ones (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return Array*
 */
Array*    array_ones(uint16_t dim, uint32_t* sizes, DataType type);
/*-----------------------------------
 *   ARRAY OPERATIONS FUNCTIONS
 *-----------------------------------*/
/**
 * Fill existing array with a value
 * @memberof Array
 */
void      array_fill(Array* array, double value);
/**
 * @brief Free array memory
 * @memberof Array
 */
void      array_free(Array* array);
/*-----------------------------------
 *   ARRAY ACCESSOR FUNCTIONS
 *-----------------------------------*/
 /**
 * @brief Get total number of elements of an array
 * @memberof Array
 */
uint64_t  array_get_num_elements(Array* array);
/**
 * @brief Get data type of the array
 * @memberof Array
 */
DataType  array_get_type(Array* array);
/**
 * @brief Get number of dimensions of an array
 * @memberof Array
 */
uint16_t  array_get_dim(Array* array);
/**
 * @brief Get vector of sizes of each dimension of
 *        the array
 * @memberof Array
 */
uint32_t* array_get_size(Array* array);
/**
 * @brief Get data vector of array
 * @memberof Array
 */
void*     array_get_data(Array* array);

#endif