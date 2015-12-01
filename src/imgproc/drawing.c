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
#include <grafeo/imgproc.h>
/*=======================
 *      PRIVATE API
 *=====================*/
enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };
static const float SinTable[] =
    { 0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f, 0.9998477f, 0.9993908f, 0.9986295f, 0.9975641f, 0.9961947f,
    0.9945219f, 0.9925462f, 0.9902681f, 0.9876883f, 0.9848078f, 0.9816272f,
    0.9781476f, 0.9743701f, 0.9702957f, 0.9659258f, 0.9612617f, 0.9563048f,
    0.9510565f, 0.9455186f, 0.9396926f, 0.9335804f, 0.9271839f, 0.9205049f,
    0.9135455f, 0.9063078f, 0.8987940f, 0.8910065f, 0.8829476f, 0.8746197f,
    0.8660254f, 0.8571673f, 0.8480481f, 0.8386706f, 0.8290376f, 0.8191520f,
    0.8090170f, 0.7986355f, 0.7880108f, 0.7771460f, 0.7660444f, 0.7547096f,
    0.7431448f, 0.7313537f, 0.7193398f, 0.7071068f, 0.6946584f, 0.6819984f,
    0.6691306f, 0.6560590f, 0.6427876f, 0.6293204f, 0.6156615f, 0.6018150f,
    0.5877853f, 0.5735764f, 0.5591929f, 0.5446390f, 0.5299193f, 0.5150381f,
    0.5000000f, 0.4848096f, 0.4694716f, 0.4539905f, 0.4383711f, 0.4226183f,
    0.4067366f, 0.3907311f, 0.3746066f, 0.3583679f, 0.3420201f, 0.3255682f,
    0.3090170f, 0.2923717f, 0.2756374f, 0.2588190f, 0.2419219f, 0.2249511f,
    0.2079117f, 0.1908090f, 0.1736482f, 0.1564345f, 0.1391731f, 0.1218693f,
    0.1045285f, 0.0871557f, 0.0697565f, 0.0523360f, 0.0348995f, 0.0174524f,
    0.0000000f, -0.0174524f, -0.0348995f, -0.0523360f, -0.0697565f, -0.0871557f,
    -0.1045285f, -0.1218693f, -0.1391731f, -0.1564345f, -0.1736482f, -0.1908090f,
    -0.2079117f, -0.2249511f, -0.2419219f, -0.2588190f, -0.2756374f, -0.2923717f,
    -0.3090170f, -0.3255682f, -0.3420201f, -0.3583679f, -0.3746066f, -0.3907311f,
    -0.4067366f, -0.4226183f, -0.4383711f, -0.4539905f, -0.4694716f, -0.4848096f,
    -0.5000000f, -0.5150381f, -0.5299193f, -0.5446390f, -0.5591929f, -0.5735764f,
    -0.5877853f, -0.6018150f, -0.6156615f, -0.6293204f, -0.6427876f, -0.6560590f,
    -0.6691306f, -0.6819984f, -0.6946584f, -0.7071068f, -0.7193398f, -0.7313537f,
    -0.7431448f, -0.7547096f, -0.7660444f, -0.7771460f, -0.7880108f, -0.7986355f,
    -0.8090170f, -0.8191520f, -0.8290376f, -0.8386706f, -0.8480481f, -0.8571673f,
    -0.8660254f, -0.8746197f, -0.8829476f, -0.8910065f, -0.8987940f, -0.9063078f,
    -0.9135455f, -0.9205049f, -0.9271839f, -0.9335804f, -0.9396926f, -0.9455186f,
    -0.9510565f, -0.9563048f, -0.9612617f, -0.9659258f, -0.9702957f, -0.9743701f,
    -0.9781476f, -0.9816272f, -0.9848078f, -0.9876883f, -0.9902681f, -0.9925462f,
    -0.9945219f, -0.9961947f, -0.9975641f, -0.9986295f, -0.9993908f, -0.9998477f,
    -1.0000000f, -0.9998477f, -0.9993908f, -0.9986295f, -0.9975641f, -0.9961947f,
    -0.9945219f, -0.9925462f, -0.9902681f, -0.9876883f, -0.9848078f, -0.9816272f,
    -0.9781476f, -0.9743701f, -0.9702957f, -0.9659258f, -0.9612617f, -0.9563048f,
    -0.9510565f, -0.9455186f, -0.9396926f, -0.9335804f, -0.9271839f, -0.9205049f,
    -0.9135455f, -0.9063078f, -0.8987940f, -0.8910065f, -0.8829476f, -0.8746197f,
    -0.8660254f, -0.8571673f, -0.8480481f, -0.8386706f, -0.8290376f, -0.8191520f,
    -0.8090170f, -0.7986355f, -0.7880108f, -0.7771460f, -0.7660444f, -0.7547096f,
    -0.7431448f, -0.7313537f, -0.7193398f, -0.7071068f, -0.6946584f, -0.6819984f,
    -0.6691306f, -0.6560590f, -0.6427876f, -0.6293204f, -0.6156615f, -0.6018150f,
    -0.5877853f, -0.5735764f, -0.5591929f, -0.5446390f, -0.5299193f, -0.5150381f,
    -0.5000000f, -0.4848096f, -0.4694716f, -0.4539905f, -0.4383711f, -0.4226183f,
    -0.4067366f, -0.3907311f, -0.3746066f, -0.3583679f, -0.3420201f, -0.3255682f,
    -0.3090170f, -0.2923717f, -0.2756374f, -0.2588190f, -0.2419219f, -0.2249511f,
    -0.2079117f, -0.1908090f, -0.1736482f, -0.1564345f, -0.1391731f, -0.1218693f,
    -0.1045285f, -0.0871557f, -0.0697565f, -0.0523360f, -0.0348995f, -0.0174524f,
    -0.0000000f, 0.0174524f, 0.0348995f, 0.0523360f, 0.0697565f, 0.0871557f,
    0.1045285f, 0.1218693f, 0.1391731f, 0.1564345f, 0.1736482f, 0.1908090f,
    0.2079117f, 0.2249511f, 0.2419219f, 0.2588190f, 0.2756374f, 0.2923717f,
    0.3090170f, 0.3255682f, 0.3420201f, 0.3583679f, 0.3746066f, 0.3907311f,
    0.4067366f, 0.4226183f, 0.4383711f, 0.4539905f, 0.4694716f, 0.4848096f,
    0.5000000f, 0.5150381f, 0.5299193f, 0.5446390f, 0.5591929f, 0.5735764f,
    0.5877853f, 0.6018150f, 0.6156615f, 0.6293204f, 0.6427876f, 0.6560590f,
    0.6691306f, 0.6819984f, 0.6946584f, 0.7071068f, 0.7193398f, 0.7313537f,
    0.7431448f, 0.7547096f, 0.7660444f, 0.7771460f, 0.7880108f, 0.7986355f,
    0.8090170f, 0.8191520f, 0.8290376f, 0.8386706f, 0.8480481f, 0.8571673f,
    0.8660254f, 0.8746197f, 0.8829476f, 0.8910065f, 0.8987940f, 0.9063078f,
    0.9135455f, 0.9205049f, 0.9271839f, 0.9335804f, 0.9396926f, 0.9455186f,
    0.9510565f, 0.9563048f, 0.9612617f, 0.9659258f, 0.9702957f, 0.9743701f,
    0.9781476f, 0.9816272f, 0.9848078f, 0.9876883f, 0.9902681f, 0.9925462f,
    0.9945219f, 0.9961947f, 0.9975641f, 0.9986295f, 0.9993908f, 0.9998477f,
    1.0000000f
};
static void
sincos_custom( int angle, float* cosval, float* sinval )
{
  angle += (angle < 0 ? 360 : 0);
  *sinval = SinTable[angle];
  *cosval = SinTable[450 - angle];
}


// Forward declaration of static functions
static void
grf_ndarray_draw_line_no_shift (GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color, int line_type);
static void
grf_ndarray_draw_line_neighbor_8 (GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);
static void
grf_ndarray_draw_line_antialiased(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color);
static void
grf_ndarray_fill_convex_poly     (GrfNDArray* array, GrfScalar2D* v, int npts, GrfScalar4D* color, int line_type, int shift);
static void
grf_ndarray_draw_line_thick      (GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int flags, int shift);
static void
grf_ndarray_draw_circle_direct   (GrfNDArray* array, GrfScalar2D center, int thickness, GrfScalar4D* color, int fill);
static void
grf_ndarray_draw_ellipse_ex      (GrfNDArray* array, GrfScalar2D center, GrfSize2D axes, int angle, int arc_start, int arc_end, GrfScalar4D* color, int thickness, int line_type);
static void
grf_ndarray_draw_polyline(GrfNDArray* array, GrfScalar2D* v, int count, uint8_t is_closed, GrfScalar4D* color, int thickness, int line_type, int shift);
static void
grf_ndarray_fill_edge_collection(GrfNDArray* array, GrfScalar4D* edges, int num_edges, GrfScalar4D* color);
static void
grf_ndarray_collect_polyedges(GrfNDArray* array, GrfScalar2D* v, int count, GrfScalar4D** edges, int* count_edges, GrfScalar4D* color, int line_type, int shift, GrfScalar2D offset);


// Draw Line without shift
static void
grf_ndarray_draw_line_no_shift(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* _color, int connectivity){
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

  int bt_pix0 = array->dim == 2?1:(int)array->size[2], bt_pix = bt_pix0;
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

  if( connectivity == GRF_NEIGHBOR_8 )  {

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
  int pix_size       = array->dim == 2?1:array->size[2];
  int* color         = ((int*)_color);

  for( i = 0; i < count; i++)  {
    for(j = 0; j < pix_size; j++) ptr[j] = (uint8_t)color[j];

    int mask = err < 0 ? -1 : 0;
    err += minusDelta + (plusDelta & mask);
    ptr += minusStep + (plusStep & mask);
  }
}
static void
grf_ndarray_draw_line_antialiased(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color){
  (void) array; (void) p1; (void) p2; (void) color;
}

static void
grf_ndarray_draw_line_neighbor_8(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D* color){
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
grf_ndarray_fill_convex_poly(GrfNDArray* array, GrfScalar2D* v, int npts, GrfScalar4D* color, int line_type, int shift){
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
              grf_ndarray_draw_line_no_shift(array, pt0, pt1, color, line_type );
          }
          else
              grf_ndarray_draw_line_neighbor_8(array, p0, p, color );
      }
      else
          grf_ndarray_draw_line_antialiased(array, p0, p, color );
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
grf_ndarray_draw_line_thick(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int flags, int shift){
  static const double INV_XY_ONE = 1./XY_ONE;

  p1.x <<= XY_SHIFT - shift;
  p1.y <<= XY_SHIFT - shift;
  p2.x <<= XY_SHIFT - shift;
  p2.y <<= XY_SHIFT - shift;

  if(thickness <= 1){
    if(line_type != GRF_ANTIALIASED){
      if(line_type == GRF_NEIGHBOR_4 || shift == 0){
        p1.x = (p1.x + (XY_ONE>>1)) >> XY_SHIFT;
        p1.y = (p1.y + (XY_ONE>>1)) >> XY_SHIFT;
        p2.x = (p2.x + (XY_ONE>>1)) >> XY_SHIFT;
        p2.y = (p2.y + (XY_ONE>>1)) >> XY_SHIFT;
        grf_ndarray_draw_line_no_shift(array, p1, p2, color, line_type);
      }
      else
        grf_ndarray_draw_line_neighbor_8(array, p1, p2, color);
    }
    else
      grf_ndarray_draw_line_antialiased(array, p1, p2, color);
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

        grf_ndarray_fill_convex_poly(array, pt, 4, color, line_type, XY_SHIFT );
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
                grf_ndarray_draw_circle_direct(array, center, (thickness + (XY_ONE>>1)) >> XY_SHIFT, color, 1 );
            }
            else
            {
                GrfSize2D size = {thickness, thickness};
                grf_ndarray_draw_ellipse_ex( array, p1, size, 0, 0, 360, color, -1, line_type );
            }
        }
        p1 = p2;
    }
  }
}

static void
grf_ndarray_draw_circle_direct   (GrfNDArray* array, GrfScalar2D center, int radius, GrfScalar4D* _color, int fill){
  GrfSize2D  size     = {array->size[1],array->size[0]};
  size_t     step     = array->step[0];
  int        pix_size = array->dim == 2?1:array->size[2];
  uint8_t*   ptr      = array->data_uint8;
  uint8_t    color[3] = {_color->x, _color->y, _color->z};
  int err = 0, dx = radius, dy = 0, plus = 1, minus = (radius << 1) - 1;
  int inside = center.x >= radius && center.x < size.width - radius &&
      center.y >= radius && center.y < size.height - radius;

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
        IGRF_HLINE( tptr0, x11, x12, _color, pix_size );
        IGRF_HLINE( tptr1, x11, x12, _color, pix_size );
      }

      tptr0 = ptr + y21 * step;
      tptr1 = ptr + y22 * step;

      if(!fill){
        IGRF_PUT_POINT( tptr0, x21 );
        IGRF_PUT_POINT( tptr1, x21 );
        IGRF_PUT_POINT( tptr0, x22 );
        IGRF_PUT_POINT( tptr1, x22 );
      }else{
        IGRF_HLINE( tptr0, x21, x22, _color, pix_size );
        IGRF_HLINE( tptr1, x21, x22, _color, pix_size );
      }
    }
    else if( x11 < size.width && x12 >= 0 && y21 < size.height && y22 >= 0 ){
      if( fill ){
        x11 = max( x11, 0 );
        x12 = min( x12, size.width - 1 );
      }

      if( (unsigned)y11 < (unsigned)size.height ){
        uint8_t *tptr = ptr + y11 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < size.width )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, _color, pix_size );
      }

      if( (unsigned)y12 < (unsigned)size.height ){
        uint8_t *tptr = ptr + y12 * step;

        if( !fill ){
          if( x11 >= 0 )
            IGRF_PUT_POINT( tptr, x11 );
          if( x12 < size.width )
            IGRF_PUT_POINT( tptr, x12 );
        }
        else
          IGRF_HLINE( tptr, x11, x12, _color, pix_size );
      }

      if( x21 < size.width && x22 >= 0 ){
        if( fill ){
          x21 = max( x21, 0 );
          x22 = min( x22, size.width - 1 );
        }

        if( (unsigned)y21 < (unsigned)size.height ){
          uint8_t *tptr = ptr + y21 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < size.width )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, _color, pix_size );
        }

        if( (unsigned)y22 < (unsigned)size.height )
        {
          uint8_t *tptr = ptr + y22 * step;

          if( !fill ){
            if( x21 >= 0 )
              IGRF_PUT_POINT( tptr, x21 );
            if( x22 < size.width )
              IGRF_PUT_POINT( tptr, x22 );
          }
          else
            IGRF_HLINE( tptr, x21, x22, _color, pix_size );
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
grf_ndarray_draw_ellipse_ex      (GrfNDArray* array, GrfScalar2D center, GrfSize2D axes, int angle, int arc_start, int arc_end, GrfScalar4D* color, int thickness, int line_type){
  axes.width = abs(axes.width), axes.height = abs(axes.height);
  int delta = (max(axes.width,axes.height)+(XY_ONE>>1))>>XY_SHIFT;
  delta = delta < 3 ? 90 : delta < 10 ? 30 : delta < 15 ? 18 : 5;

  GrfScalar2D* v = NULL;
  int count;
  grf_ellipse_to_poly( center, axes, angle, arc_start, arc_end, delta, &v, &count );

  if( thickness >= 0 )
    grf_ndarray_draw_polyline( array, &v[0], count, 0, color, thickness, line_type, XY_SHIFT );
  else if( arc_end - arc_start >= 360 )
    grf_ndarray_fill_convex_poly( array, &v[0], count, color, line_type, XY_SHIFT );
  else
  {
    v = realloc(v, (count + 1) * sizeof(GrfScalar2D));
    v[count] = center;
    GrfScalar4D* edges;
    int count_edges = 0;
    GrfScalar2D offset = {0,0};
    grf_ndarray_collect_polyedges( array,  &v[0], count+1, &edges, &count_edges, color, line_type, XY_SHIFT, offset);
    grf_ndarray_fill_edge_collection( array, edges, count_edges, color);
  }
  free(v);
}
static void
grf_ndarray_draw_polyline(GrfNDArray* array, GrfScalar2D* v, int count, uint8_t is_closed, GrfScalar4D* color, int thickness, int line_type, int shift){
  if(!v || count <= 0) return;
  int i     = is_closed ? count - 1: 0;
  int flags = 2 + !is_closed;
  GrfScalar2D p0;
  p0 = v[i];
  for(i = !is_closed; i < count; i++){
    GrfScalar2D p = v[i];
    grf_ndarray_draw_line_thick(array, p0, p, color, thickness, line_type, flags, shift);
    p0            = p;
    flags         = 2;
  }
}

static void
grf_ndarray_collect_polyedges(GrfNDArray* array, GrfScalar2D* v, int count, GrfScalar4D** edges, int* count_edges, GrfScalar4D* color, int line_type, int shift, GrfScalar2D offset){
  int i, delta = offset.y + (shift ? 1 << (shift - 1) : 0);
  GrfScalar2D pt0 = v[count-1], pt1;
  pt0.x = (pt0.x + offset.x) << (XY_SHIFT - shift);
  pt0.y = (pt0.y + delta) >> shift;

  GrfScalar4D* edges2 = *edges;
  edges2 = realloc(edges2,(count + (*count_edges) + 1)*sizeof(GrfScalar4D));
  *edges = edges2;

  for( i = 0; i < count; i++, pt0 = pt1 )
  {
    GrfScalar2D t0, t1;
    GrfScalar4D edge;

    pt1 = v[i];
    pt1.x = (pt1.x + offset.x) << (XY_SHIFT - shift);
    pt1.y = (pt1.y + delta) >> shift;

    if( line_type < GRF_ANTIALIASED )
    {
      t0.y = pt0.y; t1.y = pt1.y;
      t0.x = (pt0.x + (XY_ONE >> 1)) >> XY_SHIFT;
      t1.x = (pt1.x + (XY_ONE >> 1)) >> XY_SHIFT;
      grf_ndarray_draw_line_no_shift(array, t0, t1, color, line_type );
    }
    else
    {
      t0.x = pt0.x; t1.x = pt1.x;
      t0.y = pt0.y << XY_SHIFT;
      t1.y = pt1.y << XY_SHIFT;
      grf_ndarray_draw_line_antialiased(array, t0, t1, color );
    }

    if( pt0.y == pt1.y )
      continue;

    if( pt0.y < pt1.y )
    {
      edge.y = pt0.y;
      edge.w = pt1.y;
      edge.x = pt0.x;
    }
    else
    {
      edge.y = pt1.y;
      edge.w = pt0.y;
      edge.x = pt1.x;
    }
    edge.z = (pt1.x - pt0.x) / (pt1.y - pt0.y);
    edges2[i] = edge;
    (*count_edges)++;
  }
}

int grf_compare_edges(const void* elem1, const void* elem2){
  GrfScalar4D* e1 = (GrfScalar4D*) elem1;
  GrfScalar4D* e2 = (GrfScalar4D*) elem2;
  // First compare Y, after X, after X2 (dx)
  if     (e1->y  < e2->y ) return -1;
  else if(e1->y  > e2->y ) return  1;
  else if(e1->x  < e2->x ) return -1;
  else if(e1->x  > e2->x ) return  1;
  else if(e1->x2 < e2->x2) return -1;
  else if(e1->x2 > e2->x2) return  1;
  return 0;
}

static void
grf_ndarray_fill_edge_collection(GrfNDArray* array, GrfScalar4D* edges, int num_edges, GrfScalar4D* color){
  (void) color;
  GrfScalar4D tmp;
  int i, total = num_edges;
  //int y;
  GrfSize2D* size = (GrfSize2D*) array->size;
  //GrfScalar4D* e;
  int y_max = INT_MIN, x_max = INT_MIN, y_min = INT_MAX, x_min = INT_MAX;
  //int pix_size = array->dim < 3?1:array->size[2];

  if( total < 2 )
      return;

  for( i = 0; i < total; i++ )
  {
    GrfScalar4D* e1 = edges + i;
    // Determine x-coordinate of the end of the edge.
    // (This is not necessary x-coordinate of any vertex in the array.)
    int x1 = e1->x + (e1->y2 - e1->y) * e1->x2;
    y_min = min( y_min, e1->y );
    y_max = max( y_max, e1->y2 );
    x_min = min( x_min, e1->x );
    x_max = max( x_max, e1->x );
    x_min = min( x_min, x1 );
    x_max = max( x_max, x1 );
  }

  if( y_max < 0 || y_min >= size->height || x_max < 0 || x_min >= (size->width<<XY_SHIFT) )
    return;

  qsort(edges, num_edges, sizeof(GrfScalar4D), grf_compare_edges);

  // start drawing
  tmp.y            = INT_MAX;
  edges[num_edges] = tmp;

  //edges.push_back(tmp); // after this point we do not add
                        // any elements to edges, thus we can use pointers
  i = 0;
//  e = &edges[i];
//  y_max = min( y_max, size->height );

//  for( y = e->y0; y < y_max; y++ )
//  {
//    PolyEdge *last, *prelast, *keep_prelast;
//    int sort_flag = 0;
//    int draw = 0;
//    int clipline = y < 0;

//    prelast = &tmp;
//    last = tmp.next;
//    while( last || e->y0 == y )
//    {
//      if( last && last->y1 == y )
//      {
//        // exclude edge if y reachs its lower point
//        prelast->next = last->next;
//        last = last->next;
//        continue;
//      }
//      keep_prelast = prelast;
//      if( last && (e->y0 > y || last->x < e->x) )
//      {
//        // go to the next edge in active list
//        prelast = last;
//        last = last->next;
//      }
//      else if( i < total )
//      {
//        // insert new edge into active list if y reachs its upper point
//        prelast->next = e;
//        e->next = last;
//        prelast = e;
//        e = &edges[++i];
//      }
//      else
//          break;

//      if( draw )
//      {
//        if( !clipline )
//        {
//          // convert x's from fixed-point to image coordinates
//          uchar *timg = img.ptr(y);
//          int x1 = keep_prelast->x;
//          int x2 = prelast->x;

//          if( x1 > x2 )
//          {
//            int t = x1;

//            x1 = x2;
//            x2 = t;
//          }

//          x1 = (x1 + XY_ONE - 1) >> XY_SHIFT;
//          x2 = x2 >> XY_SHIFT;

//          // clip and draw the line
//          if( x1 < size.width && x2 >= 0 )
//          {
//            if( x1 < 0 )
//                x1 = 0;
//            if( x2 >= size.width )
//                x2 = size.width - 1;
//            IGRF_HLINE( timg, x1, x2, color, pix_size );
//          }
//        }
//        keep_prelast->x += keep_prelast->dx;
//        prelast->x += prelast->dx;
//      }
//      draw ^= 1;
//    }

//    // sort edges (using bubble sort)
//    keep_prelast = 0;

//    do
//    {
//      prelast = &tmp;
//      last = tmp.next;

//      while( last != keep_prelast && last->next != 0 )
//      {
//        PolyEdge *te = last->next;

//        // swap edges
//        if( last->x > te->x )
//        {
//          prelast->next = te;
//          last->next = te->next;
//          te->next = last;
//          prelast = te;
//          sort_flag = 1;
//        }
//        else
//        {
//          prelast = last;
//          last = te;
//        }
//      }
//      keep_prelast = prelast;
//    }
//    while( sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp );
//  }
}

GRF_INLINE uint64_t grf_rectangle_area(GrfRectangle rect){
  return rect.width * rect.height;
}
GrfScalar2D grf_rectangle_top_left(GrfRectangle rect){
  GrfScalar2D point = {rect.x, rect.y};
  return point;
}
GrfScalar2D grf_rectangle_bottom_right(GrfRectangle rect){
  GrfScalar2D point = {rect.x + rect.width, rect.y + rect.height};
  return point;
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
grf_ndarray_draw_line(GrfNDArray* array, GrfScalar2D p1, GrfScalar2D p2, GrfScalar4D *color, int thickness, int line_type, int shift){
  grf_ndarray_draw_line_thick(array, p1, p2, color, thickness, line_type, 3, shift);
}
void
grf_ndarray_draw_circle(GrfNDArray *array, GrfScalar2D center, int radius, GrfScalar4D *color, int thickness, int line_type, int shift){
  if( line_type == GRF_ANTIALIASED)
      line_type = GRF_NEIGHBOR_8;

  if( thickness > 1 || line_type >= GRF_ANTIALIASED || shift > 0 )
  {
    center.x <<= XY_SHIFT - shift;
    center.y <<= XY_SHIFT - shift;
    radius   <<= XY_SHIFT - shift;
    GrfSize2D radiuses = {radius,radius};
    grf_ndarray_draw_ellipse_ex(array, center, radiuses,
               0, 0, 360, color, thickness, line_type );
  }
  else
      grf_ndarray_draw_circle_direct(array, center, radius, color, thickness < 0 );
}
void
grf_ndarray_draw_ellipse(GrfNDArray *array, GrfScalar2D center, GrfSize2D axes, double angle, double start_angle, double end_angle, GrfScalar4D *color, int thickness, int line_type, int shift){
  if( line_type == GRF_ANTIALIASED)
      line_type = GRF_NEIGHBOR_8;

  int _angle       = grf_round(angle);
  int _start_angle = grf_round(start_angle);
  int _end_angle   = grf_round(end_angle);
  center.x    <<= XY_SHIFT - shift;
  center.y    <<= XY_SHIFT - shift;
  axes.width  <<= XY_SHIFT - shift;
  axes.height <<= XY_SHIFT - shift;

  grf_ndarray_draw_ellipse_ex(array, center, axes, _angle, _start_angle,
             _end_angle, color, thickness, line_type );
}
void
grf_ellipse_to_poly(GrfScalar2D center, GrfSize2D axes, int angle, int arc_start, int arc_end, int delta, GrfScalar2D** pts, int *count){
  float       alpha, beta;
  double      size_a = axes.width, size_b = axes.height;
  double      cx     = center.x, cy = center.y;
  GrfScalar2D prevPt = {INT_MIN,INT_MIN};
  int         i;

  while( angle < 0 )
    angle += 360;
  while( angle > 360 )
    angle -= 360;

  if( arc_start > arc_end ){
    i = arc_start;
    arc_start = arc_end;
    arc_end = i;
  }
  while( arc_start < 0 ){
    arc_start += 360;
    arc_end += 360;
  }
  while( arc_end > 360 ){
    arc_end -= 360;
    arc_start -= 360;
  }
  if( arc_end - arc_start > 360 ){
    arc_start = 0;
    arc_end = 360;
  }
  double angle2 = (double) angle;
  sincos_custom(angle2, &alpha, &beta );

  int cur_length = 1024;

  GrfScalar2D* pts2 = malloc(cur_length * sizeof(GrfScalar2D));
  *pts = pts2;

  for(i = arc_start; i < arc_end + delta; i += delta ){
    double x, y;
    angle = i;
    if( angle > arc_end )
      angle = arc_end;
    if( angle < 0 )
      angle += 360;

    x = size_a * SinTable[450-angle];
    y = size_b * SinTable[angle];
    GrfScalar2D pt;
    pt.x = grf_round( cx + x * alpha - y * beta );
    pt.y = grf_round( cy + x * beta + y * alpha );
    if( pt.x != prevPt.x || pt.y != prevPt.y ){
      // Resize list whether not sufficient
      if(*count == cur_length){
        cur_length <<= 1;
        pts2 = realloc(pts,cur_length * sizeof(GrfScalar2D));
        *pts = pts2;
      }
      pts[*count]->x = pt.x;
      pts[*count]->y = pt.y;
      (*count)++;
    }
  }


  // If there are no points, it's a zero-size polygon
  if( *count == 1) {
    *count    = 2;
    pts2 = realloc(pts,2 * sizeof(GrfScalar2D));
    pts2[0].x = center.x;pts2[0].y = center.y;
    pts2[1].x = center.x;pts2[1].y = center.y;

  }else{
    pts2 = realloc(pts,*count * sizeof(GrfScalar2D));
  }
  *pts = pts2;
}
void
grf_ndarray_draw_rectangle(GrfNDArray *array, GrfRectangle rect, GrfScalar4D *color, int thickness, int line_type, int shift){
  if(grf_rectangle_area(rect) > 0 ){
    GrfScalar2D top_left     = grf_rectangle_top_left(rect);
    GrfScalar2D point        = {1<<shift,1<<shift};
    GrfScalar2D bottom_right = grf_rectangle_bottom_right(rect);
    bottom_right.x -= point.x;
    bottom_right.y -= point.y;
    grf_ndarray_draw_rectangle_2( array, top_left, bottom_right, color, thickness, line_type, shift );
  }
}
void
grf_ndarray_draw_rectangle_2(GrfNDArray* array, GrfScalar2D top_left, GrfScalar2D bottom_right, GrfScalar4D* color, int thickness, int line_type, int shift){
  if( line_type == GRF_ANTIALIASED)
      line_type = GRF_NEIGHBOR_8;
  GrfScalar2D pt[4];
  pt[0]   = top_left;
  pt[1].x = bottom_right.x;
  pt[1].y = top_left.y;
  pt[2]   = bottom_right;
  pt[3].x = top_left.x;
  pt[3].y = bottom_right.y;

  if( thickness >= 0 )
    grf_ndarray_draw_polyline(array, pt, 4, 1, color, thickness, line_type, shift );
  else
    grf_ndarray_fill_convex_poly(array, pt, 4, color, line_type, shift );
}
