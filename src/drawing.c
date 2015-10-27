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
/*=======================
 *      PRIVATE API
 *=====================*/
enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };

// Forward declaration of static functions
static void
grf_array_draw_line_no_shift (Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color, int line_type);
static void
grf_array_draw_line_neighbor_8 (Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);
static void
grf_array_draw_line_antialiased(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);
static void
grf_array_fill_convex_poly     (Array* array, GrfScalar2D* v, int npts, GrfScalar4D* color, int line_type, int shift);
static void
grf_array_draw_line_thick      (Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int flags, int shift);
static void
grf_array_draw_circle_direct   (Array* array, GrfScalar2D center, int thickness, GrfScalar4D* color, int fill);
static void
grf_array_draw_ellipse_ex      (Array* array, GrfScalar2D center, GrfSize2D axes, int angle, int arc_start, int arc_end, GrfScalar4D* color, int thickness, int line_type);
static void
grf_array_draw_line_no_shift(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* _color, int connectivity){
  int err, count;
  uint8_t* ptr;
  int minusDelta, plusDelta;
  int minusStep, plusStep;

  uint8_t left_to_right = 1;

  // BEGIN
  count = -1;

  if( (unsigned)p1.x >= (unsigned)(array->size[1]) ||
      (unsigned)p2.x >= (unsigned)(array->size[1]) ||
      (unsigned)p1.y >= (unsigned)(array->size[0]) ||
      (unsigned)p2.y >= (unsigned)(array->size[0]) )
  {
    GrfSize2D size = {array->size[0],array->size[1]};
      if( !grf_clip_line( size, &p1, &p2 ) ){
          ptr = array->data_uint8;
          err = plusDelta = minusDelta = plusStep = minusStep = count = 0;
          return;
      }
  }

  int bt_pix0 = (int)array->size[2], bt_pix = bt_pix0;
  size_t istep = array->step[0];

  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  int s = dx < 0 ? -1 : 0;

  if( left_to_right )  {
      dx = (dx ^ s) - s;
      dy = (dy ^ s) - s;
      p1.x ^= (p1.x ^ p2.x) & s;
      p1.y ^= (p1.y ^ p2.y) & s;
  }  else  {
      dx = (dx ^ s) - s;
      bt_pix = (bt_pix ^ s) - s;
  }

  ptr = (uint8_t*)(array->data_uint8 + p1.y * istep + p1.x * bt_pix0);

  s = dy < 0 ? -1 : 0;
  dy = (dy ^ s) - s;
  istep = (istep ^ s) - s;

  s = dy > dx ? -1 : 0;

  /* conditional swaps */
  dx ^= dy & s;
  dy ^= dx & s;
  dx ^= dy & s;

  bt_pix ^= istep & s;
  istep ^= bt_pix & s;
  bt_pix ^= istep & s;

  if( connectivity == GRAFEO_NEIGHBOR_8 )  {

      err = dx - (dy + dy);
      plusDelta = dx + dx;
      minusDelta = -(dy + dy);
      plusStep = (int)istep;
      minusStep = bt_pix;
      count = dx + 1;
  }  else  { /* connectivity == 4 */

      err = 0;
      plusDelta = (dx + dx) + (dy + dy);
      minusDelta = -(dy + dy);
      plusStep = (int)istep - bt_pix;
      minusStep = bt_pix;
      count = dx + dy + 1;
  }
  // END

  int i,j;
  int pix_size       = array->size[2];
  int* color         = ((int*)_color);

  for( i = 0; i < count; i++)  {
    for(j = 0; j < pix_size; j++) ptr[j] = (uint8_t)color[j];

    int mask = err < 0 ? -1 : 0;
    err += minusDelta + (plusDelta & mask);
    ptr += minusStep + (plusStep & mask);
  }
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

  #define  IGRF_PUT_POINT(_x,_y)   \
  x = (_x); y = (_y);             \
  if( 0 <= x && x < size->width && \
      0 <= y && y < size->height ) \
  {                               \
      tptr = ptr + y*step + x*pix_size;\
      for( j = 0; j < pix_size; j++ ) \
          tptr[j] = (uint8_t)((int*)color)[j]; \
  }

  IGRF_PUT_POINT((p2.x + (XY_ONE >> 1)) >> XY_SHIFT,
                (p2.y + (XY_ONE >> 1)) >> XY_SHIFT);

  if( ax > ay )
  {
    p1.x >>= XY_SHIFT;

    while( ecount >= 0 )
    {
        IGRF_PUT_POINT(p1.x, p1.y >> XY_SHIFT);
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
        IGRF_PUT_POINT(p1.x >> XY_SHIFT, p1.y);
        p1.x += x_step;
        p1.y++;
        ecount--;
    }
  }

  #undef IGRF_PUT_POINT
}

/* helper macros: filling horizontal row */
#define IGRF_HLINE( ptr, xl, xr, color, pix_size )            \
{                                                            \
    uint8_t* hline_ptr = (uint8_t*)(ptr) + (xl)*(pix_size);      \
    uint8_t* hline_max_ptr = (uint8_t*)(ptr) + (xr)*(pix_size);  \
                                                             \
    for( ; hline_ptr <= hline_max_ptr; hline_ptr += (pix_size))\
    {                                                        \
        int hline_j;                                         \
        for( hline_j = 0; hline_j < (pix_size); hline_j++ )  \
        {                                                    \
            hline_ptr[hline_j] = (uint8_t)((int*)color)[hline_j];   \
        }                                                    \
    }                                                        \
}

static void
grf_array_fill_convex_poly(Array* array, GrfScalar2D* v, int npts, GrfScalar4D* color, int line_type, int shift){
  struct
  {
    int idx, di;
    int x, dx, ye;
  }
  edge[2];

  int xmin, xmax, ymin, ymax;
  int delta1, delta2;
  GrfScalar2D p0;
  int i, y, imin  = 0, left = 0, right = 1, x1, x2;
  int delta       = shift ? 1 << (shift - 1) : 0;
  int edges       = npts;
  uint8_t* ptr    = array->data_uint8;
  GrfSize2D* size = (GrfSize2D*) array->size;
  int pix_size    = array->dim < 3? 1:array->size[2];

  if( line_type < GRF_ANTIALIASED)
      delta1 = delta2 = XY_ONE >> 1;
  else
      delta1 = XY_ONE - 1, delta2 = 0;

  p0 = v[npts - 1];
  p0.x <<= XY_SHIFT - shift;
  p0.y <<= XY_SHIFT - shift;

  xmin = xmax = v[0].x;
  ymin = ymax = v[0].y;

  for( i = 0; i < npts; i++ )
  {
      GrfScalar2D p = v[i];
      if( p.y < ymin )
      {
          ymin = p.y;
          imin = i;
      }

      ymax = max( ymax, p.y );
      xmax = max( xmax, p.x );
      xmin = min( xmin, p.x );

      p.x <<= XY_SHIFT - shift;
      p.y <<= XY_SHIFT - shift;

      if( line_type <= 8 )
      {
          if( shift == 0 )
          {
              GrfScalar2D pt0, pt1;
              pt0.x = p0.x >> XY_SHIFT;
              pt0.y = p0.y >> XY_SHIFT;
              pt1.x = p.x >> XY_SHIFT;
              pt1.y = p.y >> XY_SHIFT;
              grf_array_draw_line_no_shift(array, pt0, pt1, color, line_type );
          }
          else
              grf_array_draw_line_neighbor_8(array, p0, p, color );
      }
      else
          grf_array_draw_line_antialiased(array, p0, p, color );
      p0 = p;
  }

  xmin = (xmin + delta) >> shift;
  xmax = (xmax + delta) >> shift;
  ymin = (ymin + delta) >> shift;
  ymax = (ymax + delta) >> shift;

  if( npts < 3 || xmax < 0 || ymax < 0 || xmin >= size->width || ymin >= size->height )
      return;

  ymax = min( ymax, size->height - 1 );
  edge[0].idx = edge[1].idx = imin;

  edge[0].ye = edge[1].ye = y = ymin;
  edge[0].di = 1;
  edge[1].di = npts - 1;

  ptr += array->step[0]*y;

  do
  {
      if( line_type < GRF_ANTIALIASED || y < ymax || y == ymin )
      {
          for( i = 0; i < 2; i++ )
          {
              if( y >= edge[i].ye )
              {
                  int idx = edge[i].idx, di = edge[i].di;
                  int xs = 0, xe, ye, ty = 0;

                  for(;;)
                  {
                      ty = (v[idx].y + delta) >> shift;
                      if( ty > y || edges == 0 )
                          break;
                      xs = v[idx].x;
                      idx += di;
                      idx -= ((idx < npts) - 1) & npts;   /* idx -= idx >= npts ? npts : 0 */
                      edges--;
                  }

                  ye = ty;
                  xs <<= XY_SHIFT - shift;
                  xe = v[idx].x << (XY_SHIFT - shift);

                  /* no more edges */
                  if( y >= ye )
                      return;

                  edge[i].ye = ye;
                  edge[i].dx = ((xe - xs)*2 + (ye - y)) / (2 * (ye - y));
                  edge[i].x = xs;
                  edge[i].idx = idx;
              }
          }
      }

      if( edge[left].x > edge[right].x )
      {
          left ^= 1;
          right ^= 1;
      }

      x1 = edge[left].x;
      x2 = edge[right].x;

      if( y >= 0 )
      {
          int xx1 = (x1 + delta1) >> XY_SHIFT;
          int xx2 = (x2 + delta2) >> XY_SHIFT;

          if( xx2 >= 0 && xx1 < size->width )
          {
              if( xx1 < 0 )
                  xx1 = 0;
              if( xx2 >= size->width )
                  xx2 = size->width - 1;
              IGRF_HLINE( ptr, xx1, xx2, color, pix_size );
          }
      }

      x1 += edge[left].dx;
      x2 += edge[right].dx;

      edge[left].x = x1;
      edge[right].x = x2;
      ptr += array->step[0];
  }
  while( ++y <= ymax );
}

void
grf_array_draw_line_thick(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int flags, int shift){
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
        grf_array_draw_line_no_shift(array, p1, p2, color, line_type);
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
        dp.x = grf_round(dy * r );
        dp.y = grf_round(dx * r );

        pt[0].x = p1.x + dp.x;
        pt[0].y = p1.y + dp.y;
        pt[1].x = p1.x - dp.x;
        pt[1].y = p1.y - dp.y;
        pt[2].x = p2.x - dp.x;
        pt[2].y = p2.y - dp.y;
        pt[3].x = p2.x + dp.x;
        pt[3].y = p2.y + dp.y;

        grf_array_fill_convex_poly(array, pt, 4, color, line_type, XY_SHIFT );
    }

    for( i = 0; i < 2; i++ )
    {
        if( flags & (i+1) )
        {
            if( line_type < GRF_ANTIALIASED )
            {
                GrfScalar2D center;
                center.x = (p1.x + (XY_ONE>>1)) >> XY_SHIFT;
                center.y = (p1.y + (XY_ONE>>1)) >> XY_SHIFT;
                grf_array_draw_circle_direct(array, center, (thickness + (XY_ONE>>1)) >> XY_SHIFT, color, 1 );
            }
            else
            {
                GrfSize2D size = {thickness, thickness};
                grf_array_draw_ellipse_ex( array, p1, size, 0, 0, 360, color, -1, line_type );
            }
        }
        p1 = p2;
    }
  }
}

static void
grf_array_draw_circle_direct   (Array* array, GrfScalar2D center, int radius, GrfScalar4D* color, int fill){
  GrfSize2D* size     = (GrfSize2D*)array->size;
  size_t     step     = array->step[0];
  int        pix_size = array->size[2];
  uint8_t*   ptr      = array->data_uint8;
  int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
  int inside = center.x >= radius && center.x < size->width - radius &&
      center.y >= radius && center.y < size->height - radius;

  #define IGRF_PUT_POINT( ptr, x )     \
    memcpy( ptr + (x)*pix_size, color, pix_size );

  while( dx >= dy )
  {
    int mask;
    int y11 = center.y - dy, y12 = center.y + dy, y21 = center.y - dx, y22 = center.y + dx;
    int x11 = center.x - dx, x12 = center.x + dx, x21 = center.x - dy, x22 = center.x + dy;

    if( inside ){
      uint8_t *tptr0 = ptr + y11 * step;
      uint8_t *tptr1 = ptr + y12 * step;

      if( !fill ){
        IGRF_PUT_POINT( tptr0, x11 );
        IGRF_PUT_POINT( tptr1, x11 );
        IGRF_PUT_POINT( tptr0, x12 );
        IGRF_PUT_POINT( tptr1, x12 );
      }else{
        IGRF_HLINE( tptr0, x11, x12, color, pix_size );
        IGRF_HLINE( tptr1, x11, x12, color, pix_size );
      }

      tptr0 = ptr + y21 * step;
      tptr1 = ptr + y22 * step;

      if(!fill){
        IGRF_PUT_POINT( tptr0, x21 );
        IGRF_PUT_POINT( tptr1, x21 );
        IGRF_PUT_POINT( tptr0, x22 );
        IGRF_PUT_POINT( tptr1, x22 );
      }else{
        IGRF_HLINE( tptr0, x21, x22, color, pix_size );
        IGRF_HLINE( tptr1, x21, x22, color, pix_size );
      }
    }
    else if( x11 < size->width && x12 >= 0 && y21 < size->height && y22 >= 0 ){
      if( fill ){
        x11 = max( x11, 0 );
        x12 = min( x12, size->width - 1 );
      }

      if( (unsigned)y11 < (unsigned)size->height ){
        uint8_t *tptr = ptr + y11 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < size->width )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, color, pix_size );
      }

      if( (unsigned)y12 < (unsigned)size->height ){
        uint8_t *tptr = ptr + y12 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < size->width )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, color, pix_size );
      }

      if( x21 < size->width && x22 >= 0 ){
        if( fill ){
          x21 = max( x21, 0 );
          x22 = min( x22, size->width - 1 );
        }

        if( (unsigned)y21 < (unsigned)size->height ){
          uint8_t *tptr = ptr + y21 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < size->width )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, color, pix_size );
        }

        if( (unsigned)y22 < (unsigned)size->height )
        {
          uint8_t *tptr = ptr + y22 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < size->width )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, color, pix_size );
        }
      }
    }
    dy++;
    err += plus;
    plus += 2;

    mask = (err <= 0) - 1;

    err -= minus & mask;
    dx += mask;
    minus -= mask & 2;
  }

  #undef  IGRF_PUT_POINT
}
static void
grf_array_draw_ellipse_ex      (Array* array, GrfScalar2D center, GrfSize2D axes, int angle, int arc_start, int arc_end, GrfScalar4D* color, int thickness, int line_type){

}

/*=======================
 *      PUBLIC API
 *=====================*/
uint8_t
grf_clip_line(GrfSize2D rectangle, GrfScalar2D *p1, GrfScalar2D *p2){
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
void
grf_array_draw_line(Array* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int shift){
  grf_array_draw_line_thick(array, p1, p2, color, thickness, line_type, 3, shift);
}
void
grf_array_draw_circle(Array *array, GrfScalar2D center, int radius, GrfScalar4D *color, int thickness, int line_type, int shift){

}
void
grf_array_draw_ellipse(Array *array, GrfScalar2D center, GrfSize2D axes, double angle, double start_angle, double end_angle, GrfScalar4D *color, int thickness, int line_type, int shift){

}
