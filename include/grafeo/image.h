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

#define PNG_SKIP_SETJMP_CHECK 1
#include <stdarg.h>

#include <grafeo/type.h>
#include <grafeo/array.h>
#include <png.h>
#include <jpeglib.h>

BEGIN_DECLS


/**
 * @brief      Read an image and generate an Array (for now, PNG or JPG)
 *
 * @param[in]  filename  the name of the image
 *
 * @return     Array containing pixels data
 */
Array* image_read(const char* filename);

/**
 * @brief      Read a PNG image and generate an Array
 *
 * @param[in]  filename  the name of the image
 *
 * @return     Array containing pixels data
 */
Array* image_read_png(const char* filename);

/**
 * @brief      Read a JPG image and generate an Array
 *
 * @param[in]  filename  the name of the image
 *
 * @return     Array containing pixels data
 */
Array* image_read_jpg(const char* filename);

/**
 * @brief image_read_pgm
 * @param filename
 * @return
 */
Array* image_read_pgm(const char* filename);

/**
 * @brief      Generates a file with name `filename` from an Array
 *
 * @param      array     Matriz de dados (height,
 * @param[in]  filename  name of file
 */
void   image_write(Array* array, const char* filename);

/**
 * @brief      Generate a PNG file with name `filename`
 *
 * @param      array     data array (height, width, number of channels)
 * @param[in]  filename  name of file
 */
void   image_write_png(Array* array, const char* filename);

/**
 * @brief      Generate a JPG file with name `filename`
 *
 * @param      array     pixels data (height, width, number of channels)
 * @param[in]  filename  name of file
 */
void   image_write_jpg(Array* array, const char* filename);

/**
 * @brief image_write_pgm
 * @param array
 * @param filename
 */
void   image_write_pgm(Array* array, const char* filename);

/**
 * @brief      Convert an image from a color space to another
 *
 * @param      array    pixels data
 * @param[in]  origin   current color space
 * @param[in]  destiny  desired color space
 * @return     New Array based on desired color space
 */
Array* image_cvt_color(Array* array, ColorType origin, ColorType destiny);

typedef enum _ImageError{
  GRAFEO_ERROR_FILE_OPEN = 0
}ImageError;

END_DECLS

#endif
