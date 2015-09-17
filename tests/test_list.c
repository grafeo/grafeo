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

static void test_list_adding_removing(void** state){
  (void)state;
  List* list = list_new();
  List* item;
  assert_non_null(list);
  list_free(list);
  list = NULL;
  list = list_prepend(list, "A"); // A
  assert_non_null(list);
  assert_null(list->next);
  assert_non_null(list->value);
  assert_string_equal(list->value, "A");
  assert_null(list->prev);

  list = list_append(list, "B");  // A->B
  assert_non_null(list);
  assert_null(list->prev);
  assert_non_null(list->next);
  assert_string_equal(list->next->value, "B");
  assert_string_equal(list->value, "A");
  assert_int_equal(list->next->prev, list);

  list = list_prepend(list, "C"); // C->A->B
  assert_non_null(list);
  assert_null(list->prev);
  assert_non_null(list->next);
  assert_non_null(list->next->next);
  assert_string_equal(list->value, "C");
  assert_string_equal(list->next->value, "A");
  assert_string_equal(list->next->next->value, "B");
  assert_int_equal(list->next->prev, list);
  assert_int_equal(list->next->next->prev, list->next);

  list = list_prepend_at(list, list->next, "D"); // C->D->A->B
  item = list;
  assert_non_null(item); // Testing C
  assert_string_equal(item->value, "C");
  assert_null(list->prev);
  item = item->next;
  assert_non_null(item); // Testing D
  assert_string_equal(item->value, "D");
  assert_non_null(item->prev);
  item = item->next;
  assert_non_null(item); // Testing A
  assert_string_equal(item->value, "A");
  assert_non_null(item->prev);
  item = item->next;
  assert_non_null(item); // Testing B
  assert_string_equal(item->value, "B");
  assert_non_null(item->prev);
  assert_null(item->next);

  list = list_append_at(list, list->next, "E"); // C->D->E->A->B
  assert_string_equal(list->next->next->prev->value, "D");
  assert_string_equal(list->next->next->value, "E");
  assert_string_equal(list->next->next->next->value, "A");

  list = list_append_at_index(list, 2, "F"); // C->D->E->F->A->B
  item = list->next->next->next;
  assert_string_equal(item->prev->value, "E");
  assert_string_equal(item->value, "F");
  assert_string_equal(item->next->value, "A");

  list = list_prepend_at_index(list, 2, "G"); // C->D->G->E->F->A->B
  item = list->next->next->next;
  assert_string_equal(item->prev->value, "G");
  assert_string_equal(item->value, "E");
  assert_string_equal(item->next->value, "F");

  list = list_remove(list, list->next);       // C->G->E->F->A->B
  assert_non_null(list);
  item = list->next;
  assert_string_equal(item->prev->value, "C");
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "E");

  list = list_remove_from_value(list, "F");   // C->G->E->A->B
  assert_non_null(list);
  item = list->next->next->next;
  assert_string_equal(item->prev->value, "E");
  assert_string_equal(item->value, "A");
  assert_string_equal(item->next->value, "B");

  list = list_remove_at_index(list, 2);       // C->G->A->B
  assert_non_null(list);
  item = list->next->next;
  assert_string_equal(item->prev->value, "G");
  assert_string_equal(item->value, "A");
  assert_string_equal(item->next->value, "B");

  list = list_remove_begin(list);             // G->A->B
  item = list->next;
  assert_string_equal(item->prev->value, "G");
  assert_string_equal(item->value, "A");
  assert_string_equal(item->next->value, "B");

  list = list_remove_end(list);               // G->A
  item = list;
  assert_string_equal(item->value, "G");
  assert_string_equal(item->next->value, "A");

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
  assert_int_equal(item->value, 0);
  assert_int_equal(list_length(list), 10);
  assert_false(list_is_empty(list));
  assert_true(list_is_empty(NULL));
  item = list_at(list, 4);
  assert_int_equal(item->value, 5);
  assert_int_equal(POINTER_TO_INT8(list_value_at(list, 6)),3);
  item = list_next(list->next);
  assert_int_equal(item->value, 7);
  item = list_prev(list->next);
  assert_int_equal(item, list);

  item = list_find(list, INT8_TO_POINTER(7));
  assert_non_null(item);
  assert_int_equal(item->prev->value, 8);
  assert_int_equal(item->value, 7);
  assert_int_equal(item->next->value, 6);

  list_free(list);
}

static void test_list_operations(void** state){
  (void)state;
  List* list = NULL, *item1 = NULL, *item2 = NULL, *list2 = NULL, *current = NULL;

  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  for(i = 0; i < 8 ; i++) list  = list_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list2 = list_prepend(list2, INT8_TO_POINTER(items[i]));

  list  = list_join(list, list2); // 7 6 5 4 3 2 1 0 9 8
  assert_non_null(list);
  assert_int_equal(list_length(list), 10);
  assert_int_equal(list->value, 7);
  List* item = list_at(list,8);
  assert_int_equal(item->value, 9);
  assert_non_null(item->prev);

  list2 = list_split_at(list, 8); // 7 6 5 4 3 2 1 0     9 8
  assert_non_null(list2);
  assert_int_equal(list_length(list), 8);
  assert_int_equal(list_length(list2), 2);
  assert_int_equal(list2->value, 9);
  item1 = list_at(list,7);
  assert_null(item1->next);
  assert_null(list2->prev);
  assert_int_equal(item, list2);

  item1 = list_at(list,3); // value = 4
  item2 = list_at(list,5); // value = 2

  list = list_swap(list, item1, item2); // 7 6 5 2 3 4 1 0
  assert_non_null(list);
  assert_int_equal(item1->value, 4);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item1->prev->value, 3);
  assert_int_equal(item1->next->value, 1);
  assert_int_equal(item2->prev->value, 5);
  assert_int_equal(item2->next->value, 3);

  item1 = list_at(list, 2); // value=5
  item2 = item1->next;      // value=2
  list = list_swap_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  assert_non_null(list);
  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);
  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->prev, item2);
  assert_int_equal(item2->prev->value, 6);
  assert_int_equal(item1->next->value, 3);

  list = list_swap_values(list, item1, item2); // 7 6 5 2 3 4 1 0
  assert_non_null(list);

  assert_int_equal(item1->value, 2);
  assert_int_equal(item2->value, 5);

  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->prev, item2);
  assert_int_equal(item2->prev->value, 6);
  assert_int_equal(item1->next->value, 3);

  list_swap_values_at(list, 2, 3); // 7 6 2 5 3 4 1 0
  assert_non_null(list);

  assert_int_equal(item1->value, 5);
  assert_int_equal(item2->value, 2);

  assert_int_equal(item2->next, item1);
  assert_int_equal(item1->prev, item2);
  assert_int_equal(item2->prev->value, 6);
  assert_int_equal(item1->next->value, 3);

  list2 = list_copy(list);
  assert_int_equal(list_length(list), list_length(list2));
  for(item1=list, item2=list2; item; item=item->next, item2=item2->next){
    assert_int_not_equal(item, item2);
    assert_int_equal(item->value, item->value);
  }

  list_free(list); list  = NULL;
  list_free(list2);list2 = NULL;
  List* list3 = NULL, *item3 = NULL;
  for(i = 0; i < 5 ; i++) list  = list_prepend(list , INT8_TO_POINTER(items[i]));
  for(i = 5; i < 8 ; i++) list2 = list_prepend(list2, INT8_TO_POINTER(items[i]));
  for(i = 8; i < 10; i++) list3 = list_prepend(list3, INT8_TO_POINTER(items[i]));

  item1 = list_at(list, 3); // value = 6
  item2 = list2;
  item3 = list3;
  uint32_t old_length  = list_length(list);// 5
  uint32_t old_length1 = 1;
  uint32_t old_length2 = list_length(item2);// 3
  uint32_t old_length3 = list_length(item3);// 2

  List* item_prev = item1->prev;     // value = 7
  List* item_next = item1->next;     // value = 5
  List* item2_end = list_end(item2); // value = 2

  list = list_replace(list, item1, item2); // 4 3 2 7 6 5 0
  uint8_t values[7] = {4,3,2,7,6,5,0};
  current = list;
  for(i = 0; i < 7; i++, current = current->next){
    assert_int_equal(current->value, values[i]);
  }
  assert_null(item1->prev);
  assert_null(item1->next);
  assert_int_equal(list_length(list), old_length - old_length1 + old_length2);
  assert_int_equal(item2          , item_prev->next);
  assert_int_equal(item2->prev    , item_prev);
  assert_int_equal(item2_end      , item_next->prev);
  assert_int_equal(item2_end->next, item_next);
  list_free(item1);

  item1 = list_at(list, 2); // value = 2
  item_prev = item1->prev;
  item_next = item1->next;
  List* item3_end = list_end(item3);

  list = list_replace_at(list, 2, item3); // 4 3 9 8 7 6 5 0
  assert_non_null(list);
  uint8_t values3[8] = {4,3,9,8,7,6,5,0};
  current = list;
  for(i = 0; i < 8; i++, current = current->next){
    assert_int_equal(current->value, values3[i]);
  }
  assert_int_equal(list_length(list), old_length - 2*old_length1 + old_length2 + old_length3);
  assert_int_equal(item3          , item_prev->next);
  assert_int_equal(item3->prev    , item_prev);
  assert_int_equal(item3_end      , item_next->prev);
  assert_int_equal(item3_end->next, item_next);

  list2 = list_reverse(list);
  assert_non_null(list2);
  uint8_t values2[8] = {0,5,6,7,8,9,3,4};
  current = list2;
  for(i = 0; i < 8; i++, current=current->next)
    assert_int_equal(current->value, values2[i]);

  current = list;

  for(i = 0; i < 8; i++, current=current->prev)
    assert_int_equal(current->value, values2[7-i]);

  list_free(item1);
  list_free(list);
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
