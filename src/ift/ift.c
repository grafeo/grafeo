#include <grafeo/ift.h>

GrfIFT* grf_ift_new(){
  return malloc(sizeof(GrfIFT));
}

GrfIFT* grf_ift_new_from_array(GrfNDArray* array, uint8_t map_dimension){
  GrfIFT* ift = grf_ift_new();
  uint32_t* array_size = grf_ndarray_get_size(array);
  ift->label        = grf_ndarray_zeros(map_dimension, array_size, GRF_UINT16);
  ift->predecessors = grf_ndarray_zeros(map_dimension, array_size, GRF_UINT64);
  ift->connectivity = grf_ndarray_zeros(map_dimension, array_size, GRF_INT64);
  ift->root         = grf_ndarray_zeros(map_dimension, array_size, GRF_UINT64);
  g_set_object(&ift->original,array);
  return ift;
}

GrfIFT*   grf_ift_apply_array(GrfNDArray *array, uint16_t map_dimension, GrfAdjacency adjacency, GrfOptimizationType optimization_type, GrfWeightFunc weight_function, GrfPathConnectivityFunc path_connectivity, GrfNDArray* seeds_indices, GrfNDArray* seeds_labels){
  // IFT structure
  GrfIFT* ift          = grf_ift_new_from_array(array, map_dimension);

  // Auxiliary structures
  uint32_t* array_size = grf_ndarray_get_size(array);
  g_autoptr(GrfNDArray) visited  = grf_ndarray_zeros(map_dimension, array_size, GRF_UINT8);
  GrfQueue* queue      = grf_queue_new();


  // Initialize maps
  int64_t seed_connectivity;
  if(optimization_type == GRF_MINIMIZATION){
    grf_ndarray_fill_max(ift->connectivity);
    seed_connectivity = 0;

  }else if(optimization_type == GRF_MAXIMIZATION){
    grf_ndarray_fill(ift->connectivity,0);
    seed_connectivity = INT64_MAX;
  }
  if(seeds_indices){
    uint64_t i;
    uint64_t* seeds_indices_data = (uint64_t*)grf_ndarray_get_data(seeds_indices);
    uint16_t* seeds_labels_data  = (uint16_t*)grf_ndarray_get_data(seeds_labels);

    int64_t*  connectivity_data = grf_ndarray_get_data(ift->connectivity);
    int16_t*  label_data        = grf_ndarray_get_data(ift->label);
    uint64_t* root_data         = grf_ndarray_get_data(ift->root);
    uint64_t* predecessors_data = grf_ndarray_get_data(ift->predecessors);

    for(i = 0; i < grf_ndarray_get_num_elements(seeds_indices); i++){
      uint64_t seed_index = seeds_indices_data[i];
      connectivity_data[seed_index] = seed_connectivity;
      label_data[seed_index]       = seeds_labels_data[i];
      root_data[seed_index]        = seed_index;
      predecessors_data[seed_index]= seed_index;
      // (re)insert unvisited neighbors
      if(optimization_type == GRF_MINIMIZATION)
        grf_pqueue_append_at(queue, INT64_TO_POINTER(seed_connectivity), UINT64_TO_POINTER(seed_index), int64_compare_function);
      else if(optimization_type == GRF_MAXIMIZATION)
        grf_pqueue_append_at(queue, INT64_TO_POINTER(seed_connectivity), UINT64_TO_POINTER(seed_index), int64_compare_function_r);
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
    case GRF_NEIGHBOR_4:num_neighbors = 4;break;
    case GRF_NEIGHBOR_8:num_neighbors = 4;break;
    case GRF_NEIGHBOR_6:num_neighbors = 6;break;
    case GRF_NEIGHBOR_18:num_neighbors = 18;break;
    case GRF_NEIGHBOR_26:num_neighbors = 26;break;
  }
  uint16_t  label_dim    = grf_ndarray_get_dim(ift->label);
  int32_t*  index_t_nd   = malloc(sizeof(int32_t)*label_dim);
  uint32_t* index_t_nd_u = malloc(sizeof(uint32_t)*label_dim);

  while(!grf_queue_is_empty(queue)){

    // Get node of minimum connectivity value (remove it from queue)
    index_s = POINTER_TO_UINT64(grf_pqueue_grf_bucket_at(queue,0,0));
    grf_pqueue_remove_begin(queue);
    grf_pqueue_shrink(queue);

    uint32_t* index_nd = (uint32_t*)grf_ndarray_index(ift->label, (int64_t)index_s);

    // Do not process this node again
    grf_ndarray_set_element(visited, index_nd, 1);

    // Process all neighbors
    for(i = 0; i < num_neighbors; i++){

      // Calculate neighbor index
      uint8_t j;
      for(j = 0; j < label_dim; j++)
        if(map_dimension == 2)
          index_t_nd[j] = (int32_t)index_nd[j] + neighbors_relative_2D[i][j];
        else
          index_t_nd[j] = (int32_t)index_nd[j] + neighbors_relative_3D[i][j];

      // Verify if it's valid (inside array region)
      if(grf_ndarray_index_is_valid(visited, index_t_nd)){

        for(j = 0; j < label_dim; j++)
          index_t_nd_u[j] = (uint32_t)index_t_nd[j];

        index_t = grf_ndarray_index_1D(ift->label, index_t_nd);

        // If it's not visited
        uint8_t*  element           = (uint8_t*)  grf_ndarray_get_element(visited, index_t_nd_u);
        if(!*element){
          // Calculate connectivity of extended path
          connectivity = path_connectivity(ift, index_s, index_t, weight_function);

          // Compare with the current connectivity
          int64_t* old_connectivity = (int64_t*)grf_ndarray_get_element(ift->connectivity, index_t_nd_u);
          if((optimization_type == GRF_MINIMIZATION && connectivity < *old_connectivity) ||
             (optimization_type == GRF_MAXIMIZATION && connectivity > *old_connectivity)){

            // remove it from queue if it's a seed
            if((optimization_type == GRF_MINIMIZATION && *old_connectivity != INT64_MAX) ||
               (optimization_type == GRF_MAXIMIZATION && *old_connectivity != 0)){
              grf_pqueue_remove_at(queue, INT64_TO_POINTER(*old_connectivity), UINT64_TO_POINTER(index_t));
              grf_pqueue_shrink(queue);
            }



            // Update predecessors, connectivity, labels and roots
            grf_ndarray_set_element(ift->predecessors, index_t_nd_u, index_s);
            grf_ndarray_set_element(ift->connectivity, index_t_nd_u, connectivity);
            grf_ndarray_set_element(ift->label,        index_t_nd_u, *(uint16_t*) grf_ndarray_get_element(ift->label, index_nd));
            grf_ndarray_set_element(ift->root,         index_t_nd_u, *(uint64_t*) grf_ndarray_get_element(ift->root,  index_nd));

            // (re)insert unvisited neighbors
            if(optimization_type == GRF_MINIMIZATION)
              grf_pqueue_append_at(queue, INT64_TO_POINTER(connectivity), UINT64_TO_POINTER(index_t), int64_compare_function);
            else if(optimization_type == GRF_MAXIMIZATION)
              grf_pqueue_append_at(queue, INT64_TO_POINTER(connectivity), UINT64_TO_POINTER(index_t), int64_compare_function_r);
          }
        } 
      }    
    }
    free(index_nd);
  }
  free(index_t_nd_u);
  free(index_t_nd);

  grf_queue_free(queue);

  return ift;
}

GrfNDArray* grf_ift_get_label(GrfIFT *ift){
  return ift->label;
}

GrfNDArray* grf_ift_get_connectivity(GrfIFT *ift){
  return ift->connectivity;
}

GrfNDArray* grf_ift_get_original(GrfIFT *ift){
  return ift->original;
}

GrfNDArray* grf_ift_get_predecessors(GrfIFT *ift){
  return ift->predecessors;
}

GrfNDArray* grf_ift_get_root(GrfIFT *ift){
  return ift->root;
}

void grf_ift_set_label(GrfIFT* ift, GrfNDArray* label){
  ift->label        = label;
}

void grf_ift_set_connectivity(GrfIFT* ift, GrfNDArray* connectivity){
  ift->connectivity = connectivity;
}

void grf_ift_set_original(GrfIFT* ift, GrfNDArray* original){
  ift->original     = original;
}

void grf_ift_set_predecessors(GrfIFT* ift, GrfNDArray* predecessors){
  ift->predecessors = predecessors;
}

void grf_ift_set_root(GrfIFT* ift, GrfNDArray* root){
  ift->root         = root;
}

double grf_path_connectivity_sum(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  int64_t* connectivity_data = (int64_t*)grf_ndarray_get_data(ift->connectivity);
  uint64_t* original_step = grf_ndarray_get_step(ift->original);
  uint16_t label_dim = grf_ndarray_get_dim(ift->label);
  return connectivity_data[index_s] +
         weight_function(ift->original,
                         index_s*original_step[label_dim-1],
                         index_t*original_step[label_dim-1]);
}

double grf_path_connectivity_max(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  int64_t* connectivity_data = (int64_t*)grf_ndarray_get_data(ift->connectivity);
  uint64_t* original_step = grf_ndarray_get_step(ift->original);
  uint16_t label_dim = grf_ndarray_get_dim(ift->label);
  return max(connectivity_data[index_s],
             weight_function(ift->original,
                             index_s*original_step[label_dim-1],
                             index_t*original_step[label_dim-1]));
}

double grf_path_connectivity_min(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  int64_t* connectivity_data = (int64_t*)grf_ndarray_get_data(ift->connectivity);
  uint64_t* original_step = grf_ndarray_get_step(ift->original);
  uint16_t label_dim = grf_ndarray_get_dim(ift->label);
  return min(connectivity_data[index_s],
             weight_function(ift->original,
                             index_s*original_step[label_dim-1],
                             index_t*original_step[label_dim-1]));
}

double _grf_path_connectivity_norm(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function, GrfNormType norm_type){
  (void) weight_function;
  uint64_t* root_data = (uint64_t*) grf_ndarray_get_data(ift->root);
  uint16_t original_dim = grf_ndarray_get_dim(ift->original);
  uint64_t index_r = root_data[index_s];
  uint32_t size = (uint32_t)original_dim;
  GrfNDArray* grf_ndarray_r = grf_ndarray_from_data(grf_ndarray_index(ift->original,index_r),1,&size,GRF_INT32);
  GrfNDArray* grf_ndarray_t = grf_ndarray_from_data(grf_ndarray_index(ift->original,index_t),1,&size,GRF_INT32);

  return grf_ndarray_norm_difference(grf_ndarray_r, grf_ndarray_t, norm_type);
}

double grf_path_connectivity_norm_l1(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  return _grf_path_connectivity_norm(ift, index_s, index_t, weight_function, GRF_NORM_L1);
}
double grf_path_connectivity_norm_l2(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  return _grf_path_connectivity_norm(ift, index_s, index_t, weight_function, GRF_NORM_L2);
}
double grf_path_connectivity_norm_l2sqr(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  return _grf_path_connectivity_norm(ift, index_s, index_t, weight_function, GRF_NORM_L2SQR);
}
double grf_path_connectivity_norm_inf(GrfIFT* ift, uint64_t index_s, uint64_t index_t, GrfWeightFunc weight_function){
  return _grf_path_connectivity_norm(ift, index_s, index_t, weight_function, GRF_NORM_INF);
}

void   grf_ift_free(GrfIFT* ift){
  if(ift->connectivity) g_clear_object(&ift->connectivity);
  if(ift->label)        g_clear_object(&ift->label);
  if(ift->original)     g_clear_object(&ift->original);
  if(ift->predecessors) g_clear_object(&ift->predecessors);
  if(ift->root)         g_clear_object(&ift->root);
  free(ift);
}

double grf_weight_diff(GrfNDArray *array, uint64_t index1, uint64_t index2){
  return fabs((double)(grf_ndarray_get_long_double_1D(array,index1) - grf_ndarray_get_long_double_1D(array,index2)));
}

double grf_weight_diff_3(GrfNDArray* array, uint64_t index1, uint64_t index2){
  long double value1 = grf_ndarray_get_long_double_1D(array, index1  )   - grf_ndarray_get_long_double_1D(array, index2),
              value2 = grf_ndarray_get_long_double_1D(array, index1+1)   - grf_ndarray_get_long_double_1D(array, index2+1),
              value3 = grf_ndarray_get_long_double_1D(array, index1+2)   - grf_ndarray_get_long_double_1D(array, index2+2);
  double result = sqrt(value1*value1 + value2*value2 + value3*value3);
  return result;
}


GrfNDArray* grf_ift_distance_transform(GrfNDArray* array, GrfNormType norm_type){

  // Define Path Connectivity
  GrfPathConnectivityFunc path_connectivity;
  switch(norm_type){
    case GRF_NORM_L1:     path_connectivity = grf_path_connectivity_norm_l1    ; break;
    case GRF_NORM_L2:     path_connectivity = grf_path_connectivity_norm_l2    ; break;
    case GRF_NORM_L2SQR:  path_connectivity = grf_path_connectivity_norm_l2sqr ; break;
    case GRF_NORM_INF:    path_connectivity = grf_path_connectivity_norm_inf   ; break;
    default: break;
  }

  // Get seeds
  uint64_t array_num_elements = grf_ndarray_get_num_elements(array);
  uint64_t seed_indices_data[array_num_elements];
  uint32_t i, s = 0;
  for(i = 0; i < array_num_elements; i++)
    if(grf_ndarray_get_long_double_1D(array, i)) seed_indices_data[s++] = i;
  g_autoptr(GrfNDArray) seed_indices = grf_ndarray_from_data(seed_indices_data, 1, &s, GRF_UINT64);
  g_autoptr(GrfNDArray) seed_labels  = grf_ndarray_ones(1, &s, GRF_UINT16);

  GrfIFT*   ift   = grf_ift_apply_array(array,                 // GrfNDArray
                                 2,                     // Map dimension
                                 GRF_NEIGHBOR_4,     // Adjacency
                                 GRF_MINIMIZATION,        // Optimization
                                 grf_weight_diff,           // Weighting Function
                                 path_connectivity,     // Connectivity
                                 seed_indices,          // Indices of seeds
                                 seed_labels            // Indices of seeds
                                 );
  // Send results
  GrfNDArray* connectivity = grf_ndarray_as_type(ift->connectivity, GRF_UINT8);
  grf_ift_free(ift);
  return connectivity;
}
