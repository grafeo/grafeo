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
#ifndef GRF_ARRAY_H
#define GRF_ARRAY_H

#include <grafeo/core.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

BEGIN_DECLS

/**
 * @brief GrfArray Structure
 *
 * Each array will have its own data
 *
 * Each array will have its own size vector
 *
 * Support multidimensional arrays (dim > 2)
 */
typedef struct 
_Array{
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
}GrfArray;
/*-----------------------------------
 *     ARRAY CREATION FUNCTIONS
 *-----------------------------------*/
/**
 * @brief Create a new array: just the structure, without
 * allocating data and size
 * @memberof GrfArray
 * $$\\sum_{i=1}^{N}\\frac{1}{i}$$
 */
GrfArray*    grf_array_new();
/**
 * @brief Create a new array: structure and space for size, without
 * allocating data and defining sizes
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_with_dim(uint16_t dim);
/**
 * @brief Create a new array with size (`sizeof(char)` bytes) and allocated 
 * data, without value.
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_with_size(uint16_t dim, uint32_t* size);
/**
 * @brief Create a new array with size and allocated 
 * data, without value.
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_with_size_type(uint16_t dim, uint32_t* size, GrfDataType type);
/**
 * @brief Create a 1D array
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_1D(uint32_t size1);
/**
 * @brief Create a 2D array
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_2D(uint32_t size1, uint32_t size2);
/**
 * @brief Create a 3D array
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_3D(uint32_t size1, uint32_t size2, uint32_t size3);
/**
 * @brief Create a 4D array
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4);
/**
 * @brief Create a 1D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_1D_type(uint32_t size1, GrfDataType type);
/**
 * @brief Create a 2D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_2D_type(uint32_t size1, uint32_t size2, GrfDataType type);
/**
 * @brief Create a 3D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, GrfDataType type);
/**
 * @brief Create a 4D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfArray
 */
GrfArray*    grf_array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, GrfDataType type);
/**
 * @brief Create a new array with same type and size as input array
 * @param array
 * @return
 */
GrfArray*    grf_array_new_like(GrfArray* array);
/**
 * @brief Create a new array filled with zeros (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return GrfArray*
 */
GrfArray*    grf_array_zeros(uint16_t dim, uint32_t* sizes, GrfDataType type);
/**
 * @brief grf_array_zeros_like
 * @param array
 * @return
 */
GrfArray*    grf_array_zeros_like(GrfArray* array);
/**
 * @brief grf_array_zeros_like_type
 * @param array
 * @param type
 * @return
 */
GrfArray*    grf_array_zeros_like_type(GrfArray* array, GrfDataType type);
/**
 * @brief Create a new array filled with ones (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return GrfArray*
 */
GrfArray*    grf_array_ones(uint16_t dim, uint32_t* sizes, GrfDataType type);
/**
 * @brief grf_array_ones_like
 * @param array
 * @return
 */
GrfArray*    grf_array_ones_like(GrfArray* array);
/**
 * @brief grf_array_ones_like_type
 * @param array
 * @param type
 * @return
 */
GrfArray*    grf_array_ones_like_type(GrfArray* array, GrfDataType type);
/**
 * @brief      Get a submatrix based on ranges
 *
 * @param      array   original array
 * @param      ranges  list of ranges. Use `grf_range_to`,
 *             `grf_range_from`,`grf_range_from_to` or RANGE_ALL
 *
 * @return     Submatrix
 */
GrfArray*    grf_array_sub(GrfArray* array, GrfRange* ranges);
/**
 * @brief Encapsulate data as an GrfArray
 * @param data
 * @param dim
 * @param size
 * @param type
 * @return
 */
GrfArray*    grf_array_from_data(void* data, uint16_t dim, uint32_t* size, GrfDataType type);
/**
 * @brief grf_array_as_type
 * @param array
 * @param type
 * @return
 */
GrfArray*    grf_array_as_type(GrfArray* array, GrfDataType type);
/**
 * @brief grf_array_circular_indices
 * @param dim
 * @param radius
 * @return
 */
GrfArray*    grf_array_circular_indices(uint16_t dim, float radius);
/**
 * @brief grf_array_copy
 * @param array
 * @return
 */
GrfArray*    grf_array_copy(GrfArray* array);
/*-----------------------------------
 *   ARRAY OPERATIONS FUNCTIONS
 *-----------------------------------*/
/**
 * Fill existing array with a value
 * @memberof GrfArray
 */
void      grf_array_fill(GrfArray* array, long double value);
/**
 * @brief grf_array_fill_max
 * @param array
 */
void      grf_array_fill_max(GrfArray* array);
/**
 * @brief grf_array_fill_min
 * @param array
 */
void      grf_array_fill_min(GrfArray* array);
/**
 * @brief Free array memory
 * @memberof GrfArray
 */
void      grf_array_free(GrfArray* array);
/**
 * @brief grf_array_sum_scalar
 * @param array
 * @param value
 */
GrfArray*    grf_array_sum_scalar(GrfArray* array, double value);
/**
 * @brief grf_array_subtract_scalar
 * @param array
 * @param value
 */
GrfArray*    grf_array_subtract_scalar(GrfArray* array, double value);
/**
 * @brief grf_array_mult_scalar
 * @param array
 * @param value
 */
GrfArray*    grf_array_mult_scalar(GrfArray* array, double value);
/**
 * @brief grf_array_divide_scalar
 * @param array
 * @param value
 */
GrfArray*    grf_array_divide_scalar(GrfArray* array, double value);
/**
 * @brief grf_array_sum
 * @param array1
 * @param array2
 * @return
 */
GrfArray*    grf_array_sum(GrfArray* array1, GrfArray* array2);
/**
 * @brief grf_array_subtract
 * @param array1
 * @param array2
 * @return
 */
GrfArray*    grf_array_subtract(GrfArray* array1, GrfArray* array2);
/**
 * @brief grf_array_mult
 * @param array1
 * @param array2
 * @return
 */
GrfArray*    grf_array_mult(GrfArray* array1, GrfArray* array2);
/**
 * @brief grf_array_divide
 * @param array1
 * @param array2
 * @return
 */
GrfArray*    grf_array_divide(GrfArray* array1, GrfArray* array2);
/**
 * @brief grf_array_sum_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfArray*    grf_array_sum_to(GrfArray* array1, GrfArray* array2, GrfArray* new_array);
/**
 * @brief grf_array_subtract_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfArray*    grf_array_subtract_to(GrfArray* array1, GrfArray* array2, GrfArray* new_array);
/**
 * @brief grf_array_mult_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfArray*    grf_array_mult_to(GrfArray* array1, GrfArray* array2, GrfArray* new_array);
/**
 * @brief grf_array_divide_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfArray*    grf_array_divide_to(GrfArray* array1, GrfArray* array2, GrfArray* new_array);
/**
 * @brief grf_array_norm_relative
 * @param array1
 * @param array2
 * @param norm_type
 * @return
 */
double grf_array_norm_difference(GrfArray* array1, GrfArray* array2, GrfNormType norm_type);
/*-----------------------------------
 *   ARRAY ACCESSOR FUNCTIONS
 *-----------------------------------*/
 /**
 * @brief Get total number of elements of an array
 * @memberof GrfArray
 */
uint64_t  grf_array_get_num_elements(GrfArray* array);
/**
 * @brief Get data type of the array
 * @memberof GrfArray
 */
GrfDataType  grf_array_get_type(GrfArray* array);
/**
 * @brief Get number of dimensions of an array
 * @memberof GrfArray
 */
uint16_t  grf_array_get_dim(GrfArray* array);
/**
 * @brief Get vector of sizes of each dimension of
 *        the array
 * @memberof GrfArray
 */
uint32_t* grf_array_get_size(GrfArray* array);
/**
 * @brief Get data vector of array
 * @memberof GrfArray
 */
void*     grf_array_get_data(GrfArray* array);
/**
 * @brief      Get an element from an array
 *
 * @param      array    { parameter_description }
 * @param      indices  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
void*     grf_array_get_element(GrfArray* array, uint32_t* indices);
/**
 * @brief grf_array_get_element_1D
 * @param array
 * @param index
 * @return
 */
void*     grf_array_get_element_1D(GrfArray* array, uint64_t index);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t* grf_array_get_step(GrfArray* array);
/**
 * @brief      Get the size in bytes of each value of the array
 *
 * @param      array  Original array
 *
 * @return     Number of bytes of each value
 */
uint8_t   grf_array_get_bitsize(GrfArray* array);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t  grf_array_get_num_bytes(GrfArray* array);
/**
 * @brief grf_array_index
 * @param array
 * @param index
 * @return
 */
int32_t*  grf_array_index(GrfArray* array, int64_t  index);
/**
 * @brief grf_array_index_1D
 * @param array
 * @param indices
 * @return
 */
int64_t   grf_array_index_1D(GrfArray* array, int32_t* indices);
/**
 * @brief grf_array_index_1D_is_valid
 * @param array
 * @param index
 * @return
 */
uint8_t   grf_array_index_1D_is_valid(GrfArray* array, int64_t index);
/**
 * @brief grf_array_index_is_valid
 * @param array
 * @param indices
 * @return
 */
uint8_t   grf_array_index_is_valid(GrfArray* array, int32_t* indices);
/**
 * @brief grf_array_set_element
 * @param array
 * @param indices
 * @param value
 */
void      grf_array_set_element(GrfArray* array, uint32_t* indices, double value);
/**
 * @brief grf_array_get_long_double_1D
 * @param array1
 * @param i
 * @return
 */
long double grf_array_get_long_double_1D(GrfArray* array1, uint64_t i);
/**
 * @brief grf_array_set_element_1D
 * @param array
 * @param i
 * @param value
 */
void grf_array_set_element_1D(GrfArray* array, uint64_t i, double value);
/*-----------------------------------
 *   ARRAY REDUCTION FUNCTIONS
 *-----------------------------------*/
/**
 * @brief Reduce an array to another array of smaller size (with axes removed)
 *
 * If axes are all axes [0,...,N-1], then an array is reduced to an 1D array with size 1
 *
 * @param array
 * @param axes
 * @param size
 * @param operation
 * @return
 */
GrfArray* grf_array_reduce(GrfArray* array, int16_t* axes, uint16_t size, GrfArrayOperation operation);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfArray*    grf_array_reduce_sum(GrfArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfArray*    grf_array_reduce_mult(GrfArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfArray*    grf_array_reduce_std(GrfArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfArray*    grf_array_reduce_max(GrfArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfArray*    grf_array_reduce_min(GrfArray* array, int16_t* axes, uint16_t size);
/**
 * @brief grf_array_reduce_sum_num
 * @param array
 * @return
 */
long double grf_array_reduce_sum_num(GrfArray* array);
/**
 * @brief grf_array_reduce_max_num
 * @param array
 * @return
 */
long double grf_array_reduce_max_num(GrfArray* array);
/**
 * @brief grf_array_reduce_min_num
 * @param array
 * @return
 */
long double grf_array_reduce_min_num(GrfArray* array);
/**
 * @brief grf_array_squeeze
 * @param array
 */
void grf_array_squeeze(GrfArray* array);
/**
 * @brief grf_array_squeeze_axis
 * @param num_axis
 * @param array
 * @param axis
 */
void grf_array_squeeze_axis(GrfArray* array, uint8_t num_axis, uint16_t* axis);
/**
 * @brief grf_array_filter
 * @param array
 * @param comparison_function
 * @param result_indices
 * @param result_values
 * @param user_data
 */
void grf_array_filter(GrfArray* array, GrfBoolDataFunc comparison_function, GrfArray** result_indices, GrfArray** result_values, void* user_data);
/*-----------------------------------
 *       ARRAY IO FUNCTIONS
 *-----------------------------------*/
/**
 * @brief grf_array_read_csv
 * @param filename
 * @return
 */
GrfArray* grf_array_read_csv(const char* filename);
/**
 * @brief grf_array_read_csv_type
 * @param filename
 * @param type
 * @return
 */
GrfArray* grf_array_read_csv_type(const char* filename, GrfDataType type);
/**
 * @brief grf_array_write_csv
 * @param array
 * @param filename
 */
void grf_array_write_csv(GrfArray* array, const char* filename);
/**
 * @brief grf_array_join
 * @param array
 * @param delimiters
 * @return
 */
char* grf_array_join(GrfArray* array, const char* delimiters);

END_DECLS

#endif
