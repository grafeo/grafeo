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

void  pqueue_append_at(Queue* pqueue, void* bucket, void* value);
void  pqueue_prepend_at(Queue* pqueue, void* bucket, void* value);
void  pqueue_prepend(Queue* pqueue, void* value);
void  pqueue_append(Queue* pqueue, void* value);
void  pqueue_remove_begin_at(Queue* pqueue, void* bucket);
void  pqueue_remove_end_at  (Queue* pqueue, void* bucket);
void  pqueue_remove_begin(Queue* pqueue);
void  pqueue_remove_end(Queue* pqueue);
void  pqueue_shrink(Queue* pqueue);
void* pqueue_at(Queue* pqueue, uint32_t bucket_index);
void* pqueue_bucket_at(Queue* pqueue, uint32_t bucket_index, uint32_t index);

/**
 * @brief pqueue_bucket_remove_begin
 * @param pqueue
 * @param bucket
 * @return
 */
void* pqueue_bucket_remove_begin(Queue* pqueue,  void* bucket);
/**
 * @brief pqueue_bucket_remove_end
 * @param pqueue
 * @param bucket
 * @return
 */
void* pqueue_bucket_remove_end(Queue* pqueue,    void* bucket);
/**
 * @brief pqueue_bucket_is_empty
 * @param pqueue
 * @param bucket
 * @return
 */
uint8_t pqueue_bucket_is_empty(Queue* pqueue,    void* bucket);

#endif
