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
#include <grafeo/image.h>

struct my_error_mgr{
  struct jpeg_error_mgr pub;
  Error* error;
  jmp_buf               setjmp_buffer;
};
typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo){
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  myerr->error = error_new_with_msg(GRAFEO_ERROR, "Error");
  longjmp(myerr->setjmp_buffer, 1);
}

Array* image_read(const char* filename, Error **error){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, 'png') == 0) return image_read_png(filename, error);
  else if(strcasecmp(ext, 'jpg') == 0) return image_read_jpg(filename, error);
}
Array* image_read_png(const char* filename, Error **error){
  png_structp png_ptr;
  png_infop info_ptr;
  char header[8];
  FILE* infile = fopen(filename, "rb");
  if(!infile){
    *error = error_new_with_msg(GRAFEO_ERROR_FILE_OPEN, "Can't open image");
    return NULL;
  }
  fread(header, 1, 8, infile);
  if(png_sig_cmp(header, 0, 8)){
    *error = error_new_with_msg(GRAFEO_ERROR_PNG_FORMAT, "File is not recognized as a PNG file");
    return NULL;
  }
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr){
    *error = error_new_with_msg(GRAFEO_ERROR_PNG_READ_STRUCT, "png_create_read_struct failed");
    return NULL;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    *error = error_new_with_msg(GRAFEO_ERROR_PNG_INFO_STRUCT, "png_create-info_struct failed");
    return NULL;
  }
  if(setjmp(png_jmpbuf(png_ptr))){
    *error = error_new_with_msg(GRAFEO_ERROR_PNG_INIT_IO, "Error during init_io");
    return NULL;
  }
  png_init_io(png_ptr, infile);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  uint32_t* size = malloc(sizeof(uint32_t) * 3);
  size[0] = png_get_image_height(png_ptr, info_ptr);
  size[1] = png_get_image_width(png_ptr, info_ptr);
  size[2] = png_get_channels(png_ptr, info_ptr);
  uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  DataType type;
  switch (bit_depth) {
  case 16:
    type = GRAFEO_UINT16; break;
  default:
    type = GRAFEO_UINT8;  break;
  }
  Array* array = array_new_with_size_type(3, size, type);
  png_read_update_info(png_ptr, info_ptr);

  // Read file
  if(setjmp(png_jmpbuf(png_ptr))){
    *error = error_new_with_msg(GRAFEO_ERROR_PNG_READ_IMAGE, "Error during reading image data");
    return NULL;
  }
  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  size_t row_stride = png_get_rowbytes(png_ptr, info_ptr);
  for(i = 0; i < cinfo.output_height; i++) buffer[i] = array->data_uint8 + row_stride * i;
  png_read_image(png_ptr, buffer);
  fclose(infile);
  free(buffer);
  return array;
}
Array* image_read_jpg(const char* filename, Error **error){
  struct    jpeg_decompress_struct cinfo;
  struct    my_error_mgr jerr;
  FILE*     infile;
  uint8_t** buffer; // 2D Pointers to rows
  int       row_stride;
  if((infile = fopen(filename, "rb")) == NULL){
    *error = error_new_with_msg(GRAFEO_ERROR_FILE_OPEN, "Can't open image");
    return NULL;
  }
  cinfo.err           = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if(setjmp(jerr.setjmp_buffer)){
    *error = jerr.error;
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  uint32_t size[3] = {cinfo.output_height, cinfo.output_width, cinfo.output_components};
  Array* array     = array_new_with_size_type(3, size, GRAFEO_UINT8);
  row_stride       = size[1]*size[2];
  buffer           = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  for(i = 0; i < size[0]; i++) buffer[i] = array->data_uint8 + row_stride * i;
  jpeg_read_scanlines(&cinfo, buffer, size[0]);
  jpeg_finish_decompress(&cinfo);
  fclose(infile);
  free(buffer);
  return array;
}
void image_write(Array* array, const char* filename, Error **error){

}
void image_write_png(Array* array, const char* filename, Error **error){

}


void image_write_jpg(Array* array, const char* filename, Error **error){

}
void image_cvt_color(Array* array, ColorType origin, ColorType destiny){

}
