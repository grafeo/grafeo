#include <grafeo/slist.h>

GrfSList*    grf_slist_new(){
  GrfSList* item = malloc(sizeof(GrfSList));
  item->next = NULL;
  grf_slist_set_value(item,NULL);
  return item;
}
GrfSList*    grf_slist_new_with_value(void* value){
  GrfSList* item = grf_slist_new();
  grf_slist_set_value(item,value);
  return item;
}

GrfSList*    grf_slist_prepend(GrfSList* list, void* value){
  return grf_slist_prepend_item(list, grf_slist_new_with_value(value));
}

GrfSList*    grf_slist_append(GrfSList* list, void* value){
  return grf_slist_append_item(list, grf_slist_new_with_value(value));
}

GrfSList*    grf_slist_prepend_at_item(GrfSList* list, GrfSList* item, void* value){
  if(list == item) return grf_slist_prepend(list, value);
  GrfSList* current = list;
  while(current && current->next != item) current=current->next;
  GrfSList* new_item = grf_slist_new_with_value(value);
  new_item->next  = item;
  current->next   = new_item;
  return list;
}

GrfSList*    grf_slist_append_at_item(GrfSList* list, GrfSList* item, void* value){
  return grf_slist_append_item_at_item(list, item, grf_slist_new_with_value(value));
}

GrfSList*    grf_slist_prepend_at(GrfSList* list, uint32_t index, void* value){
  if(index) return grf_slist_append_at_item(list, grf_slist_item_at(list, index-1), value);
  return grf_slist_append(NULL, value);
}

GrfSList*    grf_slist_append_at(GrfSList* list, uint32_t index, void* value){
  return grf_slist_append_at_item(list, grf_slist_item_at(list, index), value);
}
GrfSList*    grf_slist_append_item(GrfSList *list, GrfSList *new_item){
  if(list == NULL) return new_item;
  else{
    GrfSList* last = grf_slist_item_end(list);
    last->next = new_item;
    new_item->next = NULL;
  }
  return list;
}
GrfSList*    grf_slist_append_item_at_item(GrfSList *list, GrfSList *item, GrfSList *new_item){
  new_item->next  = item->next;
  item->next      = new_item;
  return list;
}
GrfSList*    grf_slist_append_item_at(GrfSList *list, uint32_t index, GrfSList *new_item){
  return grf_slist_append_item_at_item(list, grf_slist_item_at(list,index), new_item);
}
GrfSList*    grf_slist_prepend_item(GrfSList *list, GrfSList *new_item){
  if(list == NULL) return new_item;
  new_item->next = list;
  return new_item;
}
GrfSList*    grf_slist_prepend_item_at(GrfSList *list, uint32_t index, GrfSList *new_item){
  if(!index || !list) return grf_slist_prepend_item(list, new_item);
  return grf_slist_append_item_at(list, index-1, new_item);
}
GrfSList*    grf_slist_prepend_item_at_item(GrfSList *list, GrfSList *item, GrfSList *new_item){
  if(!item || !list) return grf_slist_prepend_item(list, new_item);
  GrfSList* before;
  for(before = list; before->next && before->next != item; before=before->next);
  return grf_slist_append_item_at_item(list, before, new_item);
}


GrfSList*    grf_slist_remove_item(GrfSList* list, GrfSList* item){
  if(!list || !item) return list;
  if(item  ==  list) return grf_slist_remove_begin(list);
  GrfSList* prev = grf_slist_prev(list, item);
  prev->next  = item->next;
  free(item);
  return list;
}

GrfSList*    grf_slist_remove(GrfSList* list, void* value){
  GrfSList* current = list;
  while(current->next && current->next->value != value) current = current->next;
  if(current->next) return grf_slist_remove_after(list, current);
  return list;
}

GrfSList*    grf_slist_remove_at(GrfSList* list, uint32_t index){
  if(!index) return grf_slist_remove_begin(list);
  return grf_slist_remove_after(list, grf_slist_item_at(list, index-1));
}

GrfSList*    grf_slist_remove_after(GrfSList* list, GrfSList* before){
  if(!list || !before) return list;
  if(before == list) return grf_slist_remove_begin(list);
  GrfSList* item  = before->next;
  before->next = item->next;
  free(item);
  return list;
}

GrfSList*    grf_slist_remove_begin(GrfSList* list){
  if(list == NULL) return NULL;
  GrfSList* item       = list;
  GrfSList* new_begin  = item->next;
  free(item);
  return new_begin;
}

GrfSList*    grf_slist_remove_end(GrfSList* list){
  if(!list) return NULL;
  if(!list->next) {free(list); return NULL;}
  GrfSList* before = list;
  while(before->next->next) before=before->next;
  free(before->next);
  before->next = NULL;
  return list;
}

void      grf_slist_free(GrfSList* list){
  if(list){
    GrfSList* item = list;
    GrfSList* next = item->next;
    while(item->next){free(item); item = next; next = item->next;}
    free(item);
  }
}

int32_t   grf_slist_index_of(GrfSList* list, void* value){
  int32_t i;
  for(i=0;list;list=list->next,i++) if(list->value == value) return i;
  return -1;
}

GrfSList*    grf_slist_item_end(GrfSList* list){
  if(!list) return NULL;
  while(list->next) list = list->next;
  return list;
}
void*     grf_slist_end(GrfSList* list){
  return grf_slist_item_end(list)->value;
}

uint32_t  grf_slist_length(GrfSList* list){
  uint32_t i;
  for(i=0;list;list=list->next, i++);
  return i;
}

uint8_t   grf_slist_is_empty(GrfSList* list){
  return list == NULL;
}

void*    grf_slist_at(GrfSList* list, uint32_t index){
  return grf_slist_item_at(list, index)->value;
}

GrfSList*    grf_slist_item_at(GrfSList* list, uint32_t index){
  uint32_t i;
  for(i = 0; i < index; i++) list = list->next;
  return list;
}

GrfSList*    grf_slist_next(GrfSList* list){
  return list->next;
}

GrfSList*    grf_slist_prev(GrfSList* list, GrfSList* item){
  while(list->next != item) list = list->next;
  return list;
}

GrfSList*    grf_slist_join(GrfSList* list1, GrfSList* list2){
  GrfSList* end1 = grf_slist_item_end(list1);
  end1->next = list2;
  return list1;
}

GrfSList*    grf_slist_split_at(GrfSList* list, uint32_t index){
  GrfSList* end1 = grf_slist_item_at(list,index-1);
  GrfSList* list2 = end1->next;
  end1->next = NULL;
  return list2;
}

GrfSList*    grf_slist_swap(GrfSList* list, GrfSList* item1, GrfSList* item2){
  void* tmp    = item1->value;
  grf_slist_set_value(item1,item2->value);
  grf_slist_set_value(item2,tmp);
  return list;
}

GrfSList*    grf_slist_swap_at(GrfSList* list, uint32_t index1, uint32_t index2){
  return grf_slist_swap(list, grf_slist_item_at(list, index1),grf_slist_item_at(list, index2));
}

GrfSList*   grf_slist_swap_items_after(GrfSList* list, GrfSList* before1, GrfSList* before2, GrfSList* item1, GrfSList* item2){
  GrfSList* tmp;

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


GrfSList*    grf_slist_swap_items(GrfSList* list, GrfSList* item1, GrfSList* item2){
  GrfSList* before1=NULL, *before2=NULL, *current, *tmp, *before;

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

  return grf_slist_swap_items_after(list, before1, before2, item1, item2);
}

GrfSList*    grf_slist_swap_items_at(GrfSList* list, uint32_t index1, uint32_t index2){
  GrfSList* item1, *item2, *before1, *before2, *before, *current;
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
  return grf_slist_swap_items_after(list, before1, before2, item1, item2);
}


GrfSList*    grf_slist_copy(GrfSList* list){
  GrfSList* list2 = NULL, *item2, *begin;
  while(list){
    item2 = grf_slist_new_with_value(list->value);
    if(list2) list2->next = item2;
    else      begin = item2;
    list2 = item2;
    list  = list->next;
  }
  return begin;
}

GrfSList*    grf_slist_replace(GrfSList* list, GrfSList* item, GrfSList* item2){
  GrfSList* before;
  for(before = list; before->next != item; before = before->next);
  list          = grf_slist_remove_after(list, before);
  return          grf_slist_append_item_at_item(list,before,item2);
}

GrfSList*    grf_slist_replace_at(GrfSList* list, uint32_t index, GrfSList* item2){
  GrfSList* before = grf_slist_item_at(list, index-1);
  list          = grf_slist_remove_after(list, before);
  return          grf_slist_append_item_at_item(list,before,item2);
}

GrfSList*    grf_slist_reverse(GrfSList* list){
  if(list && list->next){
    GrfSList* current = list, *after = current->next, *old_after;
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

GrfSList*    grf_slist_sort(GrfSList* list, GrfCompareFunc compare_function){
  return NULL;
}
GrfSList*    grf_slist_sort_with_data(GrfSList* list, GrfCompareDataFunc compare_function, void* user_data){
  return NULL;
}

uint8_t   grf_slist_is_different(GrfSList* list, GrfSList* list2){
  while(list){
    // If different values           or       lengths
    if((list->value != list2->value) || ((list->next==NULL)^(list->next==NULL)))
      return 1;
    list  = list->next;
    list2 = list2->next;
  }
  return 0;
}

uint8_t   grf_slist_is_equal(GrfSList* list, GrfSList* list2){
  return !grf_slist_is_different(list, list2);
}

GrfSList*    grf_slist_item_of(GrfSList* list, void* value){
  while(list && list->value != value) list=list->next;
  return list;
}

void grf_slist_set_value(GrfSList* list, void* value){
  list->value = value;
}
GrfSList* grf_slist_value(GrfSList* list){
  return list->value;
}
void grf_slist_set_next(GrfSList* list, GrfSList* item){
  list->next = item;
}
