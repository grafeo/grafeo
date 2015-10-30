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
#include <grafeo/queue.h>

// Alocar uma fila
GrfQueue* grf_queue_new(){
  GrfQueue* queue  = malloc(sizeof(GrfQueue));
  queue->length = 0;
  queue->begin  = NULL;
  queue->end    = NULL;
  return queue;
}
// Liberar a fila
void grf_queue_free(GrfQueue* queue){
  if(queue->begin) grf_list_free(queue->begin);
  free(queue);
}
// Adicionar no início
void grf_queue_prepend(GrfQueue* queue, void* value){
  queue->length++;
  queue->begin = grf_list_prepend(queue->begin, value);
  if(!queue->end) queue->end = queue->begin;
}
// Adicionar no final
void grf_queue_append(GrfQueue* queue, void* value){
  if(!queue->begin) grf_queue_prepend(queue,value);
  else{
    queue->length++;
    grf_list_append_at(queue->begin, queue->end, value);
    queue->end=grf_list_next(queue->end);
  }
}
// Adicionar antes de um item
void grf_queue_prepend_at(GrfQueue* queue, GrfList* item, void* value){
  queue->length++;
  queue->begin = grf_list_prepend_at(queue->begin, item, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Adicionar após um item
void grf_queue_append_at(GrfQueue* queue, GrfList* item, void* value){
  queue->length++;
  queue->begin = grf_list_append_at(queue->begin, item, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}
// Adicionar antes de item especificado pela sua posição
void grf_queue_prepend_at_index(GrfQueue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = grf_list_prepend_at_index(queue->begin, index, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Adicionar após um item especificado pela sua posição
void grf_queue_append_at_index(GrfQueue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = grf_list_append_at_index(queue->begin, index, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}
// Adicionar um item GrfList no início da fila
void grf_queue_prepend_item(GrfQueue* queue, GrfList* new_item){
  queue->length++;
  queue->begin->prev = new_item;
  grf_list_set_next(new_item, queue->begin);
  new_item->prev     = NULL;
  queue->begin       = new_item;
}
// Adicionar um item GrfList no fim da fila
void grf_queue_append_item(GrfQueue* queue, GrfList* new_item){
  queue->length++;
  grf_list_set_next(queue->end,new_item);
  new_item->prev     = queue->end;
  grf_list_set_next(new_item,NULL);
  queue->end         = new_item;
}
// Adicionar um item GrfList antes de um item
void grf_queue_prepend_item_at(GrfQueue* queue, GrfList* item, GrfList* new_item){
  queue->length++;
  queue->begin = grf_list_prepend_item_at(queue->begin, item, new_item);
}
// Adicionar um item GrfList depois de um item
void grf_queue_append_item_at(GrfQueue* queue, GrfList* item, GrfList* new_item){
  queue->length++;
  queue->begin = grf_list_append_item_at(queue->begin, item, new_item);
  if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}
// Adicionar um item GrfList antes de um item especificado pela sua posição
void grf_queue_prepend_item_at_index(GrfQueue* queue, uint32_t index, GrfList* new_item){
  queue->length++;
  queue->begin = grf_list_prepend_item_at_index(queue->begin, index, new_item);
}
// Adicionar um item GrfList depois de um item especificado pela sua posição
void grf_queue_append_item_at_index(GrfQueue* queue, uint32_t index, GrfList* new_item){
  queue->length++;
  queue->begin = grf_list_append_item_at_index(queue->begin, index, new_item);
  if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}
// Get first item
GrfList* grf_queue_begin(GrfQueue* queue){
  return queue->begin;
}
// Get last item
GrfList* grf_queue_end(GrfQueue* queue){
  return queue->end;
}
// True if length is zero
uint8_t grf_queue_is_empty(GrfQueue* queue){
  return queue->length == 0;
}
// Inverte a fila
void grf_queue_reverse(GrfQueue* queue){
  GrfList* new_end = queue->begin;
  queue->begin = grf_list_reverse(queue->begin);
  queue->end = new_end;
}
// ordena a fila
void grf_queue_sort(GrfQueue* queue, GrfCompareDataFunc compare_function, void* data){

}
// copia a fila
GrfQueue* grf_queue_copy(GrfQueue* queue){
  GrfQueue* queue2  = grf_queue_new();
  queue2->length = queue->length;
  queue2->begin  = grf_list_copy(queue->begin);
  queue2->end    = grf_list_end(queue2->begin);
  return queue2;
}
// vectorize a function within a queue
void grf_queue_foreach(GrfQueue* queue, GrfDataFunc data_function, void* data){
  grf_list_foreach(queue->begin, data_function, data);
}
// posição de um item especificado pelo seu valor
int32_t grf_queue_index_of(GrfQueue* queue, void* value){
  return grf_list_index_of(queue->begin, value);
}
// obter um item a partir de sua posição
GrfList* grf_queue_at(GrfQueue* queue, uint32_t index){
  return grf_list_at(queue->begin, index);
}
// obter o valor do item a partir de sua posição
void* grf_queue_value_at(GrfQueue* queue, uint32_t index){
  return grf_list_value(grf_queue_at(queue,index));
}
// obter o valor do item inicial
void* grf_queue_begin_value(GrfQueue* queue){
  if(grf_queue_is_empty(queue)) return NULL;
  return grf_list_value(queue->begin);
}
// obter o valor do item final
void* grf_queue_end_value(GrfQueue* queue){
  if(grf_queue_is_empty(queue)) return NULL;
  return grf_list_value(queue->end);
}
// Remover um item em uma fila
void grf_queue_remove(GrfQueue* queue, void* value){
  GrfList* item = grf_list_find(queue->begin,value);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = grf_list_remove(queue->begin, item);
  queue->length--;
}
// Remover um item em uma fila baseado em sua posição
void* grf_queue_remove_at(GrfQueue* queue, uint32_t index){
  if(queue->length <= index) return NULL;
  GrfList* item = grf_list_at(queue->begin,index);
  void* value = grf_list_value(item);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = grf_list_remove(queue->begin, item);
  queue->length--;
  return value;
}
// Remove first element
void* grf_queue_remove_begin(GrfQueue* queue){
  if(grf_queue_is_empty(queue)) return NULL;
  void* value  = grf_list_value(queue->begin);
  queue->begin = grf_list_remove_begin(queue->begin);
  if(!queue->begin) queue->end = NULL;
  queue->length--;
  return value;
}
// Remove last element
void* grf_queue_remove_end(GrfQueue* queue){
  if(grf_queue_is_empty(queue)) return NULL;
  void* value = grf_list_value(queue->end);
  queue->end   = queue->end->prev;
  queue->begin = grf_list_remove_end(queue->begin);
  queue->length--;
  return value;
}
// Obter o número de elementos da fila
uint32_t grf_queue_length(GrfQueue* queue){
  return queue->length;
}

void grf_queue_append_sorted(GrfQueue* queue, GrfCompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = grf_list_append_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}

void grf_queue_append_sorted_with_data(GrfQueue* queue, GrfCompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = grf_list_append_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}

void grf_queue_prepend_sorted(GrfQueue* queue, GrfCompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = grf_list_prepend_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}

void grf_queue_prepend_sorted_with_data(GrfQueue* queue, GrfCompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = grf_list_prepend_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(grf_list_next(queue->end)) queue->end = grf_list_next(queue->end);
}
