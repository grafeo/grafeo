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

static void test_slist_adding_removing(void** state){
  (void)state;
  SList* list = slist_new();
  SList* item;
  assert_non_null(list);
  slist_free(list);
  list = NULL;
  list = slist_prepend(list, "A"); // A
  assert_non_null(list);
  assert_null(list->next);
  assert_non_null(list->value);
  assert_string_equal(list->value, "A");

  list = slist_append(list, "B");  // A->B
  assert_non_null(list);
  assert_non_null(list->next);
  assert_string_equal(list->next->value, "B");
  assert_string_equal(list->value, "A");

  list = slist_prepend(list, "C"); // C->A->B
  assert_non_null(list);
  assert_non_null(list->next);
  assert_non_null(list->next->next);
  assert_string_equal(list->value, "C");
  assert_string_equal(list->next->value, "A");
  assert_string_equal(list->next->next->value, "B");

  list = slist_prepend_at(list, list->next, "D"); // C->D->A->B
  item = list;
  assert_non_null(item); // Testing C
  assert_string_equal(item->value, "C");
  item = item->next;
  assert_non_null(item); // Testing D
  assert_string_equal(item->value, "D");
  item = item->next;
  assert_non_null(item); // Testing A
  assert_string_equal(item->value, "A");
  item = item->next;
  assert_non_null(item); // Testing B
  assert_string_equal(item->value, "B");
  assert_null(item->next);

  list = slist_append_at(list, list->next, "E"); // C->D->E->A->B
  assert_string_equal(list->next->value, "D");
  assert_string_equal(list->next->next->value, "E");
  assert_string_equal(list->next->next->next->value, "A");

  list = slist_append_at_index(list, 2, "F"); // C->D->E->F->A->B
  item = list->next->next;
  assert_string_equal(item->value, "E");
  assert_string_equal(item->next->value, "F");
  assert_string_equal(item->next->next->value, "A");

  list = slist_prepend_at_index(list, 2, "G"); // C->D->G->E->F->A->B
  item = list->next->next;
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "E");
  assert_string_equal(item->next->next->value, "F");

  list = slist_remove(list, list->next);       // C->G->E->F->A->B
  assert_non_null(list);
  item = list;
  assert_string_equal(item->value, "C");
  assert_string_equal(item->next->value, "G");
  assert_string_equal(item->next->next->value, "E");

  list = slist_remove_from_value(list, "F");   // C->G->E->A->B
  assert_non_null(list);
  item = list->next->next;
  assert_string_equal(item->value, "E");
  assert_string_equal(item->next->value, "A");
  assert_string_equal(item->next->next->value, "B");

  list = slist_remove_at_index(list, 2);       // C->G->A->B
  assert_non_null(list);
  item = list->next;
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "A");
  assert_string_equal(item->next->next->value, "B");

  list = slist_remove_begin(list);             // G->A->B
  item = list;
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "A");
  assert_string_equal(item->next->next->value, "B");

  list = slist_remove_end(list);               // G->A
  item = list;
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "A");

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

  item = slist_begin(list);
  assert_int_equal(item, list);
  item = slist_end(list);
  assert_int_equal(item->value, 0);
  assert_int_equal(slist_length(list), 10);
  assert_false(slist_is_empty(list));
  assert_true(slist_is_empty(NULL));
  item = slist_at(list, 4);
  assert_int_equal(item->value, 5);
  assert_int_equal(POINTER_TO_INT8(slist_value_at(list, 6)),3);
  item = slist_next(list->next);
  assert_int_equal(item->value, 7);
  item = slist_prev(list->next);
  assert_int_equal(item, list);

  item  = slist_find(list, INT8_TO_POINTER(7));
  SList* item2 = slist_find(list, INT8_TO_POINTER(8));
  assert_non_null(item);
  assert_int_equal(item2->next, item);
  assert_int_equal(item->value, 7);
  assert_int_equal(item->next->value, 6);

  slist_free(list);
}

static void test_slist_operations(void** state){
  (void)state;
  SList* list = NULL, *item1 = NULL, *item2 = NULL, *list2 = NULL, *current = NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 8 ; i++) list  = slist_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list2 = slist_prepend(list2, INT8_TO_POINTER(items[i]));

  list  = slist_join(list, list2); // 7 6 5 4 3 2 1 0 9 8
  assert_non_null(list);
  assert_int_equal(slist_length(list), 10);
  assert_int_equal(list->value, 7);
  SList* item = slist_at(list,8);
  assert_int_equal(item->value, 9);

  list2 = slist_split_at(list, 8); // 7 6 5 4 3 2 1 0     9 8
  assert_non_null(list2);
  assert_int_equal(slist_length(list), 8);
  assert_int_equal(slist_length(list2), 2);
  assert_int_equal(list2->value, 9);
  item1 = slist_at(list,7);
  assert_null(item1->next);
  assert_int_equal(item, list2);

  item1 = slist_at(list,3); // value = 4
  item2 = slist_at(list,5); // value = 2

  list = slist_swap(list, item1, item2); // 7 6 5 2 3 4 1 0
  assert_non_null(list);
  assert_int_equal(item1->value, 4);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item1->next->value, 1);
  assert_int_equal(item2->next->value, 3);

  item1 = slist_at(list, 2); // value=5
  item2 = item1->next;      // value=2
  list = slist_swap_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  assert_non_null(list);
  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  list = slist_swap_values(list, item1, item2); // 7 6 5 2 3 4 1 0
  assert_non_null(list);

  assert_int_equal(item1->value, 2);
  assert_int_equal(item2->value, 5);

  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  slist_swap_values_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  assert_non_null(list);

  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);

  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->next->value, 3);

  list2 = slist_copy(list);
  assert_int_equal(slist_length(list), slist_length(list2));
  for(item1=list, item2=list2; item; item=item->next, item2=item2->next){
    assert_int_not_equal(item, item2);
    assert_int_equal(item->value, item->value);
  }

  slist_free(list); list  = NULL;
  slist_free(list2);list2 = NULL;
  SList* list3 = NULL, *item3 = NULL;
  for(i = 0; i < 5 ; i++) list  = slist_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 5; i < 8 ; i++) list2 = slist_prepend(list2, INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list3 = slist_prepend(list3, INT8_TO_POINTER(items[i]));

  item1 = slist_at(list, 3); // value = 6
  item2 = list2;
  item3 = list3;
  uint32_t old_length  = slist_length(list);// 5
  uint32_t old_length1 = 1;
  uint32_t old_length2 = slist_length(item2);// 3
  uint32_t old_length3 = slist_length(item3);// 2

  SList* item_prev = slist_find(list,INT8_TO_POINTER(7)); // value = 7
  SList* item_next = item1->next;     // value = 5
  SList* item2_end = slist_end(item2); // value = 2

  list = slist_replace(list, item1, item2); // 4 3 2 7 6 5 0
  uint8_t values[7] = {4,3,2,7,6,5,0};
  current = list;
  for(i = 0; i < 7; i++, current = current->next){
    assert_int_equal(current->value, values[i]);
  }
  assert_null(item1->next);
  assert_int_equal(slist_length(list), old_length - old_length1 + old_length2);
  assert_int_equal(item2          , item_prev->next);
  assert_int_equal(item2_end->next, item_next);
  slist_free(item1);

  item1     = slist_at(list, 2); // value = 2
  item_prev = slist_at(list, 1); // value = 3
  item_next = item1->next;
  SList* item3_end = slist_end(item3);

  list = slist_replace_at(list, 2, item3); // 4 3 9 8 7 6 5 0
  assert_non_null(list);
  uint8_t values3[8] = {4,3,9,8,7,6,5,0};
  current = list;
  for(i = 0; i < 8; i++, current = current->next){
    assert_int_equal(current->value, values3[i]);
  }
  assert_int_equal(slist_length(list), old_length - 2*old_length1 + old_length2 + old_length3);
  assert_int_equal(item3          , item_prev->next);
  assert_int_equal(item3_end->next, item_next);

  list2 = slist_reverse(list);
  assert_non_null(list2);
  uint8_t values2[8] = {0,5,6,7,8,9,3,4};
  current = list2;
  for(i = 0; i < 8; i++, current=current->next)
    assert_int_equal(current->value, values2[i]);

  current = list;

  slist_free(item1);
  slist_free(list);
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
  const struct CMUnitTest tests[]={
    cmocka_unit_test(test_slist_adding_removing),
    cmocka_unit_test(test_slist_accessors),
    cmocka_unit_test(test_slist_operations),
    cmocka_unit_test(test_slist_comparisons),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
