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
#include <grafeo/pqueue.h>

void pqueue_append_at(Queue *pqueue, void *value_bucket, void *value){
  List* bucket_item = NULL;
  uint8_t is_new    = 1;
  for(bucket_item = queue_begin(pqueue); bucket_item; bucket_item = list_next(bucket_item)){
    Bucket* bucket = (Bucket*)list_value(bucket_item);
    // Found the bucket
    if(bucket_value(bucket) == value_bucket){
      queue_append(bucket_queue(bucket), value);
      is_new = 0;
    }
    else if(bucket_value(bucket) > value_bucket) break;
  }
  // Insert the new bucket if found proper place
  if(is_new){
    Bucket* new_bucket = bucket_new();
    bucket_set_queue(new_bucket, queue_new());
    bucket_set_value(new_bucket, value_bucket);
    queue_append(bucket_queue(new_bucket),value);
    if(bucket_item) queue_prepend_at(pqueue, bucket_item, new_bucket);
    else            queue_append(pqueue, new_bucket);
  }
}
void  pqueue_prepend_at(Queue* pqueue, void* bucket_value, void* value){

}
void  pqueue_remove_begin_at(Queue* pqueue, void* bucket){

}
void  pqueue_remove_end_at  (Queue* pqueue, void* bucket){

}
void  pqueue_remove_begin(Queue* pqueue){

}
void  pqueue_remove_end(Queue* pqueue){

}
void  pqueue_shrink(Queue* pqueue){

}
void* pqueue_at(Queue* pqueue, uint32_t index){

}
void* pqueue_bucket_at(Queue* pqueue, uint32_t bucket_index, uint32_t index){

}
