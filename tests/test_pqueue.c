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
#include <string.h>
#include <grafeo/pqueue.h>

static void helper_grf_pqueue_elements(GrfQueue* pqueue, uint8_t* grf_pqueue_values, uint8_t** grf_bucket_values, uint8_t grf_pqueue_length, uint8_t* grf_bucket_lengths){
  // Test queue length
  assert_int_equal(grf_queue_length(pqueue), grf_pqueue_length);
  uint8_t i;
  GrfList* current = pqueue->begin;
  for(i = 0, current = grf_queue_begin(pqueue); i < grf_pqueue_length; i++, current = grf_list_next(current)){
    GrfBucket* bucket = (GrfBucket*)grf_list_value(current);
    GrfQueue* queueBucket = grf_bucket_queue(bucket);
    // Test bucket length
    assert_int_equal(grf_queue_length(queueBucket), grf_bucket_lengths[i]);
    // Test bucket value
    assert_int_equal(grf_bucket_value(bucket), grf_pqueue_values[i]);
    // Test bucket queue values
    GrfList* currentb;
    uint8_t j;
    for(j = 0, currentb = grf_queue_begin(queueBucket); j < grf_bucket_lengths[i]; j++, currentb = grf_list_next(currentb))
      assert_int_equal(grf_list_value(currentb),grf_bucket_values[i][j]);
  }
}

static void test_grf_pqueue_adding_removing(void ** state){
  (void) state;
  GrfQueue* pqueue  = grf_queue_new();
#define IP(x) INT8_TO_POINTER(x)
  // Appending a value in a bucket
  grf_pqueue_append_at(pqueue,IP(7), IP(5), int64_compare_function);
  uint8_t  buckets1[1]        = {7};
  uint8_t  bucketslists011[1]  = {5};
  uint8_t* bucketslists1[1]   = {bucketslists011};
  uint8_t  bucketslengths1[1] = {1};
  helper_grf_pqueue_elements(pqueue, buckets1, bucketslists1, 1, bucketslengths1);

  // Appending another value in a bucket
  grf_pqueue_append_at(pqueue,IP(7), IP(3), int64_compare_function);
  uint8_t  bucketslists21[2]  = {5,3};
  uint8_t* bucketslists2[1]   = {bucketslists21};
  uint8_t  bucketslengths2[1] = {2};
  helper_grf_pqueue_elements(pqueue, buckets1, bucketslists2, 1, bucketslengths2);

  // Appending another value in another bucket
  grf_pqueue_append_at(pqueue,IP(9), IP(6), int64_compare_function);
  uint8_t  buckets3[2]        = {7,9};
  uint8_t  bucketslists31[2]  = {5,3};
  uint8_t  bucketslists32[1]  = {6};
  uint8_t* bucketslists3[2]   = {bucketslists31,bucketslists32};
  uint8_t  bucketslengths3[2] = {2,1};
  helper_grf_pqueue_elements(pqueue, buckets3, bucketslists3, 2, bucketslengths3);

  // Prepending a value in a new bucket
  grf_pqueue_prepend_at(pqueue, IP(3), IP(4), int64_compare_function);
  uint8_t  buckets4[3]        = {3,7,9};
  uint8_t  bucketslists41[1]  = {4};
  uint8_t  bucketslists42[2]  = {5,3};
  uint8_t  bucketslists43[1]  = {6};
  uint8_t* bucketslists4[3]   = {bucketslists41,bucketslists42,bucketslists43};
  uint8_t  bucketslengths4[3] = {1,2,1};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists4, 3, bucketslengths4);

  // Prepending a value in a existante bucket
  grf_pqueue_prepend_at(pqueue, IP(7), IP(4), int64_compare_function);
  uint8_t  bucketslists52[3]  = {4,5,3};
  uint8_t* bucketslists5[3]   = {bucketslists41,bucketslists52,bucketslists43};
  uint8_t  bucketslengths5[3] = {1,3,1};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists5, 3, bucketslengths5);

  // Removing the first value in a bucket
  grf_pqueue_remove_begin_at(pqueue, IP(7));
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists4, 3, bucketslengths4);

  // Removing the last value in a bucket
  grf_pqueue_remove_end_at(pqueue, IP(7));
  uint8_t  bucketslists62[1]  = {5};
  uint8_t* bucketslists6[3]   = {bucketslists41,bucketslists62,bucketslists43};
  uint8_t  bucketslengths6[3] = {1,1,1};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists6, 3, bucketslengths6);

  // Removing the first value of the first bucket
  // It doesn't automatically remove the empty bucket
  grf_pqueue_remove_begin(pqueue);
  uint8_t  bucketslists71[0]  = {};
  uint8_t  bucketslists72[1]  = {5};
  uint8_t* bucketslists7[3]   = {bucketslists71,bucketslists72,bucketslists43};
  uint8_t  bucketslengths7[3] = {0,1,1};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists7, 3, bucketslengths7);

  // Adding a value in the empty bucket
  grf_pqueue_prepend_at(pqueue, IP(3), IP(10), int64_compare_function);
  uint8_t  bucketslists81[1]  = {10};
  uint8_t* bucketslists8[3]   = {bucketslists81,bucketslists72,bucketslists43};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists8, 3, bucketslengths6);

  // Adding another value in the empty bucket
  grf_pqueue_prepend_at(pqueue, IP(3), IP(11), int64_compare_function);
  uint8_t  bucketslists91[2]  = {11,10};
  uint8_t* bucketslists9[3]   = {bucketslists91,bucketslists72,bucketslists43};
  uint8_t  bucketslengths9[3] = {2,1,1};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists9, 3, bucketslengths9);

  // Removing the last value of the first bucket
  grf_pqueue_remove_end(pqueue);
  uint8_t  bucketslists101[3]  = {11};
  uint8_t* bucketslists10[3]   = {bucketslists101,bucketslists72,bucketslists43};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists10, 3, bucketslengths6);

  // Removing the last value of the first bucket
  grf_pqueue_remove_end(pqueue);
  uint8_t* bucketslists11[3]   = {bucketslists71,bucketslists72,bucketslists43};
  helper_grf_pqueue_elements(pqueue, buckets4, bucketslists11, 3, bucketslengths7);

  // Shrink GrfQueue (removing first bucket if it's empty)
  grf_pqueue_shrink(pqueue);
  uint8_t  buckets12[3]        = {7,9};
  uint8_t* bucketslists12[3]   = {bucketslists72,bucketslists43};
  helper_grf_pqueue_elements(pqueue, buckets12, bucketslists12, 2, bucketslengths6);

  // Looking for the bucket value
  void* value = grf_pqueue_at(pqueue, 1);
  assert_int_equal(value, 9);

  // Looking for a value in a bucket queue
  value = grf_pqueue_grf_bucket_at(pqueue, 1, 0);
  assert_int_equal(value, 6);

  grf_queue_free(pqueue);
}



int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_grf_pqueue_adding_removing),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
