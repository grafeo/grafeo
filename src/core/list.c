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
#include <grafeo/core.h>
GrfList* grf_list_new(){
  GrfList* item = malloc(sizeof(GrfList));
  grf_list_set_next(item,NULL);
  grf_list_set_prev(item,NULL);
  grf_list_set_value(item,NULL);

  return item;
}
GrfList* grf_list_new_with_value(void* value){
  GrfList* list = grf_list_new();
  grf_list_set_value(list, value);
  return list;
}

GrfList* grf_list_append(GrfList* list, void* value){
  GrfList* item = grf_list_new_with_value(value);

  if(list == NULL) return item;
  else{
    GrfList* last = grf_list_end(list);
    grf_list_set_next(last,item);
    grf_list_set_prev(item,last);
    grf_list_set_next(item,NULL);
  }
  return list;
}

void grf_list_set_next(GrfList* item, GrfList* next){
  item->base.next = (GrfSList*)next;
}
void grf_list_set_prev(GrfList* item, GrfList* prev){
  item->prev = prev;
}
void grf_list_set_value(GrfList* item, void* value){
  item->base.value = value;
}

GrfList* grf_list_append_at(GrfList *list, GrfList *item, void *value){
  GrfList* begin = grf_list_begin(list);
  GrfList* new_item = grf_list_new_with_value(value);
  return grf_list_append_item_at(begin, item, new_item);
}

GrfList* grf_list_append_at_index(GrfList *list, uint32_t index, void *value){
  return grf_list_append_at(list, grf_list_at(list, index), value);
}

void grf_list_free(GrfList *list){
  if(list){
    GrfList* item = grf_list_begin(list);
    GrfList* next = grf_list_next(item);
    while(grf_list_next(item)){free(item); item = next; next = grf_list_next(item);}
    free(item);
  }
}

GrfList* grf_list_prepend(GrfList *list, void *value){
  GrfList* item  = grf_list_new_with_value(value);
  GrfList* begin = grf_list_begin(list);
  if(begin == NULL) return item;
  grf_list_set_next(item , begin);
  grf_list_set_prev(begin, item );
  return item;
}

GrfList* grf_list_prepend_at(GrfList *list, GrfList *item, void *value){
  GrfList* begin = grf_list_begin(list);
  GrfList* new_item = grf_list_new_with_value(value);
  return grf_list_prepend_item_at(begin,item,new_item);
}

GrfList* grf_list_prepend_item(GrfList *list, GrfList *new_item){
  if(list == NULL) return new_item;
  grf_list_set_next(new_item, list);
  grf_list_set_prev(list, new_item);
  return new_item;
}

GrfList* grf_list_prepend_at_index(GrfList *list, uint32_t index, void *value){
  return grf_list_prepend_at(list, grf_list_at(list, index), value);
}

GrfList* grf_list_remove(GrfList *list, GrfList *item){
  if(item == NULL) return list;
  GrfList* begin  = grf_list_begin(list);
  if(grf_list_next(item)) grf_list_set_prev(grf_list_next(item),grf_list_prev(item));
  if(grf_list_prev(item)) grf_list_set_next(grf_list_prev(item),grf_list_next(item));
  else begin   = grf_list_next(item);
  free(item);
  return begin;
}

GrfList* grf_list_prepend_item_at(GrfList* list, GrfList* item, GrfList* item_new){
  grf_list_set_next(item_new,item);
  grf_list_set_prev(item_new,grf_list_prev(item));
  grf_list_set_prev(item, item_new);
  if(grf_list_prev(item_new)) grf_list_set_next(grf_list_prev(item_new),item_new);
  else list = item_new;
  return list;
}

GrfList* grf_list_append_item_at(GrfList* list, GrfList* item, GrfList* item_new){
  grf_list_set_prev(item_new,item);
  grf_list_set_next(item_new,grf_list_next(item));
  grf_list_set_next(item, item_new);
  if(grf_list_next(item_new)) grf_list_set_prev(grf_list_next(item_new),item_new);

  return list;
}

GrfList* grf_list_prepend_item_at_index(GrfList* list, uint32_t index, GrfList* item_new){
  return grf_list_prepend_item_at(list, grf_list_at(list, index), item_new);
}

GrfList* grf_list_append_item_at_index(GrfList* list, uint32_t index, GrfList* item_new){
  return grf_list_append_item_at(list, grf_list_at(list, index), item_new);
}

GrfList* grf_list_remove_at_index(GrfList *list, uint32_t index){
  return grf_list_remove(list,grf_list_at(list, index));
}

GrfList* grf_list_remove_begin(GrfList *list){
  if(list == NULL) return NULL;
  GrfList* item       = grf_list_begin(list);
  GrfList* new_begin  = grf_list_next(item);
  if(new_begin) grf_list_set_prev(new_begin,NULL);
  free(item);
  return new_begin;
}

GrfList* grf_list_remove_end(GrfList *list){
  if(list == NULL) return NULL;
  GrfList* begin = grf_list_begin(list);
  GrfList* end   = grf_list_end(list);
  if(begin == end) begin = NULL;
  else grf_list_set_next(grf_list_prev(end),NULL);
  free(end);
  return begin;
}

GrfList* grf_list_remove_from_value(GrfList *list, void *value){
  return grf_list_remove(list, grf_list_find(list, value));
}
int32_t grf_list_index_of(GrfList* list, void* value){
  if(!list) return -1;
  int32_t  i        = 0;
  GrfList     *current = list;
  while(current && grf_list_value(current) != value) {current = grf_list_next(current);i++;}
  if(current) return i;
  return -1;
}

GrfList*    grf_list_begin(GrfList* list){
  if(list){
    GrfList* current = list;
    while(current->prev != NULL) current = current->prev;
    return current;
  }
  return NULL;
}

GrfList*    grf_list_end(GrfList* list){
  if(list){
    GrfList* current = list;
    while(grf_list_next(current) != NULL) current = grf_list_next(current);
    return current;
  }
  return NULL;
}

uint32_t grf_list_length(GrfList* list){
  uint32_t i = 0;
  GrfList* current = list;
  while(current){current = grf_list_next(current); i++;}
  return i;
}

uint8_t  grf_list_is_empty(GrfList* list){
  return list == NULL;
}

GrfList*    grf_list_at(GrfList* list, uint32_t index){
  uint32_t i = 0;
  GrfList* current = list;
  while(i++ < index && grf_list_next(current)) current = grf_list_next(current);
  if(i < index) return NULL;
  return current;
}

void*    grf_list_value_at(GrfList* list, uint32_t index){
  GrfList* item = grf_list_at(list, index);
  if(item) return grf_list_value(item);
  return NULL;
}

GrfList*    grf_list_next(GrfList* list){
  return (GrfList*)list->base.next;
}

GrfList*    grf_list_prev(GrfList* list){
  return list->prev;
}

GrfList*    grf_list_join(GrfList *list1, GrfList *list2){
  GrfList* end1 = grf_list_end(list1);
  grf_list_set_next(end1,list2);
  grf_list_set_prev(list2,end1);
  return grf_list_begin(list1);
}

GrfList*    grf_list_split_at(GrfList *list, uint32_t index){
  GrfList* list2 = grf_list_at(list, index);
  grf_list_set_next(grf_list_prev(list2),NULL);
  grf_list_set_prev(list2,NULL);
  return list2;
}

GrfList*    grf_list_swap_items(GrfList* list, GrfList* item1, GrfList* item2){
  GrfList* item1_prev = grf_list_prev(item1);
  GrfList* item1_next = grf_list_next(item1);
  GrfList* item2_prev = grf_list_prev(item2);
  GrfList* item2_next = grf_list_next(item2);

  // Change specified items
  if(item1_prev != item2)  grf_list_set_next(item1,item2_next);
  else                     grf_list_set_next(item1,item2);
  if(item1_next != item2)  grf_list_set_prev(item1,item2_prev);
  else                     grf_list_set_prev(item1,item2);
  if(item2_prev != item1)  grf_list_set_next(item2,item1_next);
  else                     grf_list_set_next(item2,item1);
  if(item2_next != item1)  grf_list_set_prev(item2,item1_prev);
  else                     grf_list_set_prev(item2,item1);

  // Change neighbor items
  if(item1_prev && item1_prev != item2) grf_list_set_next(item1_prev,item2);
  if(item1_next && item1_next != item2) grf_list_set_prev(item1_next,item2);
  if(item2_prev && item2_prev != item1) grf_list_set_next(item2_prev,item1);
  if(item2_next && item2_next != item1) grf_list_set_prev(item2_next,item1);
  return grf_list_begin(list);
}

GrfList*    grf_list_swap_items_at(GrfList* list, uint32_t index1, uint32_t index2){
  GrfList* item1 = grf_list_at(list, index1);
  GrfList* item2 = grf_list_at(list, index2);
  return grf_list_swap_items(list, item1, item2);
}

GrfList*    grf_list_swap(GrfList* list, GrfList* item1, GrfList* item2){
  void* tmp_value = grf_list_value(item1);
  grf_list_set_value(item1,grf_list_value(item2));
  grf_list_set_value(item2,tmp_value);
  return list;
}

GrfList*    grf_list_swap_at(GrfList* list, uint32_t index1, uint32_t index2){
  GrfList* item1 = grf_list_at(list, index1);
  GrfList* item2 = grf_list_at(list, index2);
  return grf_list_swap(list, item1, item2);
}

GrfList*    grf_list_copy(GrfList* list){
  GrfList* begin;
  GrfList* list2 = NULL;
  GrfList* item;
  while(list){
    item = grf_list_new();
    grf_list_set_value(item,grf_list_value(list));
    if(list2){
      grf_list_set_next(list2,item);
      grf_list_set_prev(item,list2);
    }
    else begin = item;
    list2 = item;
    list  = grf_list_next(list);
  }
  return begin;
}

GrfList*    grf_list_replace(GrfList* list, GrfList* item, GrfList* item2){
  GrfList* item_prev = grf_list_prev(item);
  GrfList* item_next = grf_list_next(item);

  grf_list_set_next(item_prev, item2);
  grf_list_set_prev(item_next, item2);
  grf_list_set_prev(item2, item_prev);
  grf_list_set_next(item2, item_next);

  free(item);

  return grf_list_begin(list);
}

GrfList*    grf_list_replace_at(GrfList* list, uint32_t index, GrfList* item2){
  GrfList* item = grf_list_at(list, index);
  return grf_list_replace(list, item, item2);
}

uint8_t  grf_list_is_different(GrfList* list, GrfList* list2){
  while(list) {
    if(grf_list_value(list) != grf_list_value(list2)) return 1;
    list = grf_list_next(list);
    list2 = grf_list_next(list2);
  }
  // They have different size
  if((list && grf_list_next(list)) || (list2 && grf_list_next(list2))) return 1;
  return 0;
}

uint8_t  grf_list_is_equal(GrfList* list, GrfList* list2){
  return !grf_list_is_different(list, list2);
}

GrfList* grf_list_find(GrfList *list, void *value){
  GrfList* current = grf_list_begin(list);
  while(grf_list_next(current) && grf_list_value(current) != value) current = grf_list_next(current);
  if(grf_list_value(current) != value) return NULL;
  else return current;
}

GrfList* grf_list_reverse(GrfList *list){
  GrfList *after, *before;
  while(list){
    before = grf_list_prev(list);
    after  = grf_list_next(list);

    grf_list_set_next(list, before);
    grf_list_set_prev(list, after);

    before = list;
    list   = after;
  }
  return before;
}

void grf_list_foreach(GrfList* list, GrfDataFunc data_function, void* value){
  GrfList* current;
  for(current = list; current; current = grf_list_next(current))
    data_function(current, value);
}

GrfList* grf_list_append_sorted(GrfList* list, GrfCompareFunc compare_function, void* value){
  GrfList* current = list, *before = NULL;
  if(!list) return grf_list_prepend(NULL, value);
  while(current && compare_function(grf_list_value(current), value) <= 0){
    before  = current;
    current = grf_list_next(current);
  }
  if(!current) return grf_list_append_at(list,before,value);
  return grf_list_prepend_at(list, current, value);
}
GrfList* grf_list_append_sorted_with_data(GrfList* list, GrfCompareDataFunc compare_function, void* value, void* user_data){
  GrfList* current = list, *before = NULL;
  if(!list) return grf_list_prepend(NULL, value);
  while(current && compare_function(grf_list_value(current), value, user_data) <= 0){
    before  = current;
    current = grf_list_next(current);
  }
  if(!current) return grf_list_append_at(list,before,value);
  return grf_list_prepend_at(list, current, value);
}
GrfList* grf_list_prepend_sorted(GrfList* list, GrfCompareFunc compare_function, void* value){
  GrfList* current = list, *before = NULL;
  if(!list) return grf_list_prepend(NULL, value);
  while(current && compare_function(grf_list_value(current), value) < 0){
    before  = current;
    current = grf_list_next(current);
  }
  if(!current) return grf_list_append_at(list,before,value);
  return grf_list_prepend_at(list, current, value);
}
GrfList* grf_list_prepend_sorted_with_data(GrfList* list, GrfCompareDataFunc compare_function, void* value, void* user_data){
  GrfList* current = list, *before = NULL;
  if(!list) return grf_list_prepend(NULL, value);
  while(current && compare_function(grf_list_value(current), value, user_data) < 0){
    before  = current;
    current = grf_list_next(current);
  }
  if(!current) return grf_list_append_at(list,before,value);
  return grf_list_prepend_at(list, current, value);
}
void* grf_list_value(GrfList* item){
  return item->base.value;
}
