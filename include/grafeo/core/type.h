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
#ifndef GRF_TYPE_H
#define GRF_TYPE_H
#include <inttypes.h>
#include <grafeo/core.h>

BEGIN_DECLS

/**
 * @brief Enumeration for data types
 * 
 */
typedef enum _GrfDataType{
  GRF_UINT8 = 0,
  GRF_UINT16,
  GRF_UINT32,
  GRF_UINT64,
  GRF_INT8,
  GRF_INT16,
  GRF_INT32,
  GRF_INT64,
  GRF_FLOAT,
  GRF_DOUBLE,
}GrfDataType;

typedef enum _GrfArrayOperation{
  GRF_SUM = 0,
  GRF_MULT,
  GRF_MAX,
  GRF_MIN,
  GRF_STD,
  GRF_MEAN
}GrfArrayOperation;

typedef enum _GrfColorType{
  GRF_BIN = 0,
  GRF_GRAY,
  GRF_RGB,
  GRF_XYZ,
  GRF_YUV,
  GRF_YCBCR,
  GRF_LUV,
  GRF_LAB,
  GRF_BGR,
  GRF_RGBA,
  GRF_BGRA,
}GrfColorType;

typedef enum _GrfAdjacency{
  GRF_NEIGHBOR_4,
  GRF_NEIGHBOR_6,
  GRF_NEIGHBOR_8,
  GRF_NEIGHBOR_18,
  GRF_NEIGHBOR_26
}GrfAdjacency;

typedef enum _GrfNormType{
  GRF_NORM_L1         = 1,
  GRF_NORM_L2         = 2,
  GRF_NORM_L2SQR      = 3,
  GRF_NORM_INF        = 4,
  GRF_NORM_HAMMING    = 5,
  GRF_NORM_HAMMING2   = 6,
  GRF_NORM_TYPE_MASK  = 6,
  GRF_NORM_RELATIVE   = 7,
  GRF_NORM_MINMAX     = 8,
}GrfNormType;

typedef enum _GrfFittingMode{
  GRF_FITTING_NONE,
  GRF_FITTING_NORMAL,
  GRF_FITTING_FULL,
}GrfFittingMode;

typedef enum _GrfOrientation{
  GRF_ORIENTATION_HORIZONTAL,
  GRF_ORIENTATION_VERTICAL
}GrfOrientation;

typedef enum _GrfSliceType{
  GRF_SLICE_SAGITAL,
  GRF_SLICE_CORONAL,
  GRF_SLICE_AXIAL,
  GRF_SLICE_CUSTOM
}GrfSliceType;

/**
  * @brief Optimization type: Minimization or Maximization
  */
typedef enum _GrfOptimizationType{
  GRF_MINIMIZATION,
  GRF_MAXIMIZATION
} GrfOptimizationType;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef int8_t (*GrfCompareFunc)    (void* data1, void* data2);
typedef int8_t (*GrfCompareDataFunc)(void* data1, void* data2, void* user_data);
typedef uint8_t(*GrfBoolDataFunc)(void* data1, void* user_data);
typedef void (*GrfDataFunc)       (void* data1, void* user_data);

int8_t int64_compare_function_r(void* value1, void* value2);
int8_t int64_compare_function  (void* value1, void* value2);

// Primitive types
typedef uint8_t  grfbool_t;
typedef uint16_t grfdim_t;
typedef uint32_t grfsize_t;
typedef uint64_t grfindex_t;

/**
 * @brief Structure for 2D array of scalar values
 */
typedef struct _GrfScalar2D{
  int x; /**< x position (or first value if viewed as array) */
  int y; /**< y position (or second value if viewed as array) */
}GrfScalar2D;

/**
 * @brief Structure for 4D array of scalar values
 */
typedef struct _GrfScalar4D{
  int x; /**< x position (or first value if viewed as array) */
  int y; /**< y position (or second value if viewed as array) */
  union{int z;int width;int x2;}; /**< z position (or third value if viewed as array) */
  union{int w;int height;int y2;}; /**< w position (or fourth value if viewed as array) */
}GrfScalar4D;
#define grf_scalar4D_new(v1,v2,v3,v4) {v1,v2,{v3},{v4}};
#define grf_scalar4D_fill(var,v1,v2,v3,v4) var.x = v1;var.y = v2;var.z = v3;var.w = v4;
typedef GrfScalar4D GrfRectangle;

/**
 * @brief Structure for 2D array of scalar values
 */
typedef struct _GrfSize2D{
  uint32_t width; /**< width (size in x direction) */
  uint32_t height; /**< height (size in y direction) */
}GrfSize2D;

#define INT8_TO_POINTER(i)   ((void*)(int64_t)(i))
#define INT16_TO_POINTER(i)  ((void*)(int64_t)(i))
#define INT32_TO_POINTER(i)  ((void*)(int64_t)(i))
#define INT64_TO_POINTER(i)  ((void*)(int64_t)(i))
#define UINT8_TO_POINTER(i)  ((void*)(uint64_t)(i))
#define UINT16_TO_POINTER(i) ((void*)(uint64_t)(i))
#define UINT32_TO_POINTER(i) ((void*)(uint64_t)(i))
#define UINT64_TO_POINTER(i) ((void*)(uint64_t)(i))

#define POINTER_TO_INT8(i)   ((int8_t)  (int64_t)(i))
#define POINTER_TO_INT16(i)  ((int16_t) (int64_t)(i))
#define POINTER_TO_INT32(i)  ((int32_t) (int64_t)(i))
#define POINTER_TO_INT64(i)  ((int64_t) (int64_t)(i))
#define POINTER_TO_UINT8(i)  ((uint8_t) (uint64_t)(i))
#define POINTER_TO_UINT16(i) ((uint16_t)(uint64_t)(i))
#define POINTER_TO_UINT32(i) ((uint32_t)(uint64_t)(i))
#define POINTER_TO_UINT64(i) ((uint64_t)(uint64_t)(i))

END_DECLS

#endif
