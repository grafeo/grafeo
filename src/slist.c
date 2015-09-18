#include <grafeo/slist.h>

SList*    slist_new(){

}

SList*    slist_prepend(SList* list, void* value){

}

SList*    slist_append(SList* list, void* value){

}

SList*    slist_prepend_at(SList* list, SList* item, void* value){

}

SList*    slist_append_at(SList* list, SList* item, void* value){

}

SList*    slist_prepend_at_index(SList* list, uint32_t index, void* value){

}

SList*    slist_append_at_index(SList* list, uint32_t index, void* value){

}

SList*    slist_remove(SList* list, SList* item){

}

SList*    slist_remove_from_value(SList* list, void* value){

}

SList*    slist_remove_at_index(SList* list, uint32_t index){

}

SList*    slist_remove_begin(SList* list){

}

SList*    slist_remove_end(SList* list){

}

void      slist_free(SList* list){

}

int32_t   slist_index_of(SList* list, void* value){

}

SList*    slist_begin(SList* list){

}

SList*    slist_end(SList* list){

}

uint32_t  slist_length(SList* list){

}

uint8_t   slist_is_empty(SList* list){

}

SList*    slist_at(SList* list, uint32_t index){

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
