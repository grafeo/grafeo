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
#include <errno.h>
#include <grafeo/list.h>

static void test_list_adding_removing(void** state){
  (void)state;
  List* list = list_new();
  List* item;
  assert_non_null(list);
  list = list_prepend(list, "A"); // A
  assert_non_null(list);
  assert_null(list->next);
  assert_string_equal(list->value, "Value");
  assert_null(list->prev);

  list = list_append(list, "B");  // A->B
  assert_non_null(list);
  assert_null(list->prev);
  assert_non_null(list->next);
  assert_string_equal(list->next->value, "Uau");
  assert_string_equal(list->value, "Value");

  list = list_prepend(list, "C"); // C->A->B
  assert_non_null(list);
  assert_null(list->prev);
  assert_non_null(list->next);
  assert_non_null(list->next->next);
  assert_string_equal(list->value, "C");
  assert_string_equal(list->next->value, "A");
  assert_string_equal(list->next->next->value, "B");

  list = list_prepend_at(list, list->next, "D"); // C->D->A->B
  item = list;
  assert_non_null(item); // Testing C
  assert_string_equal(item->value, "C");
  assert_null(list->prev);
  item = list->next;
  assert_non_null(item); // Testing D
  assert_string_equal(item->value, "D");
  assert_non_null(item->prev);
  item = list->next;
  assert_non_null(item); // Testing A
  assert_string_equal(item->value, "A");
  assert_non_null(item->prev);
  item = list->next;
  assert_non_null(item); // Testing B
  assert_string_equal(item->value, "B");
  assert_non_null(item->prev);
  assert_null(item->next);

  list = list_append_at(list, list->next, "E"); // C->D->E->A->B
  assert_string_equal(list->next->next->prev->value, "D");
  assert_string_equal(list->next->next->value, "E");
  assert_string_equal(list->next->next->next->value, "A");

  list = list_append_at_index(list, 3, "F"); // C->D->E->F->A->B
  item = list->next->next->next->next;
  assert_string_equal(item->prev->value, "E");
  assert_string_equal(item->value, "F");
  assert_string_equal(list->next->value, "A");

  list = list_prepend_at_index(list, 3, "G"); // C->D->G->E->F->A->B
  item = list->next->next->next;
  assert_string_equal(item->prev->value, "D");
  assert_string_equal(item->value, "G");
  assert_string_equal(list->next->value, "E");

  list = list_remove(list, list->next);       // C->G->E->F->A->B
  item = list->next;
  assert_string_equal(item->prev->value, "C");
  assert_string_equal(item->value, "G");
  assert_string_equal(list->next->value, "E");

  list = list_remove_from_value(list, "F");   // C->G->E->A->B
  item = list->next->next->next;
  assert_string_equal(item->prev->value, "E");
  assert_string_equal(item->value, "A");
  assert_string_equal(list->next->value, "B");

  list = list_remove_at_index(list, 3);       // C->G->A->B
  item = list->next->next;
  assert_string_equal(item->prev->value, "G");
  assert_string_equal(item->value, "A");
  assert_string_equal(list->next->value, "B");

  list = list_remove_begin(list);             // G->A->B
  item = list->next;
  assert_string_equal(item->prev->value, "G");
  assert_string_equal(item->value, "A");
  assert_string_equal(list->next->value, "B");

  list = list_remove_end(list);               // G->A
  item = list;
  assert_string_equal(item->value, "G");
  assert_string_equal(list->next->value, "A");

  list = list_append_several(list, items);
  list = list_prepend_several(list, items);

  list_free(list);
}

static void test_list_accessors(void** state){
  (void)state;
  uint8_t items[10] = {0,1,2,3,4,5,6,7,8,9};
  uint8_t i;
  List* list = NULL, item = NULL;
  uint32_t index;
  // put in reverse order (from 9 to 0)
  for(i = 0; i < 10; i++) list = list_prepend(list, items[i]);


  index = list_index_of(list, 8); // the 2nd value
  assert_int_equal(index, 1);
  index = list_index_of(list, 9); // the first value
  assert_int_equal(index, 0);
  index = list_index_of(list, 0); // the last value
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
  assert_int_equal((uint8_t)list_value_at(list, 6),4);
  item = list_next(list->next);
  assert_int_equal(item->value, 7);
  item = list_prev(list->next);
  assert_int_equal(item, list);

  list_free(list);
}

static void test_list_operations(void** state){
  (void)state;
  List* list;
  list_swap(list, item1, item2);
  list_swap_at(list, 2, 3);
  list_swap_values(list, item1, item2);
  list_swap_values_at(list, 2, 3);
  list2 = list_copy(list);
  list_replace(list, item1, item2);
  list_replace_at(list, 2, "Test");
  list_free(list);
}

static void test_list_comparisons(void** state){
  (void)state;
  assert_true(list_is_different(list1, list2));
  assert_false(list_is_equal(list1, list2));
  list_free(list1);
  list_free(list2);
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
