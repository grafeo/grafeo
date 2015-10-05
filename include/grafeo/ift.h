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

BEGIN_DECLS

/**
 * @brief Structure for managing IFT data
 */
typedef struct _IFT{
  Array* original;    /**< Original array */
  Array* label;       /**< Label map */
  Array* predecessors;/**< Predecessors map */
  Array* connectivity;/**< Connectivity map */
  Array* root;        /**< Root map */
}IFT;

/**
  * @brief Optimization type for IFT: Minimization (Primal) or Maximization (Dual)
  * @memberof IFT
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
 * @memberof IFT
 */
IFT*   ift_new();
/**
 * @brief ift_new_from_array
 * @param array
 * @param map_dimension
 * @return
 * @memberof IFT
 */
IFT*   ift_new_from_array(Array* array, uint8_t map_dimension);
/**
 * @brief Run IFT in an array
 * @param array
 * @param map_dimension
 * @param adjacency
 * @param optimization_type
 * @param weight_function
 * @param path_connectivity
 * @param seeds_indices
 * @param seeds_labels
 * @return
 * @memberof IFT
 */
IFT*   ift_apply_array(Array* array, 
                       uint16_t map_dimension,
                       Adjacency adjacency,
                       IFTOptimization optimization_type,
                       WeightFunc weight_function,
                       PathConnectivityFunc path_connectivity,
                       Array* seeds_indices,
                       Array* seeds_labels);
/**
 * @brief path_connectivity_sum
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double path_connectivity_sum(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
/**
 * @brief path_connectivity_max
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double path_connectivity_max(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
/**
 * @brief path_connectivity_min
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double path_connectivity_min(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
/**
 * @brief path_connectivity_euc
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double path_connectivity_euc(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function);
/**
 * @brief weight_diff
 * @param array
 * @param index1
 * @param index2
 * @return
 */
double weight_diff(Array *array, uint64_t index1, uint64_t index2);
/**
 * @brief weight_diff_3
 * @param array
 * @param index1
 * @param index2
 * @return
 */
double weight_diff_3(Array* array, uint64_t index1, uint64_t index2);
/**
 * @brief ift_free
 * @param ift
 * @memberof IFT
 */
void   ift_free(IFT* ift);

/* ====================== *
 * IFT ACCESSOR FUNCTIONS
 * ====================== */
/**
 * @brief Get the label map of the IFT
 * @param ift
 * @return
 * @memberof IFT
 */
Array* ift_get_label(IFT* ift);
/**
 * @brief Get the predecessors map of the IFT
 * @param ift
 * @return
 */
Array* ift_get_predecessors(IFT* ift);
/**
 * @brief Get connectivity map of the IFT
 * @param ift
 * @return
 * @memberof IFT
 */
Array* ift_get_connectivity(IFT* ift);
/**
 * @brief Get root map of the IFT
 * @param ift
 * @return
 */
Array* ift_get_root(IFT* ift);
/**
 * @brief Get original array (e.g. image) of the IFT
 * @param ift
 * @return
 * @memberof IFT
 */
Array* ift_get_original(IFT* ift);
/**
 * @brief Set label map of the IFT
 * @param ift
 * @param label
 * @memberof IFT
 */
void ift_set_label(IFT* ift, Array* label);
/**
 * @brief Set connectivity map of the IFT
 * @param ift
 * @param connectivity
 * @memberof IFT
 */
void ift_set_connectivity(IFT* ift, Array* connectivity);
/**
 * @brief Set original array of the IFT
 * @param ift
 * @param original
 * @memberof IFT
 */
void ift_set_original(IFT* ift, Array* original);
/**
 * @brief Set predecessors map of the IFT
 * @param ift
 * @param predecessors
 * @memberof IFT
 */
void ift_set_predecessors(IFT* ift, Array* predecessors);
/**
 * @brief Set root map of the IFT
 * @param ift
 * @param root
 * @memberof IFT
 */
void ift_set_root(IFT* ift, Array* root);

END_DECLS

#endif
