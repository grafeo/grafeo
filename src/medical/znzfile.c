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

/**
 * @file  znzfile.c
 * @brief Low level i/o interface to compressed and noncompressed files.
 *
 * This library provides an interface to both compressed (gzip/zlib) and
 * uncompressed (normal) file IO.  The functions are written to have the
 * same interface as the standard file IO functions.
 *
 * To use this library instead of normal file IO, the following changes
 * are required:
 *  - replace all instances of FILE* with znzFile
 *  - change the name of all function calls, replacing the initial character
 *    f with the znz  (e.g. fseek becomes znzseek)
 *    one exception is rewind() -> znzrewind()
 *  - add a third parameter to all calls to znzopen (previously fopen)
 *    that specifies whether to use compression (1) or not (0)
 *  - use znz_isnull rather than any (pointer == NULL) comparisons in the code
 *    for znzfile types (normally done after a return from znzopen)
 *
 * NB: seeks for writable files with compression are quite restricted
 */
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct GrfZnzFilePrivate{
  gboolean withz;/**< TRUE = compressed file */
  FILE* nzfptr;  /**< pointer to file */
#ifdef HAVE_ZLIB
  gzFile zfptr;  /**< pointer to zlib file */
#endif
}GrfZnzFilePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GrfZnzFile, grf_znzfile, G_TYPE_OBJECT)

/**
 * @brief Close a ZLib file
 * @param znzfile
 * @return
 */
static int
grf_znzfile_close(GrfZnzFile* znzfile){
  int retval;
  GrfZnzFilePrivate *priv = grf_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(priv->zfptr != NULL) {retval = gzclose(priv->zfptr);}
#endif
  if(priv->nzfptr != NULL){retval = fclose(priv->nzfptr);}
  return retval;
}

/**
 * @brief Dispose referenced objects of the zlib file
 * @param object
 */
static void
grf_znzfile_dispose(GObject* object){
  G_OBJECT_CLASS(grf_znzfile_parent_class)->dispose(object);
}

/**
 * @brief grf_znzfile_finalize
 * @param object
 */
static void
grf_znzfile_finalize(GObject* object){
  GrfZnzFile        *znzfile = GRF_ZNZFILE(object);

  grf_znzfile_close(znzfile);

  G_OBJECT_CLASS(grf_znzfile_parent_class)->finalize(object);
}

/**
 * @brief grf_znzfile_init
 * @param self
 */
static void
grf_znzfile_init(GrfZnzFile *self){
  GrfZnzFilePrivate *priv    = grf_znzfile_get_instance_private(self);
  priv->nzfptr = NULL;
  priv->withz  = FALSE;
#ifdef HAVE_ZLIB
  priv->zfptr  = NULL;
#endif
}

/**
 * @brief grf_znzfile_class_init
 * @param klass
 */
static void
grf_znzfile_class_init(GrfZnzFileClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = grf_znzfile_finalize;
  object_class->dispose = grf_znzfile_dispose;
}

/**
 * @brief Return a new instance of GrfZnzFile
 * @return
 */
static GrfZnzFile*
grf_znzfile_new(){
  return g_object_new(GRF_TYPE_ZNZFILE, NULL);
}


/* we already assume ints are 4 bytes */
#undef  ZNZ_MAX_BLOCK_SIZE
#define ZNZ_MAX_BLOCK_SIZE (1<<30)
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
GrfZnzFile*
grf_znzfile_open(const char *path, const char *mode, gboolean use_compression){
  GrfZnzFile* znzfile     = grf_znzfile_new();
  GrfZnzFilePrivate *priv = grf_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(use_compression){
    priv->withz = TRUE;
    priv->zfptr = gzopen(path,mode);
    if(priv->zfptr == NULL){
      g_clear_object(&znzfile);
      return NULL;
    }
  } else {
#endif
    priv->withz = FALSE;
    priv->nzfptr = fopen(path,mode);
    if(priv->nzfptr == NULL){
      g_clear_object(&znzfile);
      return NULL;
    }
#ifdef HAVE_ZLIB
  }
#endif
  return znzfile;
}

GrfZnzFile*
grf_znzfile_dopen(int fd, const char *mode, gboolean use_compression){
  GrfZnzFile* znzfile     = grf_znzfile_new();
  GrfZnzFilePrivate *priv = grf_znzfile_get_instance_private(znzfile);
#ifdef HAVE_ZLIB
  if(use_compression){
    priv->withz = TRUE;
    priv->zfptr = gzdopen(fd,mode);
  } else {
#endif
    priv->withz = FALSE;
#ifndef HAVE_FDOPEN
    priv->nzfptr = fdopen(fd, mode);
#endif
#ifdef HAVE_ZLIB
  }
#endif
  return znzfile;
}

//int grf_Xznzclose(GrfZnzFile * file)
//{
//  int retval = 0;
//  if (*file!=NULL) {
//#ifdef HAVE_ZLIB
//    if ((*file)->zfptr!=NULL)  { retval = gzclose((*file)->zfptr); }
//#endif
//    if ((*file)->nzfptr!=NULL) { retval = fclose((*file)->nzfptr); }

//    free(*file);
//    *file = NULL;
//  }
//  return retval;
//}

//size_t grf_znzread(void* buf, size_t size, size_t nmemb, GrfZnzFile file)
//{
//  size_t     remain = size*nmemb;
//  char     * cbuf = (char *)buf;
//  unsigned   n2read;
//  int        nread;

//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) {
//    /* gzread/write take unsigned int length, so maybe read in int pieces
//       (noted by M Hanke, example given by M Adler)   6 July 2010 [rickr] */
//    while( remain > 0 ) {
//       n2read = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
//       nread = gzread(file->zfptr, (void *)cbuf, n2read);
//       if( nread < 0 ) return nread; /* returns -1 on error */

//       remain -= nread;
//       cbuf += nread;

//       /* require reading n2read bytes, so we don't get stuck */
//       if( nread < (int)n2read ) break;  /* return will be short */
//    }

//    /* warn of a short read that will seem complete */
//    if( remain > 0 && remain < size )
//       fprintf(stderr,"** znzread: read short by %u bytes\n",(unsigned)remain);

//    return nmemb - remain/size;   /* return number of members processed */
//  }
//#endif
//  return fread(buf,size,nmemb,file->nzfptr);
//}

//size_t grf_znzwrite(const void* buf, size_t size, size_t nmemb, GrfZnzFile file)
//{
//  size_t     remain = size*nmemb;
//  const char * cbuf = (const char *)buf;
//  unsigned   n2write;
//  int        nwritten;

//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) {
//    while( remain > 0 ) {
//       n2write = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
//       nwritten = gzwrite(file->zfptr, (const void *)cbuf, n2write);

//       /* gzread returns 0 on error, but in case that ever changes... */
//       if( nwritten < 0 ) return nwritten;

//       remain -= nwritten;
//       cbuf += nwritten;

//       /* require writing n2write bytes, so we don't get stuck */
//       if( nwritten < (int)n2write ) break;
//    }

//    /* warn of a short write that will seem complete */
//    if( remain > 0 && remain < size )
//      fprintf(stderr,"** znzwrite: write short by %u bytes\n",(unsigned)remain);

//    return nmemb - remain/size;   /* return number of members processed */
//  }
//#endif
//  return fwrite(buf,size,nmemb,file->nzfptr);
//}

//long grf_znzseek(GrfZnzFile file, long offset, int whence)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return (long) gzseek(file->zfptr,offset,whence);
//#endif
//  return fseek(file->nzfptr,offset,whence);
//}

//int grf_znzrewind(GrfZnzFile stream)
//{
//  if (stream==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  /* On some systems, gzrewind() fails for uncompressed files.
//     Use gzseek(), instead.               10, May 2005 [rickr]

//     if (stream->zfptr!=NULL) return gzrewind(stream->zfptr);
//  */

//  if (stream->zfptr!=NULL) return (int)gzseek(stream->zfptr, 0L, SEEK_SET);
//#endif
//  rewind(stream->nzfptr);
//  return 0;
//}

//long grf_znztell(GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return (long) gztell(file->zfptr);
//#endif
//  return ftell(file->nzfptr);
//}

//int grf_znzputs(const char * str, GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzputs(file->zfptr,str);
//#endif
//  return fputs(str,file->nzfptr);
//}


//char * grf_znzgets(char* str, int size, GrfZnzFile file)
//{
//  if (file==NULL) { return NULL; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzgets(file->zfptr,str,size);
//#endif
//  return fgets(str,size,file->nzfptr);
//}


//int znzflush(GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzflush(file->zfptr,Z_SYNC_FLUSH);
//#endif
//  return fflush(file->nzfptr);
//}


//int znzeof(GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzeof(file->zfptr);
//#endif
//  return feof(file->nzfptr);
//}


//int grf_znzputc(int c, GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzputc(file->zfptr,c);
//#endif
//  return fputc(c,file->nzfptr);
//}


//int grf_znzgetc(GrfZnzFile file)
//{
//  if (file==NULL) { return 0; }
//#ifdef HAVE_ZLIB
//  if (file->zfptr!=NULL) return gzgetc(file->zfptr);
//#endif
//  return fgetc(file->nzfptr);
//}

//#if !defined (WIN32)
//int grf_znzprintf(GrfZnzFile stream, const char *format, ...)
//{
//  int retval=0;
//  char *tmpstr;
//  va_list va;
//  if (stream==NULL) { return 0; }
//  va_start(va, format);
//#ifdef HAVE_ZLIB
//  if (stream->zfptr!=NULL) {
//    int size;  /* local to HAVE_ZLIB block */
//    size = strlen(format) + 1000000;  /* overkill I hope */
//    tmpstr = (char *)calloc(1, size);
//    if( tmpstr == NULL ){
//       fprintf(stderr,"** ERROR: znzprintf failed to alloc %d bytes\n", size);
//       return retval;
//    }
//    vsprintf(tmpstr,format,va);
//    retval=gzprintf(stream->zfptr,"%s",tmpstr);
//    free(tmpstr);
//  } else
//#endif
//  {
//   retval=vfprintf(stream->nzfptr,format,va);
//  }
//  va_end(va);
//  return retval;
//}

//#endif

