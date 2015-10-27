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
#ifndef GRF_DRAWING_H
#define GRF_DRAWING_H
#define GRF_ANTIALIASED 16
#include <grafeo/array.h>
#include <grafeo/hal.h>
#include <float.h>

BEGIN_DECLS
void    grf_array_draw_line(Array* array, GrfScalar2D p0, GrfScalar2D p1, GrfScalar4D *color, int thickness, int line_type, int shift);
uint8_t grf_clip_line(GrfSize2D size_scaled, GrfScalar2D* p1, GrfScalar2D* p2);
void    grf_array_draw_circle(Array* array, GrfScalar2D center, int radius, GrfScalar4D* color, int thickness, int line_type, int shift);
void    grf_array_draw_ellipse(Array* array, GrfScalar2D center, GrfSize2D axes,double angle, double start_angle, double end_angle, GrfScalar4D* color, int thickness, int line_type, int shift);
void    grf_array_draw_rectangle(Array* array, GrfRectangle rect, GrfScalar4D* color, int thickness, int lineType, int shift);
END_DECLS
#endif
