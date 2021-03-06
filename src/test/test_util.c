#include <oto.h>

void test_vector_i64() {
    size_t cap = 3;
    VectorI64 *vec = new_vector_i64(cap);

    TEST_EQ_NOT_PRINT(vec->length, 0);
    TEST_EQ_NOT_PRINT(vec->capacity, cap);

    for (uint64_t i = 0; i < 5; i++) {
        vector_i64_append(vec, i);
    }

    TEST_EQ_NOT_PRINT(vec->length, 5);
    TEST_EQ_NOT_PRINT(vec->capacity, cap + 100);

    TEST_EQ_NOT_PRINT(vec->data[2], 2);
    TEST_EQ_NOT_PRINT(vec->data[4], 4);

    vector_i64_set(vec, 2, 10);
    TEST_EQ_NOT_PRINT(vec->data[2], 10);

    free_vector_i64(vec);
}

void test_fileio() {
    TEST_EQ_NOT_PRINT(is_otofile("aaa.oto"), true);
    TEST_EQ_NOT_PRINT(is_otofile("./abc/aaa.oto"), true);
    TEST_EQ_NOT_PRINT(is_otofile("aaa.ot"), false);
    TEST_EQ_NOT_PRINT(is_otofile(".oto"), true);
}

void test_string() {
    TEST_EQ_NOT_PRINT(to_lower('A'), 'a');
    TEST_EQ_NOT_PRINT(to_lower('Z'), 'z');
    TEST_EQ_NOT_PRINT(to_lower('m'), 'm');
    TEST_EQ_NOT_PRINT(to_upper('a'), 'A');
    TEST_EQ_NOT_PRINT(to_upper('z'), 'Z');
    TEST_EQ_NOT_PRINT(to_upper('B'), 'B');

    TEST_EQ_NOT_PRINT(strncmp_cs("ABC", "ABC", 3), 0);
    TEST_NE_NOT_PRINT(strncmp_cs("AB", "ABC", 3),  0);
    TEST_NE_NOT_PRINT(strncmp_cs("ABC", "AB", 3),  0);
    TEST_EQ_NOT_PRINT(strncmp_cs("ABC", "abc", 3), 0);
    TEST_NE_NOT_PRINT(strncmp_cs("abc", "aBi", 3), 0);
    TEST_EQ_NOT_PRINT(strncmp_cs("abc", "aBi", 2), 0);

    TEST_EQ_NOT_PRINT(strcmp_cs("ABC", "ABC"),  0);
    TEST_EQ_NOT_PRINT(strcmp_cs("abc", "ABC"),  0);
    TEST_NE_NOT_PRINT(strcmp_cs("ABC", "AB"),   0);
    TEST_NE_NOT_PRINT(strcmp_cs("AB", "ABC"),   0);
    TEST_NE_NOT_PRINT(strcmp_cs("ABC", "ABCD"), 0);
    TEST_NE_NOT_PRINT(strcmp_cs("BCD", "ABD"),  0);
}

void test_map() {
    Map *map = new_map();

    map_puti(map, "aaa.oto", 10);
    map_puti(map, "bbb.oto", 20);
    map_puti(map, "ccc.oto", 30);

    TEST_EQ_NOT_PRINT(map_geti(map, "aaa.oto"), 10);
    TEST_EQ_NOT_PRINT(map_geti(map, "bbb.oto"), 20);
    TEST_EQ_NOT_PRINT(map_geti(map, "ccc.oto"), 30);

    map_inc_val(map, "bbb.oto");
    map_inc_val(map, "bbb.oto");
    TEST_EQ_NOT_PRINT(map_geti(map, "bbb.oto"), 22);

    map_dec_val(map, "aaa.oto");
    map_dec_val(map, "ccc.oto");
    TEST_EQ_NOT_PRINT(map_geti(map, "aaa.oto"), 9);
    TEST_EQ_NOT_PRINT(map_geti(map, "ccc.oto"), 29);
    
    // map_printi(map);

    free_map(map);
}

void test_slice() {
    VectorI64 *veci64 = new_vector_i64(10);

    for (int64_t i = 0; i < 10; i++) {
        vector_i64_append(veci64, i);
        TEST_EQ_NOT_PRINT(veci64->data[i], i);
    }

    SliceI64 *slice = new_slice_i64(veci64, 2, 8);
    TEST_EQ_NOT_PRINT(slice->length, 6);

    for (int64_t i = 0; i < slice->length; i++) {
        TEST_EQ_NOT_PRINT(slice_i64_get(slice, i), veci64->data[i + 2]);
        // printf("slice[%d] : %d\n", i, slice_i64_get(slice, i));
    }

    TEST_EQ_NOT_PRINT(slice_i64_get(slice, -1), 0);
    TEST_EQ_NOT_PRINT(slice_i64_get(slice, 10), 0);

    SliceI64 *slice2 = new_slice_i64_from_slice(slice, 1, 4);
    TEST_EQ_NOT_PRINT(slice2->length, 3);

    for (int64_t i = 0; i < slice2->length; i++) {
        TEST_EQ_NOT_PRINT(slice_i64_get(slice2, i), slice_i64_get(slice, i + 1));
        // printf("slice2[%d] : %d\n", i, slice_i64_get(slice2, i));
    }

    free_slice_i64(slice);
    free_slice_i64(slice2);
}

void test_stack() {
    Stack *stack = new_stack();

    stack_pushi(stack, 1);
    stack_pushi(stack, 2);
    stack_pushi(stack, 3);
    stack_pushi(stack, 4);
    stack_pushi(stack, 5);
    stack_pushi(stack, 6);

    int64_t val = 0;
    
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 6);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 5);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 4);
    TEST_EQ_NOT_PRINT(stack_peeki(stack), 3);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 3);

    stack_pushi(stack, 10);
    stack_pushi(stack, 15);

    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 15);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 10);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 2);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 1);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 0);
    val = stack_popi(stack);
    TEST_EQ_NOT_PRINT(val, 0);
}

int main(void) {
    test_vector_i64();
    test_fileio();
    test_map();
    test_string();
    test_slice();
    test_stack();
}
