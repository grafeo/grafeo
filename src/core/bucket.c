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
#include <grafeo/core.h>

GrfBucket* grf_bucket_new(){
  return malloc(sizeof(GrfBucket));
}

GrfQueue* grf_bucket_queue(GrfBucket *bucket){
  return bucket->queue;
}

void *grf_bucket_value(GrfBucket *bucket)
{
  return bucket->value;
}

void grf_bucket_set_queue(GrfBucket *bucket, GrfQueue *queue)
{
  bucket->queue = queue;
}

void grf_bucket_set_value(GrfBucket *bucket, void *value)
{
  bucket->value = value;
}

void grf_bucket_free(GrfBucket *bucket)
{
  free(bucket);
}
