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
#ifndef IFT_ARRAY_H
#define IFT_ARRAY_H
#include <grafeo/array.h>
#include <grafeo/queue.h>
/**
  * @brief Structure for managing IFT data
  */
typedef struct _IFT{
  Array* original;
  Array* label;
  Array* predecessors;
  Array* connectivity;
  Array* root;
}IFT;
/* ====================== *
 *    IFT ALGORITHMS
 * ====================== */
/**
 * @brief Allocates a new IFT structure instance
 * @return the instance
 */
IFT*   ift_new();
IFT*   ift_apply_array(Array* array, Adjacency adjacency, double (*weight_function)(Array* array, uint64_t index1, uint64_t index2),double (*path_connectivity)(double connectivity_value, double weight_value));
void   path_connectivity_sum();
void   path_connectivity_max();
void   path_connectivity_min();
void   path_connectivity_euc();
void   ift_free(IFT* ift);

/* ====================== *
 * IFT ACCESSOR FUNCTIONS
 * ====================== */
/**
 * @brief ift_get_label
 * @param ift
 * @return
 */
Array* ift_get_label(IFT* ift);
/**
 * @brief ift_get_predecessors
 * @param ift
 * @return
 */
Array* ift_get_predecessors(IFT* ift);
/**
 * @brief ift_get_connectivity
 * @param ift
 * @return
 */
Array* ift_get_connectivity(IFT* ift);
/**
 * @brief ift_get_root
 * @param ift
 * @return
 */
Array* ift_get_root(IFT* ift);
/**
 * @brief ift_get_original
 * @param ift
 * @return
 */
Array* ift_get_original(IFT* ift);
/**
 * @brief ift_set_label
 * @param ift
 * @param label
 */
void ift_set_label(IFT* ift, Array* label);
/**
 * @brief ift_set_connectivity
 * @param ift
 * @param connectivity
 */
void ift_set_connectivity(IFT* ift, Array* connectivity);
/**
 * @brief ift_set_original
 * @param ift
 * @param original
 */
void ift_set_original(IFT* ift, Array* original);
/**
 * @brief ift_set_predecessors
 * @param ift
 * @param predecessors
 */
void ift_set_predecessors(IFT* ift, Array* predecessors);
/**
 * @brief ift_set_root
 * @param ift
 * @param root
 */
void ift_set_root(IFT* ift, Array* root);

#endif
