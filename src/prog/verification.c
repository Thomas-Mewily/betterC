#include "prog.h"

void vec_check()
{
    vec* v = vec_empty(int);
    vec_free_lazy(v);
    memory_check_0_alloc();

    v = vec_empty(int);
    vec_push(v, int, 1);
    vec_push(v, int, 2);
    vec_push(v, int, 3);
    check(vec_length(v) == 3);
    check(vec_get(v, int, 0) == 1);
    check(vec_get(v, int, 1) == 2);
    check(vec_get(v, int, 2) == 3);
    check(vec_index_valid(v, -1) == false);
    check(vec_index_valid(v, 3) == false);

    vec_remove_end(v, int);
    vec_remove_end(v, int);
    check(vec_get(v, int, 0) == 1);
    check(vec_length(v) == 1);

    vec_free_lazy(v);
    memory_check_0_alloc();
}

void verification()
{
    vec_check();
}