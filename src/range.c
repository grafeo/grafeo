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
#include <grafeo/range.h>

static GrfRangeItem* rangeitem_new_with_value(int64_t value){
	GrfRangeItem* rangeitem = malloc(sizeof(GrfRangeItem));
	rangeitem->value = value;
	return rangeitem;
}

void grf_range_from_to(GrfRange* range, int64_t from, int64_t to){
	range->from = rangeitem_new_with_value(from);
	range->to   = rangeitem_new_with_value(to);
}

void grf_range_from(GrfRange* range, int64_t from){
	range->from = rangeitem_new_with_value(from);
	range->to   = NULL;
}

void grf_range_to(GrfRange* range, int64_t to){
	range->from = NULL;
	range->to   = rangeitem_new_with_value(to);
}

void grf_range_all(GrfRange* range){
	range->from = NULL;
	range->to   = NULL;
}