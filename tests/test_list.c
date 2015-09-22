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
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <grafeo/list.h>

void helper_test_list_str(List* begin, char** values, uint8_t qty){
  List* current = begin, *before;
  assert_non_null(current);
  assert_null(list_prev(current));
  uint8_t i;
  for(i = 0; i < qty; i++, current=list_next(current)){
    assert_non_null(list_value(current));
    assert_string_equal(values[i],list_value(current));
    before = current;
  }
  assert_null(current);
  current = before;
  for(i = 0; i < qty; i++, current=current->prev){
    assert_non_null(list_value(current));
    assert_string_equal(values[qty-1-i],list_value(current));
  }
}

void helper_test_list_uint8(List* begin, uint8_t* values, uint8_t qty){
  List* current = begin, *before;
  assert_non_null(current);
  assert_null(list_prev(current));
  uint8_t i;
  for(i = 0; i < qty; i++, current=list_next(current)){
    assert_int_equal(values[i],list_value(current));
    before = current;
  }
  assert_null(current);
  current = before;
  for(i = 0; i < qty; i++, current=current->prev){
    assert_int_equal(values[qty-1-i],list_value(current));
  }
}

static void test_list_adding_removing(void** state){
  (void)state;
  List* list = list_new();
  assert_non_null(list);
  list_free(list);
  list = NULL;
  list = list_prepend(list, "A"); // A
  char* values1[3] = {"C","A","B"};
  helper_test_list_str(list, values1+1, 1);

  list = list_append(list, "B");  // A->B
  helper_test_list_str(list, values1+1, 2);

  list = list_prepend(list, "C"); // C->A->B
  helper_test_list_str(list, values1, 3);

  list = list_prepend_at(list, list_next(list), "D"); // C->D->A->B
  char* values2[4] = {"C","D","A","B"};
  helper_test_list_str(list, values2, 4);

  list = list_append_at(list, list_next(list), "E"); // C->D->E->A->B
  char* values3[5] = {"C","D","E","A","B"};
  helper_test_list_str(list, values3, 5);

  list = list_append_at_index(list, 2, "F"); // C->D->E->F->A->B
  char* values4[6] = {"C","D","E","F","A","B"};
  helper_test_list_str(list, values4, 6);

  list = list_prepend_at_index(list, 2, "G"); // C->D->G->E->F->A->B
  char* values5[7] = {"C","D","G","E","F","A","B"};
  helper_test_list_str(list, values5, 7);

  list = list_remove(list, list_next(list));       // C->G->E->F->A->B
  char* values6[6] = {"C","G","E","F","A","B"};
  helper_test_list_str(list, values6, 6);

  list = list_remove_from_value(list, "F");   // C->G->E->A->B
  char* values7[5] = {"C","G","E","A","B"};
  helper_test_list_str(list, values7, 5);

  list = list_remove_at_index(list, 2);       // C->G->A->B
  char* values8[4] = {"C","G","A","B"};
  helper_test_list_str(list, values8, 4);

  list = list_remove_begin(list);             // G->A->B
  char* values9[3] = {"G","A","B"};
  helper_test_list_str(list, values9, 3);

  list = list_remove_end(list);               // G->A
  char* values10[2] = {"G","A"};
  helper_test_list_str(list, values10, 2);

  list_free(list);
}

static void test_list_accessors(void** state){
  (void)state;
  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  List* list = NULL, *item = NULL;
  uint32_t index;
  // put in reverse order (from 9 to 0)
  for(i = 0; i < 10; i++) list = list_prepend(list, INT8_TO_POINTER(items[i]));


  index = list_index_of(list, INT8_TO_POINTER(8)); // the 2nd value
  assert_int_equal(index, 1);
  index = list_index_of(list, INT8_TO_POINTER(9)); // the first value
  assert_int_equal(index, 0);
  index = list_index_of(list, INT8_TO_POINTER(0)); // the last value
  assert_int_equal(index, 9);

  item = list_begin(list);
  assert_int_equal(item, list);
  item = list_end(list);
  assert_int_equal(list_value(item), 0);
  assert_int_equal(list_length(list), 10);
  assert_false(list_is_empty(list));
  assert_true(list_is_empty(NULL));
  item = list_at(list, 4);
  assert_int_equal(list_value(item), 5);
  assert_int_equal(POINTER_TO_INT8(list_value_at(list, 6)),3);
  item = list_next(list_next(list));
  assert_int_equal(list_value(item), 7);
  item = list_prev(list_next(list));
  assert_int_equal(item, list);

  item = list_find(list, INT8_TO_POINTER(7));
  assert_non_null(item);
  assert_int_equal(list_value(list_prev(item)), 8);
  assert_int_equal(list_value(item), 7);
  assert_int_equal(list_value(list_next(item)), 6);

  list_free(list);
}

static void test_list_operations(void** state){
  (void)state;
  List* list = NULL, *item1 = NULL, *item2 = NULL, *list2 = NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 8 ; i++) list  = list_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list2 = list_prepend(list2, INT8_TO_POINTER(items[i]));

  // Joining two lists
  list  = list_join(list, list2); // 7 6 5 4 3 2 1 0 9 8
  uint8_t values1[10] = {7,6,5,4,3,2,1,0,9,8};
  helper_test_list_uint8(list, values1, 10);

  // Splitting a list
  list2 = list_split_at(list, 8); // 7 6 5 4 3 2 1 0     9 8
  helper_test_list_uint8(list , values1, 8);
  helper_test_list_uint8(list2, values1+8, 2);

  // Swapping two items
  item1 = list_at(list,3); // value = 4
  item2 = list_at(list,5); // value = 2
  list = list_swap_items(list, item1, item2); // 7 6 5 2 3 4 1 0
  uint8_t values2[8] = {7,6,5,2,3,4,1,0};
  helper_test_list_uint8(list, values2, 8);
  assert_int_equal(list_value(item1), 4);
  assert_int_equal(list_value(item2), 2);

  // Swapping two items by indices
  item1 = list_at(list, 2); // value=5
  item2 = list_next(item1);      // value=2
  list = list_swap_items_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  uint8_t values3[8] = {7,6,2,5,3,4,1,0};
  helper_test_list_uint8(list, values3, 8);
  assert_int_equal(list_value(item1), 5);
  assert_int_equal(list_value(item2), 2);

  // Swapping two values
  list = list_swap(list, item1, item2); // 7 6 5 2 3 4 1 0
  helper_test_list_uint8(list, values2, 8);
  assert_int_equal(list_value(item1), 2);
  assert_int_equal(list_value(item2), 5);

  // Swapping two values by indices
  list_swap_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  helper_test_list_uint8(list, values3, 8);
  assert_int_equal(list_value(item1), 5);
  assert_int_equal(list_value(item2), 2);

  // Duplicating a list
  list2 = list_copy(list);
  assert_int_equal(list_length(list), list_length(list2));
  for(item1=list, item2=list2; item1; item1=list_next(item1), item2=list_next(item2)){
    assert_int_not_equal(item1, item2);
    assert_int_equal(list_value(item1), list_value(item2));
  }

  list_free(list); list  = NULL;
  list_free(list2);list2 = NULL;
  List* list3 = NULL, *item3 = NULL;
  for(i = 0; i < 5 ; i++) list  = list_prepend(list , INT8_TO_POINTER(items[i])); // 4 3 2 1 0
  for(i = 5; i < 8 ; i++) list2 = list_prepend(list2, INT8_TO_POINTER(items[i])); // 7 6 5
  for(i = 8; i < 10; i++) list3 = list_prepend(list3, INT8_TO_POINTER(items[i])); // 9 8

  // Replacing an item by another
  item1 = list_at(list, 3); // value = 1
  item2 = list2;            // value = 7
  item3 = list3;            // value = 9

  list = list_replace(list, item1, item2);  // 4 3 2 7 0
  uint8_t values4[5] = {4,3,2,7,0};
  helper_test_list_uint8(list, values4,5);

  // Replace an item by index
  item1 = list_at(list, 2); // value = 2
  list = list_replace_at(list, 2, item3); // 4 3 9 7 0
  uint8_t values5[5] = {4,3,9,7,0};
  helper_test_list_uint8(list, values5, 5);

  // Reversing a list
  list2 = list_reverse(list);
  uint8_t values6[5] = {0,7,9,3,4};
  helper_test_list_uint8(list2, values6, 5);

  list_free(list2);
}

static void test_list_comparisons(void** state){
  (void)state;
  List *list1=NULL, *list2=NULL, *list=NULL, *list3=NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 5 ; i++) list  = list_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 0; i < 5 ; i++) list1 = list_prepend(list1, INT8_TO_POINTER(items[i]));
  for(i = 0; i < 6 ; i++) list2 = list_prepend(list2, INT8_TO_POINTER(items[i]));
  for(i = 5; i < 10; i++) list3 = list_prepend(list3, INT8_TO_POINTER(items[i]));

  assert_true (list_is_equal    (list, list1));
  assert_true (list_is_different(list, list2));
  assert_true (list_is_different(list, list3));

  assert_false(list_is_different(list, list1));
  assert_false(list_is_equal    (list, list2));
  assert_false(list_is_equal    (list, list3));

  list_free(list1);
  list_free(list2);
  list_free(list3);
  list_free(list);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_list_adding_removing),
    cmocka_unit_test(test_list_accessors),
    cmocka_unit_test(test_list_operations),
    cmocka_unit_test(test_list_comparisons),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
