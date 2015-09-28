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
  list_set_next(item,NULL);
  list_set_prev(item,NULL);
  list_set_value(item,NULL);

  return item;
}
List* list_new_with_value(void* value){
  List* list = list_new();
  list_set_value(list, value);
  return list;
}

List* list_append(List* list, void* value){
  List* item = list_new_with_value(value);

  if(list == NULL) return item;
  else{
    List* last = list_end(list);
    list_set_next(last,item);
    list_set_prev(item,last);
    list_set_next(item,NULL);
  }
  return list;
}

void list_set_next(List* item, List* next){
  item->base.next = (SList*)next;
}
void list_set_prev(List* item, List* prev){
  item->prev = prev;
}
void list_set_value(List* item, void* value){
  item->base.value = value;
}

List* list_append_at(List *list, List *item, void *value){
  List* begin = list_begin(list);
  List* new_item = list_new_with_value(value);
  return list_append_item_at(begin, item, new_item);
}

List* list_append_at_index(List *list, uint32_t index, void *value){
  return list_append_at(list, list_at(list, index), value);
}

void list_free(List *list){
  if(list){
    List* item = list_begin(list);
    List* next = list_next(item);
    while(list_next(item)){free(item); item = next; next = list_next(item);}
    free(item);
  }
}

List* list_prepend(List *list, void *value){
  List* item  = list_new_with_value(value);
  List* begin = list_begin(list);
  if(begin == NULL) return item;
  list_set_next(item , begin);
  list_set_prev(begin, item );
  return item;
}

List* list_prepend_at(List *list, List *item, void *value){
  List* begin = list_begin(list);
  List* new_item = list_new_with_value(value);
  return list_prepend_item_at(begin,item,new_item);
}

List* list_prepend_item(List *list, List *new_item){
  if(list == NULL) return new_item;
  list_set_next(new_item, list);
  list_set_prev(list, new_item);
  return new_item;
}

List* list_prepend_at_index(List *list, uint32_t index, void *value){
  return list_prepend_at(list, list_at(list, index), value);
}

List* list_remove(List *list, List *item){
  if(item == NULL) return list;
  List* begin  = list_begin(list);
  if(list_next(item)) list_set_prev(list_next(item),list_prev(item));
  if(list_prev(item)) list_set_next(list_prev(item),list_next(item));
  else begin   = list_next(item);
  free(item);
  return begin;
}

List* list_prepend_item_at(List* list, List* item, List* item_new){
  list_set_next(item_new,item);
  list_set_prev(item_new,list_prev(item));
  list_set_prev(item, item_new);
  if(list_prev(item_new)) list_set_next(list_prev(item_new),item_new);
  else list = item_new;
  return list;
}

List* list_append_item_at(List* list, List* item, List* item_new){
  list_set_prev(item_new,item);
  list_set_next(item_new,list_next(item));
  list_set_next(item, item_new);
  if(list_next(item_new)) list_set_prev(list_next(item_new),item_new);

  return list;
}

List* list_prepend_item_at_index(List* list, uint32_t index, List* item_new){
  return list_prepend_item_at(list, list_at(list, index), item_new);
}

List* list_append_item_at_index(List* list, uint32_t index, List* item_new){
  return list_append_item_at(list, list_at(list, index), item_new);
}

List* list_remove_at_index(List *list, uint32_t index){
  return list_remove(list,list_at(list, index));
}

List* list_remove_begin(List *list){
  if(list == NULL) return NULL;
  List* item       = list_begin(list);
  List* new_begin  = list_next(item);
  if(new_begin) list_set_prev(new_begin,NULL);
  free(item);
  return new_begin;
}

List* list_remove_end(List *list){
  if(list == NULL) return NULL;
  List* begin = list_begin(list);
  List* end   = list_end(list);
  if(begin == end) begin = NULL;
  else list_set_next(list_prev(end),NULL);
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
  while(current && list_value(current) != value) {current = list_next(current);i++;}
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
    while(list_next(current) != NULL) current = list_next(current);
    return current;
  }
  return NULL;
}

uint32_t list_length(List* list){
  uint32_t i = 0;
  List* current = list;
  while(current){current = list_next(current); i++;}
  return i;
}

uint8_t  list_is_empty(List* list){
  return list == NULL;
}

List*    list_at(List* list, uint32_t index){
  uint32_t i = 0;
  List* current = list;
  while(i++ < index && list_next(current)) current = list_next(current);
  if(i < index) return NULL;
  return current;
}

void*    list_value_at(List* list, uint32_t index){
  List* item = list_at(list, index);
  if(item) return list_value(item);
  return NULL;
}

List*    list_next(List* list){
  return (List*)list->base.next;
}

List*    list_prev(List* list){
  return list->prev;
}

List*    list_join(List *list1, List *list2){
  List* end1 = list_end(list1);
  list_set_next(end1,list2);
  list_set_prev(list2,end1);
  return list_begin(list1);
}

List*    list_split_at(List *list, uint32_t index){
  List* list2 = list_at(list, index);
  list_set_next(list_prev(list2),NULL);
  list_set_prev(list2,NULL);
  return list2;
}

List*    list_swap_items(List* list, List* item1, List* item2){
  List* item1_prev = list_prev(item1);
  List* item1_next = list_next(item1);
  List* item2_prev = list_prev(item2);
  List* item2_next = list_next(item2);

  // Change specified items
  if(item1_prev != item2)  list_set_next(item1,item2_next);
  else                     list_set_next(item1,item2);
  if(item1_next != item2)  list_set_prev(item1,item2_prev);
  else                     list_set_prev(item1,item2);
  if(item2_prev != item1)  list_set_next(item2,item1_next);
  else                     list_set_next(item2,item1);
  if(item2_next != item1)  list_set_prev(item2,item1_prev);
  else                     list_set_prev(item2,item1);

  // Change neighbor items
  if(item1_prev && item1_prev != item2) list_set_next(item1_prev,item2);
  if(item1_next && item1_next != item2) list_set_prev(item1_next,item2);
  if(item2_prev && item2_prev != item1) list_set_next(item2_prev,item1);
  if(item2_next && item2_next != item1) list_set_prev(item2_next,item1);
  return list_begin(list);
}

List*    list_swap_items_at(List* list, uint32_t index1, uint32_t index2){
  List* item1 = list_at(list, index1);
  List* item2 = list_at(list, index2);
  return list_swap_items(list, item1, item2);
}

List*    list_swap(List* list, List* item1, List* item2){
  void* tmp_value = list_value(item1);
  list_set_value(item1,list_value(item2));
  list_set_value(item2,tmp_value);
  return list;
}

List*    list_swap_at(List* list, uint32_t index1, uint32_t index2){
  List* item1 = list_at(list, index1);
  List* item2 = list_at(list, index2);
  return list_swap(list, item1, item2);
}

List*    list_copy(List* list){
  List* begin;
  List* list2 = NULL;
  List* item;
  while(list){
    item = list_new();
    list_set_value(item,list_value(list));
    if(list2){
      list_set_next(list2,item);
      list_set_prev(item,list2);
    }
    else begin = item;
    list2 = item;
    list  = list_next(list);
  }
  return begin;
}

List*    list_replace(List* list, List* item, List* item2){
  List* item_prev = list_prev(item);
  List* item_next = list_next(item);

  list_set_next(item_prev, item2);
  list_set_prev(item_next, item2);
  list_set_prev(item2, item_prev);
  list_set_next(item2, item_next);

  free(item);

  return list_begin(list);
}

List*    list_replace_at(List* list, uint32_t index, List* item2){
  List* item = list_at(list, index);
  return list_replace(list, item, item2);
}

uint8_t  list_is_different(List* list, List* list2){
  while(list) {
    if(list_value(list) != list_value(list2)) return 1;
    list = list_next(list);
    list2 = list_next(list2);
  }
  // They have different size
  if((list && list_next(list)) || (list2 && list_next(list2))) return 1;
  return 0;
}

uint8_t  list_is_equal(List* list, List* list2){
  return !list_is_different(list, list2);
}

List* list_find(List *list, void *value){
  List* current = list_begin(list);
  while(list_next(current) && list_value(current) != value) current = list_next(current);
  if(list_value(current) != value) return NULL;
  else return current;
}

List* list_reverse(List *list){
  List *after, *before;
  while(list){
    before = list_prev(list);
    after  = list_next(list);

    list_set_next(list, before);
    list_set_prev(list, after);

    before = list;
    list   = after;
  }
  return before;
}

void list_foreach(List* list, DataFunc data_function, void* value){
  List* current;
  for(current = list; current; current = list_next(current))
    data_function(current, value);
}

List* list_append_sorted(List* list, CompareFunc compare_function, void* value){
  List* current = list, *before = NULL;
  if(!list) return list_prepend(NULL, value);
  while(current && compare_function(list_value(current), value) <= 0){
    before  = current;
    current = list_next(current);
  }
  if(!current) return list_append_at(list,before,value);
  return list_prepend_at(list, current, value);
}
List* list_append_sorted_with_data(List* list, CompareDataFunc compare_function, void* value, void* user_data){
  List* current = list, *before = NULL;
  if(!list) return list_prepend(NULL, value);
  while(current && compare_function(list_value(current), value, user_data) <= 0){
    before  = current;
    current = list_next(current);
  }
  if(!current) return list_append_at(list,before,value);
  return list_prepend_at(list, current, value);
}
List* list_prepend_sorted(List* list, CompareFunc compare_function, void* value){
  List* current = list, *before = NULL;
  if(!list) return list_prepend(NULL, value);
  while(current && compare_function(list_value(current), value) < 0){
    before  = current;
    current = list_next(current);
  }
  if(!current) return list_append_at(list,before,value);
  return list_prepend_at(list, current, value);
}
List* list_prepend_sorted_with_data(List* list, CompareDataFunc compare_function, void* value, void* user_data){
  List* current = list, *before = NULL;
  if(!list) return list_prepend(NULL, value);
  while(current && compare_function(list_value(current), value, user_data) < 0){
    before  = current;
    current = list_next(current);
  }
  if(!current) return list_append_at(list,before,value);
  return list_prepend_at(list, current, value);
}
void* list_value(List* item){
  return item->base.value;
}
