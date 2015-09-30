#include <grafeo/ift.h>

IFT* ift_new(){
  return malloc(sizeof(IFT));
}

IFT* ift_new_from_array(Array* array){
  IFT* ift = ift_new();
  ift->label        = array_zeros_like_type(array, GRAFEO_UINT16);
  ift->predecessors = array_zeros_like_type(array, GRAFEO_UINT64);
  ift->connectivity = array_zeros_like_type(array, GRAFEO_INT64);
  ift->root         = array_zeros_like_type(array, GRAFEO_UINT64);
  ift->original     = array;
  return ift;
}

IFT*   ift_apply_array(Array *array, Adjacency adjacency, IFTOptimization optimization_type, WeightFunc weight_function, PathConnectivityFunc path_connectivity){
  // IFT structure
  IFT* ift          = ift_new_from_array(array);

  // Auxiliary structures
  Array* visited    = array_zeros_like(array);
  Queue* queue      = queue_new();

  // Initialize maps
  if(optimization_type == GRAFEO_IFT_MIN){
    array_fill_min(ift->connectivity);
    // update seed connectivity

  }else if(optimization_type == GRAFEO_IFT_MAX){
    array_fill_max(ift->connectivity);
    // update seed connectivity

  }

  // Standard neighbors
  int32_t neighbors_relative[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};

  // Process all nodes
  uint64_t index, neighbor_index; // indices for nodes s and t
  uint8_t  i,num_neighbors;       // for iterating neighbors
  int64_t  connectivity;            // connectivity for extended path <r...s,t>

  if     (adjacency == GRAFEO_NEIGHBOR_4) num_neighbors = 4;
  else if(adjacency == GRAFEO_NEIGHBOR_8) num_neighbors = 8;

  while(!queue_is_empty(queue)){

    // Get node of minimum connectivity value (remove it from queue)
    index = POINTER_TO_UINT64(pqueue_bucket_at(queue,0,0));
    pqueue_remove_begin(queue);

    uint32_t* index_nd = (uint32_t*)array_index(array, (int64_t)index);

    // Do not process this node again
    array_set_element(visited, index_nd, 1);

    // Process all neighbors
    for(i = 0; i < num_neighbors; i++){

      // Calculate neighbor index
      neighbor_index = (int64_t)index + array_index_1D(array,neighbors_relative[i]);

      // Verify if it's valid (inside array region)
      if(array_index_1D_is_valid(visited, neighbor_index)){

        // If it's not visited
        uint32_t* neighbor_index_nd = (uint32_t*) array_index(array,neighbor_index);
        uint8_t*  element           = (uint8_t*)  array_get_element(visited, neighbor_index_nd);
        if(!*element){
          // Calculate connectivity of extended path
          connectivity = path_connectivity(ift, index, neighbor_index, weight_function);

          // Compare with the current connectivity
          int64_t* old_connectivity = (int64_t*)array_get_element(ift->connectivity, neighbor_index_nd);
          if(connectivity < *old_connectivity){

            // remove it from queue if it's a seed
            if((optimization_type == GRAFEO_IFT_MIN && *old_connectivity == 0) ||
               (optimization_type == GRAFEO_IFT_MAX && *old_connectivity == INT64_MAX))
            {
              Bucket* bucket      = (Bucket*)list_value(queue_begin(queue));
              queue_remove(bucket_queue(bucket),INT64_TO_POINTER(neighbor_index));
            }


            // Update predecessors, connectivity, labels and roots
            array_set_element(ift->predecessors, neighbor_index_nd, index);
            array_set_element(ift->connectivity, neighbor_index_nd, connectivity);
            array_set_element(ift->label,        neighbor_index_nd, *(uint8_t*) array_get_element(ift->label, index_nd));
            array_set_element(ift->root,         neighbor_index_nd, *(uint64_t*) array_get_element(ift->root,  index_nd));

            // (re)insert unvisited neighbors
            if(optimization_type == GRAFEO_IFT_MIN)
              pqueue_append_at(queue, UINT8_TO_POINTER(connectivity), neighbor_index_nd, int64_compare_function_r);
            else if(optimization_type == GRAFEO_IFT_MAX)
              pqueue_append_at(queue, UINT8_TO_POINTER(connectivity), neighbor_index_nd, int64_compare_function);
          }
        }
      }
    }
    free(index_nd);
  }

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
  return 0;
}

double path_connectivity_max(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return 0;
}

double path_connectivity_min(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return 0;
}

double path_connectivity_euc(IFT* ift, uint64_t index_s, uint64_t index_t, WeightFunc weight_function){
  return 0;
}

void   ift_free(IFT* ift){
  free(ift);
}
