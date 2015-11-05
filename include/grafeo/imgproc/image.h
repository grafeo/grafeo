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
#ifndef GRF_IMAGE_H
#define GRF_IMAGE_H

#define PNG_SKIP_SETJMP_CHECK 1
#include <stdarg.h>

#include <grafeo/core.h>
#include <png.h>
#include <jpeglib.h>

BEGIN_DECLS


/**
 * @brief      Read an image and generate an GrfArray (for now, PNG or JPG)
 *
 * @param[in]  filename  the name of the image
 *
 * @return     GrfArray containing pixels data
 */
GrfArray* grf_image_read(const char* filename);

/**
 * @brief      Read a PNG image and generate an GrfArray
 *
 * @param[in]  filename  the name of the image
 *
 * @return     GrfArray containing pixels data
 */
GrfArray* grf_image_read_png(const char* filename);

/**
 * @brief      Read a JPG image and generate an GrfArray
 *
 * @param[in]  filename  the name of the image
 *
 * @return     GrfArray containing pixels data
 */
GrfArray* grf_image_read_jpg(const char* filename);

/**
 * @brief grf_image_read_pgm
 * @param filename
 * @return
 */
GrfArray* grf_image_read_pgm(const char* filename);
/**
 * @brief grf_image_read_ppm
 * @param filename
 * @return
 */
GrfArray* grf_image_read_ppm(const char* filename);

/**
 * @brief      Generates a file with name `filename` from an GrfArray
 *
 * @param      array     Matriz de dados (height,
 * @param[in]  filename  name of file
 */
void   grf_image_write(GrfArray* array, const char* filename);

/**
 * @brief      Generate a PNG file with name `filename`
 *
 * @param      array     data array (height, width, number of channels)
 * @param[in]  filename  name of file
 */
void   grf_image_write_png(GrfArray* array, const char* filename);

/**
 * @brief      Generate a JPG file with name `filename`
 *
 * @param      array     pixels data (height, width, number of channels)
 * @param[in]  filename  name of file
 */
void   grf_image_write_jpg(GrfArray* array, const char* filename);

/**
 * @brief grf_image_write_pgm
 * @param array
 * @param filename
 */
void   grf_image_write_pgm(GrfArray* array, const char* filename);
/**
 * @brief grf_image_write_ppm
 * @param array
 * @param filename
 */
void   grf_image_write_ppm(GrfArray* array, const char* filename);

/**
 * @brief      Convert an image from a color space to another
 *
 * @param      array    pixels data
 * @param[in]  origin   current color space
 * @param[in]  destiny  desired color space
 * @return     New GrfArray based on desired color space
 */
GrfArray* grf_image_cvt_color(GrfArray* array, GrfColorType origin, GrfColorType destiny);

typedef enum _GrfImageError{
  GRF_ERROR_FILE_OPEN = 0
}GrfImageError;

END_DECLS

#endif
