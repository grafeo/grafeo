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
#ifndef GRF_CHART_CONFIG_H
#define GRF_CHART_CONFIG_H

typedef enum{
  GRF_PROJECTION_CARTESIAN,
  GRF_PROJECTION_SPHERICAL
} GrfChartProjection;

typedef enum{
  GRF_CAPSTYLE_BUTT,
  GRF_CAPSTYLE_ROUND,
  GRF_CAPSTYLE_PROJECTING
} GrfChartCapStyle;

typedef enum{
  GRF_JOINSTYLE_MITTER,
  GRF_JOINSTYLE_ROUND,
  GRF_JOINSTYLE_BEVEL
} GrfChartJoinStyle;

typedef enum{
  GRF_DRAWSTYLE_DEFAULT,
  GRF_DRAWSTYLE_STEPS_PRE,
  GRF_DRAWSTYLE_STEPS_MID,
  GRF_DRAWSTYLE_STEPS_POST,
} GrfChartDrawStyle;

typedef enum{
  GRF_FILLSTYLE_FULL,
  GRF_FILLSTYLE_LEFT,
  GRF_FILLSTYLE_RIGHT,
  GRF_FILLSTYLE_BOTTOM,
  GRF_FILLSTYLE_TOP,
  GRF_FILLSTYLE_NONE,
}GrfChartFillStyle;

typedef enum{
  GRF_CHARTMARKER_POINT,
  GRF_CHARTMARKER_PIXEL,
  GRF_CHARTMARKER_CIRCLE,
  GRF_CHARTMARKER_TRIANGLE_DOWN,
  GRF_CHARTMARKER_TRIANGLE_UP,
  GRF_CHARTMARKER_TRIANGLE_LEFT,
  GRF_CHARTMARKER_TRIANGLE_RIGHT,
  GRF_CHARTMARKER_TRI_DOWN,
  GRF_CHARTMARKER_TRI_UP,
  GRF_CHARTMARKER_TRI_LEFT,
  GRF_CHARTMARKER_TRI_RIGHT,
  GRF_CHARTMARKER_OCTAGON,
  GRF_CHARTMARKER_SQUARE,
  GRF_CHARTMARKER_PENTAGON,
  GRF_CHARTMARKER_STAR,
  GRF_CHARTMARKER_HEXAGON1,
  GRF_CHARTMARKER_HEXAGON2,
  GRF_CHARTMARKER_PLUS,
  GRF_CHARTMARKER_X,
  GRF_CHARTMARKER_DIAMOND,
  GRF_CHARTMARKER_THIN_DIAMOND,
  GRF_CHARTMARKER_VLINE,
  GRF_CHARTMARKER_HLINE,
  GRF_CHARTMARKER_TICKDOWN,
  GRF_CHARTMARKER_TICKUP,
  GRF_CHARTMARKER_TICKLEFT,
  GRF_CHARTMARKER_TICKRIGHT,
  GRF_CHARTMARKER_CARETUP,
  GRF_CHARTMARKER_CARETDOWN,
  GRF_CHARTMARKER_CARETLEFT,
  GRF_CHARTMARKER_CARETRIGHT,
}GrfChartMarker;

#endif
