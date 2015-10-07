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
#include <grafeo/slist.h>

void helper_test_slist_str(SList* begin, char** values, uint8_t qty){
  SList* current = begin;
  assert_non_null(current);
  uint8_t i;
  for(i = 0; i < qty; i++, current=current->next){
    assert_non_null(current->value);
    assert_string_equal(values[i],current->value);
  }
  assert_null(current);
}

void helper_test_slist_uint8(SList* begin, uint8_t* values, uint8_t qty){
  SList* current = begin;
  assert_non_null(current);
  uint8_t i;
  for(i = 0; i < qty; i++, current=current->next){
    assert_int_equal(values[i],current->value);
  }
  assert_null(current);
}


static void test_slist_adding_removing(void** state){
  (void)state;
  char* values1[3]={"C","A","B"};
  char* values2[4]={"C","D","A","B"};
  char* values3[5]={"C","D","E","A","B"};
  char* values4[6]={"C","D","E","F","A","B"};
  char* values5[7]={"C","D","G","E","F","A","B"};
  char* values6[6]={"C","G","E","F","A","B"};
  char* values7[5]={"C","G","E","A","B"};
  char* values8[4]={"C","G","A","B"};
  char* values9[3]={"G","A","B"};
  char* values10[2]={"G","A"};
  char* values11[3]={"G","A","H"};
  char* values12[4]={"G","A","I","H"};
  char* values13[5]={"G","A","K","I","H"};
  char* values14[6]={"L","G","A","K","I","H"};
  char* values15[7]={"L","G","M","A","K","I","H"};
  char* values16[8]={"L","G","M","N","A","K","I","H"};
  SList* list = slist_new();
  assert_non_null(list);
  slist_free(list);
  list = slist_new_with_value("2");
  assert_non_null(list);
  assert_string_equal(list->value, "2");
  slist_free(list);

  list = NULL;
  list = slist_prepend(list, "A"); // A
  helper_test_slist_str(list,&values1[1],1);

  list = slist_append(list, "B");  // A->B
  helper_test_slist_str(list,&values1[1],2);

  list = slist_prepend(list, "C"); // C->A->B
  helper_test_slist_str(list,values1,3);

  list = slist_prepend_at_item(list, list->next, "D"); // C->D->A->B
  helper_test_slist_str(list,values2,4);

  list = slist_append_at_item(list, list->next, "E"); // C->D->E->A->B
  helper_test_slist_str(list,values3,5);

  list = slist_append_at(list, 2, "F");               // C->D->E->F->A->B
  helper_test_slist_str(list,values4,6);

  list = slist_prepend_at(list, 2, "G");              // C->D->G->E->F->A->B
  helper_test_slist_str(list,values5,7);

  list = slist_remove_item(list, list->next);         // C->G->E->F->A->B
  helper_test_slist_str(list,values6,6);

  list = slist_remove(list, "F");                     // C->G->E->A->B
  helper_test_slist_str(list,values7,5);

  list = slist_remove_at(list, 2);                    // C->G->A->B
  helper_test_slist_str(list,values8,4);

  list = slist_remove_begin(list);                    // G->A->B
  helper_test_slist_str(list,values9,3);

  list = slist_remove_end(list);                     // G->A
  helper_test_slist_str(list,values10,2);

  list = slist_append_item(list, slist_prepend(NULL,"H"));
  helper_test_slist_str(list,values11,3);

  list = slist_append_item_at(list, 1, slist_prepend(NULL, "I"));
  helper_test_slist_str(list,values12,4);

  list = slist_append_item_at_item(list, list->next, slist_prepend(NULL,"K"));
  helper_test_slist_str(list,values13,5);

  list = slist_prepend_item(list, slist_prepend(NULL,"L"));
  helper_test_slist_str(list,values14,6);

  list = slist_prepend_item_at(list, 2, slist_prepend(NULL, "M"));
  helper_test_slist_str(list,values15,7);

  list = slist_prepend_item_at_item(list, list->next->next->next, slist_prepend(NULL,"N"));
  helper_test_slist_str(list,values16,8);

  slist_free(list);
}

static void test_slist_accessors(void** state){
  (void)state;
  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  SList* list = NULL, *item = NULL;
  uint32_t index;
  // put in reverse order (from 9 to 0)
  for(i = 0; i < 10; i++) list = slist_prepend(list, INT8_TO_POINTER(items[i]));


  index = slist_index_of(list, INT8_TO_POINTER(8)); // the 2nd value
  assert_int_equal(index, 1);
  index = slist_index_of(list, INT8_TO_POINTER(9)); // the first value
  assert_int_equal(index, 0);
  index = slist_index_of(list, INT8_TO_POINTER(0)); // the last value
  assert_int_equal(index, 9);
  item  = slist_item_of(list,  INT8_TO_POINTER(9)); // the first value
  assert_int_equal(list, item);

  item = slist_item_end(list);
  assert_int_equal(item->value, 0);
  assert_int_equal(slist_length(list), 10);
  assert_false(slist_is_empty(list));
  assert_true(slist_is_empty(NULL));
  assert_int_equal(slist_end(list), 0);
  item = slist_item_at(list, 4);
  assert_int_equal(item->value, 5);
  assert_int_equal(POINTER_TO_INT8(slist_at(list, 6)),3);
  item = slist_next(list->next);
  assert_int_equal(item->value, 7);
  item = slist_prev(list, list->next);
  assert_int_equal(item, list);

  slist_free(list);
}

static void test_slist_operations(void** state){
  (void)state;
  SList* list = NULL, *item1 = NULL, *item2 = NULL, *list2 = NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 8 ; i++) list  = slist_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list2 = slist_prepend(list2, INT8_TO_POINTER(items[i]));

  list  = slist_join(list, list2); // 7 6 5 4 3 2 1 0 9 8
  uint8_t values1[10] = {7, 6, 5, 4, 3, 2, 1, 0, 9, 8};
  helper_test_slist_uint8(list, values1, 10);

  list2 = slist_split_at(list, 8); // 7 6 5 4 3 2 1 0     9 8
  helper_test_slist_uint8(list, values1, 8);
  helper_test_slist_uint8(list2, &values1[8], 2);

  item1 = slist_item_at(list,3); // value = 4
  item2 = slist_item_at(list,5); // value = 2
  list = slist_swap_items(list, item1, item2); // 7 6 5 2 3 4 1 0
  uint8_t values2[8] = {7, 6, 5, 2, 3, 4, 1, 0};
  helper_test_slist_uint8(list, values2, 8);
  assert_int_equal(item1->value, 4);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item1->next->value, 1);
  assert_int_equal(item2->next->value, 3);

  item1 = slist_item_at(list, 2); // value=5
  item2 = item1->next;      // value=2
  list = slist_swap_items_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  uint8_t values3[8] = {7, 6, 2, 5, 3, 4, 1, 0};
  helper_test_slist_uint8(list, values3, 8);
  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  list = slist_swap(list, item1, item2); // 7 6 5 2 3 4 1 0
  uint8_t values4[8] = {7, 6, 5, 2, 3, 4, 1, 0};
  helper_test_slist_uint8(list, values4, 8);
  assert_int_equal(item1->value, 2);
  assert_int_equal(item2->value, 5);
  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  slist_swap_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  uint8_t values5[8] = {7, 6, 2, 5, 3, 4, 1, 0};
  helper_test_slist_uint8(list, values5, 8);
  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  list2 = slist_copy(list);
  helper_test_slist_uint8(list2, values5, 8);
  assert_int_equal(slist_length(list), slist_length(list2));
  for(item1=list, item2=list2; item1; item1=item1->next, item2=item2->next){
    assert_int_not_equal(item1, item2);
    assert_int_equal(item1->value, item2->value);
  }

  slist_free(list); list  = NULL;
  slist_free(list2);list2 = NULL;
  SList* list3 = NULL, *item3 = NULL;
  for(i = 0; i < 5 ; i++) list  = slist_prepend(list , INT8_TO_POINTER(items[i])); // 4 3 2 1 0
  for(i = 5; i < 8 ; i++) list2 = slist_prepend(list2, INT8_TO_POINTER(items[i])); // 7 6 5
  for(i = 8; i < 10; i++) list3 = slist_prepend(list3, INT8_TO_POINTER(items[i])); // 9 8

  item1 = slist_item_at(list, 3);            // value = 1
  item2 = list2;                             // value = 7
  item3 = list3;                             // value = 9
  uint32_t old_length  = slist_length(list); // 5
  SList* item_prev = slist_item_of(list,INT8_TO_POINTER(2)); // value = 2

  list2 = list2->next; // 6 5
  list  = slist_replace(list, item1, item2);  // 4 3 2 7 0
  uint8_t values6[5] = {4,3,2,7,0};
  helper_test_slist_uint8(list, values6, 5);
  assert_int_equal(slist_length(list), old_length);
  assert_int_equal(item2          , item_prev->next);

  item1     = slist_item_at(list, 2); // value = 2
  item_prev = slist_item_at(list, 1); // value = 3

  list = slist_replace_at(list, 2, item3); // 4 3 9 7 0
  uint8_t values7[5] = {4,3,9,7,0};
  helper_test_slist_uint8(list, values7, 5);
  assert_int_equal(slist_length(list), old_length);
  assert_int_equal(item3          , item_prev->next);

  list2 = slist_reverse(list);
  assert_non_null(list2);
  uint8_t values8[5] = {0,7,9,3,4};
  helper_test_slist_uint8(list2, values8, 5);

  slist_free(list2);
}

static void test_slist_comparisons(void** state){
  (void)state;
  SList *list1=NULL, *list2=NULL, *list=NULL, *list3=NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 5 ; i++) list  = slist_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 0; i < 5 ; i++) list1 = slist_prepend(list1, INT8_TO_POINTER(items[i]));
  for(i = 0; i < 6 ; i++) list2 = slist_prepend(list2, INT8_TO_POINTER(items[i]));
  for(i = 5; i < 10; i++) list3 = slist_prepend(list3, INT8_TO_POINTER(items[i]));

  assert_true (slist_is_equal    (list, list1));
  assert_true (slist_is_different(list, list2));
  assert_true (slist_is_different(list, list3));

  assert_false(slist_is_different(list, list1));
  assert_false(slist_is_equal    (list, list2));
  assert_false(slist_is_equal    (list, list3));

  slist_free(list1);
  slist_free(list2);
  slist_free(list3);
  slist_free(list);
}

int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[4]={
    cmocka_unit_test(test_slist_adding_removing),
    cmocka_unit_test(test_slist_accessors),
    cmocka_unit_test(test_slist_operations),
    cmocka_unit_test(test_slist_comparisons),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
