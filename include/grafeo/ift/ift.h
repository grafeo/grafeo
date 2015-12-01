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
#include <grafeo/core.h>

BEGIN_DECLS

/**
 * @brief Structure for managing IFT data
 */
typedef struct _GrfIFT{
  GrfNDArray* original;    /**< Original array */
  GrfNDArray* label;       /**< Label map */
  GrfNDArray* predecessors;/**< Predecessors map */
  GrfNDArray* connectivity;/**< Connectivity map */
  GrfNDArray* root;        /**< Root map */
}GrfIFT;

/**
 * Function to calculate weights between elements of an array.
 * Probably general graphs won't be using this function, because they will have
 * edges instances with weight values.
 */
typedef double
(*GrfWeightFunc)(GrfNDArray* array, uint64_t index1, uint64_t index2);

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
(*GrfPathConnectivityFunc)(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);

/* ====================== *
 *    IFT ALGORITHMS
 * ====================== */
/**
 * @brief Allocates a new IFT structure instance
 * @return the instance
 * @memberof GrfIFT
 */
GrfIFT*   grf_ift_new();
/**
 * @brief grf_ift_new_from_array
 * @param array
 * @param map_dimension
 * @return
 * @memberof GrfIFT
 */
GrfIFT*   grf_ift_new_from_array(GrfNDArray* array, uint8_t map_dimension);
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
 * @memberof GrfIFT
 */
GrfIFT*   grf_ift_apply_array(GrfNDArray* array,
                       uint16_t map_dimension,
                       GrfAdjacency adjacency,
                       GrfOptimizationType optimization_type,
                       GrfWeightFunc weight_function,
                       GrfPathConnectivityFunc path_connectivity,
                       GrfNDArray* seeds_indices,
                       GrfNDArray* seeds_labels);
/**
 * @brief path_connectivity_sum
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_sum(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_max
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_max(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_min
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_min(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_norm_l1
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_norm_l1(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_norm_l2
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_norm_l2(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_norm_l2sqr
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_norm_l2sqr(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief path_connectivity_norm_inf
 * @param ift
 * @param index_s
 * @param index_t
 * @param weight_function
 * @return
 */
double grf_path_connectivity_norm_inf(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function);
/**
 * @brief weight_diff
 * @param array
 * @param index1
 * @param index2
 * @return
 */
double grf_weight_diff(GrfNDArray *array, uint64_t index1, uint64_t index2);
/**
 * @brief weight_diff_3
 * @param array
 * @param index1
 * @param index2
 * @return
 */
double grf_weight_diff_3(GrfNDArray* array, uint64_t index1, uint64_t index2);
/**
 * @brief grf_ift_free
 * @param ift
 * @memberof GrfIFT
 */
void   grf_ift_free(GrfIFT* ift);

/* ====================== *
 * IFT ACCESSOR FUNCTIONS
 * ====================== */
/**
 * @brief Get the label map of the IFT
 * @param ift
 * @return
 * @memberof GrfIFT
 */
GrfNDArray* grf_ift_get_label(GrfIFT* ift);
/**
 * @brief Get the predecessors map of the IFT
 * @param ift
 * @return
 */
GrfNDArray* grf_ift_get_predecessors(GrfIFT* ift);
/**
 * @brief Get connectivity map of the IFT
 * @param ift
 * @return
 * @memberof GrfIFT
 */
GrfNDArray* grf_ift_get_connectivity(GrfIFT* ift);
/**
 * @brief Get root map of the IFT
 * @param ift
 * @return
 */
GrfNDArray* grf_ift_get_root(GrfIFT* ift);
/**
 * @brief Get original array (e.g. image) of the IFT
 * @param ift
 * @return
 * @memberof GrfIFT
 */
GrfNDArray* grf_ift_get_original(GrfIFT* ift);
/**
 * @brief Set label map of the IFT
 * @param ift
 * @param label
 * @memberof GrfIFT
 */
void grf_ift_set_label(GrfIFT* ift, GrfNDArray* label);
/**
 * @brief Set connectivity map of the IFT
 * @param ift
 * @param connectivity
 * @memberof GrfIFT
 */
void grf_ift_set_connectivity(GrfIFT* ift, GrfNDArray* connectivity);
/**
 * @brief Set original array of the IFT
 * @param ift
 * @param original
 * @memberof GrfIFT
 */
void grf_ift_set_original(GrfIFT* ift, GrfNDArray* original);
/**
 * @brief Set predecessors map of the IFT
 * @param ift
 * @param predecessors
 * @memberof GrfIFT
 */
void grf_ift_set_predecessors(GrfIFT* ift, GrfNDArray* predecessors);
/**
 * @brief Set root map of the IFT
 * @param ift
 * @param root
 * @memberof GrfIFT
 */
void grf_ift_set_root(GrfIFT* ift, GrfNDArray* root);
/* ====================== *
 *     IFT OPERATIONS
 * ====================== */
/**
 * @brief grf_ift_distance_transform
 * @param array
 * @param norm_type
 * @return
 */
GrfNDArray* grf_ift_distance_transform(GrfNDArray* array, GrfNormType norm_type);

END_DECLS

#endif
