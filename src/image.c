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
#include <grafeo/image.h>

Array* image_read(const char* filename){

}
Array* image_read_png(const char* filename){

}
Array* image_read_jpg(const char* filename){

}
void image_write(Array* array, const char* filename){

}
void image_write_png(Array* array, const char* filename){

}
void image_write_jpg(Array* array, const char* filename, Error){
    struct    jpeg_decompress_struct cinfo;
    struct    my_error_mgr jerr;
    FILE*     infile;
    uint8_t** buffer; // 2D Pointers to rows
    int       row_stride;
    if((infile = fopen(filename, "rb")) == NULL){
        fprintf(stderr, "can't open ")
    }
}
void image_cvt_color(Array* array, ColorType origin, ColorType destiny){

}