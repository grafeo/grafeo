#include <grafeo/array.h>
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    return array;
}

Array*    array_new_with_dim(uint16_t dim){
    Array* array        = array_new();
    array->dim          = dim;
    array->size         = malloc(sizeof(uint32_t) * dim);
    return array;
}
Array*    array_new_with_size(uint16_t dim, uint32_t* size){
    Array* array        = array_new_with_dim(dim);
    uint16_t i;
    array->num_elements = 1;
    for(i = 0; i < dim; i++){
        array->size[i] = size[i];
        array->num_elements *= size[i];
    }
    array->data         = malloc(sizeof(uint8_t) * array->num_elements);
    return array;
}

Array*    array_new_1D(uint32_t size1){
    Array* array        = array_new_with_size(1, &size1);
    return array;
}
Array*    array_new_2D(uint32_t size1, uint32_t size2){
    uint32_t sizes[2]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    Array* array        = array_new_with_size(2, sizes);
    return array;
}
Array*    array_new_3D(u_int32_t size1, u_int32_t size2, u_int32_t size3){
    uint32_t sizes[3]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    Array* array        = array_new_with_size(3, sizes);
    return array;
}
Array*    array_new_4D(u_int32_t size1, u_int32_t size2, u_int32_t size3, u_int32_t size4){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    Array* array        = array_new_with_size(4, sizes);
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

