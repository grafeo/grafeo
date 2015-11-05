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
#ifndef GRF_QUEUE_H
#define GRF_QUEUE_H
#include <grafeo/core.h>

BEGIN_DECLS

/**
  * @brief GrfQueue structure
  */
typedef struct _GrfQueue{
  uint32_t   length; /**< Number of elements */
  GrfList  * begin;  /**< Initial element */
  GrfList  * end;    /**< Final element */
}GrfQueue;

/**
 * @brief Alocar uma fila
 * @return
 */
GrfQueue* grf_queue_new();
/**
 * @brief Liberar a fila
 * @return
 */
void grf_queue_free(GrfQueue* queue);
/**
 * @brief Adicionar no início
 * @return
 */
void grf_queue_prepend(GrfQueue* queue, void* value);
/**
 * @brief Adicionar no final
 * @return
 */
void grf_queue_append(GrfQueue* queue, void* value);
/**
 * @brief Adicionar antes de um item
 * @return
 */
void grf_queue_prepend_at(GrfQueue* queue, GrfList* item, void* value);
/**
 * @brief Adicionar após um item
 * @return
 */
void grf_queue_append_at(GrfQueue* queue, GrfList* item, void* value);
/**
 * @brief Adicionar antes de item especificado pela sua posição
 * @return
 */
void grf_queue_prepend_at_index(GrfQueue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar após um item especificado pela sua posição
 * @return
 */
void grf_queue_append_at_index(GrfQueue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar um item GrfList no início da fila
 * @return
 */
void grf_queue_prepend_item(GrfQueue* queue, GrfList* new_item);
/**
 * @brief Adicionar um item GrfList no fim da fila
 * @return
 */
void grf_queue_append_item(GrfQueue* queue, GrfList* new_item);
/**
 * @brief Adicionar um item GrfList antes de um item
 * @return
 */
void grf_queue_prepend_item_at(GrfQueue* queue, GrfList* item, GrfList* new_item);
/**
 * @brief Adicionar um item GrfList depois de um item
 * @return
 */
void grf_queue_append_item_at(GrfQueue* queue, GrfList* item, GrfList* new_item);
/**
 * @brief Adicionar um item GrfList antes de um item especificado pela sua posição
 * @return
 */
void grf_queue_prepend_item_at_index(GrfQueue* queue, uint32_t index, GrfList* new_item);
/**
 * @brief Adicionar um item GrfList depois de um item especificado pela sua posição
 * @return
 */
void grf_queue_append_item_at_index(GrfQueue* queue, uint32_t index, GrfList* new_item);
/**
 * @brief Get first item
 * @return
 */
GrfList* grf_queue_begin(GrfQueue* queue);
/**
 * @brief Get last item
 * @return
 */
GrfList* grf_queue_end(GrfQueue* queue);
/**
 * @brief True if length is zero
 * @return
 */
uint8_t grf_queue_is_empty(GrfQueue* queue);
/**
 * @brief Inverte a fila
 * @return
 */
void grf_queue_reverse(GrfQueue* queue);
/**
 * @brief ordena a fila
 * @return
 */
void grf_queue_sort(GrfQueue* queue, GrfCompareDataFunc compare_function, void* data);
/**
 * @brief copia a fila
 * @return
 */
GrfQueue* grf_queue_copy(GrfQueue* queue);
/**
 * @brief vectorize a function within a queue
 * @return
 */
void grf_queue_foreach(GrfQueue* queue, GrfDataFunc func, void* data);
/**
 * @brief posição de um item especificado pelo seu valor
 * @return
 */
int32_t grf_queue_index_of(GrfQueue* queue, void* value);
/**
 * @brief obter um item a partir de sua posição
 * @return
 */
GrfList* grf_queue_at(GrfQueue* queue, uint32_t index);
/**
 * @brief obter o valor do item a partir de sua posição
 * @return
 */
void* grf_queue_value_at(GrfQueue* queue, uint32_t index);
/**
 * @brief obter o valor do item inicial
 * @return
 */
void* grf_queue_begin_value(GrfQueue* queue);
/**
 * @brief obter o valor do item final
 * @return
 */
void* grf_queue_end_value(GrfQueue* queue);
/**
 * @brief Remover um item em uma fila
 * @return
 */
void grf_queue_remove(GrfQueue* queue, void* value);
/**
 * @brief Remover um item em uma fila baseado em sua posição
 * @return
 */
void* grf_queue_remove_at(GrfQueue* queue, uint32_t index);
/**
 * @brief grf_queue_remove_begin
 * @param queue
 * @return
 */
void* grf_queue_remove_begin(GrfQueue* queue);
/**
 * @brief grf_queue_remove_end
 * @param queue
 * @return
 */
void* grf_queue_remove_end(GrfQueue* queue);
/**
 * @brief Obter o número de elementos da fila
 * @return
 */
uint32_t grf_queue_length(GrfQueue* queue);
/**
 * @brief grf_queue_append_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void grf_queue_append_sorted(GrfQueue* queue, GrfCompareFunc compare_function, void* value);
/**
 * @brief grf_queue_append_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 */
void grf_queue_append_sorted_with_data(GrfQueue* queue, GrfCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief grf_queue_prepend_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void grf_queue_prepend_sorted(GrfQueue* queue, GrfCompareFunc compare_function, void* value);
/**
 * @brief grf_queue_prepend_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 * @param user_data
 */
void grf_queue_prepend_sorted_with_data(GrfQueue* queue, GrfCompareDataFunc compare_function, void* value, void* user_data);

END_DECLS

#endif
