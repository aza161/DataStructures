//
// Created by ahmad on 1/9/25.
//

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

typedef struct DYNAMIC_ARRAY dynamic_array;


dynamic_array* initialize_dynamic_array_empty(size_t data_size);
dynamic_array* initialize_dynamic_array_sized(int size, size_t data_size);
dynamic_array* initialize_dynamic_array_from(dynamic_array* list, size_t data_size);
int add(dynamic_array* list, void* data);
int insert(dynamic_array* list, int index, void* data);
int add_all(dynamic_array* list, dynamic_array* other_list);
void clear(dynamic_array* list);
dynamic_array* clone(dynamic_array* list);
int contains(dynamic_array* list, void* data);
void ensure_capacity(dynamic_array* list, int capacity);
void* get(dynamic_array* list, int i);
int index_of(dynamic_array* list, void* data);
int is_empty(dynamic_array* list);
int last_index_of(dynamic_array* list, void* data);
void* remove_at(dynamic_array* list, int index);
int remove_element(dynamic_array* list, void* data);
int remove_all(dynamic_array* list, dynamic_array* other_list);
void remove_range(dynamic_array* list, int start, int end);
size_t size(dynamic_array* list);
void sort(dynamic_array* list, int (*compar)(const void*, const void*));
dynamic_array* get_sub_list(dynamic_array* list, int start, int end);
void trim_to_size(dynamic_array* list);

#endif //DYNAMIC_ARRAY_H
