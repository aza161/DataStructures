/**************************************************************************
 *   Dynamic Array.c  --  This file is part of Data Structures Library.   *
 *                                                                        *
 *   Copyright (C) 2025 Ahmad Al Rabia.                                   *
 *                                                                        *
 *   Data Structures Library is free software: you can redistribute it    *
 *   and/or modify it.                                                    *
 *                                                                        *
 *   Data Structures Library is distributed in the hope that it will be   *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty  *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *                                                                        *
 **************************************************************************/

#include "dynamic_array.h"

/* Structure type. */
typedef struct DYNAMIC_ARRAY
{
    /* Points to the data in the list */
    void* data;
    /* The number of elements in the list */
    size_t size;
    /* The size of a single data element in bytes */
    size_t data_size;
    /* The actual capacity of the allocated memory for the list */
    size_t capacity;
} dynamic_array;


static dynamic_array* allocate_dynamic_array(const size_t data_size, const size_t capacity)
{
    if (capacity > SIZE_MAX / data_size)
    {
        return nullptr;
    }

    dynamic_array* list = calloc(1, sizeof(dynamic_array));

    if (!list)
    {
        return nullptr;
    }


    list->data = calloc(capacity, data_size);
    list->size = 0;
    list->data_size = data_size;
    list->capacity = capacity;

    if (!list->data)
    {
        free(list);
        return nullptr;
    }

    return list;
}

dynamic_array* dynarr_initialize_empty(const size_t data_size)
{
    return allocate_dynamic_array(data_size, DEFAULT_CAPACITY);
}

dynamic_array* dynarr_initialize_sized(const size_t capacity, const size_t data_size)
{
    return allocate_dynamic_array(data_size, capacity);
}

dynamic_array* dynarr_initialize_from(const dynamic_array* list, const size_t data_size)
{
    if ((!list) || (list->data_size != data_size))
    {
        return nullptr;
    }

    dynamic_array* new_list = allocate_dynamic_array(data_size, list->capacity);

    if (!new_list)
    {
        return nullptr;
    }

    new_list->size = list->size;

    memcpy(new_list->data, list->data, new_list->size * new_list->data_size);

    return new_list;
}

void dynarr_destroy(dynamic_array* list)
{
    if (!list)
    {
        return;
    }

    free(list->data);
    free(list);
}

static bool dynarr_expand(dynamic_array* list, const size_t new_capacity)
{
    if (new_capacity < list->capacity)
    {
        return false;
    }

    void* data_ptr = realloc(list->data, new_capacity * list->data_size);

    if (!data_ptr)
    {
        return false;
    }

    list->data = data_ptr;

    list->capacity = new_capacity;
    return true;
}

static bool dynarr_double_capacity(dynamic_array* list)
{
    /* First condition checks if capacity overflows size_t, second checks if the number of elements overflows size_t */
    if ((list->capacity > SIZE_MAX / 2) || (list->capacity << 1 > SIZE_MAX / list->data_size))
    {
        return false;
    }

    const size_t new_capacity = list->capacity << 1;

    if (!dynarr_expand(list, new_capacity))
    {
        return false;
    }

    return true;
}

static bool dynarr_grow_capacity(dynamic_array* list, const size_t extra_capacity)
{
    /**
     * First condition checks if capacity reached the limit of size_t
     * second checks if extra_capacity would underflow the maximum number of elements (SIZE_MAX / list->data_size)
     * third checks if the number of elements overflows size_t
     */
    if ((list->capacity == SIZE_MAX) ||
        (extra_capacity > ((SIZE_MAX / list->data_size) - list->capacity)) ||
        (list->capacity > ((SIZE_MAX / list->data_size) - extra_capacity)))
    {
        return false;
    }

    const size_t new_capacity = list->capacity + extra_capacity;

    if (!dynarr_expand(list, new_capacity))
    {
        return false;
    }

    return true;
}

bool dynarr_add(dynamic_array* list, const void* data, const size_t data_size)
{
    if ((!list || !data) || (list->data_size != data_size) || (list->capacity == SIZE_MAX / list->data_size))
    {
        return false;
    }

    /*
     * list->size should never be > list->capacity in practice
     * we put it here just in case our logic fails somewhere else
     */
    if (list->size >= list->capacity)
    {
        if (!dynarr_double_capacity(list))
        {
            const size_t extra_capacity = (SIZE_MAX / list->data_size) - list->capacity;
            if (!dynarr_grow_capacity(list, extra_capacity))
            {
                return false;
            }
        }
    }

    void* dest = (unsigned char*)(list->data) + (list->size * data_size);
    memcpy(dest, data, data_size);
    list->size++;
    return true;
}

static bool shift_elements_right(const dynamic_array* list, const size_t index)
{
    if (index >= list->size)
    {
        return false;
    }

    const size_t data_size = list->data_size;

    void* dest = (unsigned char*)(list->data) + ((index + 1) * data_size);
    const void* src = (unsigned char*)(list->data) + (index * data_size);
    const size_t numbytes = (list->size - index) * data_size;
    memmove(dest, src, numbytes);
    return true;
}

bool dynarr_set(dynamic_array* list, const size_t index, const void* data, const size_t data_size)
{
    if ((!list || !data) || (index >= list->size) || (list->data_size != data_size))
    {
        return false;
    }

    void* dest = (unsigned char*)(list->data) + (index * data_size);
    memcpy(dest, data, data_size);
    return true;
}

bool dynarr_insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size)
{
    if ((!list || !data) || (index >= list->size) || (list->data_size != data_size))
    {
        return false;
    }

    if (!shift_elements_right(list, index))
    {
        return false;
    }

    dynarr_set(list, index, data, data_size);

    list->size++;

    return true;
}

bool dynarr_add_all(dynamic_array* list, dynamic_array* other_list)
{
    // Last condition checks if adding elements from other_list would exceed the maximum capacity limit.
    if ((!list || !other_list) ||
        (list == other_list) ||
        (other_list->size == 0) ||
        (list->data_size != other_list->data_size) ||
        (list->size > ((SIZE_MAX / list->data_size) - other_list->size)))
    {
        return false;
    }

    if (list->capacity < (other_list->size + list->size))
    {
        const size_t extra_capacity = (other_list->size + list->size) - list->capacity;
        if (!dynarr_grow_capacity(list, extra_capacity))
        {
            return false;
        }
    }

    const void* src = (unsigned char*)(other_list->data);
    void* dest = (unsigned char*)(list->data) + (list->size * list->data_size);
    memcpy(dest, src, other_list->data_size * other_list->size);
    list->size += other_list->size;
    return true;
}

// Note: If the list stores pointers or other lists they should be freed individually before calling dynarr_clear()
// As that would cause a memory leak.
void dynarr_clear(dynamic_array* list)
{
    if (!list)
    {
        return;
    }

    list->size = 0;
}

dynamic_array* dynarr_clone(const dynamic_array* list)
{
    return dynarr_initialize_from(list, list->data_size);
}

bool dynarr_contains(const dynamic_array* list, const void* data, const size_t data_size)
{
    if ((!list || !data) || (list->data_size != data_size))
    {
        return false;
    }

    for (size_t i = 0; i < list->size; ++i)
    {
        const void* src = (unsigned char*)(list->data) + (i * data_size);
        if (!memcmp(data, src, data_size))
        {
            return true;
        }
    }
    return false;
}

bool dynarr_ensure_capacity(dynamic_array* list, const size_t capacity)
{
    /* Third condition checks if the new number of capacity bytes overflows size_t */
    if ((!list) || (list->size > capacity) || (capacity > SIZE_MAX / list->data_size))
    {
        return false;
    }

    if (list->capacity != capacity)
    {
        void* data_ptr = realloc(list->data, list->data_size * capacity);

        if (!data_ptr)
        {
            return false;
        }

        list->data = data_ptr;
        list->capacity = capacity;
    }
    return true;
}

void* dynarr_get(dynamic_array* list, const size_t index)
{
    if ((!list) || (index >= list->size))
    {
        return nullptr;
    }

    return (unsigned char*)(list->data) + (index * list->data_size);
}

bool dynarr_index_of(const dynamic_array* list, const void* data, const size_t data_size, size_t* index)
{
    if ((!list || !data || !index) || (list->data_size != data_size))
    {
        return false;
    }

    for (size_t i = 0; i < list->size; ++i)
    {
        const void* src = (unsigned char*)(list->data) + (i * data_size);
        if (!memcmp(data, src, data_size))
        {
            *index = i;
            return true;
        }
    }

    return false;
}

int dynarr_is_empty(const dynamic_array* list)
{
    return list->size == 0;
}

bool dynarr_last_index_of(dynamic_array* list, const void* data, const size_t data_size, size_t* index)
{
    if ((!list || !data || !index) || (list->data_size != data_size))
    {
        return false;
    }

    for (size_t i = list->size; i > 0; --i)
    {
        const void* src = (unsigned char*)(list->data) + ((i - 1) * data_size);
        if (!memcmp(data, src, data_size))
        {
            *index = i - 1;
            return true;
        }
    }

    return false;
}

void* dynarr_remove_at(dynamic_array* list, const size_t index)
{
    if ((!list) || (index >= list->size))
    {
        return nullptr;
    }

    void* res = malloc(list->data_size);

    if (!res)
    {
        return nullptr;
    }

    const void* element_src = (unsigned char*)(list->data) + (index * list->data_size);
    memcpy(res, element_src, list->data_size);

    if (index < list->size - 1)
    {
        const void* src = (unsigned char*)(list->data) + ((index + 1) * list->data_size);
        void* dest = (unsigned char*)(list->data) + (index * list->data_size);
        const size_t buffer_size = (list->size - index - 1) * list->data_size;
        memmove(dest, src, buffer_size);
    }

    list->size--;

    if (list->size <= list->capacity / 4)
    {
        dynarr_ensure_capacity(list, list->capacity / 2);
    }

    return res;
}

bool dynarr_remove_element(dynamic_array* list, const void* data, const size_t data_size)
{
    if ((!list || !data) || (list->data_size != data_size))
    {
        return false;
    }

    size_t i;

    if (!dynarr_index_of(list, data, data_size, &i))
    {
        return false;
    }

    void* removed = dynarr_remove_at(list, i);

    if (!removed)
    {
        return false;
    }

    const bool res = memcmp(removed, data, data_size) == 0;

    free(removed);

    return res;
}

bool dynarr_remove_all(dynamic_array* list, dynamic_array* other_list)
{
    if ((!list || !other_list) || (list->data_size != other_list->data_size))
    {
        return false;
    }

    for (size_t i = 0; i < other_list->size; ++i)
    {
        const void* to_be_removed = (unsigned char*)(other_list->data) + (i * other_list->data_size);
        dynarr_remove_element(list, to_be_removed, other_list->data_size);
    }

    return true;
}


// THE USER MUST FREE ANY POINTERS BEFORE USING THIS FUNCTION!!!!
// The element list->data[end] is excluded and not removed.
void dynarr_remove_range(dynamic_array* list, const size_t start, const size_t end)
{
    if ((!list) || (start >= list->size) || (end > list->size) || (start >= end))
    {
        return;
    }

    const void* src = (unsigned char*)(list->data) + (end * list->data_size);
    void* dest = (unsigned char*)(list->data) + (start * list->data_size);
    const size_t numbytes = (end - start) * list->data_size;

    memmove(dest, src, numbytes);

    list->size -= (end - start);

    if (list->size <= list->capacity / 4)
    {
        dynarr_ensure_capacity(list, list->capacity / 2);
    }
}

size_t dynarr_size(dynamic_array* list)
{
    if (!list)
    {
        return 0;
    }

    return list->size;
}

void dynarr_sort(dynamic_array* list, int (*compar)(const void*, const void*))
{
    if ((!list || !compar) || (list->size == 0))
    {
        return;
    }

    qsort(list->data, list->size, list->data_size, compar);
}

dynamic_array* dynarr_get_sub_list(dynamic_array* list, const size_t start, const size_t end)
{
    if ((!list) || (start >= list->size) || (end > list->size) || (start >= end))
    {
        return nullptr;
    }

    const size_t sub_list_size = end - start;


    dynamic_array* res = dynarr_initialize_sized(sub_list_size, list->data_size);

    if (!res)
    {
        return nullptr;
    }

    const void* src = (unsigned char*)(list->data) + (start * list->data_size);
    memcpy(res->data, src, sub_list_size * res->data_size);

    res->size = sub_list_size;

    return res;
}

void dynarr_trim_to_size(dynamic_array* list)
{
    if (!list)
    {
        return;
    }

    dynarr_ensure_capacity(list, list->size);
}
