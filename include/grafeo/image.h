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
#ifndef GRAFEO_IMAGE_H
#define GRAFEO_IMAGE_H

#include <grafeo/type.h>
#include <grafeo/array.h>
#include <stdio.h>
#include <stdarg.h>
#include <jpeglib.h>
#include <png.h>
#include <setjmp.h>

/**
 * @brief      { function_description }
 *
 * @param[in]  filename  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array* image_read(const char* filename);

/**
 * @brief      { function_description }
 *
 * @param[in]  filename  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array* image_read_png(const char* filename);

/**
 * @brief      { function_description }
 *
 * @param[in]  filename  { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
Array* image_read_jpg(const char* filename);

/**
 * @brief      { function_description }
 *
 * @param      array     { parameter_description }
 * @param[in]  filename  { parameter_description }
 */
void   image_write(Array* array, const char* filename);

/**
 * @brief      { function_description }
 *
 * @param      array     { parameter_description }
 * @param[in]  filename  { parameter_description }
 */
void   image_write_png(Array* array, const char* filename);

/**
 * @brief      { function_description }
 *
 * @param      array     { parameter_description }
 * @param[in]  filename  { parameter_description }
 */
void   image_write_jpg(Array* array, const char* filename);

/**
 * @brief      { function_description }
 *
 * @param      array    { parameter_description }
 * @param[in]  origin   { parameter_description }
 * @param[in]  destiny  { parameter_description }
 */
void   image_cvt_color(Array* array, ColorType origin, ColorType destiny);

typedef enum _ImageError{
  GRAFEO_ERROR_FILE_OPEN = 0
}ImageError;


#endif
