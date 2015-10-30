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
#ifndef GRF_RANGE_H
#define GRF_RANGE_H
#include <grafeo/macro.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

BEGIN_DECLS

/**
 * @brief End of a range
 */
typedef struct _GrfRangeItem{
  int64_t value; /**< Value of object representing an range end */
}GrfRangeItem;

/**
 * @brief Range structure
 */
typedef struct _GrfRange{
  GrfRangeItem* from; /**< Start of range */
  GrfRangeItem* to;   /**< End (exclusive) of range */
}GrfRange;

/**
 * @brief      { function_description }
 *
 * @param      range  { parameter_description }
 * @param[in]  from   { parameter_description }
 * @param[in]  to     { parameter_description }
 */
void grf_range_from_to(GrfRange* range, int64_t from, int64_t to);

/**
 * @brief      { function_description }
 *
 * @param      range  { parameter_description }
 * @param[in]  from   { parameter_description }
 */
void grf_range_from(GrfRange* range, int64_t from);

/**
 * @brief      { function_description }
 *
 * @param      range  { parameter_description }
 * @param[in]  to     { parameter_description }
 */
void grf_range_to(GrfRange* range, int64_t to);

/**
 * @brief      { function_description }
 *
 * @param      range  { parameter_description }
 */
void grf_range_all(GrfRange* range);

END_DECLS

#endif
