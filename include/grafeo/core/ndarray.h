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
#ifndef GRF_NDARRAY_H
#define GRF_NDARRAY_H

#include <glib-object.h>
#include <grafeo/core.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

G_BEGIN_DECLS
#define GRF_TYPE_NDARRAY grf_ndarray_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfNDArray, grf_ndarray, GRF, NDARRAY, GObject)

typedef struct _GrfNDArrayClass{
  GObjectClass parent_class;
}GrfNDArrayClass;

/*-----------------------------------
 *     ARRAY CREATION FUNCTIONS
 *-----------------------------------*/
/**
 * @brief Create a new array: just the structure, without
 * allocating data and size
 * @memberof GrfNDArray
 * $$\\sum_{i=1}^{N}\\frac{1}{i}$$
 */
GrfNDArray*    grf_ndarray_new();
/**
 * @brief Create a new array: structure and space for size, without
 * allocating data and defining sizes
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_with_dim(uint16_t dim);
/**
 * @brief Create a new array with size (`sizeof(char)` bytes) and allocated 
 * data, without value.
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_with_size(uint16_t dim, uint32_t* size);
/**
 * @brief Create a new array with size and allocated 
 * data, without value.
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_with_size_type(uint16_t dim, uint32_t* size, GrfDataType type);
/**
 * @brief Create a 1D array
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_1D(uint32_t size1);
/**
 * @brief Create a 2D array
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_2D(uint32_t size1, uint32_t size2);
/**
 * @brief Create a 3D array
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_3D(uint32_t size1, uint32_t size2, uint32_t size3);
/**
 * @brief Create a 4D array
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4);
/**
 * @brief Create a 1D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_1D_type(uint32_t size1, GrfDataType type);
/**
 * @brief Create a 2D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_2D_type(uint32_t size1, uint32_t size2, GrfDataType type);
/**
 * @brief Create a 3D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, GrfDataType type);
/**
 * @brief Create a 4D array (each elem: sizeof(elementsize) bytes)
 * @memberof GrfNDArray
 */
GrfNDArray*    grf_ndarray_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, GrfDataType type);
/**
 * @brief Create a new array with same type and size as input array
 * @param array
 * @return
 */
GrfNDArray*    grf_ndarray_new_like(GrfNDArray* array);
/**
 * @brief Create a new array filled with zeros (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return GrfNDArray*
 */
GrfNDArray*    grf_ndarray_zeros(uint16_t dim, uint32_t* sizes, GrfDataType type);
/**
 * @brief grf_ndarray_zeros_like
 * @param array
 * @return
 */
GrfNDArray*    grf_ndarray_zeros_like(GrfNDArray* array);
/**
 * @brief grf_ndarray_zeros_like_type
 * @param array
 * @param type
 * @return
 */
GrfNDArray*    grf_ndarray_zeros_like_type(GrfNDArray* array, GrfDataType type);
/**
 * @brief Create a new array filled with ones (each elem has type `type`)
 * 
 * @param dim ...
 * @param size ...
 * @param type ...
 * @return GrfNDArray*
 */
GrfNDArray*    grf_ndarray_ones(uint16_t dim, uint32_t* sizes, GrfDataType type);
/**
 * @brief grf_ndarray_ones_like
 * @param array
 * @return
 */
GrfNDArray*    grf_ndarray_ones_like(GrfNDArray* array);
/**
 * @brief grf_ndarray_ones_like_type
 * @param array
 * @param type
 * @return
 */
GrfNDArray*    grf_ndarray_ones_like_type(GrfNDArray* array, GrfDataType type);
/**
 * @brief      Get a submatrix based on ranges
 *
 * @param      array   original array
 * @param      ranges  list of ranges. Use `grf_range_to`,
 *             `grf_range_from`,`grf_range_from_to` or RANGE_ALL
 *
 * @return     Submatrix
 */
GrfNDArray*    grf_ndarray_sub(GrfNDArray* array, GrfRange* ranges);
/**
 * @brief Encapsulate data as an GrfNDArray
 * @param data
 * @param dim
 * @param size
 * @param type
 * @return
 */
GrfNDArray*    grf_ndarray_from_data(void* data, uint16_t dim, uint32_t* size, GrfDataType type);
/**
 * @brief grf_ndarray_as_type
 * @param array
 * @param type
 * @return
 */
GrfNDArray*    grf_ndarray_as_type(GrfNDArray* array, GrfDataType type);
/**
 * @brief grf_ndarray_circular_indices
 * @param dim
 * @param radius
 * @return
 */
GrfNDArray*    grf_ndarray_circular_indices(uint16_t dim, float radius);
/**
 * @brief grf_ndarray_copy
 * @param array
 * @return
 */
GrfNDArray*    grf_ndarray_copy(GrfNDArray* array);
/*-----------------------------------
 *   ARRAY OPERATIONS FUNCTIONS
 *-----------------------------------*/
/**
 * Fill existing array with a value
 * @memberof GrfNDArray
 */
void      grf_ndarray_fill(GrfNDArray* array, long double value);
/**
 * @brief grf_ndarray_fill_max
 * @param array
 */
void      grf_ndarray_fill_max(GrfNDArray* array);
/**
 * @brief grf_ndarray_fill_min
 * @param array
 */
void      grf_ndarray_fill_min(GrfNDArray* array);
/**
 * @brief Free array memory
 * @memberof GrfNDArray
 */
void      grf_ndarray_free(GrfNDArray* array);
/**
 * @brief grf_ndarray_sum_scalar
 * @param array
 * @param value
 */
GrfNDArray*    grf_ndarray_sum_scalar(GrfNDArray* array, double value);
/**
 * @brief grf_ndarray_subtract_scalar
 * @param array
 * @param value
 */
GrfNDArray*    grf_ndarray_subtract_scalar(GrfNDArray* array, double value);
/**
 * @brief grf_ndarray_mult_scalar
 * @param array
 * @param value
 */
GrfNDArray*    grf_ndarray_mult_scalar(GrfNDArray* array, double value);
/**
 * @brief grf_ndarray_divide_scalar
 * @param array
 * @param value
 */
GrfNDArray*    grf_ndarray_divide_scalar(GrfNDArray* array, double value);
/**
 * @brief grf_ndarray_sum
 * @param array1
 * @param array2
 * @return
 */
GrfNDArray*    grf_ndarray_sum(GrfNDArray* array1, GrfNDArray* array2);
/**
 * @brief grf_ndarray_subtract
 * @param array1
 * @param array2
 * @return
 */
GrfNDArray*    grf_ndarray_subtract(GrfNDArray* array1, GrfNDArray* array2);
/**
 * @brief grf_ndarray_mult
 * @param array1
 * @param array2
 * @return
 */
GrfNDArray*    grf_ndarray_mult(GrfNDArray* array1, GrfNDArray* array2);
/**
 * @brief grf_ndarray_divide
 * @param array1
 * @param array2
 * @return
 */
GrfNDArray*    grf_ndarray_divide(GrfNDArray* array1, GrfNDArray* array2);
/**
 * @brief grf_ndarray_sum_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfNDArray*    grf_ndarray_sum_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array);
/**
 * @brief grf_ndarray_subtract_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfNDArray*    grf_ndarray_subtract_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array);
/**
 * @brief grf_ndarray_mult_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfNDArray*    grf_ndarray_mult_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array);
/**
 * @brief grf_ndarray_divide_to
 * @param array1
 * @param array2
 * @param new_array
 * @return
 */
GrfNDArray*    grf_ndarray_divide_to(GrfNDArray* array1, GrfNDArray* array2, GrfNDArray* new_array);
/**
 * @brief grf_ndarray_norm_relative
 * @param array1
 * @param array2
 * @param norm_type
 * @return
 */
double grf_ndarray_norm_difference(GrfNDArray* array1, GrfNDArray* array2, GrfNormType norm_type);
/*-----------------------------------
 *   ARRAY ACCESSOR FUNCTIONS
 *-----------------------------------*/
 /**
 * @brief Get total number of elements of an array
 * @memberof GrfNDArray
 */
uint64_t  grf_ndarray_get_num_elements(GrfNDArray* array);
/**
 * @brief Get data type of the array
 * @memberof GrfNDArray
 */
GrfDataType  grf_ndarray_get_elemtype(GrfNDArray* array);
/**
 * @brief Get number of dimensions of an array
 * @memberof GrfNDArray
 */
uint16_t  grf_ndarray_get_dim(GrfNDArray* array);
/**
 * @brief Get vector of sizes of each dimension of
 *        the array
 * @memberof GrfNDArray
 */
uint32_t* grf_ndarray_get_size(GrfNDArray* array);
/**
 * @brief Get data vector of array
 * @memberof GrfNDArray
 */
void*     grf_ndarray_get_data(GrfNDArray* array);
/**
 * @brief      Get an element from an array
 *
 * @param      array    { parameter_description }
 * @param      indices  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
void*     grf_ndarray_get_element(GrfNDArray* array, uint32_t* indices);
/**
 * @brief grf_ndarray_get_element_1D
 * @param array
 * @param index
 * @return
 */
void*     grf_ndarray_get_element_1D(GrfNDArray* array, uint64_t index);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t* grf_ndarray_get_step(GrfNDArray* array);
/**
 * @brief      Get the size in bytes of each value of the array
 *
 * @param      array  Original array
 *
 * @return     Number of bytes of each value
 */
uint8_t   grf_ndarray_get_bitsize(GrfNDArray* array);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
uint64_t  grf_ndarray_get_num_bytes(GrfNDArray* array);
/**
 * @brief grf_ndarray_index
 * @param array
 * @param index
 * @return
 */
int32_t*  grf_ndarray_index(GrfNDArray* array, int64_t  index);
/**
 * @brief grf_ndarray_index_1D
 * @param array
 * @param indices
 * @return
 */
int64_t   grf_ndarray_index_1D(GrfNDArray* array, int32_t* indices);
/**
 * @brief grf_ndarray_index_1D_is_valid
 * @param array
 * @param index
 * @return
 */
uint8_t   grf_ndarray_index_1D_is_valid(GrfNDArray* array, int64_t index);
/**
 * @brief grf_ndarray_index_is_valid
 * @param array
 * @param indices
 * @return
 */
uint8_t   grf_ndarray_index_is_valid(GrfNDArray* array, int32_t* indices);
/**
 * @brief grf_ndarray_set_element
 * @param array
 * @param indices
 * @param value
 */
void      grf_ndarray_set_element(GrfNDArray* array, uint32_t* indices, double value);
/**
 * @brief grf_ndarray_get_long_double_1D
 * @param array1
 * @param i
 * @return
 */
long double grf_ndarray_get_long_double_1D(GrfNDArray* array1, uint64_t i);
/**
 * @brief grf_ndarray_set_element_1D
 * @param array
 * @param i
 * @param value
 */
void grf_ndarray_set_element_1D(GrfNDArray* array, uint64_t i, double value);
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
GrfNDArray* grf_ndarray_reduce(GrfNDArray* array, int16_t* axes, uint16_t size, GrfNDArrayOperation operation);
/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfNDArray*    grf_ndarray_reduce_sum(GrfNDArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfNDArray*    grf_ndarray_reduce_mult(GrfNDArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfNDArray*    grf_ndarray_reduce_std(GrfNDArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfNDArray*    grf_ndarray_reduce_max(GrfNDArray* array, int16_t* axes, uint16_t size);

/**
 * @brief      { function_description }
 *
 * @param      array  { parameter_description }
 * @param      axes   { parameter_description }
 * @param[in]  size   { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
GrfNDArray*    grf_ndarray_reduce_min(GrfNDArray* array, int16_t* axes, uint16_t size);
/**
 * @brief grf_ndarray_reduce_sum_num
 * @param array
 * @return
 */
long double grf_ndarray_reduce_sum_num(GrfNDArray* array);
/**
 * @brief grf_ndarray_reduce_max_num
 * @param array
 * @return
 */
long double grf_ndarray_reduce_max_num(GrfNDArray* array);
/**
 * @brief grf_ndarray_reduce_min_num
 * @param array
 * @return
 */
long double grf_ndarray_reduce_min_num(GrfNDArray* array);
/**
 * @brief grf_ndarray_squeeze
 * @param array
 */
void grf_ndarray_squeeze(GrfNDArray* array);
/**
 * @brief grf_ndarray_squeeze_axis
 * @param num_axis
 * @param array
 * @param axis
 */
void grf_ndarray_squeeze_axis(GrfNDArray* array, uint8_t num_axis, uint16_t* axis);
/**
 * @brief grf_ndarray_filter
 * @param array
 * @param comparison_function
 * @param result_indices
 * @param result_values
 * @param user_data
 */
void grf_ndarray_filter(GrfNDArray* array, GrfBoolDataFunc comparison_function, GrfNDArray** result_indices, GrfNDArray** result_values, void* user_data);
/*-----------------------------------
 *       ARRAY IO FUNCTIONS
 *-----------------------------------*/
/**
 * @brief grf_ndarray_read_csv
 * @param filename
 * @return
 */
GrfNDArray* grf_ndarray_read_csv(const char* filename);
/**
 * @brief grf_ndarray_read_csv_type
 * @param filename
 * @param type
 * @return
 */
GrfNDArray* grf_ndarray_read_csv_type(const char* filename, GrfDataType type);
/**
 * @brief grf_ndarray_write_csv
 * @param array
 * @param filename
 */
void grf_ndarray_write_csv(GrfNDArray* array, const char* filename);
/**
 * @brief grf_ndarray_join
 * @param array
 * @param delimiters
 * @return
 */
char* grf_ndarray_join(GrfNDArray* array, const char* delimiters);
/*-----------------------------------
 *     GEOMETRIC TRANSFORMATIONS
 *-----------------------------------*/
#define grf_to_rad(angle) (angle / 180.0 * M_PI  )
#define grf_to_deg(angle) (angle * 180.0 * M_1_PI)
/**
 * @brief grf_ndarray_eye
 * @param size
 * @param type
 * @return
 */
GrfNDArray*
grf_ndarray_eye(uint32_t size, GrfDataType type);
/**
 * @brief grf_ndarray_translate
 * @param array
 * @param offset
 * @return
 */
GrfNDArray*
grf_ndarray_translate(GrfNDArray* array, GrfNDArray* offset);
/**
 * @brief grf_ndarray_translate_4f
 * @param array
 * @param x
 * @param y
 * @param z
 * @return
 */
GrfNDArray*
grf_ndarray_translate_4f(GrfNDArray* array, float x, float y, float z);
/**
 * @brief grf_ndarray_transpose
 * @param array
 */
void
grf_ndarray_transpose(GrfNDArray* array);
/**
 * @brief grf_ndarray_scale
 * @param array
 * @param scale
 * @return
 */
GrfNDArray*
grf_ndarray_scale(GrfNDArray* array, GrfNDArray* scale);
/**
 * @brief grf_ndarray_scale_4f
 * @param array
 * @param x
 * @param y
 * @param z
 * @return
 */
GrfNDArray*
grf_ndarray_scale_4f(GrfNDArray* array, float x, float y, float z);
/**
 * @brief grf_ndarray_ortho
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 * @return
 */
GrfNDArray*
grf_ndarray_ortho(float left,float right,float bottom,float top,float near,float far);
/**
 * @brief grf_ndarray_perspective
 * @param fov
 * @param aspect
 * @param near
 * @param far
 * @return
 */
GrfNDArray*
grf_ndarray_perspective(float fov, float aspect, float near, float far);
/**
 * @brief grf_ndarray_dot
 * @param array
 * @param array2
 * @return
 */
GrfNDArray*
grf_ndarray_dot(GrfNDArray* array, GrfNDArray* array2);
/**
 * @brief grf_ndarray_normalize
 * @param array
 * @return
 */
GrfNDArray*
grf_ndarray_normalize(GrfNDArray* array);
/**
 * @brief grf_ndarray_rotate_4f
 * @param array
 * @param angle_rad
 * @param x
 * @param y
 * @param z
 * @return
 */
GrfNDArray*
grf_ndarray_rotate_4f(GrfNDArray* array, float angle_rad, float x,float y,float z);
/**
 * @brief grf_ndarray_cross
 * @param v1
 * @param v2
 * @return
 */
GrfNDArray*
grf_ndarray_cross(GrfNDArray* v1, GrfNDArray* v2);
/**
 * @brief grf_ndarray_lookat_4f
 * @param px
 * @param py
 * @param pz
 * @param tx
 * @param ty
 * @param tz
 * @param ux
 * @param uy
 * @param uz
 * @return
 */
GrfNDArray*
grf_ndarray_lookat_4f(float px,float py,float pz,
                 float tx,float ty,float tz,
                 float ux,float uy,float uz);

G_END_DECLS

#endif
