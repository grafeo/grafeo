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
  return malloc(sizeof(List));
}

List* list_append(List* list, void* value){
  return NULL;
}

List* list_append_at(List *list, List *item, void *value){
  return NULL;
}

List* list_append_at_index(List *list, uint32_t index, void *value){
  return NULL;
}

void list_free(List *list){

}

List* list_prepend(List *list, void *value){
  return NULL;
}

List* list_prepend_at(List *list, List *item, void *value){
  return NULL;
}

List* list_prepend_at_index(List *list, uint32_t index, void *value){
  return NULL;
}

List* list_remove(List *list, List *item){
  return NULL;
}

List* list_remove_at_index(List *list, uint32_t index){
  return NULL;
}

List* list_remove_begin(List *list){
  return NULL;
}

List* list_remove_end(List *list){
  return NULL;
}

List* list_remove_from_value(List *list, void *value){
  return NULL;
}
uint32_t list_index_of(List* list, void* value){
  return 0;
}

List*    list_begin(List* list){
  return NULL;
}

List*    list_end(List* list){
  return NULL;
}

uint32_t list_length(List* list){
  return 0;
}

uint8_t  list_is_empty(List* list){
  return 0;
}

List*    list_at(List* list, uint32_t index){
  return NULL;
}

void*    list_value_at(List* list, uint32_t index){
  return NULL;
}

List*    list_next(List* list){
  return NULL;
}

List*    list_prev(List* list){
  return NULL;
}

List*    list_swap(List* list, List* item1, List* item2){
  return NULL;
}

List*    list_swap_at(List* list, uint32_t index1, uint32_t index2){
  return NULL;
}

List*    list_swap_values(List* list, List* item1, List* item2){
  return NULL;
}

List*    list_swap_values_at(List* list, uint32_t index1, uint32_t index2){
  return NULL;
}

List*    list_copy(List* list){
  return NULL;
}

List*    list_replace(List* list, List* item, void* value){
  return NULL;
}

List*    list_replace_at(List* list, uint32_t index, void* value){
  return NULL;
}

uint8_t  list_is_different(List* list, List* list2){
  return 0;
}

uint8_t  list_is_equal(List* list, List* list2){
  return 0;
}
