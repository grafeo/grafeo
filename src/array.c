#include <grafeo/array.h>
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    array->dim = 0;
    array->size = NULL;
    array->num_elements = 0;
    array->num_bytes = 0;
    array->type = GRAFEO_UINT8;
    return array;
}

Array*    array_new_with_dim(uint16_t dim){
    Array* array        = array_new();
    array->dim          = dim;
    array->size         = malloc(sizeof(uint32_t) * dim);
    return array;
}
Array*    array_new_with_size(uint16_t dim, uint32_t* size){
    return array_new_with_size_type(dim, size, GRAFEO_UINT8);
}

Array*    array_new_with_size_type(uint16_t dim, uint32_t* size, DataType type){
    Array* array        = array_new_with_dim(dim);
    uint16_t i;
    array->num_elements = 1;
    for(i = 0; i < dim; i++){
        array->size[i] = size[i];
        array->num_elements *= size[i];
    }
    size_t bitsize;
    switch(type){
        case GRAFEO_UINT8:  bitsize = sizeof(uint8_t); break;
        case GRAFEO_UINT16: bitsize = sizeof(uint16_t);break;
        case GRAFEO_UINT32: bitsize = sizeof(uint32_t);break;
        case GRAFEO_UINT64: bitsize = sizeof(uint64_t);break;
        case GRAFEO_INT8:   bitsize = sizeof(int8_t);  break;
        case GRAFEO_INT16:  bitsize = sizeof(int16_t); break;
        case GRAFEO_INT32:  bitsize = sizeof(int32_t); break;
        case GRAFEO_INT64:  bitsize = sizeof(int64_t); break;
        case GRAFEO_FLOAT:  bitsize = sizeof(float);   break;
        case GRAFEO_DOUBLE: bitsize = sizeof(double);  break;
    }
    array->num_bytes    = bitsize * array->num_elements;
    array->data         = malloc(array->num_bytes);
    array->type         = type;
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
Array*    array_new_3D(uint32_t size1, uint32_t size2, uint32_t size3){
    uint32_t sizes[3]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    Array* array        = array_new_with_size(3, sizes);
    return array;
}
Array*    array_new_4D(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    Array* array        = array_new_with_size(4, sizes);
    return array;
}

Array*    array_new_1D_type(uint32_t size1, DataType type){
    return array_new_with_size_type(1, &size1, type);
}
Array*    array_new_2D_type(uint32_t size1, uint32_t size2, DataType type){
    uint32_t sizes[2]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    return array_new_with_size_type(2, sizes, type);

}
Array*    array_new_3D_type(uint32_t size1, uint32_t size2, uint32_t size3, DataType type){
    uint32_t sizes[3]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    return array_new_with_size_type(3, sizes, type);
}
Array*    array_new_4D_type(uint32_t size1, uint32_t size2, uint32_t size3, uint32_t size4, DataType type){
    uint32_t sizes[4]; 
    sizes[0]            = size1;
    sizes[1]            = size2;
    sizes[2]            = size3;
    sizes[3]            = size4;
    return array_new_with_size_type(4, sizes, type);
}
Array*    array_zeros(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    memset(array->data, 0, array->num_bytes);
    return array;
}
Array*    array_ones(uint16_t dim, uint32_t* sizes, DataType type){
    Array* array = array_new_with_size_type(dim, sizes, type);
    array_fill(array,1);
    return array;
}
void array_fill(Array* array, double value){
    uint64_t i;
    for(i = 0; i < array->num_elements; i++){
        switch(array->type){
            case GRAFEO_UINT8:  array->data_uint8[i] = (uint8_t)value;break;
            case GRAFEO_UINT16: array->data_uint16[i] = (uint16_t)value;break;
            case GRAFEO_UINT32: array->data_uint32[i] = (uint32_t)value;break;
            case GRAFEO_UINT64: array->data_uint64[i] = (uint64_t)value;break;
            case GRAFEO_INT8:   array->data_int8[i] = (int8_t)value;break;
            case GRAFEO_INT16:  array->data_int16[i] = (int16_t)value;break;
            case GRAFEO_INT32:  array->data_int32[i] = (int32_t)value;break;
            case GRAFEO_INT64:  array->data_int64[i] = (int64_t)value;break;
            case GRAFEO_FLOAT:  array->data_float[i] = (float)value;break;
            case GRAFEO_DOUBLE: array->data_double[i] = (double)value;break;
        }
    }
}

uint64_t array_get_num_elements(Array* array){
    return array->num_elements;
}
DataType  array_get_type(Array* array){
    return array->type;
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

