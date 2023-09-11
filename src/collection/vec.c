#include "collection.h"

vec* _vec_empty()
{
    vec* v = make(vec);
    v->length = 0;
    v->max_capacity = 0;
    v->values = null;
    //v->sizeof_value = sizeof_value;
    return v;
}

vec* _vec_new_with_capacity(usize sizeof_value, usize capacity)
{
    vec* v = make(vec);
    v->length = 0;
    v->max_capacity = capacity;
    v->values = malloc(sizeof_value*capacity);
    return v;
}

vec* _vec_new_with_length(usize sizeof_value, usize capacity)
{
    vec* v = make(vec);
    v->length = capacity;
    v->max_capacity = capacity;
    v->values = malloc(sizeof_value*capacity);
    return v;
}

void vec_free_lazy(vec* v)
{
    if(v == null) return;
    free(v->values);
    v->length = 0;
    v->max_capacity = 0;
    v->values = null;
    free(v);
}

void vec_set_capacity(vec* v, usize sizeof_value, usize capacity)
{
    v->values = (void*)realloc(v->values, capacity * sizeof_value);
    v->max_capacity = capacity;
}

void vec_set_capacity_min(vec* v, usize sizeof_value, usize capacity)
{
    if (capacity > v->max_capacity)
    {
        vec_set_capacity(v, sizeof_value, capacity);
    }
}

void vec_resize_twice_if_needed(vec* v, usize sizeof_value)
{
    if ((usize)(v->length + 1) > v->max_capacity)
    {
        vec_set_capacity(v, sizeof_value, v->max_capacity < 2 ? 2 : v->max_capacity * 2);
    }
}

void vec_shrink_if_needed(vec* v, usize sizeof_value)
{
    if ((usize)(v->length+4) <= v->max_capacity / 2)
    {
        vec_set_capacity(v, sizeof_value, v->max_capacity / 2);
    }
}

vec* vec_clone(vec* v, usize sizeof_value)
{
    if(v == null) { return null; }

    vec* result = make(vec);
    result->length = v->length;
    result->max_capacity = v->max_capacity;

    usize nbByteToCopy = result->max_capacity*sizeof_value;
    result->values = malloc(nbByteToCopy);
    memcpy(result->values, v->values, nbByteToCopy);
    return result;
}

void vec_copy(vec* source, vec* destination, usize sizeof_value, usize sourceByteIdx, usize sourceByteLength, usize destinationByteIdx)
{
    if(destinationByteIdx+sourceByteLength > destination->max_capacity)
    {
        vec_set_capacity_min(destination, sizeof_value, (destinationByteIdx+sourceByteLength)/sizeof_value);
    }

    if((usize)destination + (usize)(destinationByteIdx) > (usize)source + (usize)sourceByteIdx)
    {
        for (usize i = sourceByteLength-1; i < (usize)sourceByteLength; i--)
        {
            ((byte*)destination->values)[destinationByteIdx + i] = ((char*)(source->values))[sourceByteIdx + i];
        }
    }else
    {
        for (usize i = 0; i < (usize)sourceByteLength; i++)
        {
            ((byte*)destination->values)[destinationByteIdx + i] = ((char*)(source->values))[sourceByteIdx + i];
        }
    }
}

bool vec_index_valid(vec* v, usize idx) { return idx < (usize)v->length; }

bool vec_equal(vec* a, vec* b, usize sizeof_value)
{
    if(a->length != b->length) { return false; }

    if(a->values == b->values)
    {
        return true; // même adresse mémoire
    }

    return memcmp(a->values,b->values, a->length*sizeof_value) == 0;
}

void vec_clear(vec* v)
{
    v->length = 0;
    v->max_capacity = 0;
    free(v->values);
    v->values = null;
}