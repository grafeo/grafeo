#include <grafeo/ift.h>

IFT* ift_new(){
  return malloc(sizeof(IFT));
}

IFT* ift_new_from_array(Array* array, uint8_t map_dimension){
  IFT* ift = ift_new();
  ift->label        = array_zeros(map_dimension, array->size, GRAFEO_UINT16);
  ift->predecessors = array_zeros(map_dimension, array->size, GRAFEO_UINT64);
  ift->connectivity = array_zeros(map_dimension, array->size, GRAFEO_INT64);
  ift->root         = array_zeros(map_dimension, array->size, GRAFEO_UINT64);
  ift->original     = array;
  return ift;
}

IFT*   ift_apply_array(Array *array, uint16_t map_dimension, Adjacency adjacency, IFTOptimization optimization_type, WeightFunc weight_function, PathConnectivityFunc path_connectivity, Array* seeds_indices, Array* seeds_labels){
  // IFT structure
  IFT* ift          = ift_new_from_array(array, map_dimension);

  // Auxiliary structures
  Array* visited    = array_zeros(map_dimension, array->size, GRAFEO_UINT8);
  Queue* queue      = queue_new();


  // Initialize maps
  int64_t seed_connectivity;
  if(optimization_type == GRAFEO_IFT_MIN){
    array_fill_max(ift->connectivity);
    seed_connectivity = 0;

  }else if(optimization_type == GRAFEO_IFT_MAX){
    array_fill(ift->connectivity,0);
    seed_connectivity = INT64_MAX;
  }
  if(seeds_indices){
    uint64_t i;
    for(i = 0; i < seeds_indices->num_elements; i++){
      uint64_t seed_index = seeds_indices->data_uint64[i];
      ift->connectivity->data_int64[seed_index] = seed_connectivity;
      ift->label->data_uint16[seed_index]       = seeds_labels->data_uint16[i];
      ift->root->data_uint64[seed_index]        = seed_index;
      ift->predecessors->data_uint64[seed_index]= seed_index;
      // (re)insert unvisited neighbors
      if(optimization_type == GRAFEO_IFT_MIN)
        pqueue_append_at(queue, INT64_TO_POINTER(seed_connectivity), UINT64_TO_POINTER(seed_index), int64_compare_function);
      else if(optimization_type == GRAFEO_IFT_MAX)
        pqueue_append_at(queue, INT64_TO_POINTER(seed_connectivity), UINT64_TO_POINTER(seed_index), int64_compare_function_r);
    }
  }

  // Standard neighbors
  int32_t neighbors_relative_2D[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};
  int32_t neighbors_relative_3D[26][3] = {// neighbor-6
                                          {-1,0,0},{1,0,0},{0,-1,0},{0,1,0},{0,0,-1},{0,0,1},
                                          // neighbor-18
                                          {-1,-1, 0},{-1,1,0},{1,-1, 0},{1, 1,0},{-1,0,-1},{-1,0,1},
                                          { 1, 0,-1},{ 1,0,1},{0,-1,-1},{0,-1,1},{ 0,1,-1},{ 0,1,1},
                                          // neighbor-26
                                          {-1,-1,-1},{-1,-1,1},{-1,1,-1},{-1,1,1},
                                          { 1,-1,-1},{ 1,-1,1},{ 1,1,-1},{ 1,1,1}};

//  int32_t** neighbors_relative;
//  if(map_dimension == 2) neighbors_relative = neighbors_relative_2D;
//  else                   neighbors_relative = neighbors_relative_3D;

  // Process all nodes
  uint64_t index_s; // indices for nodes s and t
  int64_t  index_t;
  uint8_t  i,num_neighbors;       // for iterating neighbors
  int64_t  connectivity;            // connectivity for extended path <r...s,t>

  switch(adjacency){
    case GRAFEO_NEIGHBOR_4:num_neighbors = 4;break;
    case GRAFEO_NEIGHBOR_8:num_neighbors = 4;break;
    case GRAFEO_NEIGHBOR_6:num_neighbors = 6;break;
    case GRAFEO_NEIGHBOR_18:num_neighbors = 18;break;
    case GRAFEO_NEIGHBOR_26:num_neighbors = 26;break;
  }

  int32_t*  index_t_nd   = malloc(sizeof(int32_t)*ift->label->dim);
  uint32_t* index_t_nd_u = malloc(sizeof(uint32_t)*ift->label->dim);

  while(!queue_is_empty(queue)){

    // Get node of minimum connectivity value (remove it from queue)
    index_s = POINTER_TO_UINT64(pqueue_bucket_at(queue,0,0));
    pqueue_remove_begin(queue);
    pqueue_shrink(queue);

    uint32_t* index_nd = (uint32_t*)array_index(ift->label, (int64_t)index_s);

    // Do not process this node again
    array_set_element(visited, index_nd, 1);

    // Process all neighbors
    for(i = 0; i < num_neighbors; i++){

      // Calculate neighbor index
      uint8_t j;
      for(j = 0; j < ift->label->dim; j++)
        if(map_dimension == 2)
          index_t_nd[j] = (int32_t)index_nd[j] + neighbors_relative_2D[i][j];
        else
          index_t_nd[j] = (int32_t)index_nd[j] + neighbors_relative_3D[i][j];

      // Verify if it's valid (inside array region)
      if(array_index_is_valid(visited, index_t_nd)){

        for(j = 0; j < ift->label->dim; j++)
          index_t_nd_u[j] = (uint32_t)index_t_nd[j];

        index_t = array_index_1D(ift->label, index_t_nd);

        // If it's not visited
        uint8_t*  element           = (uint8_t*)  array_get_element(visited, index_t_nd_u);
        if(!*element){
          // Calculate connectivity of extended path
          connectivity = path_connectivity(ift, index_s, index_t, weight_function);

          // Compare with the current connectivity
          int64_t* old_connectivity = (int64_t*)array_get_element(ift->connectivity, index_t_nd_u);
          if((optimization_type == GRAFEO_IFT_MIN && connectivity < *old_connectivity) ||
             (optimization_type == GRAFEO_IFT_MAX && connectivity > *old_connectivity)){

            // remove it from queue if it's a seed
            if((optimization_type == GRAFEO_IFT_MIN && *old_connectivity != INT64_MAX) ||
               (optimization_type == GRAFEO_IFT_MAX && *old_connectivity != 0)){
              pqueue_remove_at(queue, INT64_TO_POINTER(*old_connectivity), UINT64_TO_POINTER(index_t));
              pqueue_shrink(queue);
            }



            // Update predecessors, connectivity, labels and roots
            array_set_element(ift->predecessors, index_t_nd_u, index_s);
            array_set_element(ift->connectivity, index_t_nd_u, connectivity);
            array_set_element(ift->label,        index_t_nd_u, *(uint16_t*) array_get_element(ift->label, index_nd));
            array_set_element(ift->root,         index_t_nd_u, *(uint64_t*) array_get_element(ift->root,  index_nd));

            // (re)insert unvisited neighbors
            if(optimization_type == GRAFEO_IFT_MIN)
              pqueue_append_at(queue, INT64_TO_POINTER(connectivity), UINT64_TO_POINTER(index_t), int64_compare_function);
            else if(optimization_type == GRAFEO_IFT_MAX)
              pqueue_append_at(queue, INT64_TO_POINTER(connectivity), UINT64_TO_POINTER(index_t), int64_compare_function_r);
          }
        } 
      }    
    }
    free(index_nd);
  }
  free(index_t_nd_u);
  free(index_t_nd);

  array_free(visited);
  queue_free(queue);

  return ift;
}

Array* ift_get_label(IFT *ift){
  return ift->label;
}

Array* ift_get_connectivity(IFT *ift){
  return ift->connectivity;
}

Array* ift_get_original(IFT *ift){
  return ift->original;
}

Array* ift_get_predecessors(IFT *ift){
  return ift->predecessors;
}

Array* ift_get_root(IFT *ift){
  return ift->root;
}

void ift_set_label(IFT* ift, Array* label){
  ift->label        = label;
}

void ift_set_connectivity(IFT* ift, Array* connectivity){
  ift->connectivity = connectivity;
}

void ift_set_original(IFT* ift, Array* original){
  ift->original     = original;
}

void ift_set_predecessors(IFT* ift, Array* predecessors){
  ift->predecessors = predecessors;
}

void ift_set_root(IFT* ift, Array* root){
  ift->root         = root;
}

double path_connectivity_sum(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return ift->connectivity->data_int64[index_s] +
         weight_function(ift->original,
                         index_s*ift->original->step[ift->label->dim-1],
                         index_t*ift->original->step[ift->label->dim-1]);
}

double path_connectivity_max(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return max(ift->connectivity->data_int64[index_s],
             weight_function(ift->original,
                             index_s*ift->original->step[ift->label->dim-1],
                             index_t*ift->original->step[ift->label->dim-1]));
}

double path_connectivity_min(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return min(ift->connectivity->data_int64[index_s],
             weight_function(ift->original,
                             index_s*ift->original->step[ift->label->dim-1],
                             index_t*ift->original->step[ift->label->dim-1]));
}

double path_connectivity_euc(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  (void) weight_function;
  uint64_t index_r = ift->root->data_uint64[index_s];
  uint32_t size = (uint32_t)ift->original->dim;
  Array* array_r = array_from_data(array_index(ift->original,index_r),1,&size,GRAFEO_INT32);
  Array* array_t = array_from_data(array_index(ift->original,index_t),1,&size,GRAFEO_INT32);

  return array_euclidian_distance(array_r, array_t);
}

double _path_connectivity_norm(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function, NormType norm_type){
  (void) weight_function;
  uint64_t index_r = ift->root->data_uint64[index_s];
  uint32_t size = (uint32_t)ift->original->dim;
  Array* array_r = array_from_data(array_index(ift->original,index_r),1,&size,GRAFEO_INT32);
  Array* array_t = array_from_data(array_index(ift->original,index_t),1,&size,GRAFEO_INT32);

  return array_norm_difference(array_r, array_t, norm_type);
}

double path_connectivity_norm_l1(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return _path_connectivity_norm(ift, index_s, index_t, weight_function, GRAFEO_NORM_L1);
}
double path_connectivity_norm_l2(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return _path_connectivity_norm(ift, index_s, index_t, weight_function, GRAFEO_NORM_L2);
}
double path_connectivity_norm_l2sqr(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return _path_connectivity_norm(ift, index_s, index_t, weight_function, GRAFEO_NORM_L2SQR);
}
double path_connectivity_norm_inf(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return _path_connectivity_norm(ift, index_s, index_t, weight_function, GRAFEO_NORM_INF);
}

void   ift_free(IFT* ift){
  free(ift);
}

double weight_diff(Array *array, uint64_t index1, uint64_t index2){
  switch (array->type){
    case GRAFEO_UINT8:  return fabs((double) array->data_uint8[index1]  - (double) array->data_uint8[index2]);break;
    case GRAFEO_UINT16: return fabs((double) array->data_uint16[index1] - (double) array->data_uint16[index2]);break;
    case GRAFEO_UINT32: return fabs((double) array->data_uint32[index1] - (double) array->data_uint32[index2]);break;
    case GRAFEO_UINT64: return fabs((double) array->data_uint64[index1] - (double) array->data_uint64[index2]);break;
    case GRAFEO_INT8:   return fabs((double) array->data_int8[index1]   - (double) array->data_int8[index2]);break;
    case GRAFEO_INT16:  return fabs((double) array->data_int16[index1]  - (double) array->data_int16[index2]);break;
    case GRAFEO_INT32:  return fabs((double) array->data_int32[index1]  - (double) array->data_int32[index2]);break;
    case GRAFEO_INT64:  return fabs((double) array->data_int64[index1]  - (double) array->data_int64[index2]);break;
    case GRAFEO_FLOAT:  return fabs((double) array->data_float[index1]  - (double) array->data_float[index2]);break;
    case GRAFEO_DOUBLE: return fabs((double) array->data_double[index1] - (double) array->data_double[index2]);break;
  }
  return 0;
}

double weight_diff_3(Array* array, uint64_t index1, uint64_t index2){
  long double value1 = array_get_long_double_1D(array, index1  )   - array_get_long_double_1D(array, index2),
              value2 = array_get_long_double_1D(array, index1+1)   - array_get_long_double_1D(array, index2+1),
              value3 = array_get_long_double_1D(array, index1+2)   - array_get_long_double_1D(array, index2+2);
  double result = sqrt(value1*value1 + value2*value2 + value3*value3);
  return result;
}


Array* ift_distance_transform(Array* array, NormType norm_type){

  // Define Path Connectivity
  PathConnectivityFunc path_connectivity;
  switch(norm_type){
    case GRAFEO_NORM_L1:     path_connectivity = path_connectivity_norm_l1    ; break;
    case GRAFEO_NORM_L2:     path_connectivity = path_connectivity_norm_l2    ; break;
    case GRAFEO_NORM_L2SQR:  path_connectivity = path_connectivity_norm_l2sqr ; break;
    case GRAFEO_NORM_INF:    path_connectivity = path_connectivity_norm_inf   ; break;
    default: break;
  }

  // Get seeds
  uint64_t seed_indices_data[array->num_elements];
  uint32_t i, s = 0;
  for(i = 0; i < array->num_elements; i++)
    if(array_get_long_double_1D(array, i)) seed_indices_data[s++] = i;
  Array* seed_indices = array_from_data(seed_indices_data, 1, &s, GRAFEO_UINT64);
  Array* seed_labels  = array_ones(1, &s, GRAFEO_UINT16);

  IFT*   ift   = ift_apply_array(array,                 // Array
                                 2,                     // Map dimension
                                 GRAFEO_NEIGHBOR_4,     // Adjacency
                                 GRAFEO_IFT_MIN,        // Optimization
                                 weight_diff,           // Weighting Function
                                 path_connectivity,     // Connectivity
                                 seed_indices,          // Indices of seeds
                                 seed_labels            // Indices of seeds
                                 );
  // Send results
  Array* connectivity = array_as_type(ift->connectivity, GRAFEO_UINT8);
  array_free(seed_indices);
  array_free(seed_labels);
  ift_free(ift);
  return connectivity;
}
