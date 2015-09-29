#include <grafeo/ift.h>

IFT* ift_new(){
  return malloc(sizeof(IFT));
}

IFT* ift_new_from_array(Array* array){
  IFT* ift = ift_new();
  ift->label        = array_zeros_like(array);
  ift->predecessors = array_zeros_like(array);
  ift->connectivity = array_zeros_like(array);
  ift->root         = array_zeros_like(array);
  return ift;
}

IFT*   ift_apply_array(Array* array, Adjacency adjacency, double (*weight_function)(Array* array, uint64_t index1, uint64_t index2), double (*path_connectivity)(double connectivity_value, double weight_value)){
  // IFT structure
  IFT* ift          = ift_new_from_array(array);

  // Auxiliary structures
  Array* visited    = array_zeros_like(array);
  Queue* queue      = queue_new();

  // Define seeds
  queue_insert();

  //
  neighbors_relative[8][] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};

  uint64_t i;
  int64_t neighbor_index;
  // Process all nodes
  while(!queue_is_empty(queue)){
    // get node with minimum connectivity value
    uint64_t index = queue_push_head(0);

    // set visible (to not process this node again)
    array_set_value(visited, index, 1);

    // Process all neighbors
    for(i = 0; i < neighbors_relative[8];i++){
      neighbor_index = (int64_t)index + array_get_index_1D(array,neighbors_relative[i]);

      // if index inside limits of array and not visited
      if(array_is_in_bound(visited, neighbor_index) && !(uint8_t)array_get_element(visited, neighbor_index)){
        connectivity = path_connectivity(array_get_element(array, index), weight_function(array, index, neighbor_index));

        // If calculated connectivity is better than current one
        if(connectivity < array_get_element(ift->connectivity, neighbor_index)){

          // Update it (remove if from queue if it's a seed)
          if(array_get_element(ift->connectivity, neighbor_index) == INT64_MAX)
            queue_remove(queue,neighbor_index);
          // Update predecessors, connectivity and labels
          array_set_element(ift->predecessors, neighbor_index, index);
          array_set_element(ift->connectivity, neighbor_index, connectivity);
          array_set_element(ift->label, neighbor_index, array_get_element(ift->label, index));
          // (re)insert unvisited neighbors
          queue_insert(queue, neighbor_index);
        }
      }
    }
  }
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



void   path_connectivity_sum(){

}

void   path_connectivity_max(){

}

void   path_connectivity_min(){

}

void   path_connectivity_euc(){

}

void   ift_free(IFT* ift){
  free(ift);
}
