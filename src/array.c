#include <grafeo/array.h>
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    return array;
}

Array*    array_new_1D(uint32_t size1){
    Array* array        = array_new();
    array->dim          = 1;
    array->size         = malloc(sizeof(uint32_t));
    array->size[0]      = size1;
    array->num_elements = size1;
    array->data         = malloc(sizeof(uint8_t) * array->num_elements);
    return array;
}

uint64_t array_get_num_elements(Array* array){
    return array->num_elements;
}
DataType  array_get_type(Array* array){
    return GRAFEO_UINT8;
}
uint16_t array_get_dim(Array* array){
    return array->dim;
}
uint32_t*array_get_size(Array* array){
    return array->size;
}
void* array_get_data(Array* array){
    return array->data;
}
void      array_free(Array* array){
    if(array->data) free(array->data);
    if(array->size) free(array->size);
    free(array);
}

