#ifndef collection_vec
#define collection_vec
#include "collection.h"

typedef struct
{
    void* values;
    usize max_capacity;
    usize length;
} vec;

#define vec_index_valid_or_die(v, idx) check(vec_index_valid(v, idx));

#define vec_length(v) (v->length)
#define vec_capacity(v) (v->max_capacity)
#define vec_values_array(v) (v->values)

// crash if outside of range
#define vec_get(v, type, idx) (((type*)v->values)[(vec_index_valid(v, (usize)idx)) ? ((usize)idx): ((usize)-1)])
#define vec_set(v, type, idx, value) vec_get(v, type, idx) = value

#define vec_peek(v, type) (vec_get(v, type, v->length-1))
// pas sur que ça marche
//#define vec_pop(v, type)  (vec_get(v, type, --(v->length)))

#define vec_empty(type) (_vec_empty())
#define vec_with_capacity(type, capacity) (_vec_new_with_capacity(sizeof(type), capacity))
#define vec_with_length(type, capacity) (_vec_with_length(sizeof(type), capacity))

#define vec_push(v, type, element) vec_add(v, type, element)
#define vec_add(v, type, element)\
{\
    vec* __v = (v);\
    vec_resize_twice_if_needed(__v, sizeof(type));\
    ((type*)__v->values)[__v->length++] = (type)(element);\
}

#define vec_remove_end(v, type)\
{\
    vec* _v = (v);\
    check(_v->length > 0);\
    _v->length--;\
    vec_shrink_if_needed(_v, sizeof(type));\
}

//todo check index

#define vec_remove_at(v, type, idx)\
{\
    vec* _v = (v);\
    int _idx = (idx);\
    vec_index_valid_or_die(_v, _idx);\
    if(_idx+1 != _v->length)\
    {\
        vec_copy(_v, _v, sizeof(type), (_idx+1)*_v->sizeof_value, (_v->length - _idx-1)*_v->sizeof_value, _idx*_v->sizeof_value);\
    }\
    _v->length--;\
    vec_shrink_if_needed(_v, sizeof(type));\
}

#define equal_fn(x,y) ((x) == (y))

#define vec_contains(v, type, value, bool_ptr_result) vec_contains_custom(v,type,value,equal_fn,bool_ptr_result)
#define vec_contains_custom(v, type, equality_fn_ptr, value, bool_ptr_result)\
{\
    usize _idx = usize_error;\
    vec_index_of(v,type,value,equality_fn_ptr);\
    *bool_ptr_result = (_idx == usize_error);\
}

#define vec_index_of_custom(v, type, equality_fn_ptr, value, usize_result_ptr)\
{\
    vec* _v = (v);\
    type _value = (value);\
    usize _idx = usize_error;\
\
    repeat(_i, _v->length)\
    {\
        if(equality_fn_ptr(vec_get(_v, type, _i), _value))\
        {\
            _idx = _i;\
            break;\
        }\
    }\
    *usize_result_ptr = _idx;\
}

#define vec_index_of(v, type, value, usize_result_ptr)\
    vec_index_of_custom(v, type, value, equal_fn, usize_result_ptr)


// vec_index_valid_or_die(_v, _idx);\
// range not checked
#define vec_insert(v, type, idx, value)\
{\
    vec* _v = (v);\
    int _idx = (idx);\
    type _value = (value);\
    vec_resize_twice_if_needed(_v, sizeof(type));\
    vec_copy(_v, _v, sizeof(type), _idx*_v->sizeof_value, (_v->length - _idx)*_v->sizeof_value, (_idx + 1)*_v->sizeof_value);\
    ((type*)_v->values)[_idx] = _value;\
    _v->length++;\
}

#define vec_plus(a, b, type, result_ptr)\
{\
    vec* _a = (a);\
    vec* _b = (b);\
    check_msg("vec_add() : Can't add a and b ", vec_length(_a) == vec_length(_b));\
    *result_ptr = _vec_new_with_capacity(sizeof(value), _a->length);\
    repeat(_i, _a->length)\
    {\
        vec_set(*result_ptr, type, _i, vec_get(_a, type, _i) + vec_get(_b, type, _i));\
    }\
}

#define vec_fprintf_metadata(f,v,type)\
{\
    vec* _v = (v);\
    if(_v != null)\
    {\
        fprintf(f, #v" : vec<%s> of %i elements of %i bytes. (max capacity : %i) : ", to_string(type), _v->length, sizeof(type), _v->max_capacity);\
    }else\
    {\
        fprintf(f, #v" : vec null");\
    }\
}

#define vec_printf_metadata(f,v,type) vec_fprintf_metadata(stdout, v, type)

#define vec_fprintf(f, v, type, format)\
{\
    vec* _v = (v);\
    vec_fprintf_metadata(f, v, type);\
    fprintf(f, "\n");\
    if(_v != null)\
    {\
        repeat(_i, _v->length)\
        {\
            fprintf(f, format, (type)vec_get(v, type, _i));\
        }\
    }\
    fprintf(f, "\n");\
}



#define vec_fprintf_struct(f, v, type, fn_ptr_printing)\
{\
    vec* _v = (v);\
    vec_fprintf_metadata(f, v, type);\
    if(_v != null)\
    {\
        repeat(_i, _v->length)\
        {\
            fn_ptr_printing(f, (type)vec_get(v, type, _i));\
        }\
    }\
    fprintf(f, "\n");\
}

#define vec_printf(v, type, format)\
    vec_fprintf(stdout, v, type, format)

vec* _vec_empty();
vec* _vec_new_with_capacity(usize sizeof_value, usize capacity);
vec* _vec_new_with_length(usize sizeof_value, usize capacity);

void vec_free_lazy(vec* v);
void vec_set_capacity(vec* v, usize sizeof_value, usize capacity);
void vec_set_capacity_min(vec* v, usize sizeof_value, usize capacity);
void vec_resize_twice_if_needed(vec* v, usize sizeof_value);
void vec_shrink_if_needed(vec* v, usize sizeof_value);
vec* vec_clone(vec* v, usize sizeof_value);
bool vec_index_valid(vec* v, usize idx);
bool vec_equal(vec* a, vec* b, usize sizeof_value);

void vec_copy(vec* source, vec* destination, usize sizeof_value, usize sourceByteIdx, usize sourceByteLength, usize destinationByteIdx);
void vec_clear(vec* v);

#define vec_printf_int(v)   vec_printf(v, int, "%i, ");
#define vec_printf_float(v) vec_printf(v, float, "%f, ");
#define vec_printf_ptr(v)   vec_printf(v, void*, "%p, ");

#endif