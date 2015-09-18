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
#ifndef GRAFEO_SLIST_H
#define GRAFEO_SLIST_H
typedef struct _SList{
  void* value;
  struct SList* next;
}SList;
/**
 * @brief slist_new
 * @return
 */
SList*    slist_new();
/**
 * @brief slist_prepend
 * @param list
 * @param value
 * @return
 */
SList*    slist_prepend(SList* list, void* value);
/**
 * @brief slist_append
 * @param list
 * @param value
 * @return
 */
SList*    slist_append(SList* list, void* value);
/**
 * @brief slist_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
SList*    slist_prepend_at(SList* list, SList* item, void* value);
/**
 * @brief slist_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
SList*    slist_append_at(SList* list, SList* item, void* value);
/**
 * @brief slist_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
SList*    slist_prepend_at_index(SList* list, uint32_t index, void* value);
/**
 * @brief slist_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
SList*    slist_append_at_index(SList* list, uint32_t index, void* value);
/**
 * @brief slist_remove
 * @param list
 * @param item
 * @return
 */
SList*    slist_remove(SList* list, SList* item);
/**
 * @brief slist_remove_from_value
 * @param list
 * @param value
 * @return
 */
SList*    slist_remove_from_value(SList* list, void* value);
/**
 * @brief slist_remove_at_index
 * @param list
 * @param index
 * @return
 */
SList*    slist_remove_at_index(SList* list, uint32_t index);
/**
 * @brief slist_remove_begin
 * @param list
 * @return
 */
SList*    slist_remove_begin(SList* list);
/**
 * @brief slist_remove_end
 * @param list
 * @return
 */
SList*    slist_remove_end(SList* list);
/**
 * @brief slist_free
 * @param list
 */
void      slist_free(SList* list);
/**
 * @brief slist_index_of
 * @param list
 * @param value
 * @return
 */
int32_t   slist_index_of(SList* list, void* value);
/**
 * @brief slist_begin
 * @param list
 * @return
 */
SList*    slist_begin(SList* list);
/**
 * @brief slist_end
 * @param list
 * @return
 */
SList*    slist_end(SList* list);
/**
 * @brief slist_length
 * @param list
 * @return
 */
uint32_t  slist_length(SList* list);
/**
 * @brief slist_is_empty
 * @param list
 * @return
 */
uint8_t   slist_is_empty(SList* list);
/**
 * @brief slist_at
 * @param list
 * @param index
 * @return
 */
SList*    slist_at(SList* list, uint32_t index);
/**
 * @brief slist_value_at
 * @param list
 * @param index
 * @return
 */
void*     slist_value_at(SList* list, uint32_t index);
/**
 * @brief slist_next
 * @param list
 * @return
 */
SList*    slist_next(SList* list);
/**
 * @brief slist_prev
 * @param list
 * @return
 */
SList*    slist_prev(SList* list);
/**
 * @brief slist_join
 * @param list1
 * @param list2
 * @return
 */
SList*    slist_join(SList* list1, SList* list2);
/**
 * @brief slist_split_at
 * @param list
 * @param index
 * @return
 */
SList*    slist_split_at(SList* list, uint32_t index);
/**
 * @brief slist_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
SList*    slist_swap(SList* list, SList* item1, SList* item2);
/**
 * @brief slist_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
SList*    slist_swap_at(SList* list, uint32_t index1, uint32_t index2);
/**
 * @brief slist_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
SList*    slist_swap_values(SList* list, SList* item1, SList* item2);
/**
 * @brief slist_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
SList*    slist_swap_values_at(SList* list, uint32_t index1, uint32_t index2);
/**
 * @brief slist_copy
 * @param list
 * @return
 */
SList*    slist_copy(SList* list);
/**
 * @brief slist_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
SList*    slist_replace(SList* list, SList* item, SList* item2);
/**
 * @brief slist_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
SList*    slist_replace_at(SList* list, uint32_t index, SList* item2);
/**
 * @brief slist_reverse
 * @param list
 * @return
 */
SList*    slist_reverse(SList* list);
/**
 * @brief slist_sort
 * @param list
 * @return
 */
SList*    slist_sort(SList* list, CompareFunc compare_function);
/**
 * @brief slist_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t   slist_is_different(SList* list, SList* list2);
/**
 * @brief slist_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t   slist_is_equal(SList* list, SList* list2);
/**
 * @brief slist_find
 * @param list
 * @param value
 * @return
 */
SList*    slist_find(SList* list, void* value);

#endif
