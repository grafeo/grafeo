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
#include <grafeo/drawing.h>

enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };

void grf_array_draw_line(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int shift){
  int dx, dy; // Manhattan distance between points
  int ecount; // Number of elements
  int ax, ay;
  GrfSize2D* size        = (GrfSize2D*) grf_array_get_size(array);
  GrfSize2D  size_scaled = {size->size1, size->size2};
  uint32_t
  size_scaled.size1      = ;

  // We need to clip the line
  // If the line is completely outside rectangle, exit
  if(!grf_clip_line(size_scaled, p1, p2)) return;

  // Manhattan distance
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;

  //
  j  =  dx < 0? -1:0;
  ax = (dx ^ j) - j;

  i  =  dy < 0? -1:0;
  ay = (dy ^ i) - i;

  // Change axis
  if(ax > ay){
    dx    = ax;
    dy    = (dy ^ j) - j;
    p1.x ^= p2.x & j;
    p2.x ^= p1.x & j;
    p1.x ^= p2.x & j;
    p1.x ^= p2.y & j;
    p2.x ^= p1.y & j;
    p1.x ^= p2.y & j;

    x_step = XY_ONE;
    y_step = (int)(((int64_t) dy << XY_SHIFT) / (ax | 1));
    ecount = (p2.x - p1.x) >> XY_SHIFT;
  }else{
    dy    = ay;
    dx    = (dx ^ i) - i;
    p1.x ^= p2.x & i;
    p2.x ^= p1.x & i;
    p1.x ^= p2.x & i;
    p1.x ^= p2.y & i;
    p2.x ^= p1.y & i;
    p1.x ^= p2.y & i;

    x_step = (int)(((int64_t) dx << XY_SHIFT) / (ay | 1));
    y_step = XY_ONE;
    ecount = (p2.y - p1.y) >> XY_SHIFT;
  }

  p1.x += (XY_ONE >> 1);
  p2.x += (XY_ONE >> 1);

  // If colored image
  if(pix_size == 3){
    #define GRF_PUT_POINT(_x,_y)    \
    x = (_x); y = (_y);             \
    if(0 <= x && x < size.size1 &&  \
       0 <= y && y < size.size2){   \
      tptr = ptr + y*step + x*3;    \
      tptr[0] = (uint8_t) cr;       \
      tptr[1] = (uint8_t) cb;       \
      tptr[2] = (uint8_t) cg;       \
    }
    GRF_PUT_POINT((p2.x + (XY_ONE >> 1)) >> XY_SHIFT,
                  (p2.y + (XY_ONE >> 1)) >> XY_SHIFT);
    if(ax > ay){
      p1.x >>= XY_SHIFT;
      while(ecount >= 0){
        GRF_PUT_POINT(p1.x, p1.y >> XY_SHIFT);
        p1.x++;
        p1.y += y_step;
        ecount--;
      }
    }else{
      p1.y >>= XY_SHIFT;
      while(ecount >= 0){
        GRF_PUT_POINT(p1.x >> XY_SHIFT, p1.y);
        p1.x += x_step;
        p1.y++;
        ecount--;
      }
    }
    #undef GRF_PUT_POINT
  }
  // If grayscale image
  else if(pix_size == 1){
    #define GRF_PUT_POINT(_x,_y)    \
    x = (_x); y = (_y);             \
    if(0 <= x && x < size.size1 &&  \
       0 <= y && y < size.size2){   \
      tptr = ptr + y*step + x;      \
      tptr[0] = (uint8_t) cr;       \
    }
    GRF_PUT_POINT((p2.x + (XY_ONE >> 1)) >> XY_SHIFT,
                  (p2.y + (XY_ONE >> 1)) >> XY_SHIFT);
    if(ax > ay){
      p1.x >>= XY_SHIFT;
      while(ecount >= 0){
        GRF_PUT_POINT(p1.x, p1.y >> XY_SHIFT);
        p1.x++;
        p1.y += y_step;
        ecount--;
      }
    }else{
      p1.y >>= XY_SHIFT;
      while(ecount >= 0){
        GRF_PUT_POINT(p1.x >> XY_SHIFT, p1.y);
        p1.x += x_step;
        p1.y++;
        ecount--;
      }
    }
    #undef GRF_PUT_POINT
  }

}
