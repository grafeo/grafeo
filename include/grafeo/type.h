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
#ifndef GRAFEO_TYPE_H
#define GRAFEO_TYPE_H
#include <inttypes.h>

/**
 * @brief Enumeration for data types
 * 
 */
typedef enum _DataType{
    GRAFEO_UINT8 = 0,
    GRAFEO_UINT16,
    GRAFEO_UINT32,
    GRAFEO_UINT64,
    GRAFEO_INT8,
    GRAFEO_INT16,
    GRAFEO_INT32,
    GRAFEO_INT64,
    GRAFEO_FLOAT,
    GRAFEO_DOUBLE,
}DataType;

typedef enum _ArrayOperation{
  GRAFEO_SUM = 0,
  GRAFEO_MULT,
  GRAFEO_MAX,
  GRAFEO_MIN,
  GRAFEO_STD,
  GRAFEO_MEAN
}ArrayOperation;

typedef enum _ColorType{
  GRAFEO_BIN = 0,
  GRAFEO_GRAY,
  GRAFEO_RGB,
  GRAFEO_XYZ,
  GRAFEO_YUV,
  GRAFEO_YCBCR,
  GRAFEO_LUV,
  GRAFEO_LAB,
}ColorType;

typedef enum _Adjacency{
  GRAFEO_NEIGHBOR_4,
  GRAFEO_NEIGHBOR_6,
  GRAFEO_NEIGHBOR_8,
  GRAFEO_NEIGHBOR_18,
  GRAFEO_NEIGHBOR_26
}Adjacency;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif
