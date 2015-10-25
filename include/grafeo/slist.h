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
#ifndef GRF_SLIST_H
#define GRF_SLIST_H
#include <stdint.h>
#include <stdlib.h>
#include <grafeo/type.h>

BEGIN_DECLS

/**
 * @brief Single-Linked GrfList
 */
typedef struct _GrfSList{
  void* value;        /**< Value of an item of the list */
  struct _GrfSList* next;/**< Link to the next list item */
}GrfSList;
/**
 * @brief grf_slist_new
 * @return
 */
GrfSList*    grf_slist_new();
/**
 * @brief grf_slist_new_with_value
 * @param value
 * @return
 */
GrfSList*    grf_slist_new_with_value(void* value);
/**
 * @brief grf_slist_prepend
 * @param list
 * @param value
 * @return
 */
GrfSList*    grf_slist_prepend(GrfSList* list, void* value);
/**
 * @brief grf_slist_append
 * @param list
 * @param value
 * @return
 */
GrfSList*    grf_slist_append(GrfSList* list, void* value);
/**
 * @brief grf_slist_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
GrfSList*    grf_slist_prepend_at_item(GrfSList* list, GrfSList* item, void* value);
/**
 * @brief grf_slist_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
GrfSList*    grf_slist_append_at_item(GrfSList* list, GrfSList* item, void* value);
/**
 * @brief grf_slist_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
GrfSList*    grf_slist_prepend_at(GrfSList* list, uint32_t index, void* value);
/**
 * @brief grf_slist_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
GrfSList*    grf_slist_append_at(GrfSList* list, uint32_t index, void* value);
/**
 * @brief grf_slist_append_item
 * @param list
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_append_item(GrfSList* list, GrfSList* new_item);
/**
 * @brief grf_slist_append_item_at
 * @param list
 * @param index
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_append_item_at(GrfSList* list, uint32_t index, GrfSList* new_item);
/**
 * @brief grf_slist_append_item_at_item
 * @param list
 * @param item
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_append_item_at_item(GrfSList* list, GrfSList* item, GrfSList* new_item);
/**
 * @brief grf_slist_prepend_item
 * @param list
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_prepend_item(GrfSList* list, GrfSList* new_item);
/**
 * @brief grf_slist_prepend_item_at
 * @param list
 * @param index
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_prepend_item_at(GrfSList* list, uint32_t index, GrfSList* new_item);
/**
 * @brief grf_slist_prepend_item_at_item
 * @param list
 * @param item
 * @param new_item
 * @return
 */
GrfSList*    grf_slist_prepend_item_at_item(GrfSList* list, GrfSList* item, GrfSList* new_item);
/**
 * @brief grf_slist_remove
 * @param list
 * @param item
 * @return
 */
GrfSList*    grf_slist_remove_item(GrfSList* list, GrfSList* item);
/**
 * @brief grf_slist_remove_from_value
 * @param list
 * @param value
 * @return
 */
GrfSList*    grf_slist_remove(GrfSList* list, void* value);
/**
 * @brief grf_slist_remove_after
 * @param list
 * @param before
 * @return
 */
GrfSList*    grf_slist_remove_after(GrfSList* list, GrfSList* before);
/**
 * @brief grf_slist_remove_at_index
 * @param list
 * @param index
 * @return
 */
GrfSList*    grf_slist_remove_at(GrfSList* list, uint32_t index);
/**
 * @brief grf_slist_remove_begin
 * @param list
 * @return
 */
GrfSList*    grf_slist_remove_begin(GrfSList* list);
/**
 * @brief grf_slist_remove_end
 * @param list
 * @return
 */
GrfSList*    grf_slist_remove_end(GrfSList* list);
/**
 * @brief grf_slist_free
 * @param list
 */
void      grf_slist_free(GrfSList* list);
/**
 * @brief grf_slist_index_of
 * @param list
 * @param value
 * @return
 */
int32_t   grf_slist_index_of(GrfSList* list, void* value);
/**
 * @brief grf_slist_item_of
 * @param list
 * @param value
 * @return
 */
GrfSList*    grf_slist_item_of(GrfSList* list, void* value);
/**
 * @brief grf_slist_end
 * @param list
 * @return
 */
GrfSList*    grf_slist_item_end(GrfSList* list);
/**
 * @brief grf_slist_end
 * @param list
 * @return
 */
void*     grf_slist_end(GrfSList* list);
/**
 * @brief grf_slist_length
 * @param list
 * @return
 */
uint32_t  grf_slist_length(GrfSList* list);
/**
 * @brief grf_slist_is_empty
 * @param list
 * @return
 */
uint8_t   grf_slist_is_empty(GrfSList* list);
/**
 * @brief grf_slist_item_at
 * @param list
 * @param index
 * @return
 */
GrfSList*    grf_slist_item_at(GrfSList* list, uint32_t index);
/**
 * @brief grf_slist_at
 * @return
 */
void*     grf_slist_at(GrfSList* list, uint32_t index);
/**
 * @brief grf_slist_next
 * @param list
 * @return
 */
GrfSList*    grf_slist_next(GrfSList* list);
/**
 * @brief grf_slist_prev
 * @param list
 * @param item
 * @return
 */
GrfSList*    grf_slist_prev(GrfSList* list, GrfSList* item);
/**
 * @brief grf_slist_join
 * @param list1
 * @param list2
 * @return
 */
GrfSList*    grf_slist_join(GrfSList* list1, GrfSList* list2);
/**
 * @brief grf_slist_split_at
 * @param list
 * @param index
 * @return
 */
GrfSList*    grf_slist_split_at(GrfSList* list, uint32_t index);
/**
 * @brief grf_slist_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
GrfSList*    grf_slist_swap(GrfSList* list, GrfSList* item1, GrfSList* item2);
/**
 * @brief grf_slist_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
GrfSList*    grf_slist_swap_at(GrfSList* list, uint32_t index1, uint32_t index2);
/**
 * @brief grf_slist_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
GrfSList* grf_slist_swap_items(GrfSList* list, GrfSList* item1, GrfSList* item2);
/**
 * @brief grf_slist_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
GrfSList* grf_slist_swap_items_at(GrfSList* list, uint32_t index1, uint32_t index2);
/**
 * @brief grf_slist_copy
 * @param list
 * @return
 */
GrfSList* grf_slist_copy(GrfSList* list);
/**
 * @brief grf_slist_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
GrfSList* grf_slist_replace(GrfSList* list, GrfSList* item, GrfSList* item2);
/**
 * @brief grf_slist_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
GrfSList* grf_slist_replace_at(GrfSList* list, uint32_t index, GrfSList* item2);
/**
 * @brief grf_slist_reverse
 * @param list
 * @return
 */
GrfSList* grf_slist_reverse(GrfSList* list);
/**
 * @brief grf_slist_sort
 * @param list
 * @return
 */
GrfSList* grf_slist_sort(GrfSList* list, GrfCompareFunc compare_function);
/**
 * @brief grf_slist_sort_with_data
 * @param list
 * @param compare_function
 * @param user_data
 * @return
 */
GrfSList* grf_slist_sort_with_data(GrfSList* list, GrfCompareDataFunc compare_function, void* user_data);
/**
 * @brief grf_slist_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t   grf_slist_is_different(GrfSList* list, GrfSList* list2);
/**
 * @brief grf_slist_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t   grf_slist_is_equal(GrfSList* list, GrfSList* list2);
/**
 * @brief grf_slist_value
 * @param list
 * @return
 */
GrfSList* grf_slist_value(GrfSList* list);
/**
 * @brief grf_slist_set_next
 * @param list
 * @param item
 * @return
 */
void      grf_slist_set_next(GrfSList* list, GrfSList* item);
/**
 * @brief grf_slist_set_value
 * @param list
 * @param value
 */
void      grf_slist_set_value(GrfSList* list, void* value);

END_DECLS

#endif
