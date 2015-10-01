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

void pqueue_append_at(Queue *pqueue, void *value_bucket, void *value, CompareFunc compare_func){
  List* bucket_item = NULL;
  uint8_t is_new    = 1;
  for(bucket_item = queue_begin(pqueue); bucket_item; bucket_item = list_next(bucket_item)){
    Bucket* bucket = (Bucket*)list_value(bucket_item);
    // Found the bucket
    uint8_t comparison = compare_func(bucket_value(bucket), value_bucket);
    if(comparison == 0){
      queue_append(bucket_queue(bucket), value);
      is_new = 0;
      break;
    }
    else if(comparison == 1) break;
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

// Adicionar na lista de bucket o valor
void  pqueue_prepend_at(Queue* pqueue, void* value_bucket, void* value, CompareFunc compare_func){
  List* bucket_item = NULL;
  uint8_t is_new    = 1;
  // Procura pelo bucket
  for(bucket_item = queue_begin(pqueue); bucket_item; bucket_item = list_next(bucket_item)){
    Bucket* bucket = (Bucket*)list_value(bucket_item);
    // Found the bucket
    uint8_t comparison = compare_func(bucket_value(bucket), value_bucket);
    if(comparison == 0){
      queue_prepend(bucket_queue(bucket), value);
      is_new = 0;
      break;
    }
    else if(comparison == 1) break;
  }
  if(is_new){
    Bucket* new_bucket = bucket_new();
    bucket_set_queue(new_bucket, queue_new());
    bucket_set_value(new_bucket, value_bucket);
    // Add the item to the bucket queue
    queue_prepend(bucket_queue(new_bucket), value);
    // Add the bucket to the main queue
    if(bucket_item) queue_prepend_at(pqueue, bucket_item, new_bucket);
    else            queue_append(pqueue, new_bucket);
  }
}
void  pqueue_remove_at(Queue* pqueue, void *value_bucket, void* value){
  Bucket* bucket      = pqueue_bucket_of(pqueue, value_bucket);
  queue_remove(bucket_queue(bucket),value);
}

Bucket* pqueue_bucket_of(Queue* pqueue, void* value_bucket){
  for(List* item=pqueue->begin;item;item = list_next(item)){
    Bucket* bucket = (Bucket*)list_value(item);
    if(bucket_value(bucket)==value_bucket)
      return bucket;
  }
  return NULL;
}

void  pqueue_remove_begin_at(Queue* pqueue, void* value_bucket){
  List* bucket_item = NULL;
  for(bucket_item = queue_begin(pqueue); bucket_item; bucket_item = list_next(bucket_item)){
    Bucket* bucket = (Bucket*) list_value(bucket_item);
    if(bucket_value(bucket) == value_bucket){
      queue_remove_begin(bucket_queue(bucket));
      break;
    }
  }
}
void  pqueue_remove_end_at  (Queue* pqueue, void* value_bucket){
  List* bucket_item = NULL;
  for(bucket_item = queue_begin(pqueue); bucket_item; bucket_item = list_next(bucket_item)){
    Bucket* bucket = (Bucket*) list_value(bucket_item);
    if(bucket_value(bucket) == value_bucket){
      queue_remove_end(bucket_queue(bucket));
      break;
    }
  }
}
void  pqueue_remove_begin(Queue* pqueue){
  // Get the begin of queue and remove its first item
  queue_remove_begin(bucket_queue((Bucket*)list_value(queue_begin(pqueue))));
}
void  pqueue_remove_end(Queue* pqueue){
  // Get the begin of queue and remove its last item
  queue_remove_end(bucket_queue((Bucket*)list_value(queue_begin(pqueue))));
}
void  pqueue_shrink(Queue* pqueue){
  while(!queue_is_empty(pqueue)){
    Bucket* bucket = (Bucket*)list_value(queue_begin(pqueue));
    if(queue_is_empty(bucket_queue(bucket))){
      queue_free(bucket_queue(bucket));
      bucket_free(bucket);
      queue_remove_begin(pqueue);
    }
    else break;
  }
}
void* pqueue_at(Queue* pqueue, uint32_t index){
  return bucket_value((Bucket*) list_value(queue_at(pqueue,index)));
}
void* pqueue_bucket_at(Queue* pqueue, uint32_t bucket_index, uint32_t index){
  return list_value(queue_at(bucket_queue((Bucket*) list_value(queue_at(pqueue,bucket_index))),index));
}
