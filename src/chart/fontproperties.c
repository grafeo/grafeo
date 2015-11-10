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
#include <grafeo/chart.h>
/*=========================
 *      PRIVATE API
 *=======================*/
typedef struct _GrfFontPropertiesPrivate{
  char**         family;
  GrfFontStyle   style;
  GrfFontVariant variant;
  GrfFontStretch stretch;
  GrfFontWeight  weight;
  float          size;
}GrfFontPropertiesPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfFontProperties, grf_font_properties, G_TYPE_OBJECT)

static void
grf_font_properties_init(GrfFontProperties *self){

}

static void
grf_font_properties_class_init(GrfFontPropertiesClass *klass){

}


/*=========================
 *      PUBLIC API
 *=======================*/