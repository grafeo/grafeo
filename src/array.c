#include <grafeo/array.h>
Array*    array_new(){
    Array* array = malloc(sizeof(Array));
    return array;
}

u_int64_t array_get_num_elements(Array* array){
    return 0;
}
DataType  array_get_type(Array* array){
    return GRAFEO_UINT8;
}
u_int16_t array_get_dim(Array* array){
    return 0;
}
void      array_free(Array* array){

}