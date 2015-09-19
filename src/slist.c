#include <grafeo/slist.h>

SList*    slist_new(){
  SList* item = malloc(sizeof(SList));
  item->next = NULL;
  item->value = NULL;
  return item;
}

SList*    slist_prepend(SList* list, void* value){
  SList* item = slist_new();
  SList* begin = list;
  item->value = value;
  if(begin == NULL) return item;
  item->next = begin;
  return item;
}

SList*    slist_append(SList* list, void* value){
  SList* item = slist_new();
  item->value = value;

  if(list == NULL) return item;
  else{
    SList* last = slist_end(list);
    last->next = item;
    item->next = NULL;
  }
  return list;
}

SList*    slist_prepend_at_item(SList* list, SList* item, void* value){
  if(list == item) return slist_prepend(list, value);
  SList* current = list;
  while(current && current->next != item) current=current->next;
  SList* new_item = slist_new();
  new_item->value = value;
  new_item->next  = item;
  current->next   = new_item;
  return list;
}

SList*    slist_append_at_item(SList* list, SList* item, void* value){
  SList* new_item = slist_new();
  new_item->value = value;
  new_item->next  = item->next;
  item->next      = new_item;
  return list;
}

SList*    slist_prepend_at(SList* list, uint32_t index, void* value){
  if(index) return slist_append_at_item(list, slist_at(list, index-1), value);
  return slist_append(NULL, value);
}

SList*    slist_append_at(SList* list, uint32_t index, void* value){
  return slist_append_at_item(list, slist_at(list, index), value);
}
SList*    slist_append_item(SList *list, SList *new_item){
  return NULL;
}
SList*    slist_append_item_at_item(SList *list, SList *item, SList *new_item){
  return NULL;
}
SList*    slist_append_item_at(SList *list, uint32_t index, SList *new_item){
  return NULL;
}
SList*    slist_prepend_item(SList *list, SList *new_item){
  return NULL;
}
SList*    slist_prepend_item_at(SList *list, uint32_t index, SList *new_item){
  return NULL;
}
SList*    slist_prepend_item_at_item(SList *list, SList *item, SList *new_item){
  return NULL;
}


SList*    slist_remove_item(SList* list, SList* item){
  return list;
  SList* current = list;
  while(current && current->next != item) current=current->next;

//  if(item == NULL) return list;
//  SList* begin  = list;
//  if(item->next) item->next->prev = item->prev;
//  if(item->prev) item->prev->next = item->next;
//  else begin   = item->next;
//  free(item);
//  return begin;
}

SList*    slist_remove(SList* list, void* value){
  return slist_remove_item(list, slist_find(list, value));
}

SList*    slist_remove_at(SList* list, uint32_t index){
  return slist_remove_item(list,slist_at(list, index));
}

SList*    slist_remove_begin(SList* list){
  if(list == NULL) return NULL;
  SList* item       = list;
  SList* new_begin  = item->next;
  free(item);
  return new_begin;
}

SList*    slist_remove_end(SList* list){
//  SList* current = list;
  return list;
//  while(current->next->next) current=current->next;


//  if(list == NULL) return NULL;
//  SList* begin = list;

//  SList* end   = slist_end(list);
//  if(begin == end) begin = NULL;
//  else end->prev->next = NULL;
//  free(end);
//  return begin;
}

void      slist_free(SList* list){
  if(list){
    SList* item = list;
    SList* next = item->next;
    while(item->next){free(item); item = next; next = item->next;}
    free(item);
  }
}

int32_t   slist_index_of(SList* list, void* value){

}

SList*    slist_begin(SList* list){

}

SList*    slist_end(SList* list){
  if(!list) return NULL;
  SList* current = list;
  while(current->next) current = current->next;
  return current;
}

uint32_t  slist_length(SList* list){

}

uint8_t   slist_is_empty(SList* list){

}

SList*    slist_at(SList* list, uint32_t index){
  uint32_t i;
  for(i = 0; i < index; i++) list = list->next;
  return list;
}

void*     slist_value_at(SList* list, uint32_t index){

}

SList*    slist_next(SList* list){

}

SList*    slist_prev(SList* list){

}

SList*    slist_join(SList* list1, SList* list2){

}

SList*    slist_split_at(SList* list, uint32_t index){

}

SList*    slist_swap(SList* list, SList* item1, SList* item2){

}

SList*    slist_swap_at(SList* list, uint32_t index1, uint32_t index2){

}

SList*    slist_swap_values(SList* list, SList* item1, SList* item2){

}

SList*    slist_swap_values_at(SList* list, uint32_t index1, uint32_t index2){

}

SList*    slist_copy(SList* list){

}

SList*    slist_replace(SList* list, SList* item, SList* item2){

}

SList*    slist_replace_at(SList* list, uint32_t index, SList* item2){

}

SList*    slist_reverse(SList* list){

}

SList*    slist_sort(SList* list, CompareFunc compare_function){

}

uint8_t   slist_is_different(SList* list, SList* list2){

}

uint8_t   slist_is_equal(SList* list, SList* list2){

}

SList*    slist_find(SList* list, void* value){

}
