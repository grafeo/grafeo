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
#ifndef GRAFEO_PQUEUE_H
#define GRAFEO_PQUEUE_H

#include <grafeo/bucket.h>
#include <grafeo/queue.h>

/**
 * @brief Append a value in a bucket list.
 * @param pqueue The priority queue
 * @param bucket The bucket key
 * @param value The value to be inserted in a bucket list
 */
void  pqueue_append_at(Queue* pqueue, void* bucket, void* value);
/**
 * @brief Prepend a value in a bucket list.
 * @param pqueue The priority queue
 * @param bucket The bucket key
 * @param value The value to be inserted in a bucket list
 */
void  pqueue_prepend_at(Queue* pqueue, void* bucket, void* value);
/**
 * @brief Remove the first item of the bucket list
 * @param pqueue
 * @param bucket
 */
void  pqueue_remove_begin_at(Queue* pqueue, void* bucket);
/**
 * @brief Remove the last item of the bucket list
 * @param pqueue The priority queue
 * @param bucket the bucket key
 */
void  pqueue_remove_end_at  (Queue* pqueue, void* bucket);
/**
 * @brief Remove the first item of the current bucket
 * @param pqueue The priority queue
 */
void  pqueue_remove_begin(Queue* pqueue);
/**
 * @brief Remove the last item of the current
 * @param pqueue The priority queue
 */
void  pqueue_remove_end(Queue* pqueue);
/**
 * @brief Remove current bucket if it's empty
 * @param pqueue The priority queue
 */
void  pqueue_shrink(Queue* pqueue);
/**
 * @brief Get key of nth bucket (n = bucket_index+1)
 * @param pqueue
 * @param bucket_index
 * @return
 */
void* pqueue_at(Queue* pqueue, uint32_t bucket_index);
/**
 * @brief Get ith value of nth bucket (n = bucket_index+1 | i = index+1)
 * @param pqueue The priority queue
 * @param bucket_index the position of the bucket in the priority queue
 * @param index The position of the value in the bucket list
 * @return value
 */
void* pqueue_bucket_at(Queue* pqueue, uint32_t bucket_index, uint32_t index);

#endif
