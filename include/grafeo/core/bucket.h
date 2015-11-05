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
#ifndef GRF_BUCKET_H
#define GRF_BUCKET_H
#include <grafeo/core.h>

BEGIN_DECLS
/**
 * @brief Bucket structure, just a queue with an additional attribute which
 * is the same for every item
 *
 * You can put additional attributes just by extending Bucket, or inserting
 * a struct of attributes in the `value` variable.
 */
typedef struct _GrfBucket{
  GrfQueue* queue; /**< GrfQueue of elements in the bucket */
  void*  value; /**< Unique attribute of the queue */
}GrfBucket;
/**
 * @brief grf_bucket_new
 * @return
 */
GrfBucket* grf_bucket_new();
/**
 * @brief grf_bucket_queue
 * @param bucket
 * @return
 */
GrfQueue*  grf_bucket_queue(GrfBucket* bucket);
/**
 * @brief grf_bucket_value
 * @param bucket
 * @return
 */
void*   grf_bucket_value(GrfBucket* bucket);
/**
 * @brief grf_bucket_set_queue
 * @param bucket
 * @param queue
 */
void    grf_bucket_set_queue(GrfBucket* bucket,GrfQueue* queue);
/**
 * @brief grf_bucket_set_value
 * @param bucket
 * @param value
 */
void    grf_bucket_set_value(GrfBucket* bucket, void* value);
/**
 * @brief grf_bucket_free
 * @param bucket
 */
void    grf_bucket_free(GrfBucket* bucket);

END_DECLS

#endif
