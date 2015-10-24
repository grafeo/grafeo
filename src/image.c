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
  else if(strcasecmp(ext, "pgm") == 0) return image_read_pgm(filename);
  else if(strcasecmp(ext, "ppm") == 0) return image_read_ppm(filename);
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

static void _image_read_pgm_skip_comments(char* s, size_t m, FILE* fp){
  while(fgets(s,m,fp) != NULL)
    if(s[0]!='#' && s[0]!='\n') break;
}
static Array* _image_read_ppm_pgm(const char* filename){
  char version[4];
  char line[256];
  uint64_t i;
  uint32_t width, height;
  uint16_t dim;
  uint64_t max_gray;
  FILE* fp     = fopen(filename,"rb");
  Array* array = NULL;

  fgets(version, sizeof(version), fp);
  if(version[0] == 'P' && (version[1] == '5' || version[1] == '2' || version[1] == '6')){
    if(version[1] == '6') dim = 3;
    else                  dim = 2;
    _image_read_pgm_skip_comments(line,256,fp);
    sscanf(line, "%d %d\n", &width, &height);
    _image_read_pgm_skip_comments(line,256,fp);
    sscanf(line, "%lu", &max_gray);
    uint32_t size[3] = {height, width,3};
    array = array_new_with_size_type(dim, size, GRAFEO_UINT8);
    if(version[1] == '2')
      for(i = 0; i < array->num_elements; i++)
        fscanf(fp, "%cu", &array->data_uint8[i]);
    else
      fread(array->data_uint8, sizeof(uint8_t), array->num_elements,fp);
  }
  fclose(fp);
  return array;
}

Array* image_read_pgm(const char* filename){
  return _image_read_ppm_pgm(filename);
}
Array* image_read_ppm(const char* filename){
  return _image_read_ppm_pgm(filename);
}

void image_write(Array* array, const char* filename){
  const char* ext = strrchr(filename, '.') + 1;
  if     (strcasecmp(ext, "png") == 0) image_write_png(array,filename);
  else if(strcasecmp(ext, "jpg") == 0) image_write_jpg(array,filename);
  else if(strcasecmp(ext, "pgm") == 0) image_write_pgm(array,filename);
  else if(strcasecmp(ext, "ppm") == 0) image_write_ppm(array,filename);
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

  int color_type = (array->dim > 2 && array->size[2] == 3)?PNG_COLOR_TYPE_RGB:PNG_COLOR_TYPE_GRAY;
  png_set_IHDR(png_ptr, info_ptr, array->size[1], array->size[0],
               array->bitsize << 3, color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing bytes");

  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * array->size[0]);
  uint32_t i;
  for(i = 0; i < array->size[0]; i++) buffer[i] = array->data_uint8 + array->step[0] * i * array->bitsize;
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
  cinfo.input_components = (array->dim > 2)?array->size[2]:1;
  cinfo.in_color_space = (array->dim > 2 && array->size[2] == 3)?JCS_RGB:JCS_GRAYSCALE;
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

static void _image_write_ppm_pgm(Array* array, const char* filename, char format){
  FILE* fp;
  uint32_t i_max = (uint32_t) array_reduce_max_num(array);
  fp = fopen(filename, "wb");
  fprintf(fp, "P%c\n",format);
  fprintf(fp,"%d %d\n", array->size[1], array->size[0]);
  fprintf(fp, "%d\n",i_max);
  fwrite(array->data_uint8,sizeof(uint8_t),array->num_elements,fp);
  fclose(fp);
}

void   image_write_pgm(Array* array, const char* filename){
  _image_write_ppm_pgm(array, filename, 5);
}

void   image_write_ppm(Array* array, const char* filename){
  _image_write_ppm_pgm(array, filename, 6);
}

Array* image_cvt_color(Array* array, ColorType origin, ColorType destiny){
  Array* output;
  uint64_t i;
  output = array;
  if(origin == GRAFEO_GRAY && destiny == GRAFEO_RGB){
    output = array_new_3D_type(array->size[0], array->size[1], 3, array->type);
    for(i = 0; i < array->num_elements; i++){
      output->data_uint8[3*i  ] = array->data_uint8[i];
      output->data_uint8[3*i+1] = array->data_uint8[i];
      output->data_uint8[3*i+2] = array->data_uint8[i];
    }
  }else if(origin == GRAFEO_RGB && destiny == GRAFEO_GRAY){
    output = array_new_2D_type(array->size[0], array->size[1], array->type);
    for(i = 0; i < output->num_elements; i++){
      output->data_uint8[i] = (uint8_t)(
                              0.299*(double)array->data_uint8[3*i  ] +
                              0.587*(double)array->data_uint8[3*i+1] +
                              0.114*(double)array->data_uint8[3*i+2]);
    }
  }else if(origin == GRAFEO_RGB && destiny == GRAFEO_BGR){
    output = array_copy(array);
    uint64_t size_gray = output->num_elements/3;
    uint8_t tmp;
    for(i = 0; i < size_gray; i++){
      tmp                       = output->data_uint8[3*i];
      output->data_uint8[3*i]   = output->data_uint8[3*i+2];
      output->data_uint8[3*i+2] = tmp;
    }
  }
  return output;
}
