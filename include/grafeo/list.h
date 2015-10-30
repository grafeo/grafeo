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
#ifndef GRF_LIST_H
#define GRF_LIST_H
#include <stdint.h>
#include <stdlib.h>
#include <grafeo/type.h>
#include <grafeo/slist.h>

BEGIN_DECLS

/**
  * @brief GrfList structure
  */
typedef struct _GrfList{
  GrfSList base;/**< All resources of single linked list (link to next item and value) */
  struct _GrfList* prev;/**< Link to previous item */
}GrfList;

/**
 * @brief grf_list_new
 * @return
 */
GrfList*    grf_list_new();
/**
 * @brief grf_list_prepend
 * @param list
 * @param value
 * @return
 */
GrfList*    grf_list_prepend(GrfList* list, void* value);
/**
 * @brief grf_list_append
 * @param list
 * @param value
 * @return
 */
GrfList*    grf_list_append(GrfList* list, void* value);
/**
 * @brief grf_list_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
GrfList*    grf_list_prepend_at(GrfList* list, GrfList* item, void* value);
/**
 * @brief grf_list_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
GrfList*    grf_list_append_at(GrfList* list, GrfList* item, void* value);
/**
 * @brief grf_list_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
GrfList*    grf_list_prepend_at_index(GrfList* list, uint32_t index, void* value);
/**
 * @brief grf_list_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
GrfList*    grf_list_append_at_index(GrfList* list, uint32_t index, void* value);
/**
 * @brief grf_list_prepend_item
 * @param list
 * @param new_item
 * @return
 */
GrfList*    grf_list_prepend_item(GrfList *list, GrfList *new_item);
/**
 * @brief grf_list_prepend_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
GrfList*    grf_list_prepend_item_at(GrfList* list, GrfList* item, GrfList* item_new);
/**
 * @brief grf_list_append_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
GrfList*    grf_list_append_item_at(GrfList* list, GrfList* item, GrfList* item_new);
/**
 * @brief grf_list_prepend_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
GrfList*    grf_list_prepend_item_at_index(GrfList* list, uint32_t index, GrfList* item_new);
/**
 * @brief grf_list_append_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
GrfList*    grf_list_append_item_at_index(GrfList* list, uint32_t index, GrfList* item_new);
/**
 * @brief grf_list_remove
 * @param list
 * @param item
 * @return
 */
GrfList*    grf_list_remove(GrfList* list, GrfList* item);
/**
 * @brief grf_list_remove_from_value
 * @param list
 * @param value
 * @return
 */
GrfList*    grf_list_remove_from_value(GrfList* list, void* value);
/**
 * @brief grf_list_remove_at_index
 * @param list
 * @param index
 * @return
 */
GrfList*    grf_list_remove_at_index(GrfList* list, uint32_t index);
/**
 * @brief grf_list_remove_begin
 * @param list
 * @return
 */
GrfList*    grf_list_remove_begin(GrfList* list);
/**
 * @brief grf_list_remove_end
 * @param list
 * @return
 */
GrfList*    grf_list_remove_end(GrfList* list);
/**
 * @brief grf_list_free
 * @param list
 */
void        grf_list_free(GrfList* list);
/**
 * @brief grf_list_index_of
 * @param list
 * @param value
 * @return
 */
int32_t     grf_list_index_of(GrfList* list, void* value);
/**
 * @brief grf_list_begin
 * @param list
 * @return
 */
GrfList*    grf_list_begin(GrfList* list);
/**
 * @brief grf_list_end
 * @param list
 * @return
 */
GrfList*    grf_list_end(GrfList* list);
/**
 * @brief grf_list_length
 * @param list
 * @return
 */
uint32_t    grf_list_length(GrfList* list);
/**
 * @brief grf_list_is_empty
 * @param list
 * @return
 */
uint8_t     grf_list_is_empty(GrfList* list);
/**
 * @brief grf_list_at
 * @param list
 * @param index
 * @return
 */
GrfList*    grf_list_at(GrfList* list, uint32_t index);
/**
 * @brief grf_list_value_at
 * @param list
 * @param index
 * @return
 */
void*       grf_list_value_at(GrfList* list, uint32_t index);
/**
 * @brief grf_list_next
 * @param list
 * @return
 */
GrfList*    grf_list_next(GrfList* list);
/**
 * @brief grf_list_prev
 * @param list
 * @return
 */
GrfList*    grf_list_prev(GrfList* list);
/**
 * @brief grf_list_join
 * @param list1
 * @param list2
 * @return
 */
GrfList*    grf_list_join(GrfList* list1, GrfList* list2);
/**
 * @brief grf_list_split_at
 * @param list
 * @param index
 * @return
 */
GrfList*    grf_list_split_at(GrfList* list, uint32_t index);
/**
 * @brief grf_list_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
GrfList*    grf_list_swap_items(GrfList* list, GrfList* item1, GrfList* item2);
/**
 * @brief grf_list_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
GrfList*    grf_list_swap_items_at(GrfList* list, uint32_t index1, uint32_t index2);
/**
 * @brief grf_list_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
GrfList*    grf_list_swap(GrfList* list, GrfList* item1, GrfList* item2);
/**
 * @brief grf_list_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
GrfList*    grf_list_swap_at(GrfList* list, uint32_t index1, uint32_t index2);
/**
 * @brief grf_list_copy
 * @param list
 * @return
 */
GrfList*    grf_list_copy(GrfList* list);
/**
 * @brief grf_list_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
GrfList*    grf_list_replace(GrfList* list, GrfList* item, GrfList* item2);
/**
 * @brief grf_list_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
GrfList*    grf_list_replace_at(GrfList* list, uint32_t index, GrfList* item2);
/**
 * @brief grf_list_reverse
 * @param list
 * @return
 */
GrfList*    grf_list_reverse(GrfList* list);
/**
 * @brief grf_list_sort
 * @param list
 * @return
 */
GrfList*    grf_list_sort(GrfList* list, GrfCompareFunc compare_function);
/**
 * @brief grf_list_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t     grf_list_is_different(GrfList* list, GrfList* list2);
/**
 * @brief grf_list_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t     grf_list_is_equal(GrfList* list, GrfList* list2);
/**
 * @brief grf_list_find
 * @param list
 * @param value
 * @return
 */
GrfList*    grf_list_find(GrfList* list, void* value);
/**
 * @brief grf_list_foreach
 * @param list
 * @param data_function
 * @param value
 */
void        grf_list_foreach(GrfList* list, GrfDataFunc data_function, void* value);
/**
 * @brief grf_list_append_sorted
 * @param list
 * @param compare_function
 * @param value
 */
GrfList*    grf_list_append_sorted(GrfList* list, GrfCompareFunc compare_function, void* value);
/**
 * @brief grf_list_append_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
GrfList*    grf_list_append_sorted_with_data(GrfList* list, GrfCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief grf_list_prepend_sorted
 * @param list
 * @param compare_function
 * @param value
 */
GrfList*    grf_list_prepend_sorted(GrfList* list, GrfCompareFunc compare_function, void* value);
/**
 * @brief grf_list_prepend_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
GrfList*    grf_list_prepend_sorted_with_data(GrfList* list, GrfCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief grf_list_set_next
 * @param item
 * @param next
 */
void grf_list_set_next(GrfList* item, GrfList* next);
/**
 * @brief grf_list_set_prev
 * @param item
 * @param prev
 */
void grf_list_set_prev(GrfList* item, GrfList* prev);
/**
 * @brief grf_list_set_value
 * @param item
 * @param value
 */
void grf_list_set_value(GrfList* item, void* value);
/**
 * @brief grf_list_value
 * @param item
 * @return
 */
void* grf_list_value(GrfList* item);
/**
 * @brief grf_list_new_with_value
 * @param value
 * @return
 */
GrfList* grf_list_new_with_value(void* value);

END_DECLS

#endif
