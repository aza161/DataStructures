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


dynamic_array* initialize_dynamic_array_empty(const size_t data_size);
dynamic_array* initialize_dynamic_array_sized(const size_t capacity, const size_t data_size);
dynamic_array* initialize_dynamic_array_from(const dynamic_array* list, const size_t data_size);
int add(dynamic_array* list, const void* data, const size_t data_size);
int insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size);
int add_all(dynamic_array* list, dynamic_array* other_list);
void clear(dynamic_array* list);
dynamic_array* clone(const dynamic_array* list);
int contains(const dynamic_array* list, const void* data, const size_t data_size);
void ensure_capacity(dynamic_array* list, size_t capacity);
void* get(dynamic_array* list, const size_t index);
long long int index_of(const dynamic_array* list, const void* data, const size_t data_size);
int is_empty(const dynamic_array* list);
int last_index_of(dynamic_array* list, const void* data, const size_t data_size);
void* remove_at(dynamic_array* list, const size_t index);
int remove_element(dynamic_array* list, const void* data, const size_t data_size);
int remove_all(dynamic_array* list, dynamic_array* other_list);
void remove_range(dynamic_array* list, const size_t start, const size_t end);
size_t size(dynamic_array* list);
void sort(dynamic_array* list, int (*compar)(const void*, const void*));
dynamic_array* get_sub_list(dynamic_array* list, const size_t start, const size_t end);
void trim_to_size(dynamic_array* list);

#endif //_DATASTRUCTURES_DYNAMIC_ARRAY_H
