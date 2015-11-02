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
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <grafeo/dicom.h>

static void test_grf_dicom_read(void** state){
  (void) state;
  const char* filename = "../data/MR-MONO2-16-head.dcm";
  GrfDicom* dicom = grf_dicom_read(filename);
  assert_non_null(dicom);
  assert_string_equal(dicom->filename, filename);
  assert_string_equal(dicom->modality, "MR");
  assert_true(dicom->pixel_data_tag_found);
  assert_string_equal(dicom->unit, "mm");
  assert_int_equal(dicom->image->bitsize, 8);
  assert_int_equal(dicom->image->size[0], 80);
  assert_int_equal(dicom->image->size[1], 160);
  assert_int_equal(dicom->image->size[2], 3);
  grf_dicom_free(dicom);
}
static void test_grf_dicom_write(void** state){
  (void) state;
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_grf_dicom_read),
    cmocka_unit_test(test_grf_dicom_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
