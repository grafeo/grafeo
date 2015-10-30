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

void grf_pqueue_append_at(GrfQueue *pqueue, void *value_bucket, void *value, GrfCompareFunc compare_func){
  GrfList* grf_bucket_item = NULL;
  uint8_t is_new    = 1;
  for(grf_bucket_item = grf_queue_begin(pqueue); grf_bucket_item; grf_bucket_item = grf_list_next(grf_bucket_item)){
    GrfBucket* bucket = (GrfBucket*)grf_list_value(grf_bucket_item);
    // Found the bucket
    uint8_t comparison = compare_func(grf_bucket_value(bucket), value_bucket);
    if(comparison == 0){
      grf_queue_append(grf_bucket_queue(bucket), value);
      is_new = 0;
      break;
    }
    else if(comparison == 1) break;
  }
  // Insert the new bucket if found proper place
  if(is_new){
    GrfBucket* new_bucket = grf_bucket_new();
    grf_bucket_set_queue(new_bucket, grf_queue_new());
    grf_bucket_set_value(new_bucket, value_bucket);
    grf_queue_append(grf_bucket_queue(new_bucket),value);
    if(grf_bucket_item) grf_queue_prepend_at(pqueue, grf_bucket_item, new_bucket);
    else            grf_queue_append(pqueue, new_bucket);
  }
}

// Adicionar na lista de bucket o valor
void  grf_pqueue_prepend_at(GrfQueue* pqueue, void* value_bucket, void* value, GrfCompareFunc compare_func){
  GrfList* grf_bucket_item = NULL;
  uint8_t is_new    = 1;
  // Procura pelo bucket
  for(grf_bucket_item = grf_queue_begin(pqueue); grf_bucket_item; grf_bucket_item = grf_list_next(grf_bucket_item)){
    GrfBucket* bucket = (GrfBucket*)grf_list_value(grf_bucket_item);
    // Found the bucket
    uint8_t comparison = compare_func(grf_bucket_value(bucket), value_bucket);
    if(comparison == 0){
      grf_queue_prepend(grf_bucket_queue(bucket), value);
      is_new = 0;
      break;
    }
    else if(comparison == 1) break;
  }
  if(is_new){
    GrfBucket* new_bucket = grf_bucket_new();
    grf_bucket_set_queue(new_bucket, grf_queue_new());
    grf_bucket_set_value(new_bucket, value_bucket);
    // Add the item to the bucket queue
    grf_queue_prepend(grf_bucket_queue(new_bucket), value);
    // Add the bucket to the main queue
    if(grf_bucket_item) grf_queue_prepend_at(pqueue, grf_bucket_item, new_bucket);
    else            grf_queue_append(pqueue, new_bucket);
  }
}
void  grf_pqueue_remove_at(GrfQueue* pqueue, void *value_bucket, void* value){
  GrfBucket* bucket      = grf_pqueue_grf_bucket_of(pqueue, value_bucket);
  grf_queue_remove(grf_bucket_queue(bucket),value);
}

GrfBucket* grf_pqueue_grf_bucket_of(GrfQueue* pqueue, void* value_bucket){
  GrfList* item;
  for(item=pqueue->begin;item;item = grf_list_next(item)){
    GrfBucket* bucket = (GrfBucket*)grf_list_value(item);
    if(grf_bucket_value(bucket)==value_bucket)
      return bucket;
  }
  return NULL;
}

void  grf_pqueue_remove_begin_at(GrfQueue* pqueue, void* value_bucket){
  GrfList* grf_bucket_item = NULL;
  for(grf_bucket_item = grf_queue_begin(pqueue); grf_bucket_item; grf_bucket_item = grf_list_next(grf_bucket_item)){
    GrfBucket* bucket = (GrfBucket*) grf_list_value(grf_bucket_item);
    if(grf_bucket_value(bucket) == value_bucket){
      grf_queue_remove_begin(grf_bucket_queue(bucket));
      break;
    }
  }
}
void  grf_pqueue_remove_end_at  (GrfQueue* pqueue, void* value_bucket){
  GrfList* grf_bucket_item = NULL;
  for(grf_bucket_item = grf_queue_begin(pqueue); grf_bucket_item; grf_bucket_item = grf_list_next(grf_bucket_item)){
    GrfBucket* bucket = (GrfBucket*) grf_list_value(grf_bucket_item);
    if(grf_bucket_value(bucket) == value_bucket){
      grf_queue_remove_end(grf_bucket_queue(bucket));
      break;
    }
  }
}
void  grf_pqueue_remove_begin(GrfQueue* pqueue){
  // Get the begin of queue and remove its first item
  grf_queue_remove_begin(grf_bucket_queue((GrfBucket*)grf_list_value(grf_queue_begin(pqueue))));
}
void  grf_pqueue_remove_end(GrfQueue* pqueue){
  // Get the begin of queue and remove its last item
  grf_queue_remove_end(grf_bucket_queue((GrfBucket*)grf_list_value(grf_queue_begin(pqueue))));
}
void  grf_pqueue_shrink(GrfQueue* pqueue){
  while(!grf_queue_is_empty(pqueue)){
    GrfBucket* bucket = (GrfBucket*)grf_list_value(grf_queue_begin(pqueue));
    if(grf_queue_is_empty(grf_bucket_queue(bucket))){
      grf_queue_free(grf_bucket_queue(bucket));
      grf_bucket_free(bucket);
      grf_queue_remove_begin(pqueue);
    }
    else break;
  }
}
void* grf_pqueue_at(GrfQueue* pqueue, uint32_t index){
  return grf_bucket_value((GrfBucket*) grf_list_value(grf_queue_at(pqueue,index)));
}
void* grf_pqueue_grf_bucket_at(GrfQueue* pqueue, uint32_t grf_bucket_index, uint32_t index){
  return grf_list_value(grf_queue_at(grf_bucket_queue((GrfBucket*) grf_list_value(grf_queue_at(pqueue,grf_bucket_index))),index));
}
