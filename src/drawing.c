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

// Forward declaration of static functions
static void
grf_array_draw_line_neighbor_4(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color, int line_type);
static void
grf_array_draw_line_neighbor_8(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);
static void
grf_array_draw_line_antialiased(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);

void grf_array_draw_line(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int shift){
  static const double INV_XY_ONE = 1./XY_ONE;

  p1.x <<= XY_SHIFT - shift;
  p1.y <<= XY_SHIFT - shift;
  p2.x <<= XY_SHIFT - shift;
  p2.y <<= XY_SHIFT - shift;

  if(thickness <= 1){
    if(line_type != GRF_ANTIALIASED){
      if(line_type == GRAFEO_NEIGHBOR_4 || shift == 0){
        p1.x = (p1.x + (XY_ONE>>1)) >> XY_SHIFT;
        p1.y = (p1.y + (XY_ONE>>1)) >> XY_SHIFT;
        p2.x = (p2.x + (XY_ONE>>1)) >> XY_SHIFT;
        p2.y = (p2.y + (XY_ONE>>1)) >> XY_SHIFT;
        grf_array_draw_line_neighbor_4(array, p1, p2, color, line_type);
      }
      else
        grf_array_draw_line_neighbor_8(array, p1, p2, color);
    }
    else
      grf_array_draw_line_antialiased(array, p1, p2, color);
  }else{ // Thickness > 1
    GrfScalar2D pt[4], dp = {0,0};
    double dx = (p1.x - p2.x)*INV_XY_ONE, dy = (p2.y - p1.y)*INV_XY_ONE;
    double r = dx * dx + dy * dy;
    int i, oddThickness = thickness & 1;
    thickness <<= XY_SHIFT - 1;

    if( fabs(r) > DBL_EPSILON )
    {
        r = (thickness + oddThickness*XY_ONE*0.5)/sqrt(r);
        dp.x = cvRound( dy * r );
        dp.y = cvRound( dx * r );

        pt[0].x = p0.x + dp.x;
        pt[0].y = p0.y + dp.y;
        pt[1].x = p0.x - dp.x;
        pt[1].y = p0.y - dp.y;
        pt[2].x = p1.x - dp.x;
        pt[2].y = p1.y - dp.y;
        pt[3].x = p1.x + dp.x;
        pt[3].y = p1.y + dp.y;

        FillConvexPoly( img, pt, 4, color, line_type, XY_SHIFT );
    }

    for( i = 0; i < 2; i++ )
    {
        if( flags & (i+1) )
        {
            if( line_type < CV_AA )
            {
                Point center;
                center.x = (p0.x + (XY_ONE>>1)) >> XY_SHIFT;
                center.y = (p0.y + (XY_ONE>>1)) >> XY_SHIFT;
                Circle( img, center, (thickness + (XY_ONE>>1)) >> XY_SHIFT, color, 1 );
            }
            else
            {
                EllipseEx( img, p0, cvSize(thickness, thickness),
                           0, 0, 360, color, -1, line_type );
            }
        }
        p0 = p1;
    }
  }

}
static void
grf_array_draw_line_neighbor_4(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color, int line_type){

}
static void
grf_array_draw_line_antialiased(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color){

}

static void
grf_array_draw_line_neighbor_8(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color){
  int dx, dy; // Manhattan distance between points
  int ecount; // Number of elements
  int ax, ay;
  int x,y,i,j;
  int x_step, y_step;
  int pix_size = array->dim == 2? 1:array->size[2];
  uint8_t *tptr, *ptr = array->data_uint8;
  GrfSize2D* size        = (GrfSize2D*) array->size;
  GrfSize2D  size_scaled = {size->width * XY_ONE, size->height * XY_ONE};
  uint64_t step = array->step[0];

  // We need to clip the line
  // If the line is completely outside rectangle, exit
  if(!grf_clip_line(size_scaled, &p1, &p2)) return;

  // Manhattan distance
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;

  //
  j  =  dx < 0? -1:0;
  ax = (dx ^ j) - j;

  i  =  dy < 0? -1:0;
  ay = (dy ^ i) - i;

  // Change axis
#define CHANGE
  if(ax > ay){
    dx    = ax;
    dy    = (dy ^ j) - j;
    p1.x ^= p2.x & j;// Change p1 and p2, if necessary,
    p2.x ^= p1.x & j;// to get p1.x < p2.x
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
  p1.y += (XY_ONE >> 1);

  #define  ICV_PUT_POINT(_x,_y)   \
  x = (_x); y = (_y);             \
  if( 0 <= x && x < size->width && \
      0 <= y && y < size->height ) \
  {                               \
      tptr = ptr + y*step + x*pix_size;\
      for( j = 0; j < pix_size; j++ ) \
          tptr[j] = (uint8_t)((int*)color)[j]; \
  }

  ICV_PUT_POINT((p2.x + (XY_ONE >> 1)) >> XY_SHIFT,
                (p2.y + (XY_ONE >> 1)) >> XY_SHIFT);

  if( ax > ay )
  {
    p1.x >>= XY_SHIFT;

    while( ecount >= 0 )
    {
        ICV_PUT_POINT(p1.x, p1.y >> XY_SHIFT);
        p1.x++;
        p1.y += y_step;
        ecount--;
    }
  }
  else
  {
    p1.y >>= XY_SHIFT;

    while( ecount >= 0 )
    {
        ICV_PUT_POINT(p1.x >> XY_SHIFT, p1.y);
        p1.x += x_step;
        p1.y++;
        ecount--;
    }
  }

  #undef ICV_PUT_POINT
}

uint8_t grf_clip_line(GrfSize2D rectangle, GrfScalar2D *p1, GrfScalar2D *p2){
  int64_t x1, y1, x2, y2;
  int c1, c2;
  int64_t right = rectangle.width-1, bottom = rectangle.height-1;

  if( rectangle.width <= 0 || rectangle.height <= 0 )
     return 0;

  x1 = p1->x; y1 = p1->y; x2 = p2->x; y2 = p2->y;
  c1 = (x1 < 0) + ((x1 > right) << 1) + ((y1 < 0) << 2) + ((y1 > bottom) << 3);
  c2 = (x2 < 0) + ((x2 > right) << 1) + ((y2 < 0) << 2) + ((y2 > bottom) << 3);

  if( (c1 & c2) == 0 && (c1 | c2) != 0 )
  {
    int64_t a;
    if( c1 & 12 )
    {
      a = c1 < 8 ? 0 : bottom;
      x1 +=  (a - y1) * (x2 - x1) / (y2 - y1);
      y1 = a;
      c1 = (x1 < 0) + ((x1 > right) << 1);
    }
    if( c2 & 12 )
    {
      a = c2 < 8 ? 0 : bottom;
      x2 += (a - y2) * (x2 - x1) / (y2 - y1);
      y2 = a;
      c2 = (x2 < 0) + ((x2 > right) << 1);
    }
    if( (c1 & c2) == 0 && (c1 | c2) != 0 )
    {
      if( c1 )
      {
        a = c1 == 1 ? 0 : right;
        y1 += (a - x1) * (y2 - y1) / (x2 - x1);
        x1 = a;
        c1 = 0;
      }
      if( c2 )
      {
        a = c2 == 1 ? 0 : right;
        y2 += (a - x2) * (y2 - y1) / (x2 - x1);
        x2 = a;
        c2 = 0;
      }
    }

    //assert( (c1 & c2) != 0 || (x1 | y1 | x2 | y2) >= 0 );

    p1->x = (int)x1;
    p1->y = (int)y1;
    p2->x = (int)x2;
    p2->y = (int)y2;
  }

  return (c1 | c2) == 0;
}
