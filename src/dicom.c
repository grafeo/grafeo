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
#include <grafeo/dicom.h>
#include <ctype.h>
#define GRF_DICM_OFFSET        0x80 // 128 bits
#define GRF_DICOM_IMPLICIT_VR 0x2D2D
#define GRF_READ_BITS(variable,num_bits) fread(&variable,sizeof(uint ## num_bits ## _t),1,file)

typedef enum _GrfDicomValueRepresentation{
  GRF_DICOM_VR_AE = 0x4145,
  GRF_DICOM_VR_AS = 0x4153,
  GRF_DICOM_VR_AT = 0x4154,
  GRF_DICOM_VR_CS = 0x4353,
  GRF_DICOM_VR_DA = 0x4441,
  GRF_DICOM_VR_DS = 0x4453,
  GRF_DICOM_VR_DT = 0x4454,
  GRF_DICOM_VR_FD = 0x4644,
  GRF_DICOM_VR_FL = 0x464C,
  GRF_DICOM_VR_IS = 0x4953,
  GRF_DICOM_VR_LO = 0x4C4F,
  GRF_DICOM_VR_LT = 0x4C54,
  GRF_DICOM_VR_PN = 0x504E,
  GRF_DICOM_VR_SH = 0x5348,
  GRF_DICOM_VR_SL = 0x534C,
  GRF_DICOM_VR_SS = 0x5353,
  GRF_DICOM_VR_ST = 0x5354,
  GRF_DICOM_VR_TM = 0x544D,
  GRF_DICOM_VR_UI = 0x5549,
  GRF_DICOM_VR_UL = 0x554C,
  GRF_DICOM_VR_US = 0x5553,
  GRF_DICOM_VR_UT = 0x5554,
  GRF_DICOM_VR_OB = 0x4F42,
  GRF_DICOM_VR_OW = 0x4F57,
  GRF_DICOM_VR_SQ = 0x5351,
  GRF_DICOM_VR_UN = 0x554E,
  GRF_DICOM_VR_QQ = 0x3F3F,
  GRF_DICOM_VR_RT = 0x5254,
} GrfDicomValueRepresentation;

typedef enum _GrfDicomTag{
  GRF_DICOM_TAG_PIXEL_REPRESENTATION       = 0x00280103,
  GRF_DICOM_TAG_TRANSFER_SYNTAX_UID        = 0x00020010,
  GRF_DICOM_TAG_MODALITY                   = 0x00080060,
  GRF_DICOM_TAG_SLICE_THICKNESS            = 0x00180050,
  GRF_DICOM_TAG_SLICE_SPACING              = 0x00180088,
  GRF_DICOM_TAG_SAMPLES_PER_PIXEL          = 0x00280002,
  GRF_DICOM_TAG_PHOTOMETRIC_INTERPRETATION = 0x00280004,
  GRF_DICOM_TAG_PLANAR_CONFIGURATION       = 0x00280006,
  GRF_DICOM_TAG_NUMBER_OF_FRAMES           = 0x00280008,
  GRF_DICOM_TAG_ROWS                       = 0x00280010,
  GRF_DICOM_TAG_COLUMNS                    = 0x00280011,
  GRF_DICOM_TAG_PIXEL_SPACING              = 0x00280030,
  GRF_DICOM_TAG_BITS_ALLOCATED             = 0x00280100,
  GRF_DICOM_TAG_WINDOW_CENTER              = 0x00281050,
  GRF_DICOM_TAG_WINDOW_WIDTH               = 0x00281051,
  GRF_DICOM_TAG_RESCALE_INTERCEPT          = 0x00281052,
  GRF_DICOM_TAG_RESCALE_SLOPE              = 0x00281053,
  GRF_DICOM_TAG_RED_PALETTE                = 0x00281201,
  GRF_DICOM_TAG_GREEN_PALETTE              = 0x00281202,
  GRF_DICOM_TAG_BLUE_PALETTE               = 0x00281203,
  GRF_DICOM_TAG_ICON_IMAGE_SEQUENCE        = 0x00880200,
  GRF_DICOM_TAG_PIXEL_DATA                 = 0x7FE00010,
} GrfDicomTag;

/*=================================
 * PRIVATE API
 *=================================*/


static void grf_dicom_get_bytes(size_t num_bytes, void* variable, size_t* location, FILE* file){
  fread(variable,1,num_bytes,file);
  (*location) += num_bytes;
}

static void grf_dicom_get_uint16(uint16_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[2];
  grf_dicom_get_bytes(2,b,location,file);
  uint16_t b1 = (uint16_t)b[1];
  uint16_t b0 = b[0];
  if(little_endian)
    *variable =  b1 << 8 | b0;
  else
    *variable =  b0 << 8 | b1;
}

static void grf_dicom_get_uint32(uint32_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  char b[4];
  grf_dicom_get_bytes(4,b,location,file);
  if(little_endian)
    *variable = (uint32_t) (b[3] << 24)| (uint32_t) (b[2] << 16) | (uint32_t) (b[1] << 8) | b[0];
  else
    *variable = (uint32_t) (b[0] << 24)| (uint32_t) (b[1] << 16) | (uint32_t) (b[2] << 8) | b[3];
}

static void grf_dicom_get_double(double* variable, size_t* location, FILE* file, uint8_t little_endian){
  char b[8];
  grf_dicom_get_bytes(8,b,location,file);
  if(little_endian)
    *variable = (double)(((uint64_t)b[7] << 56)|
                         ((uint64_t)b[6] << 48)|
                         ((uint64_t)b[5] << 40)|
                         ((uint64_t)b[4] << 32)|
                         ((uint64_t)b[3] << 24)|
                         ((uint64_t)b[2] << 16)|
                         ((uint64_t)b[1] << 8)|
                         ((uint64_t)b[0]));
  else
    *variable = (double)(((uint64_t)b[0] << 56)|
                         ((uint64_t)b[1] << 48)|
                         ((uint64_t)b[2] << 40)|
                         ((uint64_t)b[3] << 32)|
                         ((uint64_t)b[4] << 24)|
                         ((uint64_t)b[5] << 16)|
                         ((uint64_t)b[6] << 8)|
                         ((uint64_t)b[7]));
}
static void grf_dicom_get_float(float* variable, size_t* location, FILE* file, uint8_t little_endian){
  char b[4];
  grf_dicom_get_bytes(4,b,location,file);
  if(little_endian)
    *variable = (float) (((uint32_t)b[3] << 24)|
                         ((uint32_t)b[2] << 16)|
                         ((uint32_t)b[1] << 8)|
                         ((uint32_t)b[0]));
  else
    *variable = (float)(((uint32_t)b[0]  << 24)|
                         ((uint32_t)b[1] << 16)|
                         ((uint32_t)b[2] << 8)|
                         ((uint32_t)b[3]));
}


static void grf_dicom_get_length(uint32_t *variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t  b[4];
  uint32_t b3,b2;
  uint16_t vr;
  grf_dicom_get_bytes(4,b,location,file);
  vr = ((uint16_t)b[0] << 8) | b[1];

  switch(vr){
    case GRF_DICOM_VR_OB:
    case GRF_DICOM_VR_OW:
    case GRF_DICOM_VR_SQ:
    case GRF_DICOM_VR_UN:
    case GRF_DICOM_VR_UT:
      if((b[2] == 0) || (b[3] == 0)){
        grf_dicom_get_uint32(variable, location, file,little_endian);
      }else{
        vr = GRF_DICOM_IMPLICIT_VR;
        if(little_endian)
          *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
        else
          *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
      }
    break;
    case GRF_DICOM_VR_AE:
    case GRF_DICOM_VR_AS:
    case GRF_DICOM_VR_AT:
    case GRF_DICOM_VR_CS:
    case GRF_DICOM_VR_DA:
    case GRF_DICOM_VR_DS:
    case GRF_DICOM_VR_DT:
    case GRF_DICOM_VR_FD:
    case GRF_DICOM_VR_FL:
    case GRF_DICOM_VR_IS:
    case GRF_DICOM_VR_LO:
    case GRF_DICOM_VR_LT:
    case GRF_DICOM_VR_PN:
    case GRF_DICOM_VR_SH:
    case GRF_DICOM_VR_SL:
    case GRF_DICOM_VR_SS:
    case GRF_DICOM_VR_ST:
    case GRF_DICOM_VR_TM:
    case GRF_DICOM_VR_UI:
    case GRF_DICOM_VR_UL:
    case GRF_DICOM_VR_US:
    case GRF_DICOM_VR_QQ:
    case GRF_DICOM_VR_RT:
      b3 = (uint32_t)b[3];
      b2 = (uint32_t)b[2];
      if(little_endian)
        *variable = (b3 << 8) | b2;
      else
        *variable = (b2 << 8) | b3;
    break;
    default:
      vr = GRF_DICOM_IMPLICIT_VR;
      if(little_endian)
        *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
      else
        *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    break;
  }
}

char* trim(const char* str) {
    const char* beg = str;
    const char* end = beg + strlen(beg) - 1;

    while (isspace(*beg)) ++beg;
    while (end >= beg && isspace(*end)) --end;

    return strndup(beg, end - beg + 1);
}

char* trim_in_place(char* str) {
    char* beg = str;
    char* end = beg + strlen(beg) - 1;

    while (isspace(*beg)) ++beg;
    while (end >= beg && isspace(*end)) --end;
    end[1] = 0;

    return memmove(str, beg, end - beg + 2);
}

void grf_dicom_read_pixels(GrfDicom* dicom, size_t offset, FILE* file){
  fseek(file,offset,SEEK_SET);
  fread(dicom->image->data_uint8,1,dicom->image->num_bytes,file);
  uint64_t i;
  // Monochromatic
  uint32_t pixel32;
  uint16_t unsigned_s;
  uint8_t  signed_image = 0;
  uint64_t num_elements;
  uint64_t i2;
  uint8_t  b0, b1;
  int16_t  signed_data;
  GrfDataType oldtype;
  if(dicom->samples_per_pixel == 1 && dicom->bits_allocated == 8){
    for(i = 0; i < dicom->image->num_elements; i++){
      pixel32 = (uint32_t)dicom->image->data_uint8[i] * dicom->slope + dicom->intercept;
      if(strstr(dicom->photo_interpretation,"MONOCHROME1"))
        pixel32 = 255 - pixel32;
      dicom->image->data_uint8[i] = (uint8_t)pixel32;
    }
  }
  if(dicom->samples_per_pixel == 1 && dicom->bits_allocated == 16){
    num_elements = dicom->image->size[0] * dicom->image->size[1];
    for(i = 0; i < num_elements; i++){
      if(!dicom->pixel_representation){
        i2 = i << 1;
        b0 = dicom->image->data_uint8[i2];
        b1 = dicom->image->data_uint8[i2+1];
        unsigned_s = ((uint16_t)b1 << 8) | b0;
        pixel32 = (uint32_t) unsigned_s * dicom->slope + dicom->intercept;
        if(strstr(dicom->photo_interpretation,"MONOCHROME1"))
          pixel32 = 0xFFFF - pixel32;
      }else{
        signed_data = dicom->image->data_int16[i];
//        if(signed_data)
//          printf("oi\n");
        pixel32 = (uint32_t) signed_data * dicom->slope + dicom->intercept;
        if(strstr(dicom->photo_interpretation,"MONOCHROME1"))
          pixel32 = 0xFFFF - pixel32;
      }
      dicom->image->data_int16[i] = (int16_t) pixel32;
    }
    oldtype = dicom->image->type;
    dicom->image->type = GRF_INT16;
    long double min_dicom = grf_array_reduce_min_num(dicom->image);
    long double max_dicom = grf_array_reduce_max_num(dicom->image);
    dicom->image->type = oldtype;
    if(min_dicom < 0) signed_image = 1;
    for(i = 0; i < dicom->image->num_elements; i++){
      if(signed_image)
        dicom->image->data_uint16[i] = (uint16_t)dicom->image->data_int16[i] - INT16_MIN;
      else
        dicom->image->data_uint16[i] = (uint16_t)dicom->image->data_int16[i];
    }
  }
}

char* grf_dicom_get_lut(int length, size_t* location, FILE* file){
  return NULL;
}



/*=================================
 * PUBLIC API
 *=================================*/

GrfDicom* grf_dicom_new(){
  return (GrfDicom*)malloc(sizeof(GrfDicom));
}



GrfDicom* grf_dicom_read(const char* filename){
  char     dicm_id[5];
  uint8_t  decoding_tags = 1; // 0 if finished decoding
  uint8_t  odd_locations;
  size_t   location = 0;
  uint16_t group_word;
  uint8_t  big_endian_transfer_syntax = 0;
  uint8_t  little_endian = 1;
  uint16_t element_word;
  uint32_t n_images;
  uint16_t planar_configuration;
  double   xscale, yscale;
  uint8_t  in_sequence;
  uint16_t width, height;
  char*    scale=NULL, *scale_index=NULL;
  double   pixel_width, pixel_height, pixel_depth;
  char*    spacing = NULL;
  char*    center, *center_index;
  float    window_center;
  char*    window_width_str = NULL, *window_width_index = NULL;
  float    window_width;
  char*    intercept_str = NULL;
  char*    slope_str = NULL;
  size_t   offset;
  char*    s = NULL;
  uint32_t tag;
  uint32_t element_length;

  FILE*    file = fopen(filename, "rb");

  if(file){
    GrfDicom* dicom = grf_dicom_new();

    dicom->filename = filename;
    dicom->slope = 1;
    dicom->intercept = 0;
    dicom->unit = "mm";

    // Read 4 bytes DICM
    fseek(file,GRF_DICM_OFFSET,SEEK_SET);
    dicom->bits_allocated = 16;
    location += GRF_DICM_OFFSET;
    grf_dicom_get_bytes(4,dicm_id,&location,file);
    dicm_id[4] = 0;
    if(strcmp(dicm_id,"DICM")==0){
      while(decoding_tags){
        //  Get Next tag
        grf_dicom_get_uint16(&group_word,&location,file,little_endian);
        if(group_word == 0x0800 && big_endian_transfer_syntax)
        {
          little_endian = 0;
          group_word = 0x0008;
        }
        grf_dicom_get_uint16(&element_word,&location,file,little_endian);
        tag = (group_word << 16) | element_word;
//        if(tag == GRF_DICOM_TAG_TRANSFER_SYNTAX_UID){
//          printf("oi\n");
//        }
        grf_dicom_get_length(&element_length,&location,file,little_endian);

        // Hack to read some GE files
        if(element_length == 13 && !odd_locations)
          element_length = 10;

        // "Undefined" element length
        // This is a sort of bracket that encloses a sequence of
        // elements
        if(element_length == -1)
        {
          element_length = 0;
          in_sequence    = 1;
        }


        if(location & 1) odd_locations = 1;
        if(in_sequence){
          //grf_dicom_add_info(tag, NULL);
          continue;
        }


        switch(tag){
          case GRF_DICOM_TAG_TRANSFER_SYNTAX_UID:
            s = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,s,&location,file);
            //grf_dicom_add_info(tag, s);
            if(strstr(s,"1.2.4") || strstr(s,"1.2.5")){
              fclose(file);
              return NULL;
            }
            if(strstr(s,"1.2.840.10008.1.2.2"))
              big_endian_transfer_syntax = 1;
            free(s);
          break;
          case GRF_DICOM_TAG_MODALITY:
            dicom->modality = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,dicom->modality,&location,file);
          break;
          case GRF_DICOM_TAG_NUMBER_OF_FRAMES:
            s = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,s,&location,file);
            double frames = strtof(s,NULL);
            if(frames > 1.0)
              n_images = (uint32_t) frames;
            free(s);
          break;
          case GRF_DICOM_TAG_SAMPLES_PER_PIXEL:
            grf_dicom_get_uint16(&dicom->samples_per_pixel,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_PHOTOMETRIC_INTERPRETATION:
            dicom->photo_interpretation = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,dicom->photo_interpretation,&location,file);
            dicom->photo_interpretation = trim_in_place(dicom->photo_interpretation);
          break;
          case GRF_DICOM_TAG_PLANAR_CONFIGURATION:
            grf_dicom_get_uint16(&planar_configuration,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_ROWS:
            grf_dicom_get_uint16(&height,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_COLUMNS:
            grf_dicom_get_uint16(&width,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_PIXEL_SPACING:
            scale = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,scale,&location,file);
            xscale = yscale = 0;
            scale_index = strstr(scale,"\\");
            if(scale_index - scale == 1){
              yscale = strtof(scale,NULL);
              xscale = strtof(scale_index+1,NULL);
            }
            if(xscale != 0.0 && yscale != 0.0){
              pixel_width = xscale;
              pixel_height = yscale;
              dicom->unit = "mm";
            }
            free(scale);
          break;
          case GRF_DICOM_TAG_SLICE_THICKNESS:
          case GRF_DICOM_TAG_SLICE_SPACING:
            spacing = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,spacing,&location,file);
            pixel_depth = strtof(spacing,NULL);
            free(spacing);
          break;
          case GRF_DICOM_TAG_BITS_ALLOCATED:
            grf_dicom_get_uint16(&dicom->bits_allocated,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_PIXEL_REPRESENTATION:
            grf_dicom_get_uint16(&dicom->pixel_representation,&location,file,little_endian);
          break;
          case GRF_DICOM_TAG_WINDOW_CENTER:
            center = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,center,&location,file);
            center_index = strstr(center,"\\");
            if(center_index) center = center_index;
            window_center = strtof(center,NULL);
            free(center);
          break;
          case GRF_DICOM_TAG_WINDOW_WIDTH:
            window_width_str = malloc(element_length+1);
            grf_dicom_get_bytes(element_length,window_width_str,&location,file);
            window_width_index = strstr(window_width_str,"\\");
            if(window_width_index) window_width_str = window_width_index;
            window_width = strtof(window_width_str,NULL);
            free(window_width_str);
          break;
          case GRF_DICOM_TAG_RESCALE_INTERCEPT:
            intercept_str = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,intercept_str,&location,file);
            dicom->intercept = strtof(intercept_str,NULL);
            free(intercept_str);
          break;
          case GRF_DICOM_TAG_RESCALE_SLOPE:
            slope_str = malloc(element_length + 1);
            grf_dicom_get_bytes(element_length,slope_str,&location,file);
            dicom->slope = strtof(slope_str,NULL);
            free(slope_str);
          break;
          case GRF_DICOM_TAG_RED_PALETTE:
            dicom->reds = grf_dicom_get_lut(element_length, &location, file);
          break;
          case GRF_DICOM_TAG_GREEN_PALETTE:
            dicom->greens = grf_dicom_get_lut(element_length, &location, file);
          break;
          case GRF_DICOM_TAG_BLUE_PALETTE:
            dicom->blues = grf_dicom_get_lut(element_length, &location, file);
          break;
          case GRF_DICOM_TAG_PIXEL_DATA:
            if(element_length){
              offset = location;
              decoding_tags = 0;
            }
            dicom->pixel_data_tag_found = 1;
            uint32_t size[3] = {height,width,dicom->samples_per_pixel};
            if(dicom->bits_allocated == 16)
              dicom->image = grf_array_new_with_size_type(3,size,GRF_UINT16);
            else
              dicom->image = grf_array_new_with_size_type(3,size,GRF_UINT8);

          break;
        default:
          fseek(file,element_length,SEEK_CUR);
          location+=element_length;
        break;
        }
      }
      grf_dicom_read_pixels(dicom, offset, file);
      return dicom;
    }else{
      return NULL;
    }
  }
  return NULL;
}

void grf_dicom_free(GrfDicom* dicom){
  if(dicom->image)
    free(dicom->image);
  free(dicom);
}
