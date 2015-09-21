#include <grafeo/slist.h>

SList*    slist_new(){
  SList* item = malloc(sizeof(SList));
  item->next = NULL;
  item->value = NULL;
  return item;
}
SList*    slist_new_with_value(void* value){
  SList* item = slist_new();
  item->value = value;
  return item;
}

SList*    slist_prepend(SList* list, void* value){
  return slist_prepend_item(list, slist_new_with_value(value));
}

SList*    slist_append(SList* list, void* value){
  return slist_append_item(list, slist_new_with_value(value));
}

SList*    slist_prepend_at_item(SList* list, SList* item, void* value){
  if(list == item) return slist_prepend(list, value);
  SList* current = list;
  while(current && current->next != item) current=current->next;
  SList* new_item = slist_new_with_value(value);
  new_item->next  = item;
  current->next   = new_item;
  return list;
}

SList*    slist_append_at_item(SList* list, SList* item, void* value){
  return slist_append_item_at_item(list, item, slist_new_with_value(value));
}

SList*    slist_prepend_at(SList* list, uint32_t index, void* value){
  if(index) return slist_append_at_item(list, slist_item_at(list, index-1), value);
  return slist_append(NULL, value);
}

SList*    slist_append_at(SList* list, uint32_t index, void* value){
  return slist_append_at_item(list, slist_item_at(list, index), value);
}
SList*    slist_append_item(SList *list, SList *new_item){
  if(list == NULL) return new_item;
  else{
    SList* last = slist_item_end(list);
    last->next = new_item;
    new_item->next = NULL;
  }
  return list;
}
SList*    slist_append_item_at_item(SList *list, SList *item, SList *new_item){
  new_item->next  = item->next;
  item->next      = new_item;
  return list;
}
SList*    slist_append_item_at(SList *list, uint32_t index, SList *new_item){
  return slist_append_item_at_item(list, slist_item_at(list,index), new_item);
}
SList*    slist_prepend_item(SList *list, SList *new_item){
  if(list == NULL) return new_item;
  new_item->next = list;
  return new_item;
}
SList*    slist_prepend_item_at(SList *list, uint32_t index, SList *new_item){
  if(!index || !list) return slist_prepend_item(list, new_item);
  return slist_append_item_at(list, index-1, new_item);
}
SList*    slist_prepend_item_at_item(SList *list, SList *item, SList *new_item){
  if(!item || !list) return slist_prepend_item(list, new_item);
  SList* before;
  for(before = list; before->next && before->next != item; before=before->next);
  return slist_append_item_at_item(list, before, new_item);
}


SList*    slist_remove_item(SList* list, SList* item){
  if(!list || !item) return list;
  if(item  ==  list) return slist_remove_begin(list);
  SList* prev = slist_prev(list, item);
  prev->next  = item->next;
  free(item);
  return list;
}

SList*    slist_remove(SList* list, void* value){
  SList* current = list;
  while(current->next && current->next->value != value) current = current->next;
  if(current->next) return slist_remove_after(list, current);
  return list;
}

SList*    slist_remove_at(SList* list, uint32_t index){
  if(!index) return slist_remove_begin(list);
  return slist_remove_after(list, slist_item_at(list, index-1));
}

SList*    slist_remove_after(SList* list, SList* before){
  if(!list || !before) return list;
  if(before == list) return slist_remove_begin(list);
  SList* item  = before->next;
  before->next = item->next;
  free(item);
  return list;
}

SList*    slist_remove_begin(SList* list){
  if(list == NULL) return NULL;
  SList* item       = list;
  SList* new_begin  = item->next;
  free(item);
  return new_begin;
}

SList*    slist_remove_end(SList* list){
  if(!list) return NULL;
  if(!list->next) {free(list); return NULL;}
  SList* before = list;
  while(before->next->next) before=before->next;
  free(before->next);
  before->next = NULL;
  return list;
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
  int32_t i;
  for(i=0;list;list=list->next,i++) if(list->value == value) return i;
  return -1;
}

SList*    slist_item_end(SList* list){
  if(!list) return NULL;
  while(list->next) list = list->next;
  return list;
}
void*     slist_end(SList* list){
  return slist_item_end(list)->value;
}

uint32_t  slist_length(SList* list){
  uint32_t i;
  for(i=0;list;list=list->next, i++);
  return i;
}

uint8_t   slist_is_empty(SList* list){
  return list == NULL;
}

void*    slist_at(SList* list, uint32_t index){
  return slist_item_at(list, index)->value;
}

SList*    slist_item_at(SList* list, uint32_t index){
  uint32_t i;
  for(i = 0; i < index; i++) list = list->next;
  return list;
}

SList*    slist_next(SList* list){
  return list->next;
}

SList*    slist_prev(SList* list, SList* item){
  while(list->next != item) list = list->next;
  return list;
}

SList*    slist_join(SList* list1, SList* list2){
  SList* end1 = slist_item_end(list1);
  end1->next = list2;
  return list1;
}

SList*    slist_split_at(SList* list, uint32_t index){
  SList* end1 = slist_item_at(list,index-1);
  SList* list2 = end1->next;
  end1->next = NULL;
  return list2;
}

SList*    slist_swap(SList* list, SList* item1, SList* item2){
  void* tmp    = item1->value;
  item1->value = item2->value;
  item2->value = tmp;
  return list;
}

SList*    slist_swap_at(SList* list, uint32_t index1, uint32_t index2){
  return slist_swap(list, slist_item_at(list, index1),slist_item_at(list, index2));
}

SList*   slist_swap_items_after(SList* list, SList* before1, SList* before2, SList* item1, SList* item2){
  SList* tmp;

  // Sort items
  if(before1 == item2 || before1->next == item2){
    tmp = item1;item1=item2;item2=tmp;
  }

  // Exchange items
  tmp              = item1->next;
  item1->next      = item2->next;
  if(tmp != item2) item2->next = tmp;
  else item2->next = item1;

  // Fix previous items links to items
  if(before1 != item1) before1->next = item2;
  if(before2 != item2) before2->next = item1;

  if(list == item2) return item1;
  return list;
}


SList*    slist_swap_items(SList* list, SList* item1, SList* item2){
  SList* before1=NULL, *before2=NULL, *current, *tmp, *before;

  before = current = list;
  while(current){
    // Get previous items
    if(current == item1) before1 = before;
    if(current == item2) before2 = before;

    // If before2 was defined before before1,
    // then item2 is placed before item1
    // Let's sort it
    if(current == item1 && before2) {
      tmp = before1;before1 = before2;before2 = tmp;
      tmp = item1  ;item1   = item2  ;item2   = tmp;
    }

    // Fix before2 if item1 and item2 are neighbors
    if(item1->next == item2) before2 = item2;

    // Continue or break if items are defined
    if(before1 && before2) break;
    before  = current;
    current = current->next;
  }

  return slist_swap_items_after(list, before1, before2, item1, item2);
}

SList*    slist_swap_items_at(SList* list, uint32_t index1, uint32_t index2){
  SList* item1, *item2, *before1, *before2, *before, *current;
  uint32_t i = 0;
  // Same indices? Do nothing
  if(index1 == index2) return list;
  // Sort indices
  else if(index1 > index2) {
    uint32_t tmpi = index1;
    index1        = index2;
    index2        = tmpi;
  }
  // Iterate inside list
  before = current = list;
  while(current && i <= index2){

    // Get items and items before
    if(i == index1){ before1 = before; item1 = current;}
    if(i == index2){ before2 = before; item2 = current;}

    // Next iteration
    before  = current;
    current = current->next;
    i++;
  }
  if(index2 == index1+1) before2 = item2;

  // Swap items and return beginning
  return slist_swap_items_after(list, before1, before2, item1, item2);
}


SList*    slist_copy(SList* list){
  SList* list2 = NULL, *item2, *begin;
  while(list){
    item2 = slist_new_with_value(list->value);
    if(list2) list2->next = item2;
    else      begin = item2;
    list2 = item2;
    list  = list->next;
  }
  return begin;
}

SList*    slist_replace(SList* list, SList* item, SList* item2){
  SList* before;
  for(before = list; before->next != item; before = before->next);
  list          = slist_remove_after(list, before);
  return          slist_append_item_at_item(list,before,item2);
}

SList*    slist_replace_at(SList* list, uint32_t index, SList* item2){
  SList* before = slist_item_at(list, index-1);
  list          = slist_remove_after(list, before);
  return          slist_append_item_at_item(list,before,item2);
}

SList*    slist_reverse(SList* list){
  if(list && list->next){
    SList* current = list, *after = current->next, *old_after;
    current->next  = NULL;
    while(after){
      old_after   = after->next;
      after->next = current;
      current     = after;
      after       = old_after;
    }
    return current;
  }
  return list;
}

SList*    slist_sort(SList* list, CompareFunc compare_function){
  return NULL;
}
SList*    slist_sort_with_data(SList* list, CompareDataFunc compare_function, void* user_data){
  return NULL;
}

uint8_t   slist_is_different(SList* list, SList* list2){
  while(list){
    // If different values           or       lengths
    if((list->value != list2->value) || ((list->next==NULL)^(list->next==NULL)))
      return 1;
    list  = list->next;
    list2 = list2->next;
  }
  return 0;
}

uint8_t   slist_is_equal(SList* list, SList* list2){
  return !slist_is_different(list, list2);
}

SList*    slist_item_of(SList* list, void* value){
  while(list && list->value != value) list=list->next;
  return list;
}
