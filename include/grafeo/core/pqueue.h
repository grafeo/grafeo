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
#ifndef GRF_PQUEUE_H
#define GRF_PQUEUE_H

#include <grafeo/core.h>

BEGIN_DECLS

/**
 * @brief Append a value in a bucket list.
 * @param pqueue The priority queue
 * @param bucket The bucket key
 * @param value The value to be inserted in a bucket list
 * @param compare_function the function to be used to insert sorted buckets
 */
void  grf_pqueue_append_at(GrfQueue* pqueue, void* bucket, void* value, GrfCompareFunc compare_function);
/**
 * @brief Prepend a value in a bucket list.
 * @param pqueue The priority queue
 * @param bucket The bucket key
 * @param value The value to be inserted in a bucket list
 * @param compare_function the function to be used to insert sorted buckets
 */
void  grf_pqueue_prepend_at(GrfQueue* pqueue, void* bucket, void* value, GrfCompareFunc compare_function);
/**
 * @brief Remove the first item of the bucket list
 * @param pqueue
 * @param bucket
 */
void  grf_pqueue_remove_begin_at(GrfQueue* pqueue, void* bucket);
/**
 * @brief Remove the last item of the bucket list
 * @param pqueue The priority queue
 * @param bucket the bucket key
 */
void  grf_pqueue_remove_end_at  (GrfQueue* pqueue, void* bucket);
/**
 * @brief grf_pqueue_remove_at
 * @param pqueue
 * @param grf_bucket_value
 * @param value
 */
void  grf_pqueue_remove_at(GrfQueue* pqueue, void *grf_bucket_value, void* value);
/**
 * @brief Remove the first item of the current bucket
 * @param pqueue The priority queue
 */
void  grf_pqueue_remove_begin(GrfQueue* pqueue);
/**
 * @brief Remove the last item of the current
 * @param pqueue The priority queue
 */
void  grf_pqueue_remove_end(GrfQueue* pqueue);
/**
 * @brief Remove current bucket if it's empty
 * @param pqueue The priority queue
 */
void  grf_pqueue_shrink(GrfQueue* pqueue);
/**
 * @brief Get key of nth bucket (n = grf_bucket_index+1)
 * @param pqueue
 * @param grf_bucket_index
 * @return
 */
void* grf_pqueue_at(GrfQueue* pqueue, uint32_t grf_bucket_index);
/**
 * @brief Get ith value of nth bucket (n = grf_bucket_index+1 | i = index+1)
 * @param pqueue The priority queue
 * @param grf_bucket_index the position of the bucket in the priority queue
 * @param index The position of the value in the bucket list
 * @return value
 */
void* grf_pqueue_grf_bucket_at(GrfQueue* pqueue, uint32_t grf_bucket_index, uint32_t index);
/**
 * @brief grf_pqueue_grf_bucket_of
 * @param pqueue
 * @param grf_bucket_value
 * @return
 */
GrfBucket* grf_pqueue_grf_bucket_of(GrfQueue* pqueue, void* grf_bucket_value);

END_DECLS

#endif
