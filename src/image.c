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

static void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}

struct my_error_mgr{
  struct jpeg_error_mgr pub;
  jmp_buf               setjmp_buffer;
};
typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo){
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  longjmp(myerr->setjmp_buffer, 1);
}

Array* image_read(const char* filename){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, "png") == 0) return image_read_png(filename);
  else if(strcasecmp(ext, "jpg") == 0) return image_read_jpg(filename);
  return NULL;
}
Array* image_read_png(const char* filename){
  png_structp png_ptr;
  png_infop info_ptr;
  unsigned char header[8];
  FILE* infile = fopen(filename, "rb");
  if(!infile)
    abort_("[read_png_file] File %s could not be opened for reading", filename);

  fread(header, 1, 8, infile);
  if(png_sig_cmp(header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file", filename);

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if(setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");
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
  if(setjmp(png_jmpbuf(png_ptr))) abort_("[read_png_file] Error during read_image");
  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  size_t row_stride = png_get_rowbytes(png_ptr, info_ptr);
  uint32_t i;
  for(i = 0; i < size[0]; i++) buffer[i] = array->data_uint8 + row_stride * i;
  png_read_image(png_ptr, buffer);
  fclose(infile);
  free(buffer);
  return array;
}
Array* image_read_jpg(const char* filename){
  struct    jpeg_decompress_struct cinfo;
  struct    my_error_mgr jerr;
  FILE*     infile;
  uint8_t** buffer; // 2D Pointers to rows
  int       row_stride;
  if((infile = fopen(filename, "rb")) == NULL){
    abort_("[read_png_file] File %s could not be opened for reading", filename);
  }
  cinfo.err           = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if(setjmp(jerr.setjmp_buffer)){
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
  uint32_t i;
  for(i = 0; i < size[0]; i++) buffer[i] = array->data_uint8 + row_stride * i;
  i = 0;
  while(cinfo.output_scanline < cinfo.output_height){
    jpeg_read_scanlines(&cinfo, &buffer[i++], 1);
  }
  jpeg_finish_decompress(&cinfo);
  fclose(infile);
  free(buffer);
  return array;
}
void image_write(Array* array, const char* filename){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, "png") == 0) image_write_png(array,filename);
  else if(strcasecmp(ext, "jpg") == 0) image_write_jpg(array,filename);
}
void image_write_png(Array* array, const char* filename){
  /* create file */
  FILE *outfile = fopen(filename, "wb");
  if (!outfile)
    abort_("[write_png_file] File %s could not be opened for writing", filename);
  /* initialize stuff */
  png_structp png_ptr;
  png_infop info_ptr;
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    abort_("[write_png_file] png_create_write_struct failed");
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[write_png_file] png_create_info_struct failed");
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during init_io");
  png_init_io(png_ptr, outfile);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing header");

  int color_type = (array->size[2] == 3)?PNG_COLOR_TYPE_RGB:PNG_COLOR_TYPE_GRAY;
  png_set_IHDR(png_ptr, info_ptr, array->size[1], array->size[0],
               array->bitsize << 3, color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing bytes");

  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * array->size[0]);
  uint32_t i;
  for(i = 0; i < array->size[0]; i++) buffer[i] = array->data_uint8 + array->step[0] * i;
  png_write_image(png_ptr, buffer);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  free(buffer);
  fclose(outfile);
}
void image_write_jpg(Array* array, const char* filename){
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE* outfile;
  uint8_t* row_pointer[1];
  row_pointer[0] = array->data_uint8;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  if((outfile = fopen(filename, "wb"))==NULL){
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);
  cinfo.image_width = array->size[1];
  cinfo.image_height = array->size[0];
  cinfo.input_components = array->size[2];
  cinfo.in_color_space = (array->size[2] == 3)?JCS_RGB:JCS_GRAYSCALE;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 1, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  while(cinfo.next_scanline < array->size[0]){
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
    row_pointer[0] += array->step[0];
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}
void image_cvt_color(Array* array, ColorType origin, ColorType destiny){
}
