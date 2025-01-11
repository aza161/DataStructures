//
// Created by ahmad on 1/9/25.
//

#ifndef _DATASTRUCTURES_DYNAMIC_ARRAY_H
#define _DATASTRUCTURES_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DYNAMIC_ARRAY dynamic_array;


dynamic_array* dynarr_initialize_empty(const size_t data_size);
dynamic_array* dynarr_initialize_sized(const size_t capacity, const size_t data_size);
dynamic_array* dynarr_initialize_from(const dynamic_array* list, const size_t data_size);
void dynarr_destroy(dynamic_array* list);
int dynarr_add(dynamic_array* list, const void* data, const size_t data_size);
int dynarr_insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size);
int dynarr_add_all(dynamic_array* list, dynamic_array* other_list);
void dynarr_clear(dynamic_array* list);
dynamic_array* dynarr_clone(const dynamic_array* list);
int dynarr_contains(const dynamic_array* list, const void* data, const size_t data_size);
void dynarr_ensure_capacity(dynamic_array* list, size_t capacity);
void* dynarr_get(dynamic_array* list, const size_t index);
long long int dynarr_index_of(const dynamic_array* list, const void* data, const size_t data_size);
int dynarr_is_empty(const dynamic_array* list);
long long int dynarr_last_index_of(dynamic_array* list, const void* data, const size_t data_size);
void* dynarr_remove_at(dynamic_array* list, const size_t index);
int dynarr_remove_element(dynamic_array* list, const void* data, const size_t data_size);
int dynarr_remove_all(dynamic_array* list, dynamic_array* other_list);
void dynarr_remove_range(dynamic_array* list, const size_t start, const size_t end);
size_t dynarr_size(dynamic_array* list);
void dynarr_sort(dynamic_array* list, int (*compar)(const void*, const void*));
dynamic_array* dynarr_get_sub_list(dynamic_array* list, const size_t start, const size_t end);
void dynarr_trim_to_size(dynamic_array* list);

#endif //_DATASTRUCTURES_DYNAMIC_ARRAY_H
