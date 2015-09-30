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
#include <grafeo/pqueue.h>
#include <grafeo/macro.h>

BEGIN_DECLS

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

/**
  * @brief Optimization type for IFT: Minimization (Primal) or Maximization (Dual)
  */
typedef enum _IFTOptimization{
  GRAFEO_IFT_MIN,
  GRAFEO_IFT_MAX
} IFTOptimization;

/**
 * Function to calculate weights between elements of an array.
 * Probably general graphs won't be using this function, because they will have
 * edges instances with weight values.
 */
typedef double
(*WeightFunc)(Array* array, uint64_t index1, uint64_t index2);

/**
 * @brief Path connectivity (just the non-trivial cases).
 *
 * The trivial cases should be addressed at initialization routines. This doesn't modify
 * the IFT structure. For euclidian connectivity, you can use the ift->root map to get `r`.
 *
 * @param ift             data structure for IFT to be used for calculating connectivity
 * @param index_s         the end of current path  <r...s>
 * @param index_t         the end of extended path <r...s,t>
 * @param weight_function some path connectivity functions use the weights of edges (s,t)
 */
typedef double
(*PathConnectivityFunc)(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);

/* ====================== *
 *    IFT ALGORITHMS
 * ====================== */
/**
 * @brief Allocates a new IFT structure instance
 * @return the instance
 */
IFT*   ift_new();
/**
 * @brief Run IFT in an array
 * @param array
 * @param adjacency
 * @param optimization_type
 * @param weight_function
 * @param path_connectivity
 * @return
 */
IFT*   ift_apply_array(Array* array, 
                       Adjacency adjacency,
                       IFTOptimization optimization_type,
                       WeightFunc weight_function,
                       PathConnectivityFunc path_connectivity);
double path_connectivity_sum(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
double path_connectivity_max(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
double path_connectivity_min(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
double path_connectivity_euc(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
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

END_DECLS

#endif
