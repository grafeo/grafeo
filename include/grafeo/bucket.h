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
#ifndef GRAFEO_BUCKET_H
#define GRAFEO_BUCKET_H
#include <grafeo/queue.h>
/**
 * @brief Bucket structure, just a queue with an additional attribute which
 * is the same for every item
 *
 * You can put additional attributes just by extending Bucket, or inserting
 * a struct of attributes in the `value` variable.
 */
typedef struct _Bucket{
  Queue* queue; /**< Queue of elements in the bucket */
  void*  value; /**< Unique attribute of the queue */
}Bucket;
/**
 * @brief bucket_new
 * @return
 */
Bucket* bucket_new();
/**
 * @brief bucket_queue
 * @param bucket
 * @return
 */
Queue*  bucket_queue(Bucket* bucket);
/**
 * @brief bucket_value
 * @param bucket
 * @return
 */
void*   bucket_value(Bucket* bucket);
/**
 * @brief bucket_set_queue
 * @param bucket
 * @param queue
 */
void    bucket_set_queue(Bucket* bucket,Queue* queue);
/**
 * @brief bucket_set_value
 * @param bucket
 * @param value
 */
void    bucket_set_value(Bucket* bucket, void* value);
/**
 * @brief bucket_free
 * @param bucket
 */
void    bucket_free(Bucket* bucket);

#endif
