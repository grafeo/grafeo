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
#ifndef GRF_NIFTI1_IO_H
#define GRF_NIFTI1_IO_H
#include <grafeo/core.h>
#include <grafeo/medical.h>
BEGIN_DECLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*===========================================================================
 * ENUMS AND MACROS
 *===========================================================================*/
/*! \enum analyze_75_orient_code
 *  \brief Old-style analyze75 orientation
 *         codes.
 */
typedef enum _GrfAnalyze75OrientCode {
  GRF_A75_TRANSVERSE_UNFLIPPED = 0,
  GRF_A75_CORONAL_UNFLIPPED    = 1,
  GRF_A75_SAGITTAL_UNFLIPPED   = 2,
  GRF_A75_TRANSVERSE_FLIPPED   = 3,
  GRF_A75_CORONAL_FLIPPED      = 4,
  GRF_A75_SAGITTAL_FLIPPED     = 5,
  GRF_A75_ORIENT_UNKNOWN       = 6
} GrfAnalyze75OrientCode;

/*-------------------- Some C convenience macros ----------------------------*/

/* NIfTI-1.1 extension codes:
   see http://nifti.nimh.nih.gov/nifti-1/documentation/faq#Q21 */

#define GRF_NIFTI_ECODE_IGNORE           0  /* changed from UNKNOWN, 29 June 2005 */
#define GRF_NIFTI_ECODE_DICOM            2  /* intended for raw DICOM attributes  */
#define GRF_NIFTI_ECODE_AFNI             4  /* Robert W Cox: rwcox@nih.gov
                                           http://afni.nimh.nih.gov/afni      */
#define GRF_NIFTI_ECODE_COMMENT          6  /* plain ASCII text only              */
#define GRF_NIFTI_ECODE_XCEDE            8  /* David B Keator: dbkeator@uci.edu
                                           http://www.nbirn.net/Resources
                                                /Users/Applications/
                                                /xcede/index.htm              */
#define GRF_NIFTI_ECODE_JIMDIMINFO      10  /* Mark A Horsfield:
                                           mah5@leicester.ac.uk
                                           http://someplace/something         */
#define GRF_NIFTI_ECODE_WORKFLOW_FWDS   12  /* Kate Fissell: fissell@pitt.edu
                                           http://kraepelin.wpic.pitt.edu
                                            /~fissell/NIFTI_ECODE_WORKFLOW_FWDS
                                            /NIFTI_ECODE_WORKFLOW_FWDS.html   */
#define GRF_NIFTI_ECODE_FREESURFER      14  /* http://surfer.nmr.mgh.harvard.edu  */
#define GRF_NIFTI_ECODE_PYPICKLE        16  /* embedded Python objects
                                           http://niftilib.sourceforge.net
                                                 /pynifti                     */
/* LONI MiND codes: http://www.loni.ucla.edu/twiki/bin/view/Main/MiND */
#define GRF_NIFTI_ECODE_MIND_IDENT      18  /* Vishal Patel: vishal.patel@ucla.edu*/
#define GRF_NIFTI_ECODE_B_VALUE         20
#define GRF_NIFTI_ECODE_SPHERICAL_DIRECTION 22
#define GRF_NIFTI_ECODE_DT_COMPONENT    24
#define GRF_NIFTI_ECODE_SHC_DEGREEORDER 26  /* end LONI MiND codes                */
#define GRF_NIFTI_MAX_ECODE             26  /******* maximum extension code *******/

/* nifti_type file codes */
#define GRF_NIFTI_FTYPE_ANALYZE   0
#define GRF_NIFTI_FTYPE_NIFTI1_1  1
#define GRF_NIFTI_FTYPE_NIFTI1_2  2
#define GRF_NIFTI_FTYPE_ASCII     3
#define GRF_NIFTI_MAX_FTYPE       3    /* this should match the maximum code */

/* Orientation codes that might be returned from nifti_mat44_to_orientation().*/

#define GRF_NIFTI_L2R  1    /* Left to Right         */
#define GRF_NIFTI_R2L  2    /* Right to Left         */
#define GRF_NIFTI_P2A  3    /* Posterior to Anterior */
#define GRF_NIFTI_A2P  4    /* Anterior to Posterior */
#define GRF_NIFTI_I2S  5    /* Inferior to Superior  */
#define GRF_NIFTI_S2I  6    /* Superior to Inferior  */

/*===========================================================================
 * CLASS DECLARATION
 *===========================================================================*/
#define GRF_TYPE_NIFTI_IMAGE grf_nifti_image_get_type()
G_DECLARE_DERIVABLE_TYPE(GrfNiftiImage, grf_nifti_image, GRF, NIFTI_IMAGE, GrfNDArray)

typedef struct _GrfNiftiImageClass{
  GrfNDArrayClass parent_class;
}GrfNiftiImageClass;

/*===========================================================================
 * MEMBERS
 *===========================================================================*/
/**
 * @brief Reads a Nifti file and optionally the data, creating a
 *        GrfNiftiImage object.
 *
 * - The data buffer will be byteswapped if necessary.
 * - The data buffer will not be scaled.
 * - The data buffer is allocated with calloc().
 *
 * @param hname filename of the nifti dataset
 * @param read_data Flag, true=read data blob, false=don't read blob.
 * @return A pointer to the GrfNiftiImage data structure.
 */
GrfNiftiImage*
grf_nifti_image_read(const char *hname, gboolean read_data);
/**
 * @brief grf_nifti_image_get_nsize
 * @param image
 * @return
 */
GrfVec6
grf_nifti_image_get_nsize(GrfNiftiImage* image);
/**
 * @brief grf_nifti_image_get_dsize
 * @param image
 * @return
 */
GrfVec6
grf_nifti_image_get_dsize(GrfNiftiImage* image);
/**
 * @brief grf_nifti_image_get_qfac
 * @param image
 * @return
 */
float
grf_nifti_image_get_qfac(GrfNiftiImage* image);
/**
 * @brief grf_nifti_image_get_byteorder
 * @param image
 * @return
 */
int
grf_nifti_image_get_byteorder(GrfNiftiImage* image);





///*****===================================================================*****/
///*****         File nifti1_io.h == Declarations for nifti1_io.c          *****/
///*****...................................................................*****/
///*****            This code is released to the public domain.            *****/
///*****...................................................................*****/
///*****  Author: Robert W Cox, SSCC/DIRP/NIMH/NIH/DHHS/USA/EARTH          *****/
///*****  Date:   August 2003                                              *****/
///*****...................................................................*****/
///*****  Neither the National Institutes of Health (NIH), nor any of its  *****/
///*****  employees imply any warranty of usefulness of this software for  *****/
///*****  any purpose, and do not assume any liability for damages,        *****/
///*****  incidental or otherwise, caused by any use of this document.     *****/
///*****===================================================================*****/

///*
//   Modified by: Mark Jenkinson (FMRIB Centre, University of Oxford, UK)
//   Date: July/August 2004

//      Mainly adding low-level IO and changing things to allow gzipped files
//      to be read and written
//      Full backwards compatability should have been maintained

//   Modified by: Rick Reynolds (SSCC/DIRP/NIMH, National Institutes of Health)
//   Date: December 2004

//      Modified and added many routines for I/O.
//*/

///********************** Some sample data structures **************************/

//typedef struct {                   /** 4x4 matrix struct **/
//  float m[4][4] ;
//} mat44 ;

//typedef struct {                   /** 3x3 matrix struct **/
//  float m[3][3] ;
//} mat33 ;

///*...........................................................................*/



///*! \struct nifti_image
//    \brief High level data structure for open nifti datasets in the
//           nifti1_io API.  Note that this structure is not part of the
//           nifti1 format definition; it is used to implement one API
//           for reading/writing formats in the nifti1 format.
// */
//typedef struct {                /*!< Image storage struct **/
//  GrfNDArray array;                /*!< data in array format */

//  int ndim ;                    /*!< last dimension greater than 1 (1..7) */
//  int nx ;                      /*!< dimensions of grid array             */
//  int ny ;                      /*!< dimensions of grid array             */
//  int nz ;                      /*!< dimensions of grid array             */
//  int nt ;                      /*!< dimensions of grid array             */
//  int nu ;                      /*!< dimensions of grid array             */
//  int nv ;                      /*!< dimensions of grid array             */
//  int nw ;                      /*!< dimensions of grid array             */
//  int dim[8] ;                  /*!< dim[0]=ndim, dim[1]=nx, etc.         */
//  size_t nvox ;                    /*!< number of voxels = nx*ny*nz*...*nw   */
//  int nbyper ;                  /*!< bytes per voxel, matches datatype    */
//  int datatype ;                /*!< type of data in voxels: DT_* code    */

//  float dx ;                    /*!< grid spacings      */
//  float dy ;                    /*!< grid spacings      */
//  float dz ;                    /*!< grid spacings      */
//  float dt ;                    /*!< grid spacings      */
//  float du ;                    /*!< grid spacings      */
//  float dv ;                    /*!< grid spacings      */
//  float dw ;                    /*!< grid spacings      */
//  float pixdim[8] ;             /*!< pixdim[1]=dx, etc. */

//  float scl_slope ;             /*!< scaling parameter - slope        */
//  float scl_inter ;             /*!< scaling parameter - intercept    */

//  float cal_min ;               /*!< calibration parameter, minimum   */
//  float cal_max ;               /*!< calibration parameter, maximum   */

//  int qform_code ;              /*!< codes for (x,y,z) space meaning  */
//  int sform_code ;              /*!< codes for (x,y,z) space meaning  */

//  int freq_dim  ;               /*!< indexes (1,2,3, or 0) for MRI    */
//  int phase_dim ;               /*!< directions in dim[]/pixdim[]     */
//  int slice_dim ;               /*!< directions in dim[]/pixdim[]     */

//  int   slice_code  ;           /*!< code for slice timing pattern    */
//  int   slice_start ;           /*!< index for start of slices        */
//  int   slice_end   ;           /*!< index for end of slices          */
//  float slice_duration ;        /*!< time between individual slices   */

//  /*! quaternion transform parameters
//    [when writing a dataset, these are used for qform, NOT qto_xyz]   */
//  float quatern_b , quatern_c , quatern_d ,
//        qoffset_x , qoffset_y , qoffset_z ,
//        qfac      ;

//  mat44 qto_xyz ;               /*!< qform: transform (i,j,k) to (x,y,z) */
//  mat44 qto_ijk ;               /*!< qform: transform (x,y,z) to (i,j,k) */

//  mat44 sto_xyz ;               /*!< sform: transform (i,j,k) to (x,y,z) */
//  mat44 sto_ijk ;               /*!< sform: transform (x,y,z) to (i,j,k) */

//  float toffset ;               /*!< time coordinate offset */

//  int xyz_units  ;              /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
//  int time_units ;              /*!< dt       units: NIFTI_UNITS_* code  */

//  int nifti_type ;              /*!< 0==ANALYZE, 1==NIFTI-1 (1 file),
//                                                 2==NIFTI-1 (2 files),
//                                                 3==NIFTI-ASCII (1 file) */
//  int   intent_code ;           /*!< statistic type (or something)       */
//  float intent_p1 ;             /*!< intent parameters                   */
//  float intent_p2 ;             /*!< intent parameters                   */
//  float intent_p3 ;             /*!< intent parameters                   */
//  char  intent_name[16] ;       /*!< optional description of intent data */

//  char descrip[80]  ;           /*!< optional text to describe dataset   */
//  char aux_file[24] ;           /*!< auxiliary filename                  */

//  char *fname ;                 /*!< header filename (.hdr or .nii)         */
//  char *iname ;                 /*!< image filename  (.img or .nii)         */
//  int   iname_offset ;          /*!< offset into iname where data starts    */
//  int   swapsize ;              /*!< swap unit in image data (might be 0)   */
//  int   byteorder ;             /*!< byte order on disk (MSB_ or LSB_FIRST) */
//  void *data ;                  /*!< pointer to data: nbyper*nvox bytes     */

//  int                    num_ext ;  /*!< number of extensions in ext_list       */
//  GrfNifti1Extension   * ext_list ; /*!< array of extension structs (with data) */
//  GrfAnalyze75OrientCode analyze75_orient; /*!< for old analyze files, orient */

//} GrfNiftiImage ;



///* struct for return from nifti_image_read_bricks() */
//typedef struct {
//  int       nbricks;    /* the number of allocated pointers in 'bricks' */
//  size_t    bsize;      /* the length of each data block, in bytes      */
//  void   ** bricks;     /* array of pointers to data blocks             */
//} GrfNiftiBrickList;


///*****************************************************************************/
///*------------------ NIfTI version of ANALYZE 7.5 structure -----------------*/

///* (based on fsliolib/dbh.h, but updated for version 7.5) */

//typedef struct {
//       /* header info fields - describes the header    overlap with NIfTI */
//       /*                                              ------------------ */
//       int sizeof_hdr;                  /* 0 + 4        same              */
//       char data_type[10];              /* 4 + 10       same              */
//       char db_name[18];                /* 14 + 18      same              */
//       int extents;                     /* 32 + 4       same              */
//       short int session_error;         /* 36 + 2       same              */
//       char regular;                    /* 38 + 1       same              */
//       char hkey_un0;                   /* 39 + 1                40 bytes */

//       /* image dimension fields - describes image sizes */
//       short int dim[8];                /* 0 + 16       same              */
//       short int unused8;               /* 16 + 2       intent_p1...      */
//       short int unused9;               /* 18 + 2         ...             */
//       short int unused10;              /* 20 + 2       intent_p2...      */
//       short int unused11;              /* 22 + 2         ...             */
//       short int unused12;              /* 24 + 2       intent_p3...      */
//       short int unused13;              /* 26 + 2         ...             */
//       short int unused14;              /* 28 + 2       intent_code       */
//       short int datatype;              /* 30 + 2       same              */
//       short int bitpix;                /* 32 + 2       same              */
//       short int dim_un0;               /* 34 + 2       slice_start       */
//       float pixdim[8];                 /* 36 + 32      same              */

//       float vox_offset;                /* 68 + 4       same              */
//       float funused1;                  /* 72 + 4       scl_slope         */
//       float funused2;                  /* 76 + 4       scl_inter         */
//       float funused3;                  /* 80 + 4       slice_end,        */
//                                                     /* slice_code,       */
//                                                     /* xyzt_units        */
//       float cal_max;                   /* 84 + 4       same              */
//       float cal_min;                   /* 88 + 4       same              */
//       float compressed;                /* 92 + 4       slice_duration    */
//       float verified;                  /* 96 + 4       toffset           */
//       int glmax,glmin;                 /* 100 + 8              108 bytes */

//       /* data history fields - optional */
//       char descrip[80];                /* 0 + 80       same              */
//       char aux_file[24];               /* 80 + 24      same              */
//       char orient;                     /* 104 + 1      NO GOOD OVERLAP   */
//       char originator[10];             /* 105 + 10     FROM HERE DOWN... */
//       char generated[10];              /* 115 + 10                       */
//       char scannum[10];                /* 125 + 10                       */
//       char patient_id[10];             /* 135 + 10                       */
//       char exp_date[10];               /* 145 + 10                       */
//       char exp_time[10];               /* 155 + 10                       */
//       char hist_un0[3];                /* 165 + 3                        */
//       int views;                       /* 168 + 4                        */
//       int vols_added;                  /* 172 + 4                        */
//       int start_field;                 /* 176 + 4                        */
//       int field_skip;                  /* 180 + 4                        */
//       int omax, omin;                  /* 184 + 8                        */
//       int smax, smin;                  /* 192 + 8              200 bytes */
//} GrfNiftiAnalyze75;                                   /* total:  348 bytes */


///*****************************************************************************/
///*--------------- Prototypes of functions defined in this file --------------*/

//char *grf_nifti_datatype_string   ( int dt ) ;
//char *grf_nifti_units_string      ( int uu ) ;
//char *grf_nifti_intent_string     ( int ii ) ;
//char *grf_nifti_xform_string      ( int xx ) ;
//char *grf_nifti_slice_string      ( int ss ) ;
//char *grf_nifti_orientation_string( int ii ) ;

//int   grf_nifti_is_inttype( int dt ) ;

//mat44 grf_nifti_mat44_inverse( mat44 R ) ;

//mat33 grf_nifti_mat33_inverse( mat33 R ) ;
//mat33 grf_nifti_mat33_polar  ( mat33 A ) ;
//float grf_nifti_mat33_rownorm( mat33 A ) ;
//float grf_nifti_mat33_colnorm( mat33 A ) ;
//float grf_nifti_mat33_determ ( mat33 R ) ;
//mat33 grf_nifti_mat33_mul    ( mat33 A , mat33 B ) ;

//void  grf_nifti_swap_2bytes ( int n , void *ar ) ;
//void  grf_nifti_swap_4bytes ( int n , void *ar ) ;
//void  grf_nifti_swap_8bytes ( int n , void *ar ) ;
//void  grf_nifti_swap_16bytes( int n , void *ar ) ;
//void  grf_nifti_swap_Nbytes ( int n , int siz , void *ar ) ;

//int    grf_nifti_datatype_is_valid   (int dtype, int for_nifti);
//int    grf_nifti_datatype_from_string(const char * name);
//char * grf_nifti_datatype_to_string  (int dtype);

//int   grf_nifti_get_filesize( const char *pathname ) ;
//void  grf_swap_nifti_header ( struct GrfNifti1Header *h , int is_nifti ) ;
//void  grf_old_swap_nifti_header( struct GrfNifti1Header *h , int is_nifti );
//int   grf_nifti_swap_as_analyze( GrfNiftiAnalyze75 *h );


///* main read/write routines */

//GrfNiftiImage *grf_nifti_image_read_bricks(const char *hname , int nbricks,
//                                     const int *blist, GrfNiftiBrickList * NBL);
//int          grf_nifti_image_load_bricks(GrfNiftiImage *nim , int nbricks,
//                                     const int *blist, GrfNiftiBrickList * NBL);
//void         grf_nifti_free_NBL( GrfNiftiBrickList * NBL );

//int          grf_nifti_image_load    ( GrfNiftiImage *nim ) ;
//void         grf_nifti_image_unload  ( GrfNiftiImage *nim ) ;
//void         grf_nifti_image_free    ( GrfNiftiImage *nim ) ;

//int          grf_nifti_read_collapsed_image( GrfNiftiImage * nim, const int dims [8],
//                                         void ** data );

//int          grf_nifti_read_subregion_image( GrfNiftiImage * nim,
//                                         int *start_index, int *region_size,
//                                         void ** data );

//void         grf_nifti_image_write   ( GrfNiftiImage * nim );
//void         grf_nifti_image_write_bricks(GrfNiftiImage * nim,
//                                      const GrfNiftiBrickList * NBL);
//void         grf_nifti_image_infodump( const GrfNiftiImage * nim ) ;

//void         grf_nifti_disp_lib_hist( void ) ;     /* to display library history */
//void         grf_nifti_disp_lib_version( void ) ;  /* to display library version */
//int          grf_nifti_disp_matrix_orient( const char * mesg, mat44 mat );
//int          grf_nifti_disp_type_list( int which );


//char *       grf_nifti_image_to_ascii  ( const GrfNiftiImage * nim ) ;
//GrfNiftiImage *grf_nifti_image_from_ascii( const char * str, int * bytes_read ) ;

//size_t       grf_nifti_get_volsize(const GrfNiftiImage *nim) ;

///* basic file operations */
//int    grf_nifti_set_filenames(GrfNiftiImage * nim, const char * prefix, int check,
//                           int set_byte_order);
//char * grf_nifti_makehdrname  (const char * prefix, int nifti_type, int check,
//                           int comp);
//char * grf_nifti_makeimgname  (const char * prefix, int nifti_type, int check,
//                           int comp);
//int    grf_is_nifti_file      (const char *hname);
//char * grf_nifti_find_file_extension(const char * name);
//int    grf_nifti_is_complete_filename(const char* fname);
//int    grf_nifti_validfilename(const char* fname);

//int    grf_disp_nifti_1_header(const char * info, const GrfNifti1Header * hp ) ;
//void   grf_nifti_set_debug_level( int level ) ;
//void   grf_nifti_set_skip_blank_ext( int skip ) ;

//int    grf_valid_nifti_brick_list(GrfNiftiImage * nim , int nbricks,
//                              const int * blist, int disp_error);

///* znzFile operations */
//GrfZnzFile grf_nifti_image_open(const char * hname, char * opts, GrfNiftiImage ** nim);
//GrfZnzFile grf_nifti_image_write_hdr_img(GrfNiftiImage *nim, int write_data,
//                                  const char* opts);
//GrfZnzFile grf_nifti_image_write_hdr_img2( GrfNiftiImage *nim , int write_opts ,
//               const char* opts, GrfZnzFile imgfile, const GrfNiftiBrickList * NBL);
//size_t  grf_nifti_read_buffer(GrfZnzFile fp, void* datatptr, size_t ntot,
//                         GrfNiftiImage *nim);
//int     grf_nifti_write_all_data(GrfZnzFile fp, GrfNiftiImage * nim,
//                             const GrfNiftiBrickList * NBL);
//size_t  grf_nifti_write_buffer(GrfZnzFile fp, const void * buffer, size_t numbytes);
//GrfNiftiImage *grf_nifti_read_ascii_image(GrfZnzFile fp, char *fname, int flen,
//                         int read_data);
//GrfZnzFile grf_nifti_write_ascii_image(GrfNiftiImage *nim, const GrfNiftiBrickList * NBL,
//                         const char * opts, int write_data, int leave_open);


//void grf_nifti_datatype_sizes( int datatype , int *nbyper, int *swapsize ) ;

//void grf_nifti_mat44_to_quatern( mat44 R ,
//                             float *qb, float *qc, float *qd,
//                             float *qx, float *qy, float *qz,
//                             float *dx, float *dy, float *dz, float *qfac ) ;

//mat44 grf_nifti_quatern_to_mat44( float qb, float qc, float qd,
//                              float qx, float qy, float qz,
//                              float dx, float dy, float dz, float qfac );

//mat44 grf_nifti_make_orthog_mat44( float r11, float r12, float r13 ,
//                               float r21, float r22, float r23 ,
//                               float r31, float r32, float r33  ) ;

//int grf_nifti_short_order(void) ;              /* CPU byte order */



//void grf_nifti_mat44_to_orientation( mat44 R , int *icod, int *jcod, int *kcod ) ;

///*--------------------- Low level IO routines ------------------------------*/

//char * grf_nifti_findhdrname (const char* fname);
//char * grf_nifti_findimgname (const char* fname , int nifti_type);
//int    grf_nifti_is_gzfile   (const char* fname);

//char * grf_nifti_makebasename(const char* fname);


///* other routines */
//struct GrfNifti1Header   grf_nifti_convert_nim2nhdr(const GrfNiftiImage* nim);
//GrfNifti1Header * grf_nifti_make_new_header(const int arg_dims[], int arg_dtype);
//GrfNifti1Header * grf_nifti_read_header(const char *hname, int *swapped, int check);
//GrfNiftiImage    * grf_nifti_copy_nim_info(const GrfNiftiImage * src);
//GrfNiftiImage    * grf_nifti_make_new_nim(const int dims[], int datatype,
//                                                      int data_fill);
//GrfNiftiImage    * grf_nifti_simple_init_nim(void);
//GrfNiftiImage    * grf_nifti_convert_nhdr2nim(struct GrfNifti1Header nhdr,
//                                        const char * fname);

//int    grf_nifti_hdr_looks_good        (const GrfNifti1Header * hdr);
//int    grf_nifti_is_valid_datatype     (int dtype);
//int    grf_nifti_is_valid_ecode        (int ecode);
//int    grf_nifti_nim_is_valid          (GrfNiftiImage * nim, int complain);
//int    grf_nifti_nim_has_valid_dims    (GrfNiftiImage * nim, int complain);
//int    grf_is_valid_nifti_type         (int nifti_type);
//int    grf_nifti_test_datatype_sizes   (int verb);
//int    grf_nifti_type_and_names_match  (GrfNiftiImage * nim, int show_warn);
//int    grf_nifti_update_dims_from_array(GrfNiftiImage * nim);
//void   grf_nifti_set_iname_offset      (GrfNiftiImage *nim);
//int    grf_nifti_set_type_from_names   (GrfNiftiImage * nim);
//int    grf_nifti_add_extension(GrfNiftiImage * nim, const char * data, int len,
//                           int ecode );
//int    grf_nifti_compiled_with_zlib    (void);
//int    grf_nifti_copy_extensions (GrfNiftiImage *nim_dest,const GrfNiftiImage *nim_src);
//int    grf_nifti_free_extensions (GrfNiftiImage *nim);
//int  * grf_nifti_get_intlist     (int nvals , const char *str);
//char * grf_nifti_strdup          (const char *str);
//int    grf_valid_nifti_extensions(const GrfNiftiImage *nim);

///*------------------------------------------------------------------------*/
///*-- the rest of these apply only to nifti1_io.c, check for _NIFTI1_IO_C_ */
///*                                                    Feb 9, 2005 [rickr] */
//#ifdef GRF_NIFTI1_IO_C_

//typedef struct {
//    int debug;               /*!< debug level for status reports */
//    int skip_blank_ext;      /*!< skip extender if no extensions */
//} GrfNiftiGlobalOptions;

//typedef struct {
//    int    type;           /* should match the NIFTI_TYPE_ #define */
//    int    nbyper;         /* bytes per value, matches nifti_image */
//    int    swapsize;       /* bytes per swap piece, matches nifti_image */
//    char * name;           /* text string to match #define */
//} GrfNiftiTypeEle;

//#undef  LNI_FERR /* local nifti file error, to be compact and repetative */
//#define LNI_FERR(func,msg,file)                                      \
//            fprintf(stderr,"** ERROR (%s): %s '%s'\n",func,msg,file)

//#undef  swap_2
//#undef  swap_4
//#define swap_2(s) grf_nifti_swap_2bytes(1,&(s)) /* s: 2-byte short; swap in place */
//#define swap_4(v) grf_nifti_swap_4bytes(1,&(v)) /* v: 4-byte value; swap in place */

//                        /***** isfinite() is a C99 macro, which is
//                               present in many C implementations already *****/

//#undef IS_GOOD_FLOAT
//#undef FIXED_FLOAT

//#ifdef isfinite       /* use isfinite() to check floats/doubles for goodness */
//#  define IS_GOOD_FLOAT(x) isfinite(x)       /* check if x is a "good" float */
//#  define FIXED_FLOAT(x)   (isfinite(x) ? (x) : 0)           /* fixed if bad */
//#else
//#  define IS_GOOD_FLOAT(x) 1                               /* don't check it */
//#  define FIXED_FLOAT(x)   (x)                               /* don't fix it */
//#endif

//#undef  ASSIF                                 /* assign v to *p, if possible */
//#define ASSIF(p,v) if( (p)!=NULL ) *(p) = (v)

//#undef  MSB_FIRST
//#undef  LSB_FIRST
//#undef  REVERSE_ORDER
//#define LSB_FIRST 1
//#define MSB_FIRST 2
//#define REVERSE_ORDER(x) (3-(x))    /* convert MSB_FIRST <--> LSB_FIRST */

//#define LNI_MAX_NIA_EXT_LEN 100000  /* consider a longer extension invalid */

//#endif  /* _NIFTI1_IO_C_ section */
///*------------------------------------------------------------------------*/

#endif /* _NIFTI_IO_HEADER_ */
