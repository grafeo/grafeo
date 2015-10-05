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
#ifndef GRAFEO_QUEUE_H
#define GRAFEO_QUEUE_H
#include <grafeo/list.h>

BEGIN_DECLS

/**
  * @brief Queue structure
  */
typedef struct _Queue{
  uint32_t   length; /**< Number of elements */
  List     * begin;  /**< Initial element */
  List     * end;    /**< Final element */
}Queue;

/**
 * @brief Alocar uma fila
 * @return
 */
Queue* queue_new();
/**
 * @brief Liberar a fila
 * @return
 */
void queue_free(Queue* queue);
/**
 * @brief Adicionar no início
 * @return
 */
void queue_prepend(Queue* queue, void* value);
/**
 * @brief Adicionar no final
 * @return
 */
void queue_append(Queue* queue, void* value);
/**
 * @brief Adicionar antes de um item
 * @return
 */
void queue_prepend_at(Queue* queue, List* item, void* value);
/**
 * @brief Adicionar após um item
 * @return
 */
void queue_append_at(Queue* queue, List* item, void* value);
/**
 * @brief Adicionar antes de item especificado pela sua posição
 * @return
 */
void queue_prepend_at_index(Queue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar após um item especificado pela sua posição
 * @return
 */
void queue_append_at_index(Queue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar um item List no início da fila
 * @return
 */
void queue_prepend_item(Queue* queue, List* new_item);
/**
 * @brief Adicionar um item List no fim da fila
 * @return
 */
void queue_append_item(Queue* queue, List* new_item);
/**
 * @brief Adicionar um item List antes de um item
 * @return
 */
void queue_prepend_item_at(Queue* queue, List* item, List* new_item);
/**
 * @brief Adicionar um item List depois de um item
 * @return
 */
void queue_append_item_at(Queue* queue, List* item, List* new_item);
/**
 * @brief Adicionar um item List antes de um item especificado pela sua posição
 * @return
 */
void queue_prepend_item_at_index(Queue* queue, uint32_t index, List* new_item);
/**
 * @brief Adicionar um item List depois de um item especificado pela sua posição
 * @return
 */
void queue_append_item_at_index(Queue* queue, uint32_t index, List* new_item);
/**
 * @brief Get first item
 * @return
 */
List* queue_begin(Queue* queue);
/**
 * @brief Get last item
 * @return
 */
List* queue_end(Queue* queue);
/**
 * @brief True if length is zero
 * @return
 */
uint8_t queue_is_empty(Queue* queue);
/**
 * @brief Inverte a fila
 * @return
 */
void queue_reverse(Queue* queue);
/**
 * @brief ordena a fila
 * @return
 */
void queue_sort(Queue* queue, CompareDataFunc compare_function, void* data);
/**
 * @brief copia a fila
 * @return
 */
Queue* queue_copy(Queue* queue);
/**
 * @brief vectorize a function within a queue
 * @return
 */
void queue_foreach(Queue* queue, DataFunc func, void* data);
/**
 * @brief posição de um item especificado pelo seu valor
 * @return
 */
int32_t queue_index_of(Queue* queue, void* value);
/**
 * @brief obter um item a partir de sua posição
 * @return
 */
List* queue_at(Queue* queue, uint32_t index);
/**
 * @brief obter o valor do item a partir de sua posição
 * @return
 */
void* queue_value_at(Queue* queue, uint32_t index);
/**
 * @brief obter o valor do item inicial
 * @return
 */
void* queue_begin_value(Queue* queue);
/**
 * @brief obter o valor do item final
 * @return
 */
void* queue_end_value(Queue* queue);
/**
 * @brief Remover um item em uma fila
 * @return
 */
void queue_remove(Queue* queue, void* value);
/**
 * @brief Remover um item em uma fila baseado em sua posição
 * @return
 */
void* queue_remove_at(Queue* queue, uint32_t index);
/**
 * @brief queue_remove_begin
 * @param queue
 * @return
 */
void* queue_remove_begin(Queue* queue);
/**
 * @brief queue_remove_end
 * @param queue
 * @return
 */
void* queue_remove_end(Queue* queue);
/**
 * @brief Obter o número de elementos da fila
 * @return
 */
uint32_t queue_length(Queue* queue);
/**
 * @brief queue_append_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void queue_append_sorted(Queue* queue, CompareFunc compare_function, void* value);
/**
 * @brief queue_append_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 */
void queue_append_sorted_with_data(Queue* queue, CompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief queue_prepend_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void queue_prepend_sorted(Queue* queue, CompareFunc compare_function, void* value);
/**
 * @brief queue_prepend_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 * @param user_data
 */
void queue_prepend_sorted_with_data(Queue* queue, CompareDataFunc compare_function, void* value, void* user_data);

END_DECLS

#endif
