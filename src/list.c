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
#include <grafeo/list.h>
List* list_new(){
  List* item = malloc(sizeof(List));
  item->next = NULL;
  item->prev = NULL;
  item->value = NULL;
  return item;
}

List* list_append(List* list, void* value){
  List* item = list_new();
  item->value = value;

  if(list == NULL) return item;
  else{
    List* last = list_end(list);
    last->next = item;
    item->prev = last;
    item->next = NULL;
  }
  return list;
}

List* list_append_at(List *list, List *item, void *value){
  List* begin = list_begin(list);
  List* new_item = list_new();
  new_item->value = value;
  new_item->prev = item;
  new_item->next = item->next;
  item->next = new_item;
  if(new_item->next) new_item->next->prev = new_item;

  return begin;
}

List* list_append_at_index(List *list, uint32_t index, void *value){
  uint32_t i = 0;
  List* current = list;
  while(i++ < index && current->next) current = current->next;
  return list_append_at(list, current, value);
}

void list_free(List *list){
  List* item = list_begin(list);
  List* next = item->next;
  while(item->next){free(item); item = next; next = item->next;}
  free(item);
}

List* list_prepend(List *list, void *value){
  List* item = list_new();
  List* begin = list_begin(list);
  item->value = value;
  if(begin == NULL) return item;
  item->next = begin;
  begin->prev = item;
  return item;
}

List* list_prepend_at(List *list, List *item, void *value){
  List* begin = list_begin(list);
  List* new_item = list_new();
  new_item->value = value;

  new_item->next = item;
  new_item->prev = item->prev;
  item->prev = new_item;
  if(new_item->prev) new_item->prev->next = new_item;
  else begin = new_item;

  return begin;
}

List* list_prepend_at_index(List *list, uint32_t index, void *value){
  return list_prepend_at(list, list_at(list, index), value);
}

List* list_remove(List *list, List *item){
  if(item == NULL) return list;
  List* begin  = list_begin(list);
  if(item->next) item->next->prev = item->prev;
  if(item->prev) item->prev->next = item->next;
  else begin   = item->next;
  free(item);
  return begin;
}

List* list_remove_at_index(List *list, uint32_t index){
  return list_remove(list,list_at(list, index));
}

List* list_remove_begin(List *list){
  if(list == NULL) return NULL;
  List* item       = list_begin(list);
  List* new_begin  = item->next;
  if(new_begin) new_begin->prev = NULL;
  free(item);
  return new_begin;
}

List* list_remove_end(List *list){
  if(list == NULL) return NULL;
  List* begin = list_begin(list);
  List* end   = list_end(list);
  if(begin == end) begin = NULL;
  else end->prev->next = NULL;
  free(end);
  return begin;
}

List* list_remove_from_value(List *list, void *value){
  return list_remove(list, list_find(list, value));
}
int32_t list_index_of(List* list, void* value){
  if(!list) return -1;
  int32_t  i        = 0;
  List     *current = list;
  while(current && current->value != value) {current = current->next;i++;}
  if(current) return i;
  return -1;
}

List*    list_begin(List* list){
  if(list){
    List* current = list;
    while(current->prev != NULL) current = current->prev;
    return current;
  }
  return NULL;
}

List*    list_end(List* list){
  if(list){
    List* current = list;
    while(current->next != NULL) current = current->next;
    return current;
  }
  return NULL;
}

uint32_t list_length(List* list){
  uint32_t i = 0;
  List* current = list;
  while(current){current = current->next; i++;}
  return i;
}

uint8_t  list_is_empty(List* list){
  return list == NULL;
}

List*    list_at(List* list, uint32_t index){
  uint32_t i = 0;
  List* current = list;
  while(i++ < index && current->next) current = current->next;
  if(i < index) return NULL;
  return current;
}

void*    list_value_at(List* list, uint32_t index){
  List* item = list_at(list, index);
  if(item) return item->value;
  return NULL;
}

List*    list_next(List* list){
  return list->next;
}

List*    list_prev(List* list){
  return list->prev;
}

List*    list_join(List *list1, List *list2){
  List* end1 = list_end(list1);
  end1->next  = list2;
  list2->prev = end1;
  return list_begin(list1);
}

List*    list_split_at(List *list, uint32_t index){
  List* list2 = list_at(list, index);
  list2->prev->next = NULL;
  list2->prev = NULL;
  return list2;
}

List*    list_swap(List* list, List* item1, List* item2){
  List* item1_prev = item1->prev;
  List* item1_next = item1->next;
  List* item2_prev = item2->prev;
  List* item2_next = item2->next;

  // Change specified items
  if(item1_prev != item2)  item1->next = item2_next;
  else                     item1->next = item2;
  if(item1_next != item2)  item1->prev = item2_prev;
  else                     item1->prev = item2;
  if(item2_prev != item1)  item2->next = item1_next;
  else                     item2->next = item1;
  if(item2_next != item1)  item2->prev = item1_prev;
  else                     item2->prev = item1;

  // Change neighbor items
  if(item1_prev && item1_prev != item2) item1_prev->next = item2;
  if(item1_next && item1_next != item2) item1_next->prev = item2;
  if(item2_prev && item2_prev != item1) item2_prev->next = item1;
  if(item2_next && item2_next != item1) item2_next->prev = item1;
  return list_begin(list);
}

List*    list_swap_at(List* list, uint32_t index1, uint32_t index2){
  List* item1 = list_at(list, index1);
  List* item2 = list_at(list, index2);
  return list_swap(list, item1, item2);
}

List*    list_swap_values(List* list, List* item1, List* item2){
  void* tmp_value = item1->value;
  item1->value = item2->value;
  item2->value = tmp_value;
  return list;
}

List*    list_swap_values_at(List* list, uint32_t index1, uint32_t index2){
  List* item1 = list_at(list, index1);
  List* item2 = list_at(list, index2);
  return list_swap_values(list, item1, item2);
}

List*    list_copy(List* list){
  List* begin;
  List* list2 = NULL;
  List* item;
  while(list){
    item = list_new();
    item->value = list->value;
    if(list2){
      list2->next = item;
      item->prev  = list2;
    }
    else begin = item;
    list2 = item;
    list  = list->next;
  }
  return begin;
}

List*    list_replace(List* list, List* item, List* item2){
  List* item_prev = item->prev;
  List* item_next = item->next;
  List* item2_end = list_end(item2);
  item_prev->next = item2;
  item2->prev     = item_prev;
  item_next->prev = item2_end;
  item2_end->next = item_next;
  item->prev = NULL;
  item->next = NULL;
  return list_begin(list);
}

List*    list_replace_at(List* list, uint32_t index, List* item2){
  List* item = list_at(list, index);
  return list_replace(list, item, item2);
}

uint8_t  list_is_different(List* list, List* list2){
  while(list) {
    if(list->value != list2->value) return 1;
    list = list->next;
    list2 = list2->next;
  }
  // They have different size
  if((list && list->next) || (list2 && list2->next)) return 1;
  return 0;
}

uint8_t  list_is_equal(List* list, List* list2){
  return !list_is_different(list, list2);
}

List* list_find(List *list, void *value){
  List* current = list_begin(list);
  while(current->next && current->value != value) current = current->next;
  if(current->value != value) return NULL;
  else return current;
}

List* list_reverse(List *list){
  if(list == NULL) return NULL;
  List* current = list;
  List* current_next, *current_prev;
  List* begin = current;
  while(current){
    current_next = current->next;
    current_prev = current->prev;
    current->prev = current_next;
    current->next = current_prev;
    begin   = current;
    current = current_next;
  }
  return begin;
}
