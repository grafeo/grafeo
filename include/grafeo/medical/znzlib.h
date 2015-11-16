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
#ifndef GRF_ZNZLIB_H_
#define GRF_ZNZLIB_H_

/*
znzlib.h  (zipped or non-zipped library)

*****            This code is released to the public domain.            *****

*****  Author: Mark Jenkinson, FMRIB Centre, University of Oxford       *****
*****  Date:   September 2004                                           *****

*****  Neither the FMRIB Centre, the University of Oxford, nor any of   *****
*****  its employees imply any warranty of usefulness of this software  *****
*****  for any purpose, and do not assume any liability for damages,    *****
*****  incidental or otherwise, caused by any use of this document.     *****

*/

/*

This library provides an interface to both compressed (gzip/zlib) and
uncompressed (normal) file IO.  The functions are written to have the
same interface as the standard file IO functions.

To use this library instead of normal file IO, the following changes
are required:
 - replace all instances of FILE* with znzFile
 - change the name of all function calls, replacing the initial character
   f with the znz  (e.g. fseek becomes znzseek)
 - add a third parameter to all calls to znzopen (previously fopen)
   that specifies whether to use compression (1) or not (0)
 - use znz_isnull rather than any (pointer == NULL) comparisons in the code

NB: seeks for writable files with compression are quite restricted

*/

#include <grafeo/medical.h>
#include <grafeo/core.h>
BEGIN_DECLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* include optional check for HAVE_FDOPEN here, from deleted config.h:

   uncomment the following line if fdopen() exists for your compiler and
   compiler options
*/
/* #define HAVE_FDOPEN */


#ifdef HAVE_ZLIB
#if defined(ITKZLIB) && !defined(ITK_USE_SYSTEM_ZLIB)
#include "itk_zlib.h"
#else
#include "zlib.h"
#endif
#endif

struct GrfZnzPtr {
  int withz;
  FILE* nzfptr;
#ifdef HAVE_ZLIB
  gzFile zfptr;
#endif
} ;

/* the type for all file pointers */
typedef struct GrfZnzPtr * GrfZnzFile;


/* int znz_isnull(znzFile f); */
/* int znzclose(znzFile f); */
#define grf_znz_isnull(f) ((f) == NULL)
#define grf_znzclose(f)   grf_Xznzclose(&(f))

/* Note extra argument (use_compression) where
   use_compression==0 is no compression
   use_compression!=0 uses zlib (gzip) compression
*/

GrfZnzFile grf_znzopen(const char *path, const char *mode, int use_compression);

GrfZnzFile grf_znzdopen(int fd, const char *mode, int use_compression);

int grf_Xznzclose(GrfZnzFile * file);

size_t grf_znzread(void* buf, size_t size, size_t nmemb, GrfZnzFile file);

size_t grf_znzwrite(const void* buf, size_t size, size_t nmemb, GrfZnzFile file);

long grf_znzseek(GrfZnzFile file, long offset, int whence);

int grf_znzrewind(GrfZnzFile stream);

long grf_znztell(GrfZnzFile file);

int grf_znzputs(const char *str, GrfZnzFile file);

char * grf_znzgets(char* str, int size, GrfZnzFile file);

int grf_znzputc(int c, GrfZnzFile file);

int grf_znzgetc(GrfZnzFile file);

#if !defined(WIN32)
int grf_znzprintf(GrfZnzFile stream, const char *format, ...);
#endif

END_DECLS

#endif
