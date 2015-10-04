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
Queue* queue_new(){
  Queue* queue  = malloc(sizeof(Queue));
  queue->length = 0;
  queue->begin  = NULL;
  queue->end    = NULL;
  return queue;
}
// Liberar a fila
void queue_free(Queue* queue){
  if(queue->begin) list_free(queue->begin);
  free(queue);
}
// Adicionar no início
void queue_prepend(Queue* queue, void* value){
  queue->length++;
  queue->begin = list_prepend(queue->begin, value);
  if(!queue->end) queue->end = queue->begin;
}
// Adicionar no final
void queue_append(Queue* queue, void* value){
  if(!queue->begin) queue_prepend(queue,value);
  else{
    queue->length++;
    list_append_at(queue->begin, queue->end, value);
    queue->end=list_next(queue->end);
  }
}
// Adicionar antes de um item
void queue_prepend_at(Queue* queue, List* item, void* value){
  queue->length++;
  queue->begin = list_prepend_at(queue->begin, item, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Adicionar após um item
void queue_append_at(Queue* queue, List* item, void* value){
  queue->length++;
  queue->begin = list_append_at(queue->begin, item, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}
// Adicionar antes de item especificado pela sua posição
void queue_prepend_at_index(Queue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = list_prepend_at_index(queue->begin, index, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Adicionar após um item especificado pela sua posição
void queue_append_at_index(Queue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = list_append_at_index(queue->begin, index, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}
// Adicionar um item List no início da fila
void queue_prepend_item(Queue* queue, List* new_item){
  queue->length++;
  queue->begin->prev = new_item;
  list_set_next(new_item, queue->begin);
  new_item->prev     = NULL;
  queue->begin       = new_item;
}
// Adicionar um item List no fim da fila
void queue_append_item(Queue* queue, List* new_item){
  queue->length++;
  list_set_next(queue->end,new_item);
  new_item->prev     = queue->end;
  list_set_next(new_item,NULL);
  queue->end         = new_item;
}
// Adicionar um item List antes de um item
void queue_prepend_item_at(Queue* queue, List* item, List* new_item){
  queue->length++;
  queue->begin = list_prepend_item_at(queue->begin, item, new_item);
}
// Adicionar um item List depois de um item
void queue_append_item_at(Queue* queue, List* item, List* new_item){
  queue->length++;
  queue->begin = list_append_item_at(queue->begin, item, new_item);
  if(list_next(queue->end)) queue->end = list_next(queue->end);
}
// Adicionar um item List antes de um item especificado pela sua posição
void queue_prepend_item_at_index(Queue* queue, uint32_t index, List* new_item){
  queue->length++;
  queue->begin = list_prepend_item_at_index(queue->begin, index, new_item);
}
// Adicionar um item List depois de um item especificado pela sua posição
void queue_append_item_at_index(Queue* queue, uint32_t index, List* new_item){
  queue->length++;
  queue->begin = list_append_item_at_index(queue->begin, index, new_item);
  if(list_next(queue->end)) queue->end = list_next(queue->end);
}
// Get first item
List* queue_begin(Queue* queue){
  return queue->begin;
}
// Get last item
List* queue_end(Queue* queue){
  return queue->end;
}
// True if length is zero
uint8_t queue_is_empty(Queue* queue){
  return queue->length == 0;
}
// Inverte a fila
void queue_reverse(Queue* queue){
  List* new_end = queue->begin;
  queue->begin = list_reverse(queue->begin);
  queue->end = new_end;
}
// ordena a fila
void queue_sort(Queue* queue, CompareDataFunc compare_function, void* data){

}
// copia a fila
Queue* queue_copy(Queue* queue){
  Queue* queue2  = queue_new();
  queue2->length = queue->length;
  queue2->begin  = list_copy(queue->begin);
  queue2->end    = list_end(queue2->begin);
  return queue2;
}
// vectorize a function within a queue
void queue_foreach(Queue* queue, DataFunc data_function, void* data){
  list_foreach(queue->begin, data_function, data);
}
// posição de um item especificado pelo seu valor
int32_t queue_index_of(Queue* queue, void* value){
  return list_index_of(queue->begin, value);
}
// obter um item a partir de sua posição
List* queue_at(Queue* queue, uint32_t index){
  return list_at(queue->begin, index);
}
// obter o valor do item a partir de sua posição
void* queue_value_at(Queue* queue, uint32_t index){
  return list_value(queue_at(queue,index));
}
// obter o valor do item inicial
void* queue_begin_value(Queue* queue){
  if(queue_is_empty(queue)) return NULL;
  return list_value(queue->begin);
}
// obter o valor do item final
void* queue_end_value(Queue* queue){
  if(queue_is_empty(queue)) return NULL;
  return list_value(queue->end);
}
// Remover um item em uma fila
void queue_remove(Queue* queue, void* value){
  List* item = list_find(queue->begin,value);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = list_remove(queue->begin, item);
  queue->length--;
}
// Remover um item em uma fila baseado em sua posição
void* queue_remove_at(Queue* queue, uint32_t index){
  if(queue->length <= index) return NULL;
  List* item = list_at(queue->begin,index);
  void* value = list_value(item);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = list_remove(queue->begin, item);
  queue->length--;
  return value;
}
// Remove first element
void* queue_remove_begin(Queue* queue){
  if(queue_is_empty(queue)) return NULL;
  void* value  = list_value(queue->begin);
  queue->begin = list_remove_begin(queue->begin);
  if(!queue->begin) queue->end = NULL;
  queue->length--;
  return value;
}
// Remove last element
void* queue_remove_end(Queue* queue){
  if(queue_is_empty(queue)) return NULL;
  void* value = list_value(queue->end);
  queue->end   = queue->end->prev;
  queue->begin = list_remove_end(queue->begin);
  queue->length--;
  return value;
}
// Obter o número de elementos da fila
uint32_t queue_length(Queue* queue){
  return queue->length;
}

void queue_append_sorted(Queue* queue, CompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = list_append_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}

void queue_append_sorted_with_data(Queue* queue, CompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = list_append_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}

void queue_prepend_sorted(Queue* queue, CompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = list_prepend_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}

void queue_prepend_sorted_with_data(Queue* queue, CompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = list_prepend_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(list_next(queue->end)) queue->end = list_next(queue->end);
}
