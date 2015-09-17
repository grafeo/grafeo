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
/**
  * Queue structure
  */
typedef struct _Queue{
  uint32_t   length;
  List     * begin;
  List     * end;
}Queue;

/**
 Alocar uma fila
*/
Queue* queue_new();
/**
 Liberar a fila
*/
void queue_free(Queue* queue);
/**
 Adicionar no início
*/
Queue* queue_prepend(Queue* queue, void* value);
/**
 Adicionar no final
*/
Queue* queue_append(Queue* queue, void* value);
/**
 Adicionar antes de um item
*/
Queue* queue_prepend_at(Queue* queue, List* item, void* value);
/**
 Adicionar após um item
*/
Queue* queue_append_at(Queue* queue, List* item, void* value);
/**
 Adicionar antes de item especificado pela sua posição
*/
Queue* queue_prepend_at_index(Queue* queue, uint32_t index, void* value);
/**
 Adicionar após um item especificado pela sua posição
*/
Queue* queue_append_at_index(Queue* queue, uint32_t index, void* value);
/**
 Adicionar um item List no início da fila
*/
Queue* queue_prepend_item(Queue* queue, List* new_item);
/**
 Adicionar um item List no fim da fila
*/
Queue* queue_append_item(Queue* queue, List* new_item);
/**
 Adicionar um item List antes de um item
*/
Queue* queue_prepend_item_at(Queue* queue, List* item, List* new_item);
/**
 Adicionar um item List depois de um item
*/
Queue* queue_append_item_at(Queue* queue, List* item, List* new_item);
/**
 Adicionar um item List antes de um item especificado pela sua posição
*/
Queue* queue_prepend_item_at_index(Queue* queue, uint32_t index, List* new_item);
/**
 Adicionar um item List depois de um item especificado pela sua posição
*/
Queue* queue_append_item_at_index(Queue* queue, uint32_t index, List* new_item);
/**
 Get first item
*/
List* queue_begin(Queue* queue);
/**
 Get last item
*/
List* queue_end(Queue* queue);
/**
 True if length is zero
*/
uint8_t queue_is_empty(Queue* queue);
/**
 Inverte a fila
*/
Queue* queue_reverse(Queue* queue);
/**
 ordena a fila
*/
Queue* queue_sort(Queue* queue);
/**
 copia a fila
*/
Queue* queue_copy(Queue* queue);
/**
 vectorize a function within a queue
*/
Queue* queue_foreach(Queue* queue, );
/**
 posição de um item especificado pelo seu valor
*/
int32_t queue_index_of(Queue* queue);
/**
 obter um item a partir de sua posição
*/
List* queue_at(Queue* queue, uint32_t index);
/**
 obter o valor do item a partir de sua posição
*/
void* queue_value_at(Queue* queue, uint32_t index);
/**
 obter o valor do item inicial
*/
void* queue_begin_value(Queue* queue);
/**
 obter o valor do item final
*/
void* queue_end_value(Queue* queue);
/**
 Remover um item em uma fila
*/
Queue* queue_remove(Queue* queue, void* value);
/**
 Remover um item em uma fila baseado em sua posição
*/
Queue* queue_remove_by_index(Queue* queue, uint32_t index);
/**
 Obter o número de elementos da fila
*/
uint32_t queue_length(Queue* queue);

#endif
