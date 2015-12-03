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
#include <grafeo/medical.h>
#include <sys/types.h>
#include <sys/stat.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
/*****************************************************************************/
/*------------------ NIfTI version of ANALYZE 7.5 structure -----------------*/

/* (based on fsliolib/dbh.h, but updated for version 7.5) */

typedef struct {
   /* header info fields - describes the header    overlap with NIfTI */
   /*                                              ------------------ */
   int sizeof_hdr;                  /* 0 + 4        same              */
   char data_type[10];              /* 4 + 10       same              */
   char db_name[18];                /* 14 + 18      same              */
   int extents;                     /* 32 + 4       same              */
   short int session_error;         /* 36 + 2       same              */
   char regular;                    /* 38 + 1       same              */
   char hkey_un0;                   /* 39 + 1                40 bytes */

   /* image dimension fields - describes image sizes */
   short int dim[8];                /* 0 + 16       same              */
   short int unused8;               /* 16 + 2       intent_p1...      */
   short int unused9;               /* 18 + 2         ...             */
   short int unused10;              /* 20 + 2       intent_p2...      */
   short int unused11;              /* 22 + 2         ...             */
   short int unused12;              /* 24 + 2       intent_p3...      */
   short int unused13;              /* 26 + 2         ...             */
   short int unused14;              /* 28 + 2       intent_code       */
   short int datatype;              /* 30 + 2       same              */
   short int bitpix;                /* 32 + 2       same              */
   short int dim_un0;               /* 34 + 2       slice_start       */
   float pixdim[8];                 /* 36 + 32      same              */

   float vox_offset;                /* 68 + 4       same              */
   float funused1;                  /* 72 + 4       scl_slope         */
   float funused2;                  /* 76 + 4       scl_inter         */
   float funused3;                  /* 80 + 4       slice_end,        */
                                                 /* slice_code,       */
                                                 /* xyzt_units        */
   float cal_max;                   /* 84 + 4       same              */
   float cal_min;                   /* 88 + 4       same              */
   float compressed;                /* 92 + 4       slice_duration    */
   float verified;                  /* 96 + 4       toffset           */
   int glmax,glmin;                 /* 100 + 8              108 bytes */

   /* data history fields - optional */
   char descrip[80];                /* 0 + 80       same              */
   char aux_file[24];               /* 80 + 24      same              */
   char orient;                     /* 104 + 1      NO GOOD OVERLAP   */
   char originator[10];             /* 105 + 10     FROM HERE DOWN... */
   char generated[10];              /* 115 + 10                       */
   char scannum[10];                /* 125 + 10                       */
   char patient_id[10];             /* 135 + 10                       */
   char exp_date[10];               /* 145 + 10                       */
   char exp_time[10];               /* 155 + 10                       */
   char hist_un0[3];                /* 165 + 3                        */
   int views;                       /* 168 + 4                        */
   int vols_added;                  /* 172 + 4                        */
   int start_field;                 /* 176 + 4                        */
   int field_skip;                  /* 180 + 4                        */
   int omax, omin;                  /* 184 + 8                        */
   int smax, smin;                  /* 192 + 8              200 bytes */
} GrfNiftiAnalyze75;                                   /* total:  348 bytes */
typedef struct _GrfNiftiImagePrivate{
  int     ndim ;                  /*!< last dimension greater than 1 (1..7) */
  GrfVec7 nsize;                 /*!< array of sizes for each dimension    */
  int     dim[8] ;                /*!< dim[0]=ndim, dim[1]=nx, etc.         */
  size_t  nvox ;                  /*!< number of voxels = nx*ny*nz*...*nw   */
  int     nbyper ;                /*!< bytes per voxel, matches datatype    */
  int     datatype ;              /*!< type of data in voxels: DT_* code    */

  GrfVec7 dsize;                 /*!< array of grid spacings for each dim  */
  float   pixdim[8] ;             /*!< pixdim[1]=dx, etc. */

  float   scl_slope ;             /*!< scaling parameter - slope        */
  float   scl_inter ;             /*!< scaling parameter - intercept    */

  float   cal_min ;               /*!< calibration parameter, minimum   */
  float   cal_max ;               /*!< calibration parameter, maximum   */

  int     qform_code ;            /*!< codes for (x,y,z) space meaning  */
  int     sform_code ;            /*!< codes for (x,y,z) space meaning  */

  int     freq_dim  ;             /*!< indexes (1,2,3, or 0) for MRI    */
  int     phase_dim ;             /*!< directions in dim[]/pixdim[]     */
  int     slice_dim ;             /*!< directions in dim[]/pixdim[]     */

  int     slice_code  ;           /*!< code for slice timing pattern    */
  int     slice_start ;           /*!< index for start of slices        */
  int     slice_end   ;           /*!< index for end of slices          */
  float   slice_duration ;        /*!< time between individual slices   */

  /*! quaternion transform parameters
    [when writing a dataset, these are used for qform, NOT qto_xyz]   */
  float   quatern_b , quatern_c , quatern_d ,
          qoffset_x , qoffset_y , qoffset_z ,
          qfac      ;

  GrfMat4 qto_xyz ;               /*!< qform: transform (i,j,k) to (x,y,z) */
  GrfMat4 qto_ijk ;               /*!< qform: transform (x,y,z) to (i,j,k) */

  GrfMat4 sto_xyz ;               /*!< sform: transform (i,j,k) to (x,y,z) */
  GrfMat4 sto_ijk ;               /*!< sform: transform (x,y,z) to (i,j,k) */

  float   toffset ;               /*!< time coordinate offset */

  int     xyz_units  ;            /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
  int     time_units ;            /*!< dt       units: NIFTI_UNITS_* code  */

  int     nifti_type ;            /*!< 0==ANALYZE, 1==NIFTI-1 (1 file),
                                                 2==NIFTI-1 (2 files),
                                                 3==NIFTI-ASCII (1 file) */
  int     intent_code ;           /*!< statistic type (or something)       */
  float   intent_p1 ;             /*!< intent parameters                   */
  float   intent_p2 ;             /*!< intent parameters                   */
  float   intent_p3 ;             /*!< intent parameters                   */
  char    intent_name[16] ;       /*!< optional description of intent data */

  char    descrip[80]  ;          /*!< optional text to describe dataset   */
  char    aux_file[24] ;          /*!< auxiliary filename                  */

  char *  fname ;                 /*!< header filename (.hdr or .nii)         */
  char *  iname ;                 /*!< image filename  (.img or .nii)         */
  int     iname_offset ;          /*!< offset into iname where data starts    */
  int     swapsize ;              /*!< swap unit in image data (might be 0)   */
  int     byteorder ;             /*!< byte order on disk (MSB_ or LSB_FIRST) */
  //void * data ;                  /*!< pointer to data: nbyper*nvox bytes     */

  int                    num_ext ;  /*!< number of extensions in ext_list       */
  GrfNifti1Extension   * ext_list ; /*!< array of extension structs (with data) */
  GrfAnalyze75OrientCode analyze75_orient; /*!< for old analyze files, orient */
}GrfNiftiImagePrivate;
G_DEFINE_TYPE_WITH_PRIVATE(GrfNiftiImage, grf_nifti_image, GRF_TYPE_NDARRAY)
static void
grf_nifti_image_init(GrfNiftiImage *self){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(self);
  priv->fname = NULL;
  priv->iname = NULL;
}
static void
grf_nifti_image_class_init(GrfNiftiImageClass *klass){

}
/**
 * @brief Creates a new GrfNiftiImage object
 * @return
 */
static GrfNiftiImage*
grf_nifti_image_new(){
  return g_object_new(GRF_TYPE_NIFTI_IMAGE, NULL);
}
/**
 * @brief grf_nifti_image_set_nsize
 * @param image
 * @param nsize
 */
void
grf_nifti_image_set_nsize(GrfNiftiImage* image, GrfVec7 nsize){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize = nsize;
}
/**
 * @brief grf_nifti_image_set_dsize
 * @param image
 * @param dsize
 */
void
grf_nifti_image_set_dsize(GrfNiftiImage* image, GrfVec7 dsize){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->dsize = dsize;
}
/**
 * @brief grf_nifti_image_set_qfac
 * @param image
 * @param qfac
 */
void
grf_nifti_image_set_qfac(GrfNiftiImage* image, float qfac){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->qfac = qfac;
}
/**
 * @brief grf_nifti_image_set_byteorder
 * @param image
 * @param byteorder
 */
void
grf_nifti_image_set_byteorder(GrfNiftiImage* image, int byteorder){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->byteorder = byteorder;
}
/**
 * @brief grf_nifti_image_set_filetype
 * @param image
 * @param filetype
 */
void
grf_nifti_image_set_filetype(GrfNiftiImage* image, GrfNiftiFileType filetype){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nifti_type = filetype;
}
/**
 * @brief grf_nifti_image_get_filename
 * @param image
 * @return
 */
void
grf_nifti_image_set_filename(GrfNiftiImage* image, char* filename){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  if(priv->fname) g_free(priv->fname);
  priv->fname = g_strdup(filename);
}
/**
 * @brief grf_nifti_image_set_iname
 * @param image
 * @param filename
 */
static void
grf_nifti_image_set_iname(GrfNiftiImage* image, char* filename){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  if(priv->iname) g_free(priv->iname);
  priv->iname = g_strdup(filename);
}
static void
grf_nifti_image_set_iname_offset(GrfNiftiImage* image, int iname_offset){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->iname_offset = iname_offset;
}
static void
grf_nifti_image_set_datatype(GrfNiftiImage* image, int datatype){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->datatype = datatype;
}
static void
grf_nifti_image_set_ndim(GrfNiftiImage* image, int ndim){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->ndim = ndim;
}
static void
grf_nifti_image_set_nx(GrfNiftiImage* image, int nx){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[0] = nx;
}
static void
grf_nifti_image_set_ny(GrfNiftiImage* image, int ny){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[1] = ny;
}
static void
grf_nifti_image_set_nz(GrfNiftiImage* image, int nz){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[2] = nz;
}
static void
grf_nifti_image_set_nt(GrfNiftiImage* image, int nt){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[3] = nt;
}
static void
grf_nifti_image_set_nu(GrfNiftiImage* image, int nu){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[4] = nu;
}
static void
grf_nifti_image_set_nv(GrfNiftiImage* image, int nv){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[5] = nv;
}
static void
grf_nifti_image_set_nw(GrfNiftiImage* image, int nw){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[6] = nw;
}
static void
grf_nifti_image_set_dx(GrfNiftiImage* image, int dx){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[0] = dx;
}
static void
grf_nifti_image_set_dy(GrfNiftiImage* image, int dy){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[1] = dy;
}
static void
grf_nifti_image_set_dz(GrfNiftiImage* image, int dz){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[2] = dz;
}
static void
grf_nifti_image_set_dt(GrfNiftiImage* image, int dt){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[3] = dt;
}
static void
grf_nifti_image_set_du(GrfNiftiImage* image, int du){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[4] = du;
}
static void
grf_nifti_image_set_dv(GrfNiftiImage* image, int dv){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[5] = dv;
}
static void
grf_nifti_image_set_dw(GrfNiftiImage* image, int dw){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->nsize.data[6] = dw;
}
static void
grf_nifti_image_set_cal_min(GrfNiftiImage* image, float cal_min){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->cal_min = cal_min;
}
static void
grf_nifti_image_set_cal_max(GrfNiftiImage* image, float cal_max){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->cal_max = cal_max;
}
static void
grf_nifti_image_set_xyz_units(GrfNiftiImage* image, int xyz_units){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->xyz_units = xyz_units;
}
static void
grf_nifti_image_set_time_units(GrfNiftiImage* image, int time_units){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->time_units = time_units;
}
static void
grf_nifti_image_set_descrip(GrfNiftiImage* image, char* descrip, int max_length){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  strncpy(priv->descrip,descrip,max_length);
}
static void
grf_nifti_image_set_aux_file(GrfNiftiImage* image, char* aux_file, int max_length){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  strncpy(priv->aux_file,aux_file,max_length);
}
static void
grf_nifti_image_set_intent_p1(GrfNiftiImage* image, float intent_p1){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->intent_p1 = intent_p1;
}
static void
grf_nifti_image_set_intent_p2(GrfNiftiImage* image, float intent_p2){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->intent_p2 = intent_p2;
}
static void
grf_nifti_image_set_intent_p3(GrfNiftiImage* image, float intent_p3){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->intent_p3 = intent_p3;
}
static void
grf_nifti_image_set_scl_slope(GrfNiftiImage* image, float scl_slope){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->scl_slope = scl_slope;
}
static void
grf_nifti_image_set_scl_inter(GrfNiftiImage* image, float scl_inter){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->scl_inter = scl_inter;
}
static void
grf_nifti_image_set_intent_code(GrfNiftiImage* image, float intent_code){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->intent_code = intent_code;
}
static void
grf_nifti_image_set_toffset(GrfNiftiImage* image, float toffset){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->toffset = toffset;
}
static void
grf_nifti_image_set_intent_name(GrfNiftiImage* image, char* intent_name, int max_length){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  strncpy(priv->intent_name,intent_name,max_length);
}
static void
grf_nifti_image_set_sform_code(GrfNiftiImage* image, int sform_code){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->sform_code = sform_code;
}
static void
grf_nifti_image_set_freq_dim(GrfNiftiImage* image, int freq_dim){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->freq_dim = freq_dim;
}
static void
grf_nifti_image_set_phase_dim(GrfNiftiImage* image, int phase_dim){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->phase_dim = phase_dim;
}
static void
grf_nifti_image_set_slice_dim(GrfNiftiImage* image, int slice_dim){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->slice_dim = slice_dim;
}
static void
grf_nifti_image_set_slice_code(GrfNiftiImage* image, int slice_code){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->slice_code = slice_code;
}
static void
grf_nifti_image_set_slice_start(GrfNiftiImage* image, int slice_start){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->slice_start = slice_start;
}
static void
grf_nifti_image_set_slice_end(GrfNiftiImage* image, int slice_end){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->slice_end = slice_end;
}
static void
grf_nifti_image_set_slice_duration(GrfNiftiImage* image, float slice_duration){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->slice_duration = slice_duration;
}
static void
grf_nifti_image_set_num_ext(GrfNiftiImage* image, int num_ext){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->num_ext = num_ext;
}
static void
grf_nifti_image_set_qform_code(GrfNiftiImage* image, int qform_code){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->qform_code = qform_code;
}
static void
grf_nifti_image_set_quatern_b(GrfNiftiImage* image, float quatern_b){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->quatern_b = quatern_b;
}
static void
grf_nifti_image_set_quatern_c(GrfNiftiImage* image, float quatern_c){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->quatern_c = quatern_c;
}
static void
grf_nifti_image_set_quatern_d(GrfNiftiImage* image, float quatern_d){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->quatern_d = quatern_d;
}
static void
grf_nifti_image_set_qoffset_x(GrfNiftiImage* image, float qoffset_x){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->qoffset_x = qoffset_x;
}
static void
grf_nifti_image_set_qoffset_y(GrfNiftiImage* image, float qoffset_y){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->qoffset_y = qoffset_y;
}
static void
grf_nifti_image_set_qoffset_z(GrfNiftiImage* image, float qoffset_z){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  priv->qoffset_y = qoffset_z;
}

/*----------------------------------------------------------------------*/
/*! check the end of the filename for a valid nifti extension

    Valid extensions are currently .nii, .hdr, .img, .nia,
    or any of them followed by .gz.  Note that '.' is part of
    the extension.

    \return a pointer to the extension (within the filename), or NULL
*//*--------------------------------------------------------------------*/
static char *
grf_nifti_find_file_extension( const char * name )
{
   char * ext;
   int    len;

   if ( ! name ) return NULL;

   len = (int)strlen(name);
   if ( len < 4 ) return NULL;

   ext = (char *)name + len - 4;

   if ( (strcmp(ext, ".hdr") == 0) || (strcmp(ext, ".img") == 0) ||
        (strcmp(ext, ".nia") == 0) || (strcmp(ext, ".nii") == 0) )
      return ext;

#ifdef HAVE_ZLIB
   if ( len < 7 ) return NULL;

   ext = (char *)name + len - 7;

   if ( (strcmp(ext, ".hdr.gz") == 0) || (strcmp(ext, ".img.gz") == 0) ||
        (strcmp(ext, ".nii.gz") == 0) )
      return ext;
#endif
   return NULL;
}

/*----------------------------------------------------------------------*/
/*! return whether the filename is valid

    The name is considered valid if its length is positive, excluding
    any nifti filename extension.
    fname input         |  return | result of nifti_makebasename
    ====================================================================
    "myimage"           |  1      | "myimage"
    "myimage.tif"       |  1      | "myimage.tif"
    "myimage.tif.gz"    |  1      | "myimage.tif"
    "myimage.nii"       |  1      | "myimage"
    ".nii"              |  0      | <ERROR - basename has zero length>
    ".myhiddenimage"    |  1      | ".myhiddenimage"
    ".myhiddenimage.nii |  1      | ".myhiddenimage"
*//*--------------------------------------------------------------------*/
static gboolean
grf_nifti_validfilename(const char* fname)
{
   char * ext;

   /* check input file(s) for sanity */
   if( fname == NULL || *fname == '\0' ){
      return FALSE;
   }

   ext = grf_nifti_find_file_extension(fname);

   if ( ext && ext == fname ) {   /* then no filename prefix */
      return FALSE;
   }

   return TRUE;
}
/*----------------------------------------------------------------------*/
/*! duplicate the filename, while clearing any extension

    This allocates memory for basename which should eventually be freed.
*//*--------------------------------------------------------------------*/
static char *
grf_nifti_makebasename(const char* fname)
{
   char *basename, *ext;

   basename = g_strdup(fname);

   ext = grf_nifti_find_file_extension(basename);
   if ( ext ) *ext = '\0';  /* clear out extension */

   return basename;  /* in either case */
}
/*----------------------------------------------------------------------*/
/*! simple check for file existence

    \return 1 on existence, 0 otherwise
*//*--------------------------------------------------------------------*/
static gboolean
grf_nifti_fileexists(const char* fname)
{
  GrfZnzFile* fp = grf_znzfile_open(fname, "rb", TRUE);
  if(fp){
    g_clear_object(&fp);
    return TRUE;
  }
  return FALSE;
}

/*----------------------------------------------------------------------*/
/*! check current directory for existing header file

    \return filename of header on success and NULL if no appropriate file
            could be found

    NB: it allocates memory for hdrname which should be freed
        when no longer required
*//*-------------------------------------------------------------------*/
static char *
grf_nifti_findhdrname(const char* fname)
{
   char *basename, *hdrname, *ext;
   char  elist[2][5] = { ".hdr", ".nii" };
   int   efirst;

   /**- check input file(s) for sanity */
   if( !grf_nifti_validfilename(fname) ) return NULL;

   basename = grf_nifti_makebasename(fname);
   if( !basename ) return NULL;   /* only on string alloc failure */

   /**- return filename if it has a valid extension and exists
         (except if it is an .img file (and maybe .gz)) */
   ext = grf_nifti_find_file_extension(fname);
   if ( ext && grf_nifti_fileexists(fname) ) {
     if ( strncmp(ext,".img",4) != 0 ){
        hdrname = g_strdup(fname);
        free(basename);
        return hdrname;
     }
   }

   /* So the requested name is a basename, contains .img, or does not exist. */
   /* In any case, use basename. */

   /**- if .img, look for .hdr, .hdr.gz, .nii, .nii.gz, in that order */
   /**- else,    look for .nii, .nii.gz, .hdr, .hdr.gz, in that order */

   /* if we get more extension choices, this could be a loop */

   if ( ext && strncmp(ext,".img",4) == 0 ) efirst = 0;
   else                                     efirst = 1;

   hdrname = (char *)calloc(sizeof(char),strlen(basename)+8);
   if( !hdrname ){
      fprintf(stderr,"** nifti_findhdrname: failed to alloc hdrname\n");
      free(basename);
      return NULL;
   }

   strcpy(hdrname,basename);
   strcat(hdrname,elist[efirst]);
   if (grf_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#ifdef HAVE_ZLIB
   strcat(hdrname,".gz");
   if (grf_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#endif

   /* okay, try the other possibility */

   efirst = 1 - efirst;

   strcpy(hdrname,basename);
   strcat(hdrname,elist[efirst]);
   if (grf_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#ifdef HAVE_ZLIB
   strcat(hdrname,".gz");
   if (grf_nifti_fileexists(hdrname)) { free(basename); return hdrname; }
#endif

   /**- if nothing has been found, return NULL */
   free(basename);
   free(hdrname);
   return NULL;
}
/*----------------------------------------------------------------------*/
/*! return whether the filename ends in ".gz"
*//*--------------------------------------------------------------------*/
static gboolean
grf_nifti_is_gzfile(const char* fname)
{
  /* return true if the filename ends with .gz */
  if (fname == NULL) { return FALSE; }
#ifdef HAVE_ZLIB
  { /* just so len doesn't generate compile warning */
     int len;
     len = (int)strlen(fname);
     if (len < 3) return FALSE;  /* so we don't search before the name */
     if (strcmp(fname + strlen(fname) - 3,".gz")==0) { return TRUE; }
  }
#endif
  return FALSE;
}
/*---------------------------------------------------------------------------*/
/*! return the size of a file, in bytes

    \return size of file on success, -1 on error or no file

    changed to return int, -1 means no file or error      20 Dec 2004 [rickr]
*//*-------------------------------------------------------------------------*/
static int
grf_nifti_get_filesize( const char *pathname )
{
   struct stat buf ; int ii ;

   if( pathname == NULL || *pathname == '\0' ) return -1 ;
   ii = stat( pathname , &buf ); if( ii != 0 ) return -1 ;
   return (unsigned int)buf.st_size ;
}
/*----------------------------------------------------------------------
 * has_ascii_header  - see if the NIFTI header is an ASCII format
 *
 * If the file starts with the ASCII string "<nifti_image", then
 * process the dataset as a type-3 .nia file.
 *
 * return:  -1 on error, 1 if true, or 0 if false
 *
 * NOTE: this is NOT part of the NIFTI-1 standard
 *----------------------------------------------------------------------*/
static int
grf_nifti_has_ascii_header( GrfZnzFile* fp )
{
   char  buf[16];
   int   nread;

   if(fp == NULL) return 0;

   nread = (int)grf_znzfile_read_header(fp, buf, 1, 12);
   buf[12] = '\0';

   if( nread < 12 ) return -1;

   grf_znzfile_rewind(fp);  /* move back to the beginning, and check */

   if( strcmp(buf, "<nifti_image") == 0 ) return 1;

   return 0;
}

/*----------------------------------------------------------------------*/
/*! get the byte order for this CPU

    - GRF_LITTLE_ENDIAN means least significant byte, first
    - GRF_BIG_ENDIAN means most significant byte, first
*//*--------------------------------------------------------------------*/
static GrfEndianess
grf_nifti_short_order(void)   /* determine this CPU's byte order */
{
  union { unsigned char bb[2] ;
          short         ss    ; } fred ;

  fred.bb[0] = 1 ; fred.bb[1] = 0 ;

  return (fred.ss == 1) ? GRF_LITTLE_ENDIAN : GRF_BIG_ENDIAN;
}
/*------------------------------------------------------------------------*/
/* Un-escape a C string in place -- that is, convert XML escape sequences
   back into their characters.  (This can be done in place since the
   replacement is always smaller than the input.)  Escapes recognized are:
     -  &lt;   ->  <
     -  &gt;   ->  >
     -  &quot; ->  "
     -  &apos; ->  '
     -  &amp;  ->  &
   Also replace CR LF pair (Microsoft), or CR alone (Macintosh) with
   LF (Unix), per the XML standard.
   Return value is number of replacements made (if you care).
--------------------------------------------------------------------------*/
#undef  CR
#undef  LF
#define CR 0x0D
#define LF 0x0A
static int
unescape_string( char *str )
{
   int ii,jj , nn,ll ;

   if( str == NULL ) return 0 ;                /* no string? */
   ll = (int)strlen(str) ; if( ll == 0 ) return 0 ;

   /* scan for escapes: &something; */

   for( ii=jj=nn=0 ; ii<ll ; ii++,jj++ ){ /* scan at ii; results go in at jj */

     if( str[ii] == '&' ){  /* start of escape? */

             if( ii+3 < ll        &&   /* &lt; */
                 str[ii+1] == 'l' &&
                 str[ii+2] == 't' &&
                 str[ii+3] == ';'   ){ str[jj] = '<' ; ii += 3 ; nn++ ; }

        else if( ii+3 < ll        &&   /* &gt; */
                 str[ii+1] == 'g' &&
                 str[ii+2] == 't' &&
                 str[ii+3] == ';'   ){ str[jj] = '>' ; ii += 3 ; nn++ ; }

        else if( ii+5 < ll        &&   /* &quot; */
                 str[ii+1] == 'q' &&
                 str[ii+2] == 'u' &&
                 str[ii+3] == 'o' &&
                 str[ii+4] == 't' &&
                 str[ii+5] == ';'   ){ str[jj] = '"' ; ii += 5 ; nn++ ; }

        else if( ii+5 < ll        &&   /* &apos; */
                 str[ii+1] == 'a' &&
                 str[ii+2] == 'p' &&
                 str[ii+3] == 'o' &&
                 str[ii+4] == 's' &&
                 str[ii+5] == ';'   ){ str[jj] = '\'' ; ii += 5 ; nn++ ; }

        else if( ii+4 < ll        &&  /* &amp; */
                 str[ii+1] == 'a' &&
                 str[ii+2] == 'm' &&
                 str[ii+3] == 'p' &&
                 str[ii+4] == ';'   ){ str[jj] = '&' ; ii += 4 ; nn++ ; }

        /* although the comments above don't mention it,
           we also look for XML style numeric escapes
           of the forms &#32; (decimal) and &#xfd; (hex) */

        else if( ii+3 < ll        &&
                 str[ii+1] == '#' &&
                 isdigit(str[ii+2]) ){   /* &#dec; */

           unsigned int val='?' ; int kk=ii+3 ;
           while( kk < ll && kk != ';' ) kk++ ;
           sscanf( str+ii+2 , "%u" , &val ) ;
           str[jj] = (char) val ; ii = kk ; nn++ ;
        }

        else if( ii+4 < ll        &&
                 str[ii+1] == '#' &&
                 str[ii+2] == 'x' &&
                 isxdigit(str[ii+3]) ){   /* &#hex; */

           unsigned int val='?' ; int kk=ii+4 ;
           while( kk < ll && kk != ';' ) kk++ ;
           sscanf( str+ii+3 , "%x" , &val ) ;
           str[jj] = (char) val ; ii = kk ; nn++ ;
        }

        /* didn't start a recognized escape, so just copy as normal */

        else if( jj < ii ){ str[jj] = str[ii] ; }

     } else if( str[ii] == CR ) {  /* is a carriage return */

        if( str[ii+1] == LF ){ str[jj] = LF ; ii++ ; nn++ ; }  /* CR LF */
        else                 { str[jj] = LF ;      ; nn++ ; }  /* CR only */

     } else { /* is a normal character, just copy to output */

             if( jj < ii ){ str[jj] = str[ii] ; }
     }

     /* at this point, ii=index of last character used up in scan
                       jj=index of last character written to (jj <= ii) */
   }

   if( jj < ll ) str[jj] = '\0' ; /* end string properly */

   return nn ;
}
/*--------------------------------------------------------------------------*/
/*! Given a datatype code, set number of bytes per voxel and the swapsize.

    \param datatype nifti1 datatype code
    \param nbyper   pointer to return value: number of bytes per voxel
    \param swapsize pointer to return value: size of swap blocks

    \return appropriate values at nbyper and swapsize

    The swapsize is set to 0 if this datatype doesn't ever need swapping.

    \sa NIFTI1_DATATYPES in nifti1.h
*//*------------------------------------------------------------------------*/
#define ASSIF(p,v) if( (p)!=NULL ) *(p) = (v)
static void
grf_nifti_datatype_sizes( int datatype , int *nbyper, int *swapsize )
{
   int nb=0, ss=0 ;
   switch( datatype ){
     case GRF_DT_INT8:
     case GRF_DT_UINT8:       nb =  1 ; ss =  0 ; break ;

     case GRF_DT_INT16:
     case GRF_DT_UINT16:      nb =  2 ; ss =  2 ; break ;

     case GRF_DT_RGB24:       nb =  3 ; ss =  0 ; break ;
     case GRF_DT_RGBA32:      nb =  4 ; ss =  0 ; break ;

     case GRF_DT_INT32:
     case GRF_DT_UINT32:
     case GRF_DT_FLOAT32:     nb =  4 ; ss =  4 ; break ;

     case GRF_DT_COMPLEX64:   nb =  8 ; ss =  4 ; break ;

     case GRF_DT_FLOAT64:
     case GRF_DT_INT64:
     case GRF_DT_UINT64:      nb =  8 ; ss =  8 ; break ;

     case GRF_DT_FLOAT128:    nb = 16 ; ss = 16 ; break ;

     case GRF_DT_COMPLEX128:  nb = 16 ; ss =  8 ; break ;

     case GRF_DT_COMPLEX256:  nb = 32 ; ss = 16 ; break ;
   }

   ASSIF(nbyper,nb) ; ASSIF(swapsize,ss) ; return ;
}
/*---------------------------------------------------------------------------*/
/*! Given the quaternion parameters (etc.), compute a transformation matrix.

   See comments in nifti1.h for details.
     - qb,qc,qd = quaternion parameters
     - qx,qy,qz = offset parameters
     - dx,dy,dz = grid stepsizes (non-negative inputs are set to 1.0)
     - qfac     = sign of dz step (< 0 is negative; >= 0 is positive)

   <pre>
   If qx=qy=qz=0, dx=dy=dz=1, then the output is a rotation matrix.
   For qfac >= 0, the rotation is proper.
   For qfac <  0, the rotation is improper.
   </pre>

   \see "QUATERNION REPRESENTATION OF ROTATION MATRIX" in nifti1.h
   \see nifti_mat44_to_quatern, nifti_make_orthog_mat44,
       nifti_mat44_to_orientation

*//*-------------------------------------------------------------------------*/
static GrfMat4
grf_nifti_quatern_to_mat4( float qb, float qc, float qd,
                           float qx, float qy, float qz,
                           float dx, float dy, float dz, float qfac )
{
   GrfMat4 R ;
   double a,b=qb,c=qc,d=qd , xd,yd,zd ;

   /* last row is always [ 0 0 0 1 ] */

   memset(&R.data[12],0,sizeof(double)*3);
   R.data[15] = 1.0;

   /* compute a parameter from b,c,d */

   a = 1.0l - (b*b + c*c + d*d) ;
   if( a < 1.e-7l ){                   /* special case */
     a = 1.0l / sqrt(b*b+c*c+d*d) ;
     b *= a ; c *= a ; d *= a ;        /* normalize (b,c,d) vector */
     a = 0.0l ;                        /* a = 0 ==> 180 degree rotation */
   } else{
     a = sqrt(a) ;                     /* angle = 2*arccos(a) */
   }

   /* load rotation matrix, including scaling factors for voxel sizes */

   xd = (dx > 0.0) ? dx : 1.0l ;       /* make sure are positive */
   yd = (dy > 0.0) ? dy : 1.0l ;
   zd = (dz > 0.0) ? dz : 1.0l ;

   if( qfac < 0.0 ) zd = -zd ;         /* left handedness? */

   R.data[ 0] =        (a*a+b*b-c*c-d*d) * xd ;
   R.data[ 4] = 2.0l * (b*c-a*d        ) * yd ;
   R.data[ 8] = 2.0l * (b*d+a*c        ) * zd ;
   R.data[ 1] = 2.0l * (b*c+a*d        ) * xd ;
   R.data[ 5] =        (a*a+c*c-b*b-d*d) * yd ;
   R.data[ 9] = 2.0l * (c*d-a*b        ) * zd ;
   R.data[ 2] = 2.0l * (b*d-a*c        ) * xd ;
   R.data[ 6] = 2.0l * (c*d+a*b        ) * yd ;
   R.data[10] =        (a*a+d*d-c*c-b*b) * zd ;

   /* load offsets */

   R.data[12] = qx;
   R.data[13] = qy;
   R.data[14] = qz;

   return R ;
}
/*---------------------------------------------------------------------------*/
/*! Compute the inverse of a bordered 4x4 matrix.

     <pre>
   - Some numerical code fragments were generated by Maple 8.
   - If a singular matrix is input, the output matrix will be all zero.
   - You can check for this by examining the [3][3] element, which will
     be 1.0 for the normal case and 0.0 for the bad case.

     The input matrix should have the form:
        [ r11 r12 r13 v1 ]
        [ r21 r22 r23 v2 ]
        [ r31 r32 r33 v3 ]
        [  0   0   0   1 ]
     </pre>
*//*-------------------------------------------------------------------------*/
static GrfMat4
grf_nifti_mat4_inverse( GrfMat4 R )
{
   double r11,r12,r13,r21,r22,r23,r31,r32,r33,v1,v2,v3 , deti ;
   GrfMat4 Q ;
                                                       /*  INPUT MATRIX IS:  */
   r11 = R.data[ 0]; r12 = R.data[ 4]; r13 = R.data[ 8];  /* [ r11 r12 r13 v1 ] */
   r21 = R.data[ 1]; r22 = R.data[ 5]; r23 = R.data[ 9];  /* [ r21 r22 r23 v2 ] */
   r31 = R.data[ 2]; r32 = R.data[ 6]; r33 = R.data[10];  /* [ r31 r32 r33 v3 ] */
   v1  = R.data[12]; v2  = R.data[13]; v3  = R.data[14];  /* [  0   0   0   1 ] */

   deti = r11*r22*r33-r11*r32*r23-r21*r12*r33
         +r21*r32*r13+r31*r12*r23-r31*r22*r13 ;

   if( deti != 0.0l ) deti = 1.0l / deti ;

   Q.data[ 0] = deti*( r22*r33-r32*r23) ;
   Q.data[ 4] = deti*(-r12*r33+r32*r13) ;
   Q.data[ 8] = deti*( r12*r23-r22*r13) ;
   Q.data[12] = deti*(-r12*r23*v3+r12*v2*r33+r22*r13*v3
                     -r22*v1*r33-r32*r13*v2+r32*v1*r23) ;

   Q.data[ 1] = deti*(-r21*r33+r31*r23) ;
   Q.data[ 5] = deti*( r11*r33-r31*r13) ;
   Q.data[ 9] = deti*(-r11*r23+r21*r13) ;
   Q.data[13] = deti*( r11*r23*v3-r11*v2*r33-r21*r13*v3
                     +r21*v1*r33+r31*r13*v2-r31*v1*r23) ;

   Q.data[2]  = deti*( r21*r32-r31*r22) ;
   Q.data[6]  = deti*(-r11*r32+r31*r12) ;
   Q.data[10] = deti*( r11*r22-r21*r12) ;
   Q.data[14] = deti*(-r11*r22*v3+r11*r32*v2+r21*r12*v3
                     -r21*r32*v1-r31*r12*v2+r31*r22*v1) ;

   Q.data[3]  = Q.data[7] = Q.data[11] = 0.0l ;
   Q.data[15] = (deti == 0.0l) ? 0.0l : 1.0l ; /* failure flag if deti == 0 */

   return Q ;
}

/* macro to check lhs string against "n1"; if it matches,
   interpret rhs string as a number, and put it into nim->"n2" */
#define QQNUM(n1,n2) if( strcmp(lhs,#n1)==0 ) grf_nifti_image_set_##n2(nim,strtod(rhs,NULL))

/* same, but where "n1" == "n2" */
#define QNUM(nam)    QQNUM(nam,nam)

/* macro to check lhs string against "nam"; if it matches,
   put rhs string into nim->"nam" string, with max length = "ml" */
#define QSTR(nam,ml) if(strcmp(lhs,#nam) == 0 )                           \
                       grf_nifti_image_set_##nam(nim,rhs,ml)

/*---------------------------------------------------------------------------*/
/*! Take an XML-ish ASCII string and create a NIFTI image header to match.

    NULL is returned if enough information isn't present in the input string.
    - The image data can later be loaded with nifti_image_load().
    - The struct returned here can be liberated with nifti_image_free().
    - Not a lot of error checking is done here to make sure that the
      input values are reasonable!
*//*-------------------------------------------------------------------------*/
static GrfNiftiImage*
grf_nifti_image_from_ascii( const char *str, int * bytes_read )
{
   char lhs[1024] , rhs[1024] ;
   int ii , spos, nn;
   GrfNiftiImage *nim ;              /* will be output */

   if( str == NULL || *str == '\0' ) return NULL ;  /* bad input!? */

   /* scan for opening string */

   spos = 0 ;
   ii = sscanf( str+spos , "%1023s%n" , lhs , &nn ) ; spos += nn ;
   if( ii == 0 || strcmp(lhs,"<nifti_image") != 0 ) return NULL ;

   /* create empty image struct */
   nim = grf_nifti_image_new();
   if(!nim){
      fprintf(stderr,"** NIFA: failed to alloc nifti_image\n");
      return NULL;
   }

   grf_nifti_image_set_nsize(nim, (GrfVec7){{1,1,1,1,1,1,1}});
   grf_nifti_image_set_dsize(nim, (GrfVec7){{0,0,0,0,0,0,0}});
   grf_nifti_image_set_qfac(nim, 1.0);
   grf_nifti_image_set_byteorder(nim,grf_nifti_short_order());

   /* starting at str[spos], scan for "equations" of the form
         lhs = 'rhs'
      and assign rhs values into the struct component named by lhs */

   while(1){

     while( isspace((int) str[spos]) ) spos++ ;  /* skip whitespace */
     if( str[spos] == '\0' ) break ;       /* end of string? */

     /* get lhs string */

     ii = sscanf( str+spos , "%1023s%n" , lhs , &nn ) ; spos += nn ;
     if( ii == 0 || strcmp(lhs,"/>") == 0 ) break ;  /* end of input? */

     /* skip whitespace and the '=' marker */

     while( isspace((int) str[spos]) || str[spos] == '=' ) spos++ ;
     if( str[spos] == '\0' ) break ;       /* end of string? */

     /* if next character is a quote ', copy everything up to next '
        otherwise, copy everything up to next nonblank              */

     if( str[spos] == '\'' ){
        ii = spos+1 ;
        while( str[ii] != '\0' && str[ii] != '\'' ) ii++ ;
        nn = ii-spos-1 ; if( nn > 1023 ) nn = 1023 ;
        memcpy(rhs,str+spos+1,nn) ; rhs[nn] = '\0' ;
        spos = (str[ii] == '\'') ? ii+1 : ii ;
     } else {
        ii = sscanf( str+spos , "%1023s%n" , rhs , &nn ) ; spos += nn ;
        if( ii == 0 ) break ;  /* nothing found? */
     }
     unescape_string(rhs) ;  /* remove any XML escape sequences */

     /* Now can do the assignment, based on lhs string.
        Start with special cases that don't fit the QNUM/QSTR macros. */


     if( strcmp(lhs,"nifti_type") == 0 ){
       if( strcmp(rhs,"ANALYZE-7.5") == 0 )
         grf_nifti_image_set_filetype(nim,GRF_NIFTI_TYPE_ANALYZE);
       else if( strcmp(rhs,"NIFTI-1+")    == 0 )
         grf_nifti_image_set_filetype(nim, GRF_NIFTI_TYPE_NIFTI1_1);
       else if( strcmp(rhs,"NIFTI-1")     == 0 )
         grf_nifti_image_set_filetype(nim, GRF_NIFTI_TYPE_NIFTI1_2);
       else if( strcmp(rhs,"NIFTI-1A")    == 0 )
         grf_nifti_image_set_filetype(nim, GRF_NIFTI_TYPE_ASCII);
     }
     else if( strcmp(lhs,"header_filename") == 0 ){
       grf_nifti_image_set_filename(nim, rhs);
     }
     else if( strcmp(lhs,"image_filename") == 0 ){
       grf_nifti_image_set_iname(nim, rhs);
     }
     else if( strcmp(lhs,"sto_xyz_matrix") == 0 ){
       GrfMat4 sto_xyz;
       double* m = sto_xyz.data;
       sscanf( rhs , "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf" ,
               &m[ 0], &m[ 1], &m[ 2], &m[ 3], &m[ 4], &m[ 5], &m[ 6], &m[ 7],
               &m[ 8], &m[ 9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]);
     }
     else if( strcmp(lhs,"byteorder") == 0 ){
       if( strcmp(rhs,"MSB_FIRST") == 0 ) grf_nifti_image_set_byteorder(nim, GRF_BIG_ENDIAN);
       if( strcmp(rhs,"LSB_FIRST") == 0 ) grf_nifti_image_set_byteorder(nim, GRF_LITTLE_ENDIAN);
     }
     else QQNUM(image_offset,iname_offset) ;
     else QNUM(datatype) ;
     else QNUM(ndim) ;
     else QNUM(nx) ;
     else QNUM(ny) ;
     else QNUM(nz) ;
     else QNUM(nt) ;
     else QNUM(nu) ;
     else QNUM(nv) ;
     else QNUM(nw) ;
     else QNUM(dx) ;
     else QNUM(dy) ;
     else QNUM(dz) ;
     else QNUM(dt) ;
     else QNUM(du) ;
     else QNUM(dv) ;
     else QNUM(dw) ;
     else QNUM(cal_min) ;
     else QNUM(cal_max) ;
     else QNUM(scl_slope) ;
     else QNUM(scl_inter) ;
     else QNUM(intent_code) ;
     else QNUM(intent_p1) ;
     else QNUM(intent_p2) ;
     else QNUM(intent_p3) ;
     else QSTR(intent_name,15) ;
     else QNUM(toffset) ;
     else QNUM(xyz_units) ;
     else QNUM(time_units) ;
     else QSTR(descrip,79) ;
     else QSTR(aux_file,23) ;
     else QNUM(qform_code) ;
     else QNUM(quatern_b) ;
     else QNUM(quatern_c) ;
     else QNUM(quatern_d) ;
     else QNUM(qoffset_x) ;
     else QNUM(qoffset_y) ;
     else QNUM(qoffset_z) ;
     else QNUM(qfac) ;
     else QNUM(sform_code) ;
     else QNUM(freq_dim) ;
     else QNUM(phase_dim) ;
     else QNUM(slice_dim) ;
     else QNUM(slice_code) ;
     else QNUM(slice_start) ;
     else QNUM(slice_end) ;
     else QNUM(slice_duration) ;
     else QNUM(num_ext) ;

   } /* end of while loop */

   if( bytes_read ) *bytes_read = spos+1;         /* "process" last '\n' */

   /* do miscellaneous checking and cleanup */
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   if( priv->ndim <= 0 ){ g_clear_object(&nim); return NULL; } /* bad! */

   grf_nifti_datatype_sizes( priv->datatype, &(priv->nbyper), &(priv->swapsize) );
   if( priv->nbyper == 0 ){ g_clear_object(&nim); return NULL; } /* bad! */

   priv->dim[0] = priv->ndim ;
   priv->dim[1] = priv->nsize.data[0] ; priv->pixdim[1] = priv->dsize.data[0] ;
   priv->dim[2] = priv->nsize.data[1] ; priv->pixdim[2] = priv->dsize.data[1] ;
   priv->dim[3] = priv->nsize.data[2] ; priv->pixdim[3] = priv->dsize.data[2] ;
   priv->dim[4] = priv->nsize.data[3] ; priv->pixdim[4] = priv->dsize.data[3] ;
   priv->dim[5] = priv->nsize.data[4] ; priv->pixdim[5] = priv->dsize.data[4] ;
   priv->dim[6] = priv->nsize.data[5] ; priv->pixdim[6] = priv->dsize.data[5] ;
   priv->dim[7] = priv->nsize.data[6] ; priv->pixdim[7] = priv->dsize.data[6] ;

   priv->nvox = 1;
   uint8_t i;
   for(i = 0; i < 7; i++) priv->nvox *= priv->nsize.data[i];

   if( priv->qform_code > 0 )
     priv->qto_xyz = grf_nifti_quatern_to_mat4(
                      priv->quatern_b, priv->quatern_c, priv->quatern_d,
                      priv->qoffset_x, priv->qoffset_y, priv->qoffset_z,
                      priv->dsize.data[0],  priv->dsize.data[1],  priv->dsize.data[2],
                      priv->qfac) ;
   else
     priv->qto_xyz = grf_nifti_quatern_to_mat4(
                      0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 ,
                      priv->dsize.data[0] , priv->dsize.data[1] , priv->dsize.data[2] , 0.0 ) ;


   priv->qto_ijk = grf_nifti_mat4_inverse( priv->qto_xyz ) ;

   if( priv->sform_code > 0 )
     priv->sto_ijk = grf_nifti_mat4_inverse( priv->sto_xyz ) ;

   return nim ;
}
/*----------------------------------------------------------------------*/
/*! swap 4 bytes at a time from the given list of n sets of 4 bytes
*//*--------------------------------------------------------------------*/
static void
grf_nifti_swap_4bytes( int n , void *ar )    /* 4 bytes at a time */
{
   register int ii ;
   unsigned char * cp0 = (unsigned char *)ar, * cp1, * cp2 ;
   register unsigned char tval ;

   for( ii=0 ; ii < n ; ii++ ){
       cp1 = cp0; cp2 = cp0+3;
       tval = *cp1;  *cp1 = *cp2;  *cp2 = tval;
       cp1++;  cp2--;
       tval = *cp1;  *cp1 = *cp2;  *cp2 = tval;
       cp0 += 4;
   }
   return ;
}
/*----------------------------------------------------------------------*/
/*! check whether the extension code is valid

    \return 1 if valid, 0 otherwise
*//*--------------------------------------------------------------------*/
static gboolean
grf_nifti_is_valid_ecode( int ecode )
{
   if( ecode < GRF_NIFTI_ECODE_IGNORE  ||   /* minimum code number (0) */
       ecode > GRF_NIFTI_MAX_ECODE     ||   /* maximum code number     */
       ecode & 1 )                      /* cannot be odd           */
      return FALSE;

   return TRUE;
}
/*----------------------------------------------------------------------
 * check for valid size and code, as well as can be done
 *----------------------------------------------------------------------*/
static gboolean
grf_nifti_check_extension(GrfNiftiImage *nim, int size, int code, int rem)
{
  /* check for bad code before bad size */
  if( ! grf_nifti_is_valid_ecode(code) ) {
    return FALSE;
  }

  if( size < 16 ){
    return FALSE;
  }

  if( size > rem ){
    return FALSE;
  }

  if( size & 0xf ){
    return FALSE;
  }

  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
  if( priv->nifti_type == GRF_NIFTI_TYPE_ASCII && size > LNI_MAX_NIA_EXT_LEN ){
    return FALSE;
  }

  return TRUE;
}

/*----------------------------------------------------------------------
 * nifti_read_next_extension  - read a single extension from the file
 *
 * return (>= 0 is okay):
 *
 *     success      : esize
 *     no extension : 0
 *     error        : -1
 *----------------------------------------------------------------------*/
static int grf_nifti_read_next_extension( GrfNifti1Extension * nex, GrfNiftiImage *nim,
                                      int remain, GrfZnzFile* fp )
{
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
  int swap = priv->byteorder != grf_nifti_short_order();
  int count, size, code;

  /* first clear nex */
  nex->esize = nex->ecode = 0;
  nex->edata = NULL;

  if( remain < 16 ){
     return 0;
  }

  /* must start with 4-byte size and code */
  count = (int)grf_znzfile_read(fp, &size, 4, 1);
  if( count == 1 ) count += (int)grf_znzfile_read(fp, &code, 4, 1);

  if( count != 2 ){
     grf_znzfile_seek(fp, -4*count, SEEK_CUR); /* back up past any read */
     return 0;                        /* no extension, no error condition */
  }

  if( swap ){
     grf_nifti_swap_4bytes(1, &size);
     grf_nifti_swap_4bytes(1, &code);
  }

  if( !grf_nifti_check_extension(nim, size, code, remain) ){
     if( grf_znzfile_seek(fp, -8, SEEK_CUR) < 0 ){      /* back up past any read */
        fprintf(stderr,"** failure to back out of extension read!\n");
        return -1;
     }
     return 0;
  }

  /* now get the actual data */
  nex->esize = size;
  nex->ecode = code;

  size -= 8;  /* subtract space for size and code in extension */
  nex->edata = (char *)malloc(size * sizeof(char));
  if( !nex->edata ){
     fprintf(stderr,"** failed to allocate %d bytes for extension\n",size);
     return -1;
  }

  count = (int)grf_znzfile_read(fp,nex->edata, 1, size);
  if( count < size ){
     free(nex->edata);
     nex->edata = NULL;
     return -1;
  }

  /* success! */
  return nex->esize;
}

/*----------------------------------------------------------------------*/
/* nifti_add_exten_to_list     - add a new GrfNifti1Extension to the list

   We will append via "malloc, copy and free", because on an error,
   the list will revert to the previous one (sorry realloc(), only
   quality dolphins get to become part of St@rk!st brand tunafish).

   return 0 on success, -1 on error (and free the entire list)
*//*--------------------------------------------------------------------*/
static int grf_nifti_add_exten_to_list( GrfNifti1Extension *  new_ext,
                                    GrfNifti1Extension ** list, int new_length )
{
   GrfNifti1Extension * tmplist;

   tmplist = *list;
   *list = (GrfNifti1Extension *)malloc(new_length * sizeof(GrfNifti1Extension));

   /* check for failure first */
   if( ! *list ){
      fprintf(stderr,"** failed to alloc %d extension structs (%d bytes)\n",
              new_length, new_length*(int)sizeof(GrfNifti1Extension));
      if( !tmplist ) return -1;  /* no old list to lose */

      *list = tmplist;  /* reset list to old one */
      return -1;
   }

   /* if an old list exists, copy the pointers and free the list */
   if( tmplist ){
      memcpy(*list, tmplist, (new_length-1)*sizeof(GrfNifti1Extension));
      free(tmplist);
   }

   /* for some reason, I just don't like struct copy... */
   (*list)[new_length-1].esize = new_ext->esize;
   (*list)[new_length-1].ecode = new_ext->ecode;
   (*list)[new_length-1].edata = new_ext->edata;

   return 0;
}

/*----------------------------------------------------------------------
 * Read the extensions into the nifti_image struct   08 Dec 2004 [rickr]
 *
 * This function is called just after the header struct is read in, and
 * it is assumed the file pointer has not moved.  The value in remain
 * is assumed to be accurate, reflecting the bytes of space for potential
 * extensions.
 *
 * return the number of extensions read in, or < 0 on error
 *----------------------------------------------------------------------*/
static int
grf_nifti_read_extensions( GrfNiftiImage *nim, GrfZnzFile* fp, int remain )
{
   GrfNifti1Extender    extdr;      /* defines extension existence  */
   GrfNifti1Extension   extn;       /* single extension to process  */
   GrfNifti1Extension * Elist;      /* list of processed extensions */
   int                posn, count;

   if( !nim || !fp ) {
      return -1;
   }

   posn = grf_znzfile_tell(fp);
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   if( (posn != sizeof(GrfNifti1Header)) &&
       (priv->nifti_type != GRF_NIFTI_TYPE_ASCII) )
      fprintf(stderr,"** WARNING: posn not header size (%d, %d)\n",
              posn, (int)sizeof(GrfNifti1Header));

   if( remain < 16 )
      return 0;


   count = (int)grf_znzfile_read(fp, extdr.extension, 1, 4); /* get extender */

   if( count < 4 ){
      return 0;
   }

   if( extdr.extension[0] != 1 ){
      return 0;
   }

   remain -= 4;

   /* so we expect extensions, but have no idea of how many there may be */

   count = 0;
   Elist = NULL;
   while (grf_nifti_read_next_extension(&extn, nim, remain, fp) > 0)
   {
      if( grf_nifti_add_exten_to_list(&extn, &Elist, count+1) < 0 ){
         return -1;
      }

      remain -= extn.esize;
      count++;
   }

   priv->num_ext = count;
   priv->ext_list = Elist;

   return count;
}

/*------------------------------------------------------------------------*/
/*! check current directory for existing image file

    \param fname filename to check for
    \nifti_type  nifti_type for dataset - this determines whether to
                 first check for ".nii" or ".img" (since both may exist)

    \return filename of data/img file on success and NULL if no appropriate
            file could be found

    NB: it allocates memory for the image filename, which should be freed
        when no longer required
*//*---------------------------------------------------------------------*/
char * grf_nifti_findimgname(const char* fname , int nifti_type)
{
   char *basename, *imgname, ext[2][5] = { ".nii", ".img" };
   int   first;  /* first extension to use */

   /* check input file(s) for sanity */
   if( !grf_nifti_validfilename(fname) ) return NULL;

   basename =  grf_nifti_makebasename(fname);
   imgname = (char *)calloc(sizeof(char),strlen(basename)+8);
   if( !imgname ){
      fprintf(stderr,"** nifti_findimgname: failed to alloc imgname\n");
      free(basename);
      return NULL;
   }

   /* only valid extension for ASCII type is .nia, handle first */
   if( nifti_type == GRF_NIFTI_TYPE_ASCII ){
      strcpy(imgname,basename);
      strcat(imgname,".nia");
      if (grf_nifti_fileexists(imgname)) { free(basename); return imgname; }

   } else {

      /**- test for .nii and .img (don't assume input type from image type) */
      /**- if nifti_type = 1, check for .nii first, else .img first         */

      /* if we get 3 or more extensions, can make a loop here... */

      if (nifti_type == GRF_NIFTI_TYPE_NIFTI1_1) first = 0; /* should match .nii */
      else                                    first = 1; /* should match .img */

      strcpy(imgname,basename);
      strcat(imgname,ext[first]);
      if (grf_nifti_fileexists(imgname)) { free(basename); return imgname; }
#ifdef HAVE_ZLIB  /* then also check for .gz */
      strcat(imgname,".gz");
      if (grf_nifti_fileexists(imgname)) { free(basename); return imgname; }
#endif

      /* failed to find image file with expected extension, try the other */

      strcpy(imgname,basename);
      strcat(imgname,ext[1-first]);  /* can do this with only 2 choices */
      if (grf_nifti_fileexists(imgname)) { free(basename); return imgname; }
#ifdef HAVE_ZLIB  /* then also check for .gz */
      strcat(imgname,".gz");
      if (grf_nifti_fileexists(imgname)) { free(basename); return imgname; }
#endif
   }

   /**- if nothing has been found, return NULL */
   free(basename);
   free(imgname);
   return NULL;
}
/*----------------------------------------------------------------------
 * grf_nifti_image_load_prep  - prepare to read data
 *
 * Check nifti_image fields, open the file and seek to the appropriate
 * offset for reading.
 *
 * return NULL on failure
 *----------------------------------------------------------------------*/
static GrfZnzFile*
grf_nifti_image_load_prep( GrfNiftiImage *nim )
{
   /* set up data space, open data file and seek, then call nifti_read_buffer */
   size_t  ntot , ii , ioff;
   GrfZnzFile* fp;
   g_autofree char *tmpimgname;
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   /**- perform sanity checks */
   if( nim == NULL      || priv->iname == NULL ||
       priv->nbyper <= 0 || priv->nvox <= 0       )
   {
      return NULL;
   }

   ntot = grf_nifti_image_get_volsize(nim) ; /* total bytes to read */

   /**- open image data file */

   tmpimgname = grf_nifti_findimgname(priv->iname , priv->nifti_type);
   if( tmpimgname == NULL ){
      return NULL;
   }

   fp = grf_znzfile_open(tmpimgname, "rb", grf_nifti_is_gzfile(tmpimgname));
   if (fp == NULL){
       return NULL;  /* bad open? */
   }

   /**- get image offset: a negative offset means to figure from end of file */
   if( priv->iname_offset < 0 ){
     if( grf_nifti_is_gzfile(priv->iname) ){
       g_clear_object(&fp);
       return NULL;
     }
     ii = grf_nifti_get_filesize( priv->iname ) ;
     if( ii <= 0 ){
        g_clear_object(&fp);
        return NULL;
     }
     ioff = (ii > ntot) ? ii-ntot : 0 ;
   } else {                              /* non-negative offset   */
     ioff = priv->iname_offset ;          /* means use it directly */
   }

   /**- seek to the appropriate read position */
   if( grf_znzfile_seek(fp , (long)ioff , SEEK_SET) < 0 ){
      fprintf(stderr,"** could not seek to offset %u in file '%s'\n",
              (unsigned)ioff, priv->iname);
      g_clear_object(&fp);
      return NULL;
   }

   /**- and return the File pointer */
   return fp;
}
GrfDataType
grf_nifti_to_grf_datatype(int datatype){
  switch(datatype){
    case GRF_DT_UINT8: return GRF_UINT8;
    case GRF_DT_UINT16:return GRF_UINT16;
    case GRF_DT_UINT32:return GRF_UINT32;
    case GRF_DT_UINT64:return GRF_UINT64;
    case GRF_DT_INT8:  return GRF_INT8;
    case GRF_DT_INT16: return GRF_INT16;
    case GRF_DT_INT32: return GRF_INT32;
    case GRF_DT_INT64: return GRF_INT64;
    case GRF_DT_FLOAT: return GRF_FLOAT;
    case GRF_DT_DOUBLE:return GRF_DOUBLE;
  }
  return -1;
}
int
grf_nifti_to_datatype(GrfDataType datatype){
  switch(datatype){
    case GRF_UINT8 : return GRF_DT_UINT8 ;
    case GRF_UINT16: return GRF_DT_UINT16;
    case GRF_UINT32: return GRF_DT_UINT32;
    case GRF_UINT64: return GRF_DT_UINT64;
    case GRF_INT8  : return GRF_DT_INT8  ;
    case GRF_INT16 : return GRF_DT_INT16 ;
    case GRF_INT32 : return GRF_DT_INT32 ;
    case GRF_INT64 : return GRF_DT_INT64 ;
    case GRF_FLOAT : return GRF_DT_FLOAT ;
    case GRF_DOUBLE: return GRF_DT_DOUBLE;
  }
  return -1;
}

/*---------------------------------------------------------------------------*/
/* Routines to swap byte arrays in various ways:
    -  2 at a time:  ab               -> ba               [short]
    -  4 at a time:  abcd             -> dcba             [int, float]
    -  8 at a time:  abcdDCBA         -> ABCDdcba         [long long, double]
    - 16 at a time:  abcdefghHGFEDCBA -> ABCDEFGHhgfedcba [long double]
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*! swap each byte pair from the given list of n pairs
 *
 *  Due to alignment of structures at some architectures (e.g. on ARM),
 *  stick to char varaibles.
 *  Fixes http://bugs.debian.org/446893   Yaroslav <debian@onerussian.com>
 *
*//*--------------------------------------------------------------------*/
void grf_nifti_swap_2bytes( int n , void *ar )    /* 2 bytes at a time */
{
   register int ii ;
   unsigned char * cp1 = (unsigned char *)ar, * cp2 ;
   unsigned char   tval;

   for( ii=0 ; ii < n ; ii++ ){
       cp2 = cp1 + 1;
       tval = *cp1;  *cp1 = *cp2;  *cp2 = tval;
       cp1 += 2;
   }
   return ;
}
/*----------------------------------------------------------------------*/
/*! swap 8 bytes at a time from the given list of n sets of 8 bytes
 *
 *  perhaps use this style for the general Nbytes, as Yaroslav suggests
*//*--------------------------------------------------------------------*/
void grf_nifti_swap_8bytes( int n , void *ar )    /* 8 bytes at a time */
{
   register int ii ;
   unsigned char * cp0 = (unsigned char *)ar, * cp1, * cp2 ;
   register unsigned char tval ;

   for( ii=0 ; ii < n ; ii++ ){
       cp1 = cp0;  cp2 = cp0+7;
       while ( cp2 > cp1 )      /* unroll? */
       {
           tval = *cp1 ; *cp1 = *cp2 ; *cp2 = tval ;
           cp1++; cp2--;
       }
       cp0 += 8;
   }
   return ;
}

/*----------------------------------------------------------------------*/
/*! swap 16 bytes at a time from the given list of n sets of 16 bytes
*//*--------------------------------------------------------------------*/
void grf_nifti_swap_16bytes( int n , void *ar )    /* 16 bytes at a time */
{
   register int ii ;
   unsigned char * cp0 = (unsigned char *)ar, * cp1, * cp2 ;
   register unsigned char tval ;

   for( ii=0 ; ii < n ; ii++ ){
       cp1 = cp0;  cp2 = cp0+15;
       while ( cp2 > cp1 )
       {
           tval = *cp1 ; *cp1 = *cp2 ; *cp2 = tval ;
           cp1++; cp2--;
       }
       cp0 += 16;
   }
   return ;
}
/*----------------------------------------------------------------------*/
/*! based on siz, call the appropriate nifti_swap_Nbytes() function
*//*--------------------------------------------------------------------*/
void grf_nifti_swap_Nbytes( int n , int siz , void *ar )  /* subsuming case */
{
   switch( siz ){
     case 2:  grf_nifti_swap_2bytes ( n , ar ) ; break ;
     case 4:  grf_nifti_swap_4bytes ( n , ar ) ; break ;
     case 8:  grf_nifti_swap_8bytes ( n , ar ) ; break ;
     case 16: grf_nifti_swap_16bytes( n , ar ) ; break ;
   }
   return ;
}
#ifdef isfinite       /* use isfinite() to check floats/doubles for goodness */
#  define IS_GOOD_FLOAT(x) isfinite(x)       /* check if x is a "good" float */
#  define FIXED_FLOAT(x)   (isfinite(x) ? (x) : 0)           /* fixed if bad */
#else
#  define IS_GOOD_FLOAT(x) 1                               /* don't check it */
#  define FIXED_FLOAT(x)   (x)                               /* don't fix it */
#endif
/*----------------------------------------------------------------------*/
/*! read ntot bytes of data from an open file and byte swaps if necessary

   note that nifti_image is required for information on datatype, bsize
   (for any needed byte swapping), etc.

   This function does not allocate memory, so dataptr must be valid.
*//*--------------------------------------------------------------------*/
size_t grf_nifti_read_buffer(GrfZnzFile* fp, GrfNiftiImage *nim)
{
  size_t ii;

  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
  GrfNDArray* array = GRF_NDARRAY(nim);
  if(grf_ndarray_data_allocated(array)){
     return -1;
  }

  void*  dataptr    = grf_ndarray_get_data(array);
  size_t ntot       = grf_ndarray_get_num_bytes(array);
  ii = grf_znzfile_read(fp, dataptr, 1, ntot);             /* data input */

  /* if read was short, fail */
  if( ii < ntot ){
    return -1 ;
  }

  /* byte swap array if needed */

  if( priv->swapsize > 1 && priv->byteorder != grf_nifti_short_order() )
    grf_nifti_swap_Nbytes( (int)(ntot / priv->swapsize ), priv->swapsize , dataptr ) ;

#ifdef isfinite
{
  /* check input float arrays for goodness, and fix bad floats */
  int fix_count = 0 ;

  switch( priv->datatype ){

    case GRF_NIFTI_TYPE_FLOAT32:
    case GRF_NIFTI_TYPE_COMPLEX64:{
        register float *far = (float *)dataptr ; register size_t jj,nj ;
        nj = ntot / sizeof(float) ;
        for( jj=0 ; jj < nj ; jj++ )   /* count fixes 30 Nov 2004 [rickr] */
           if( !IS_GOOD_FLOAT(far[jj]) ){
              far[jj] = 0 ;
              fix_count++ ;
           }
      }
      break ;

    case GRF_NIFTI_TYPE_FLOAT64:
    case GRF_NIFTI_TYPE_COMPLEX128:{
        register double *far = (double *)dataptr ; register size_t jj,nj ;
        nj = ntot / sizeof(double) ;
        for( jj=0 ; jj < nj ; jj++ )   /* count fixes 30 Nov 2004 [rickr] */
           if( !IS_GOOD_FLOAT(far[jj]) ){
              far[jj] = 0 ;
              fix_count++ ;
           }
      }
      break ;
  }
}
#endif

  return ii;
}

/*----------------------------------------------------------------------
 * grf_nifti_image_load
 *----------------------------------------------------------------------*/
/*! \fn int grf_nifti_image_load( nifti_image *nim )
    \brief Load the image blob into a previously initialized nifti_image.

        - If not yet set, the data buffer is allocated with calloc().
        - The data buffer will be byteswapped if necessary.
        - The data buffer will not be scaled.

    This function is used to read the image from disk.  It should be used
    after a function such as nifti_image_read(), so that the nifti_image
    structure is already initialized.

    \param  nim pointer to a nifti_image (previously initialized)
    \return 0 on success, -1 on failure
    \sa     nifti_image_read, nifti_image_free, nifti_image_unload
*/
int grf_nifti_image_load( GrfNiftiImage *nim )
{
   /* set up data space, open data file and seek, then call nifti_read_buffer */
   size_t ntot , ii ;
   GrfZnzFile* fp ;

   /**- open the file and position the FILE pointer */
   fp = grf_nifti_image_load_prep( nim );

   if( fp == NULL ){
      return -1;
   }

   ntot = grf_nifti_image_get_volsize(nim);

   /**- if the data pointer is not yet set, get memory space for the image */

   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   GrfNDArray* array = GRF_NDARRAY(nim);
   if(!grf_ndarray_data_allocated(array)){
     uint32_t size[3] = {priv->nsize.data[0],priv->nsize.data[1],priv->nsize.data[2]};
     grf_ndarray_alloc_data(GRF_NDARRAY(nim),3,size,grf_nifti_to_grf_datatype(priv->datatype));
     if(!grf_ndarray_data_allocated(array)){
       g_clear_object(&fp);
       return -1;
     }
   }
//   if(priv->data == NULL )
//   {
//     priv->data = (void *)calloc(1,ntot) ;  /* create image memory */
//     priv->array.data = priv->data;
//     priv->array.size = malloc(sizeof(uint32_t)*3);
//     priv->array.step = malloc(sizeof(uint64_t)*3);
//     priv->array.size[0] = priv->nz;
//     priv->array.size[1] = priv->ny;
//     priv->array.size[2] = priv->nx;
//     priv->array.step[2] = 1;
//     priv->array.step[1] = priv->nx;
//     priv->array.step[0] = priv->nx * priv->ny;
//     priv->array.num_elements = priv->nx * priv->ny * priv->nz;
//     priv->array.owns_data = 0;
//     priv->array.contiguous = 1;
//     priv->array.dim = 3;
//     switch(priv->datatype){
//       case GRF_DT_UINT8: priv->array.type = GRF_UINT8; priv->array.bitsize = 1;break;
//       case GRF_DT_UINT16:priv->array.type = GRF_UINT16;priv->array.bitsize = 2;break;
//       case GRF_DT_UINT32:priv->array.type = GRF_UINT32;priv->array.bitsize = 4;break;
//       case GRF_DT_UINT64:priv->array.type = GRF_UINT64;priv->array.bitsize = 8;break;
//       case GRF_DT_INT8:  priv->array.type = GRF_INT8;  priv->array.bitsize = 1;break;
//       case GRF_DT_INT16: priv->array.type = GRF_INT16; priv->array.bitsize = 2;break;
//       case GRF_DT_INT32: priv->array.type = GRF_INT32; priv->array.bitsize = 4;break;
//       case GRF_DT_INT64: priv->array.type = GRF_INT64; priv->array.bitsize = 8;break;
//       case GRF_DT_FLOAT: priv->array.type = GRF_FLOAT; priv->array.bitsize = 4;break;
//       case GRF_DT_DOUBLE:priv->array.type = GRF_DOUBLE;priv->array.bitsize = 8;break;
//     }
//     priv->array.num_bytes = priv->array.num_elements * priv->array.bitsize;
//     if( priv->data == NULL ){
//       g_clear_object(fp);
//       return -1;
//     }
//   }

   /**- now that everything is set up, do the reading */
   ii = grf_nifti_read_buffer(fp,nim);
   if( ii < ntot ){
     g_clear_object(&fp);
     grf_ndarray_dealloc_data(array);
     return -1 ;  /* errors were printed in nifti_read_buffer() */
   }

   /**- close the file */
   g_clear_object(&fp);

   return 0 ;
}
/*----------------------------------------------------------------------*/
/*! grf_nifti_read_ascii_image  - process as a type-3 .nia image file

   return NULL on failure

   NOTE: this is NOT part of the NIFTI-1 standard
*//*--------------------------------------------------------------------*/
GrfNiftiImage * grf_nifti_read_ascii_image(GrfZnzFile* fp, char *fname, int flen,
                                     int read_data)
{
   GrfNiftiImage   * nim;
   int               slen, txt_size, remain, rv = 0;
   g_autofree char * sbuf;
   char              lfunc[25] = { "nifti_read_ascii_image" };

   if(grf_nifti_is_gzfile(fname))
     return NULL;
   slen = flen;  /* slen will be our buffer length */

   if( slen > 65530 ) slen = 65530;
   sbuf = (char *)g_malloc0(sizeof(char)*(slen+1));
   if(!sbuf){
      fprintf(stderr,"** %s: failed to alloc %d bytes for sbuf",lfunc,65530);
      return NULL;
   }
   grf_znzfile_read_header(fp, sbuf , 1 , slen);
   nim = grf_nifti_image_from_ascii( sbuf, &txt_size ) ; free( sbuf ) ;
   if( nim == NULL ){
     g_clear_object(&fp);
     return NULL;
   }
   GrfNiftiImagePrivate *priv = grf_nifti_image_get_instance_private(nim);
   priv->nifti_type = GRF_NIFTI_TYPE_ASCII;

   /* compute remaining space for extensions */
   remain = flen - txt_size - (int)grf_nifti_image_get_volsize(nim);
   if( remain > 4 ){
      /* read extensions (reposition file pointer, first) */
      grf_znzfile_seek(fp, txt_size, SEEK_SET);
      (void) grf_nifti_read_extensions(nim, fp, remain);
   }

   g_clear_object(&fp);

   priv->iname_offset = -1 ;  /* check from the end of the file */

   if(read_data) rv = grf_nifti_image_load(nim);

   /* check for nifti_image_load() failure, maybe bail out */
   if( read_data && rv != 0 ){
     g_clear_object(&nim);
     return NULL;
   }

   return nim ;
}

/*----------------------------------------------------------------------
 * check whether byte swapping is needed
 *
 * dim[0] should be in [0,7], and sizeof_hdr should be accurate
 *
 * \returns  > 0 : needs swap
 *             0 : does not need swap
 *           < 0 : error condition
 *----------------------------------------------------------------------*/
static int need_nhdr_swap( short dim0, int hdrsize )
{
   short d0    = dim0;     /* so we won't have to swap them on the stack */
   int   hsize = hdrsize;

   if( d0 != 0 ){     /* then use it for the check */
      if( d0 > 0 && d0 <= 7 ) return 0;

      grf_nifti_swap_2bytes(1, &d0);        /* swap? */
      if( d0 > 0 && d0 <= 7 ) return 1;

      return -1;        /* bad, naughty d0 */
   }

   /* dim[0] == 0 should not happen, but could, so try hdrsize */
   if( hsize == sizeof(GrfNifti1Header) ) return 0;

   grf_nifti_swap_4bytes(1, &hsize);     /* swap? */
   if( hsize == sizeof(GrfNifti1Header) ) return 1;

   return -2;     /* bad, naughty hsize */
}
/*-------------------------------------------------------------------------*/
/*! Byte swap as an ANALYZE 7.5 header
 *
 *  return non-zero on failure
*//*---------------------------------------------------------------------- */
int grf_nifti_swap_as_analyze( GrfNiftiAnalyze75 * h )
{
   if( !h ) return 1;

   grf_nifti_swap_4bytes(1, &h->sizeof_hdr);
   grf_nifti_swap_4bytes(1, &h->extents);
   grf_nifti_swap_2bytes(1, &h->session_error);

   grf_nifti_swap_2bytes(8, h->dim);
   grf_nifti_swap_2bytes(1, &h->unused8);
   grf_nifti_swap_2bytes(1, &h->unused9);
   grf_nifti_swap_2bytes(1, &h->unused10);
   grf_nifti_swap_2bytes(1, &h->unused11);
   grf_nifti_swap_2bytes(1, &h->unused12);
   grf_nifti_swap_2bytes(1, &h->unused13);
   grf_nifti_swap_2bytes(1, &h->unused14);

   grf_nifti_swap_2bytes(1, &h->datatype);
   grf_nifti_swap_2bytes(1, &h->bitpix);
   grf_nifti_swap_2bytes(1, &h->dim_un0);

   grf_nifti_swap_4bytes(8, h->pixdim);

   grf_nifti_swap_4bytes(1, &h->vox_offset);
   grf_nifti_swap_4bytes(1, &h->funused1);
   grf_nifti_swap_4bytes(1, &h->funused2);
   grf_nifti_swap_4bytes(1, &h->funused3);

   grf_nifti_swap_4bytes(1, &h->cal_max);
   grf_nifti_swap_4bytes(1, &h->cal_min);
   grf_nifti_swap_4bytes(1, &h->compressed);
   grf_nifti_swap_4bytes(1, &h->verified);

   grf_nifti_swap_4bytes(1, &h->glmax);
   grf_nifti_swap_4bytes(1, &h->glmin);

   grf_nifti_swap_4bytes(1, &h->views);
   grf_nifti_swap_4bytes(1, &h->vols_added);
   grf_nifti_swap_4bytes(1, &h->start_field);
   grf_nifti_swap_4bytes(1, &h->field_skip);

   grf_nifti_swap_4bytes(1, &h->omax);
   grf_nifti_swap_4bytes(1, &h->omin);
   grf_nifti_swap_4bytes(1, &h->smax);
   grf_nifti_swap_4bytes(1, &h->smin);

   return 0;
}
/*-------------------------------------------------------------------------*/
/*! Byte swap NIFTI-1 file header in various places and ways.

    If is_nifti, swap all (even UNUSED) fields of NIfTI header.
    Else, swap as a nifti_analyze75 struct.
*//*---------------------------------------------------------------------- */
void grf_swap_nifti_header( struct GrfNifti1Header *h , int is_nifti )
{

   /* if ANALYZE, swap as such and return */
   if( ! is_nifti ) {
      grf_nifti_swap_as_analyze((GrfNiftiAnalyze75 *)h);
      return;
   }

   /* otherwise, swap all NIFTI fields */

   grf_nifti_swap_4bytes(1, &h->sizeof_hdr);
   grf_nifti_swap_4bytes(1, &h->extents);
   grf_nifti_swap_2bytes(1, &h->session_error);

   grf_nifti_swap_2bytes(8, h->dim);
   grf_nifti_swap_4bytes(1, &h->intent_p1);
   grf_nifti_swap_4bytes(1, &h->intent_p2);
   grf_nifti_swap_4bytes(1, &h->intent_p3);

   grf_nifti_swap_2bytes(1, &h->intent_code);
   grf_nifti_swap_2bytes(1, &h->datatype);
   grf_nifti_swap_2bytes(1, &h->bitpix);
   grf_nifti_swap_2bytes(1, &h->slice_start);

   grf_nifti_swap_4bytes(8, h->pixdim);

   grf_nifti_swap_4bytes(1, &h->vox_offset);
   grf_nifti_swap_4bytes(1, &h->scl_slope);
   grf_nifti_swap_4bytes(1, &h->scl_inter);
   grf_nifti_swap_2bytes(1, &h->slice_end);

   grf_nifti_swap_4bytes(1, &h->cal_max);
   grf_nifti_swap_4bytes(1, &h->cal_min);
   grf_nifti_swap_4bytes(1, &h->slice_duration);
   grf_nifti_swap_4bytes(1, &h->toffset);
   grf_nifti_swap_4bytes(1, &h->glmax);
   grf_nifti_swap_4bytes(1, &h->glmin);

   grf_nifti_swap_2bytes(1, &h->qform_code);
   grf_nifti_swap_2bytes(1, &h->sform_code);

   grf_nifti_swap_4bytes(1, &h->quatern_b);
   grf_nifti_swap_4bytes(1, &h->quatern_c);
   grf_nifti_swap_4bytes(1, &h->quatern_d);
   grf_nifti_swap_4bytes(1, &h->qoffset_x);
   grf_nifti_swap_4bytes(1, &h->qoffset_y);
   grf_nifti_swap_4bytes(1, &h->qoffset_z);

   grf_nifti_swap_4bytes(4, h->srow_x);
   grf_nifti_swap_4bytes(4, h->srow_y);
   grf_nifti_swap_4bytes(4, h->srow_z);

   return ;
}
/*--------------------------------------------------------------------------*/
/*! check whether the given type is on the "approved" list

    The code is valid if it is non-negative, and does not exceed
    NIFTI_MAX_FTYPE.

    \return 1 if nifti_type is valid, 0 otherwise
    \sa NIFTI_FTYPE_* codes in nifti1_io.h
*//*------------------------------------------------------------------------*/
int grf_is_valid_nifti_type( int nifti_type )
{
   if( nifti_type >= GRF_NIFTI_TYPE_ANALYZE &&   /* smallest type, 0 */
       nifti_type <= GRF_NIFTI_TYPE_MAX)
      return 1;
   return 0;
}
/*----------------------------------------------------------------------*/
/*! creates a filename for storing the header, based on nifti_type

   \param   prefix      - this will be copied before the suffix is added
   \param   nifti_type  - determines the extension, unless one is in prefix
   \param   check       - check for existence (fail condition)
   \param   comp        - add .gz for compressed name

   Note that if prefix provides a file suffix, nifti_type is not used.

   NB: this allocates memory which should be freed

   \sa nifti_set_filenames
*//*-------------------------------------------------------------------*/
char * grf_nifti_makehdrname(const char * prefix, int nifti_type, int check,
                         int comp)
{
   char * iname, * ext;

   if( !grf_nifti_validfilename(prefix) ) return NULL;

   /* add space for extension, optional ".gz", and null char */
   iname = (char *)calloc(sizeof(char),strlen(prefix)+8);
   if( !iname ){ fprintf(stderr,"** small malloc failure!\n"); return NULL; }
   strcpy(iname, prefix);

   /* use any valid extension */
   if( (ext = grf_nifti_find_file_extension(iname)) != NULL ){
      if( strncmp(ext,".img",4) == 0 )
         memcpy(ext,".hdr",4);   /* then convert img name to hdr */
   }
   /* otherwise, make one up an */
   else if( nifti_type == GRF_NIFTI_FTYPE_NIFTI1_1 ) strcat(iname, ".nii");
   else if( nifti_type == GRF_NIFTI_FTYPE_ASCII )    strcat(iname, ".nia");
   else                                          strcat(iname, ".hdr");

#ifdef HAVE_ZLIB  /* then also check for .gz */
   if( comp && (!ext || !strstr(iname,".gz")) ) strcat(iname,".gz");
#endif

   /* check for existence failure */
   if( check && grf_nifti_fileexists(iname) ){
      fprintf(stderr,"** failure: header file '%s' already exists\n",iname);
      free(iname);
      return NULL;
   }


   return iname;
}


/*----------------------------------------------------------------------*/
/*! creates a filename for storing the image, based on nifti_type

   \param   prefix      - this will be copied before the suffix is added
   \param   nifti_type  - determines the extension, unless provided by prefix
   \param   check       - check for existence (fail condition)
   \param   comp        - add .gz for compressed name

   Note that if prefix provides a file suffix, nifti_type is not used.

   NB: it allocates memory which should be freed

   \sa nifti_set_filenames
*//*-------------------------------------------------------------------*/
char * grf_nifti_makeimgname(const char * prefix, int nifti_type, int check,
                         int comp)
{
   char * iname, * ext;

   if( !grf_nifti_validfilename(prefix) ) return NULL;

   /* add space for extension, optional ".gz", and null char */
   iname = (char *)calloc(sizeof(char),strlen(prefix)+8);
   if( !iname ){ fprintf(stderr,"** small malloc failure!\n"); return NULL; }
   strcpy(iname, prefix);

   /* use any valid extension */
   if( (ext = grf_nifti_find_file_extension(iname)) != NULL ){
      if( strncmp(ext,".hdr",4) == 0 )
         memcpy(ext,".img",4);   /* then convert hdr name to img */
   }
   /* otherwise, make one up */
   else if( nifti_type == GRF_NIFTI_FTYPE_NIFTI1_1 ) strcat(iname, ".nii");
   else if( nifti_type == GRF_NIFTI_FTYPE_ASCII )    strcat(iname, ".nia");
   else                                          strcat(iname, ".img");

#ifdef HAVE_ZLIB  /* then also check for .gz */
   if( comp && (!ext || !strstr(iname,".gz")) ) strcat(iname,".gz");
#endif

   /* check for existence failure */
   if( check && grf_nifti_fileexists(iname) ){
      fprintf(stderr,"** failure: image file '%s' already exists\n",iname);
      free(iname);
      return NULL;
   }


   return iname;
}
/*--------------------------------------------------------------------------*/
/*! set the nifti_type field based on fname and iname

    Note that nifti_type is changed only when it does not match
    the filenames.

    \return 0 on success, -1 on error

    \sa is_valid_nifti_type, nifti_type_and_names_match
*//*------------------------------------------------------------------------*/
int grf_nifti_set_type_from_names( GrfNiftiImage * nim )
{
   /* error checking first */
   if( !nim ){ fprintf(stderr,"** NSTFN: no nifti_image\n");  return -1; }
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);

   if( !priv->fname || !priv->iname ){
      fprintf(stderr,"** NSTFN: missing filename(s) fname @ %p, iname @ %p\n",
              priv->fname, priv->iname);
      return -1;
   }

   if( ! grf_nifti_validfilename      ( priv->fname ) ||
       ! grf_nifti_validfilename      ( priv->iname ) ||
       ! grf_nifti_find_file_extension( priv->fname ) ||
       ! grf_nifti_find_file_extension( priv->iname )
     ) {
      fprintf(stderr,"** NSTFN: invalid filename(s) fname='%s', iname='%s'\n",
              priv->fname, priv->iname);
      return -1;
   }


   /* type should be NIFTI_FTYPE_ASCII if extension is .nia */
   if( (strcmp(grf_nifti_find_file_extension( priv->fname ),".nia")==0) ) {
      priv->nifti_type = GRF_NIFTI_TYPE_ASCII;
   } else {
      /* not too picky here, do what must be done, and then verify */
      if( strcmp(priv->fname, priv->iname) == 0 )          /* one file, type 1 */
         priv->nifti_type = GRF_NIFTI_TYPE_NIFTI1_1;
      else if( priv->nifti_type == GRF_NIFTI_TYPE_NIFTI1_1 ) /* cannot be type 1 */
         priv->nifti_type = GRF_NIFTI_TYPE_NIFTI1_2;
   }

   if( grf_is_valid_nifti_type(priv->nifti_type) ) return 0;  /* success! */

   fprintf(stderr,"** NSTFN: bad nifti_type %d, for '%s' and '%s'\n",
           priv->nifti_type, priv->fname, priv->iname);

   return -1;
}
/*----------------------------------------------------------------------*/
/*! create and set new filenames, based on prefix and image type

   \param nim            pointer to nifti_image in which to set filenames
   \param prefix         (required) prefix for output filenames
   \param check          check for previous existence of filename
                         (existence is an error condition)
   \param set_byte_order flag to set nim->byteorder here
                         (this is probably a logical place to do so)

   \return 0 on successful update

   \warning this will free() any existing names and create new ones

   \sa nifti_makeimgname, nifti_makehdrname, nifti_type_and_names_match
*//*--------------------------------------------------------------------*/
int grf_nifti_set_filenames( GrfNiftiImage * nim, const char * prefix, int check,
                         int set_byte_order )
{
   int comp = grf_nifti_is_gzfile(prefix);

   if( !nim || !prefix ){
      fprintf(stderr,"** nifti_set_filenames, bad params %p, %p\n",
              (void *)nim,prefix);
      return -1;
   }
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   g_autofree char* fname = grf_nifti_makehdrname(prefix, priv->nifti_type, check, comp);
   g_autofree char* iname = grf_nifti_makeimgname(prefix, priv->nifti_type, check, comp);
   grf_nifti_image_set_filename(nim,fname);
   grf_nifti_image_set_iname(nim,iname);

   if( !priv->fname || !priv->iname ){
      return -1;
   }

   if( set_byte_order ) priv->byteorder = grf_nifti_short_order() ;

   if( grf_nifti_set_type_from_names(nim) < 0 )
      return -1;


   return 0;
}
#define REVERSE_ORDER(x) (3-(x))    /* convert MSB_FIRST <--> LSB_FIRST */
/*----------------------------------------------------------------------*/
/*! convert a nifti_1_header into a nift1_image

   \return an allocated nifti_image, or NULL on failure
*//*--------------------------------------------------------------------*/
GrfNiftiImage* grf_nifti_convert_nhdr2nim(struct GrfNifti1Header nhdr,
                                    const char * fname)
{
   int   ii , doswap , ioff ;
   int   is_nifti , is_onefile ;
   GrfNiftiImage *nim = grf_nifti_image_new();
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);

   /**- check if we must swap bytes */

   doswap = need_nhdr_swap(nhdr.dim[0], nhdr.sizeof_hdr); /* swap data flag */

   /**- determine if this is a NIFTI-1 compliant header */

   is_nifti = GRF_NIFTI_VERSION(nhdr);
   /*
    * before swapping header, record the Analyze75 orient code
    */
   if(!is_nifti)
     {
     /**- in analyze75, the orient code is at the same address as
      *   qform_code, but it's just one byte
      *   the qform_code will be zero, at which point you can check
      *   analyze75_orient if you care to.
      */
     unsigned char c = *((char *)(&nhdr.qform_code));
     priv->analyze75_orient = (GrfAnalyze75OrientCode)c;
     }
   if( doswap ) {
      grf_swap_nifti_header( &nhdr , is_nifti ) ;
   }

   /* fix bad dim[] values in the defined dimension range */
   for( ii=2 ; ii <= nhdr.dim[0] ; ii++ )
     if( nhdr.dim[ii] <= 0 ) nhdr.dim[ii] = 1 ;

   /* fix any remaining bad dim[] values, so garbage does not propagate */
   /* (only values 0 or 1 seem rational, otherwise set to arbirary 1)   */
   for( ii=nhdr.dim[0]+1 ; ii <= 7 ; ii++ )
     if( nhdr.dim[ii] != 1 && nhdr.dim[ii] != 0) nhdr.dim[ii] = 1 ;

#if 0  /* rely on dim[0], do not attempt to modify it   16 Nov 2005 [rickr] */

   /**- get number of dimensions (ignoring dim[0] now) */
   for( ii=7 ; ii >= 2 ; ii-- )            /* loop backwards until we  */
     if( nhdr.dim[ii] > 1 ) break ;        /* find a dim bigger than 1 */
   ndim = ii ;
#endif

   /**- set bad grid spacings to 1.0 */

   for( ii=1 ; ii <= nhdr.dim[0] ; ii++ ){
     if( nhdr.pixdim[ii] == 0.0         ||
         !IS_GOOD_FLOAT(nhdr.pixdim[ii])  ) nhdr.pixdim[ii] = 1.0 ;
   }

  is_onefile = is_nifti && GRF_NIFTI_ONEFILE(nhdr) ;

  if( is_nifti ) priv->nifti_type = (is_onefile) ? GRF_NIFTI_TYPE_NIFTI1_1
                                                : GRF_NIFTI_TYPE_NIFTI1_2 ;
  else           priv->nifti_type = GRF_NIFTI_TYPE_ANALYZE ;

  ii = grf_nifti_short_order() ;
  if( doswap )   priv->byteorder = REVERSE_ORDER(ii) ;
  else           priv->byteorder = ii ;


  /**- set dimensions of data array */

  priv->ndim          = priv->dim[0] = nhdr.dim[0];
  priv->nsize.data[0] = priv->dim[1] = nhdr.dim[1];
  priv->nsize.data[1] = priv->dim[2] = nhdr.dim[2];
  priv->nsize.data[2] = priv->dim[3] = nhdr.dim[3];
  priv->nsize.data[3] = priv->dim[4] = nhdr.dim[4];
  priv->nsize.data[4] = priv->dim[5] = nhdr.dim[5];
  priv->nsize.data[5] = priv->dim[6] = nhdr.dim[6];
  priv->nsize.data[6] = priv->dim[7] = nhdr.dim[7];

  for( ii=1, priv->nvox=1; ii <= nhdr.dim[0]; ii++ )
     priv->nvox *= nhdr.dim[ii];

  /**- set the type of data in voxels and how many bytes per voxel */

  priv->datatype = nhdr.datatype ;

  grf_nifti_datatype_sizes( priv->datatype , &(priv->nbyper) , &(priv->swapsize) ) ;
  if( priv->nbyper == 0 ){
    g_clear_object(&nim);
    return NULL;
  }

  /**- set the grid spacings */

  priv->dsize.data[0] = priv->pixdim[1] = nhdr.pixdim[1] ;
  priv->dsize.data[1] = priv->pixdim[2] = nhdr.pixdim[2] ;
  priv->dsize.data[2] = priv->pixdim[3] = nhdr.pixdim[3] ;
  priv->dsize.data[3] = priv->pixdim[4] = nhdr.pixdim[4] ;
  priv->dsize.data[4] = priv->pixdim[5] = nhdr.pixdim[5] ;
  priv->dsize.data[5] = priv->pixdim[6] = nhdr.pixdim[6] ;
  priv->dsize.data[6] = priv->pixdim[7] = nhdr.pixdim[7] ;

  /**- compute qto_xyz transformation from pixel indexes (i,j,k) to (x,y,z) */

  if( !is_nifti || nhdr.qform_code <= 0 ){
    /**- if not nifti or qform_code <= 0, use grid spacing for qto_xyz */

    priv->qto_xyz.data[0]  = priv->dsize.data[0] ;  /* grid spacings */
    priv->qto_xyz.data[5]  = priv->dsize.data[1] ;  /* along diagonal */
    priv->qto_xyz.data[10] = priv->dsize.data[2] ;

    /* off diagonal is zero */

    priv->qto_xyz.data[4]=priv->qto_xyz.data[8]=priv->qto_xyz.data[12] = 0.0;
    priv->qto_xyz.data[5]=priv->qto_xyz.data[9]=priv->qto_xyz.data[13] = 0.0;
    priv->qto_xyz.data[6]=priv->qto_xyz.data[10]=priv->qto_xyz.data[14] = 0.0;

    /* last row is always [ 0 0 0 1 ] */

    priv->qto_xyz.data[3]=priv->qto_xyz.data[7]=priv->qto_xyz.data[11] = 0.0;
    priv->qto_xyz.data[15]= 1.0 ;

    priv->qform_code = GRF_NIFTI_XFORM_UNKNOWN ;
  } else {
    /**- else NIFTI: use the quaternion-specified transformation */

    priv->quatern_b = FIXED_FLOAT( nhdr.quatern_b ) ;
    priv->quatern_c = FIXED_FLOAT( nhdr.quatern_c ) ;
    priv->quatern_d = FIXED_FLOAT( nhdr.quatern_d ) ;

    priv->qoffset_x = FIXED_FLOAT(nhdr.qoffset_x) ;
    priv->qoffset_y = FIXED_FLOAT(nhdr.qoffset_y) ;
    priv->qoffset_z = FIXED_FLOAT(nhdr.qoffset_z) ;

    priv->qfac = (nhdr.pixdim[0] < 0.0) ? -1.0 : 1.0 ;  /* left-handedness? */

    priv->qto_xyz = grf_nifti_quatern_to_mat4(
                      priv->quatern_b, priv->quatern_c, priv->quatern_d,
                      priv->qoffset_x, priv->qoffset_y, priv->qoffset_z,
                      priv->dsize.data[0] , priv->dsize.data[1] , priv->dsize.data[2],
                      priv->qfac                                      ) ;

    priv->qform_code = nhdr.qform_code ;
  }

  /**- load inverse transformation (x,y,z) -> (i,j,k) */

  priv->qto_ijk = grf_nifti_mat4_inverse( priv->qto_xyz ) ;

  /**- load sto_xyz affine transformation, if present */

  if( !is_nifti || nhdr.sform_code <= 0 ){
    /**- if not nifti or sform_code <= 0, then no sto transformation */

    priv->sform_code = GRF_NIFTI_XFORM_UNKNOWN ;



  } else {
    /**- else set the sto transformation from srow_*[] */

    priv->sto_xyz.data[ 0] = nhdr.srow_x[0] ;
    priv->sto_xyz.data[ 4] = nhdr.srow_x[1] ;
    priv->sto_xyz.data[ 8] = nhdr.srow_x[2] ;
    priv->sto_xyz.data[12] = nhdr.srow_x[3] ;

    priv->sto_xyz.data[ 1] = nhdr.srow_y[0] ;
    priv->sto_xyz.data[ 5] = nhdr.srow_y[1] ;
    priv->sto_xyz.data[ 9] = nhdr.srow_y[2] ;
    priv->sto_xyz.data[13] = nhdr.srow_y[3] ;

    priv->sto_xyz.data[ 2] = nhdr.srow_z[0] ;
    priv->sto_xyz.data[ 6] = nhdr.srow_z[1] ;
    priv->sto_xyz.data[10] = nhdr.srow_z[2] ;
    priv->sto_xyz.data[14] = nhdr.srow_z[3] ;

    /* last row is always [ 0 0 0 1 ] */

    priv->sto_xyz.data[3] = priv->sto_xyz.data[7]=priv->sto_xyz.data[11] = 0.0;
    priv->sto_xyz.data[15]= 1.0 ;

    priv->sto_ijk = grf_nifti_mat4_inverse( priv->sto_xyz ) ;

    priv->sform_code = nhdr.sform_code ;
  }

  /**- set miscellaneous NIFTI stuff */

  if( is_nifti ){
    priv->scl_slope   = FIXED_FLOAT( nhdr.scl_slope ) ;
    priv->scl_inter   = FIXED_FLOAT( nhdr.scl_inter ) ;

    priv->intent_code = nhdr.intent_code ;

    priv->intent_p1 = FIXED_FLOAT( nhdr.intent_p1 ) ;
    priv->intent_p2 = FIXED_FLOAT( nhdr.intent_p2 ) ;
    priv->intent_p3 = FIXED_FLOAT( nhdr.intent_p3 ) ;

    priv->toffset   = FIXED_FLOAT( nhdr.toffset ) ;

    memcpy(priv->intent_name,nhdr.intent_name,15); priv->intent_name[15] = '\0';

    priv->xyz_units  = GRF_XYZT_TO_SPACE(nhdr.xyzt_units) ;
    priv->time_units = GRF_XYZT_TO_TIME (nhdr.xyzt_units) ;

    priv->freq_dim  = GRF_DIM_INFO_TO_FREQ_DIM ( nhdr.dim_info ) ;
    priv->phase_dim = GRF_DIM_INFO_TO_PHASE_DIM( nhdr.dim_info ) ;
    priv->slice_dim = GRF_DIM_INFO_TO_SLICE_DIM( nhdr.dim_info ) ;

    priv->slice_code     = nhdr.slice_code  ;
    priv->slice_start    = nhdr.slice_start ;
    priv->slice_end      = nhdr.slice_end   ;
    priv->slice_duration = FIXED_FLOAT(nhdr.slice_duration) ;
  }

  /**- set Miscellaneous ANALYZE stuff */

  priv->cal_min = FIXED_FLOAT(nhdr.cal_min) ;
  priv->cal_max = FIXED_FLOAT(nhdr.cal_max) ;

  memcpy(priv->descrip ,nhdr.descrip ,79) ; priv->descrip [79] = '\0' ;
  memcpy(priv->aux_file,nhdr.aux_file,23) ; priv->aux_file[23] = '\0' ;

   /**- set ioff from vox_offset (but at least sizeof(header)) */

   is_onefile = is_nifti && GRF_NIFTI_ONEFILE(nhdr) ;

   if( is_onefile ){
     ioff = (int)nhdr.vox_offset ;
     if( ioff < (int) sizeof(nhdr) ) ioff = (int) sizeof(nhdr) ;
   } else {
     ioff = (int)nhdr.vox_offset ;
   }
   priv->iname_offset = ioff ;


   /**- deal with file names if set */
   if (fname!=NULL) {
       grf_nifti_set_filenames(nim,fname,0,0);
       if (priv->iname==NULL)  { g_clear_object(&nim);return NULL; }
   } else {
     priv->fname = NULL;
     priv->iname = NULL;
   }

   /* clear extension fields */
   priv->num_ext = 0;
   priv->ext_list = NULL;

   return nim;
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfNiftiImage *grf_nifti_image_read( const char *hname , gboolean read_data )
{
   struct GrfNifti1Header  nhdr;
   GrfNiftiImage          *nim;
   GrfZnzFile*             fp = NULL;
   int                     rv, ii , filesize, remaining;
   g_autofree char        *hfile=NULL;

   /**- determine filename to use for header */
   hfile = grf_nifti_findhdrname(hname);
   if( hfile == NULL ){
      return NULL;  /* check return */
   }

   if( grf_nifti_is_gzfile(hfile) ) filesize = -1;  /* unknown */
   else                         filesize = grf_nifti_get_filesize(hfile);

   fp = grf_znzfile_open(hfile, "rb", grf_nifti_is_gzfile(hfile));
   if(fp == NULL) return NULL;

   rv = grf_nifti_has_ascii_header( fp );
   if(rv < 0){
     g_clear_object(&fp);
     return NULL;
   }
   else if ( rv == 1 )  /* process special file type */
      return grf_nifti_read_ascii_image( fp, hfile, filesize, read_data );

   /* else, just process normally */

   /**- read binary header */

   ii = (int)grf_znzfile_read(fp, &nhdr , 1 , sizeof(nhdr) ) ;       /* read the thing */

   /* keep file open so we can check for exts. after nifti_convert_nhdr2nim() */

   if( ii < (int) sizeof(nhdr) ){
      g_clear_object(&fp);
      return NULL;
   }

   /* create output image struct and set it up */

   /**- convert all nhdr fields to nifti_image fields */
   nim = grf_nifti_convert_nhdr2nim(nhdr,hfile);

   if( nim == NULL ){
      g_clear_object(&fp);                                   /* close the file */
      return NULL;
   }
   GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
   /**- check for extensions (any errors here means no extensions) */
   if( GRF_NIFTI_ONEFILE(nhdr) ) remaining = priv->iname_offset - sizeof(nhdr);
   else                      remaining = filesize - sizeof(nhdr);

   (void)grf_nifti_read_extensions(nim, fp, remaining);

   g_clear_object(&fp);                                      /* close the file */

   /**- read the data if desired, then bug out */
   if( read_data ){
      if( grf_nifti_image_load( nim ) < 0 ){
        g_clear_object(&nim);
        return NULL;
      }
   }
   else {
     grf_ndarray_dealloc_data(GRF_NDARRAY(nim));
   }

   return nim ;
}

GrfVec7
grf_nifti_image_get_nsize(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->nsize;
}

GrfVec7
grf_nifti_image_get_dsize(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->dsize;
}

float
grf_nifti_image_get_qfac(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->qfac;
}

GrfEndianess
grf_nifti_image_get_byteorder(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->byteorder;
}

GrfNiftiFileType
grf_nifti_image_get_filetype(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->nifti_type;
}

int
grf_nifti_image_get_ndim(GrfNiftiImage* image){
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(image);
  return priv->ndim;
}
size_t
grf_nifti_image_get_volsize(GrfNiftiImage *nim)
{
  GrfNiftiImagePrivate* priv = grf_nifti_image_get_instance_private(nim);
  return (size_t)(priv->nbyper) * (size_t)(priv->nvox) ; /* total bytes */
}
