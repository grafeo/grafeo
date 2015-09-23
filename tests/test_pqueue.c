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

static void helper_pqueue_elements(Queue* pqueue, uint8_t* pqueue_values, uint8_t** bucket_values, uint8_t pqueue_length, uint8_t* bucket_lengths){
  // Test queue length
  assert_int_equal(queue_length(pqueue), pqueue_length);
  uint8_t i;
  List* current = pqueue->begin;
  for(i = 0, current = queue_begin(pqueue); i < pqueue_length; i++, current = list_next(current)){
    Bucket* bucket = (Bucket*)list_value(current);
    Queue* queueBucket = bucket_queue(bucket);
    // Test bucket length
    assert_int_equal(queue_length(queueBucket), bucket_lengths[i]);
    // Test bucket value
    assert_int_equal(bucket_value(bucket), pqueue_values[i]);
    // Test bucket queue values
    List* currentb;
    uint8_t j;
    for(j = 0, currentb = queue_begin(queueBucket); j < bucket_lengths[i]; j++, currentb = list_next(currentb))
      assert_int_equal(list_value(currentb),bucket_values[i][j]);
  }
}

static void test_pqueue_adding_removing(void ** state){
  (void) state;
  Queue* pqueue  = queue_new();
#define IP(x) INT8_TO_POINTER(x)
  // Appending a value in a bucket
  pqueue_append(pqueue,IP(7), IP(5));
  uint8_t  buckets1[1]        = {7};
  uint8_t  bucketslists11[1]  = {5};
  uint8_t* bucketslists1[1]   = {bucketslists11};
  uint8_t  bucketslengths1[1] = {1};
  helper_pqueue_elements(pqueue, buckets1, bucketslists1, 1, bucketslengths1);

  // Appending another value in a bucket
  pqueue_append(pqueue,IP(7), IP(3));
  uint8_t  bucketslists21[2]  = {5,3};
  uint8_t* bucketslists2[1]   = {bucketslists21};
  uint8_t  bucketslengths2[1] = {2};
  helper_pqueue_elements(pqueue, buckets1, bucketslists2, 1, bucketslengths2);

  // Appending another value in another bucket
  pqueue_append(pqueue,IP(9), IP(6));
  uint8_t  buckets3[2]        = {7,9};
  uint8_t  bucketslists31[2]  = {5,3};
  uint8_t  bucketslists32[1]  = {6};
  uint8_t* bucketslists3[2]   = {bucketslists31,bucketslists32};
  uint8_t  bucketslengths3[2] = {2,1};
  helper_pqueue_elements(pqueue, buckets3, bucketslists3, 2, bucketslengths3);

  // Prepending a value in a new bucket
  pqueue_prepend(pqueue, IP(3), IP(4));
  uint8_t  buckets4[3]        = {3,7,9};
  uint8_t  bucketslists41[1]  = {4};
  uint8_t  bucketslists42[2]  = {5,3};
  uint8_t  bucketslists43[1]  = {6};
  uint8_t* bucketslists4[2]   = {bucketslists41,bucketslists42,bucketslists43};
  uint8_t  bucketslengths4[3] = {1,2,1};
  helper_pqueue_elements(pqueue, buckets4, bucketslists4, 3, bucketslengths4);

  // Prepending a value in a existante bucket
  pqueue_prepend(pqueue, IP(7), IP(4));
  uint8_t  bucketslists52[3]  = {4,5,3};
  uint8_t* bucketslists5[3]   = {bucketslists41,bucketslists52,bucketslists43};
  uint8_t  bucketslengths5[3] = {1,3,1};
  helper_pqueue_elements(pqueue, buckets4, bucketslists5, 3, bucketslengths5);

  // Removing the first value in a bucket
  pqueue_remove_begin_of(pqueue, IP(7));
  helper_pqueue_elements(pqueue, buckets4, bucketslists4, 3, bucketslengths4);

  // Removing the last value in a bucket
  pqueue_remove_end_of(pqueue, IP(7));
  uint8_t  bucketslists62[1]  = {5};
  uint8_t* bucketslists6[3]   = {bucketslists41,bucketslists62,bucketslists43};
  uint8_t  bucketslengths6[2] = {1,1,1};
  helper_pqueue_elements(pqueue, buckets4, bucketslists6, 3, bucketslengths6);

  // Removing the first value of the first bucket
  // It doesn't automatically remove the empty bucket
  pqueue_remove_begin(pqueue);
  uint8_t  bucketslists71[0]  = {};
  uint8_t  bucketslists72[1]  = {5};
  uint8_t* bucketslists7[2]   = {bucketslists71,bucketslists72,bucketslists43};
  uint8_t  bucketslengths7[2] = {0,1,1};
  helper_pqueue_elements(pqueue, buckets4, bucketslists7, 3, bucketslengths7);

  // Adding a value in the empty bucket
  pqueue_prepend(pqueue, IP(3), IP(10));
  uint8_t  bucketslists81[0]  = {10};
  uint8_t* bucketslists8[3]   = {bucketslists81,bucketslists72,bucketslists43};
  helper_pqueue_elements(pqueue, buckets4, bucketslists8, 3, bucketslengths6);

  // Adding another value in the empty bucket
  pqueue_prepend(pqueue, IP(3), IP(11));
  uint8_t  bucketslists91[2]  = {11,10};
  uint8_t* bucketslists9[3]   = {bucketslists91,bucketslists72,bucketslists43};
  uint8_t  bucketslengths9[2] = {2,1,1};
  helper_pqueue_elements(pqueue, buckets4, bucketslists9, 3, bucketslengths9);

  // Removing the last value of the first bucket
  pqueue_remove_end(pqueue);
  helper_pqueue_elements(pqueue, buckets4, {{11},{5},{6}}, 3, {1,1,1});

  // Removing the last value of the first bucket
  pqueue_remove_end(pqueue);
  helper_pqueue_elements(pqueue, buckets4, {{},{5},{6}}, 3, {0,1,1});

  // Shrink Queue (removing first bucket if it's empty)
  pqueue_shrink(pqueue);
  helper_pqueue_elements(pqueue, {7,9}, {{5},{6}}, 2, {1,1});

  // Looking for the bucket value
  void* value = pqueue_at(pqueue, 1);
  assert_int_equal(value, 9);

  // Looking for a value in a bucket queue
  value = pqueue_bucket_at(pqueue, 1, 0);
  assert_int_equal(value, 6);

  queue_free(pqueue);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_pqueue_adding_removing),
    cmocka_unit_test(test_pqueue_accessors),
    cmocka_unit_test(test_pqueue_operations),
    cmocka_unit_test(test_pqueue_comparisons),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
