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
#include <grafeo/range.h>
#include <math.h>
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
    uint8_t   bitsize;      /**< number of bytes of each value*/
    DataType  type;         /**< data type of each element */ 
    uint64_t* step;         /**< number of elements to increase an axis value */ 
    uint8_t   contiguous;   /**< it can be iterated using a common loop */
    uint8_t   owns_data;    /**< owner of its data */
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
 * @brief Create a new array with same type and size as input array
 * @param array
 * @return
 */
Array*    array_new_like(Array* array);
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
 * @brief array_zeros_like
 * @param array
 * @return
 */
Array*    array_zeros_like(Array* array);
/**
 * @brief array_zeros_like_type
 * @param array
 * @param type
 * @return
 */
Array*    array_zeros_like_type(Array* array, DataType type);
/**
 * @brief Create a new array filled with ones (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return Array*
 */
Array*    array_ones(uint16_t dim, uint32_t* sizes, DataType type);
/**
 * @brief array_ones_like
 * @param array
 * @return
 */
Array*    array_ones_like(Array* array);
/**
 * @brief array_ones_like_type
 * @param array
 * @param type
 * @return
 */
Array*    array_ones_like_type(Array* array, DataType type);
/**
 * @brief      Get a submatrix based on ranges
 *
 * @param      array   original array
 * @param      ranges  list of ranges. Use `range_to`,
 *             `range_from`,`range_from_to` or RANGE_ALL
 *
 * @return     Submatrix
 */
Array*    array_sub(Array* array, Range* ranges);
/**
 * @brief Encapsulate data as an Array
 * @param data
 * @param dim
 * @param size
 * @param type
 * @return
 */
Array*    array_from_data(void* data, uint16_t dim, uint32_t* size, DataType type);

/*-----------------------------------
 *   ARRAY OPERATIONS FUNCTIONS
 *-----------------------------------*/
/**
 * Fill existing array with a value
 * @memberof Array
 */
void      array_fill(Array* array, long double value);
/**
 * @brief array_fill_max
 * @param array
 */
void      array_fill_max(Array* array);
/**
 * @brief array_fill_min
 * @param array
 */
void      array_fill_min(Array* array);
/**
 * @brief Free array memory
 * @memberof Array
 */
void      array_free(Array* array);
/**
 * @brief array_sum_scalar
 * @param array
 * @param value
 */
Array*    array_sum_scalar(Array* array, double value);
/**
 * @brief array_subtract_scalar
 * @param array
 * @param value
 */
Array*    array_subtract_scalar(Array* array, double value);
/**
 * @brief array_mult_scalar
 * @param array
 * @param value
 */
Array*    array_mult_scalar(Array* array, double value);
/**
 * @brief array_divide_scalar
 * @param array
 * @param value
 */
Array*    array_divide_scalar(Array* array, double value);
/**
 * @brief array_sum
 * @param array1
 * @param array2
 * @return
 */
Array*    array_sum(Array* array1, Array* array2);
/**
 * @brief array_subtract
 * @param array1
 * @param array2
 * @return
 */
Array*    array_subtract(Array* array1, Array* array2);
/**
 * @brief array_mult
 * @param array1
 * @param array2
 * @return
 */
Array*    array_mult(Array* array1, Array* array2);
/**
 * @brief array_divide
 * @param array1
 * @param array2
 * @return
 */
Array*    array_divide(Array* array1, Array* array2);
/**
 * @brief array_sum_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
Array*    array_sum_to(Array* array1, Array* array2, Array* new_array);
/**
 * @brief array_subtract_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
Array*    array_subtract_to(Array* array1, Array* array2, Array* new_array);
/**
 * @brief array_mult_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
Array*    array_mult_to(Array* array1, Array* array2, Array* new_array);
/**
 * @brief array_divide_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
Array*    array_divide_to(Array* array1, Array* array2, Array* new_array);
/**
 * @brief array_euclidian_distance
 * @param array1
 * @param array2
 * @return
 */
long double array_euclidian_distance(Array* array1, Array* array2);
/**
 * @brief array_square_euclidian_distance
 * @param array1
 * @param array2
 * @return
 */
long double array_square_euclidian_distance(Array* array1, Array* array2);
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
/**
 * @brief      Get an element from an array
 *
 * @param      array    { parameter_description }
 * @param      indices  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
void*     array_get_element(Array* array, uint32_t* indices);
/**
 * @brief array_get_element_1D
 * @param array
 * @param index
 * @return
 */
void*     array_get_element_1D(Array* array, uint64_t index);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t* array_get_step(Array* array);
/**
 * @brief      Get the size in bytes of each value of the array
 *
 * @param      array  Original array
 *
 * @return     Number of bytes of each value
 */
uint8_t   array_get_bitsize(Array* array);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t  array_get_num_bytes(Array* array);
/**
 * @brief array_index
 * @param array
 * @param index
 * @return
 */
int32_t*  array_index(Array* array, int64_t  index);
/**
 * @brief array_index_1D
 * @param array
 * @param indices
 * @return
 */
int64_t   array_index_1D(Array* array, int32_t* indices);
/**
 * @brief array_index_1D_is_valid
 * @param array
 * @param index
 * @return
 */
uint8_t   array_index_1D_is_valid(Array* array, int64_t index);
/**
 * @brief array_index_is_valid
 * @param array
 * @param indices
 * @return
 */
uint8_t   array_index_is_valid(Array* array, int32_t* indices);
/**
 * @brief array_set_element
 * @param array
 * @param indices
 * @param value
 */
void      array_set_element(Array* array, uint32_t* indices, double value);

/*-----------------------------------
 *   ARRAY REDUCTION FUNCTIONS
 *-----------------------------------*/
/**
 * @brief array_reduce
 * @param array
 * @param axes
 * @param size
 * @param operation
 * @return
 */
Array* array_reduce(Array* array, int16_t* axes, uint16_t size, ArrayOperation operation);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array*    array_reduce_sum(Array* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array*    array_reduce_mult(Array* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array*    array_reduce_std(Array* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array*    array_reduce_max(Array* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array*    array_reduce_min(Array* array, int16_t* axes, uint16_t size);


#endif
