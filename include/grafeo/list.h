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
#ifndef GRAFEO_LIST_H
#define GRAFEO_LIST_H
#include <stdint.h>
#include <stdlib.h>
#include <grafeo/type.h>
#include <grafeo/slist.h>
/**
  * @brief List structure
  */
typedef struct _List{
  SList base;/**< All resources of single linked list (link to next item and value) */
  struct _List* prev;/**< Link to previous item */
}List;

/**
 * @brief list_new
 * @return
 */
List*    list_new();
/**
 * @brief list_prepend
 * @param list
 * @param value
 * @return
 */
List*    list_prepend(List* list, void* value);
/**
 * @brief list_append
 * @param list
 * @param value
 * @return
 */
List*    list_append(List* list, void* value);
/**
 * @brief list_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
List*    list_prepend_at(List* list, List* item, void* value);
/**
 * @brief list_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
List*    list_append_at(List* list, List* item, void* value);
/**
 * @brief list_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
List*    list_prepend_at_index(List* list, uint32_t index, void* value);
/**
 * @brief list_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
List*    list_append_at_index(List* list, uint32_t index, void* value);
/**
 * @brief list_prepend_item
 * @param list
 * @param new_item
 * @return
 */
List* list_prepend_item(List *list, List *new_item);
/**
 * @brief list_prepend_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
List* list_prepend_item_at(List* list, List* item, List* item_new);
/**
 * @brief list_append_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
List* list_append_item_at(List* list, List* item, List* item_new);
/**
 * @brief list_prepend_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
List* list_prepend_item_at_index(List* list, uint32_t index, List* item_new);
/**
 * @brief list_append_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
List* list_append_item_at_index(List* list, uint32_t index, List* item_new);
/**
 * @brief list_remove
 * @param list
 * @param item
 * @return
 */
List*    list_remove(List* list, List* item);
/**
 * @brief list_remove_from_value
 * @param list
 * @param value
 * @return
 */
List*    list_remove_from_value(List* list, void* value);
/**
 * @brief list_remove_at_index
 * @param list
 * @param index
 * @return
 */
List*    list_remove_at_index(List* list, uint32_t index);
/**
 * @brief list_remove_begin
 * @param list
 * @return
 */
List*    list_remove_begin(List* list);
/**
 * @brief list_remove_end
 * @param list
 * @return
 */
List*    list_remove_end(List* list);
/**
 * @brief list_free
 * @param list
 */
void     list_free(List* list);
/**
 * @brief list_index_of
 * @param list
 * @param value
 * @return
 */
int32_t list_index_of(List* list, void* value);
/**
 * @brief list_begin
 * @param list
 * @return
 */
List*    list_begin(List* list);
/**
 * @brief list_end
 * @param list
 * @return
 */
List*    list_end(List* list);
/**
 * @brief list_length
 * @param list
 * @return
 */
uint32_t list_length(List* list);
/**
 * @brief list_is_empty
 * @param list
 * @return
 */
uint8_t  list_is_empty(List* list);
/**
 * @brief list_at
 * @param list
 * @param index
 * @return
 */
List*    list_at(List* list, uint32_t index);
/**
 * @brief list_value_at
 * @param list
 * @param index
 * @return
 */
void*    list_value_at(List* list, uint32_t index);
/**
 * @brief list_next
 * @param list
 * @return
 */
List*    list_next(List* list);
/**
 * @brief list_prev
 * @param list
 * @return
 */
List*    list_prev(List* list);
/**
 * @brief list_join
 * @param list1
 * @param list2
 * @return
 */
List*    list_join(List* list1, List* list2);
/**
 * @brief list_split_at
 * @param list
 * @param index
 * @return
 */
List*    list_split_at(List* list, uint32_t index);
/**
 * @brief list_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
List*    list_swap_items(List* list, List* item1, List* item2);
/**
 * @brief list_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
List*    list_swap_items_at(List* list, uint32_t index1, uint32_t index2);
/**
 * @brief list_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
List*    list_swap(List* list, List* item1, List* item2);
/**
 * @brief list_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
List*    list_swap_at(List* list, uint32_t index1, uint32_t index2);
/**
 * @brief list_copy
 * @param list
 * @return
 */
List*    list_copy(List* list);
/**
 * @brief list_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
List*    list_replace(List* list, List* item, List* item2);
/**
 * @brief list_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
List*    list_replace_at(List* list, uint32_t index, List* item2);
/**
 * @brief list_reverse
 * @param list
 * @return
 */
List*    list_reverse(List* list);
/**
 * @brief list_sort
 * @param list
 * @return
 */
List*    list_sort(List* list, CompareFunc compare_function);
/**
 * @brief list_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t  list_is_different(List* list, List* list2);
/**
 * @brief list_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t  list_is_equal(List* list, List* list2);
/**
 * @brief list_find
 * @param list
 * @param value
 * @return
 */
List*    list_find(List* list, void* value);
/**
 * @brief list_foreach
 * @param list
 * @param data_function
 * @param value
 */
void     list_foreach(List* list, DataFunc data_function, void* value);
/**
 * @brief list_append_sorted
 * @param list
 * @param compare_function
 * @param value
 */
List*    list_append_sorted(List* list, CompareFunc compare_function, void* value);
/**
 * @brief list_append_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
List*    list_append_sorted_with_data(List* list, CompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief list_prepend_sorted
 * @param list
 * @param compare_function
 * @param value
 */
List*    list_prepend_sorted(List* list, CompareFunc compare_function, void* value);
/**
 * @brief list_prepend_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
List*    list_prepend_sorted_with_data(List* list, CompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief list_set_next
 * @param item
 * @param next
 */
void list_set_next(List* item, List* next);
/**
 * @brief list_set_prev
 * @param item
 * @param prev
 */
void list_set_prev(List* item, List* prev);
/**
 * @brief list_set_value
 * @param item
 * @param value
 */
void list_set_value(List* item, void* value);
/**
 * @brief list_value
 * @param item
 * @return
 */
void* list_value(List* item);
/**
 * @brief list_new_with_value
 * @param value
 * @return
 */
List* list_new_with_value(void* value);
#endif
