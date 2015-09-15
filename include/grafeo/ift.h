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
Array* ift_get_label(ift);
/**
 * @brief ift_get_predecessors
 * @param ift
 * @return
 */
Array* ift_get_predecessors(ift);
/**
 * @brief ift_get_connectivity
 * @param ift
 * @return
 */
Array* ift_get_connectivity(ift);
/**
 * @brief ift_get_root
 * @param ift
 * @return
 */
Array* ift_get_root(ift);
/**
 * @brief ift_get_original
 * @param ift
 * @return
 */
Array* ift_get_original(IFT* ift);

#endif
