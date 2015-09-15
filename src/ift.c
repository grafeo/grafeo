#include <grafeo/ift.h>

IFT*   ift_apply_array(Array* array, Adjacency adjacency, double (*weight_function)(Array* array, uint64_t index1, uint64_t index2), double (*path_connectivity)(double connectivity_value, double weight_value)){
  IFT* ift = ift_new();
  ift->label        = array_zeros_like(array);
  ift->predecessors = array_zeros_like(array);
  ift->connectivity = array_zeros_like(array);
  ift->root         = array_zeros_like(array);

  Array* visited    = array_zeros_like(array);

  Queue* queue = queue_new();
  // Insert seeds
  queue_insert();

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

Array* ift_get_label(){

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

}
