/**************************************************************************
 *   dynamic_array.h  --  This file is part of Data Structures Library.   *
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

#ifndef _DATASTRUCTURES_DYNAMIC_ARRAY_H
#define _DATASTRUCTURES_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* The Default capacity for the list */
#define DEFAULT_CAPACITY 2

/* Structure type. */
typedef struct DYNAMIC_ARRAY dynamic_array;

/**
 * Initializes an empty dynamic array of capacity @DEFAULT_CAPACITY.
 *
 * @param data_size The size of the data type to be stored in the list (in bytes).
 * @returns a pointer to the dynamic array initialized. */
dynamic_array* dynarr_initialize_empty(const size_t data_size);

/**
 * Initializes a dynamic array with a specified capacity.
 * @param capacity The initial capacity of the list.
 * @param data_size The size of the data type to be stored in the list (in bytes).
 * @returns a pointer to the dynamic array initialized. */
dynamic_array* dynarr_initialize_sized(const size_t capacity, const size_t data_size);

/**
 * Initializes a dynamic array from another list.
 * @param list The list from which to initialize.
 * @param data_size The size of the data type to be stored in the list (in bytes).
 * @returns a pointer to the dynamic array initialized. */
dynamic_array* dynarr_initialize_from(const dynamic_array* list, const size_t data_size);

/**
 * Destroys a dynamic array and frees the allocated memory.
 * @param list The dynamic array to be destroyed. */
void dynarr_destroy(dynamic_array* list);


bool dynarr_get(const dynamic_array* list, const size_t index, void* out_data, const size_t data_size);
dynamic_array* dynarr_get_sub_list(const dynamic_array* list, const size_t start, const size_t end);


bool dynarr_remove_at(dynamic_array* list, const size_t index, void* out_data, const size_t data_size);
bool dynarr_remove_element(dynamic_array* list, const void* data, const size_t data_size);
bool dynarr_remove_all(dynamic_array* list, const dynamic_array* other_list);
void dynarr_remove_range(dynamic_array* list, const size_t start, const size_t end);


bool dynarr_add(dynamic_array* list, const void* data, const size_t data_size);
bool dynarr_add_all(dynamic_array* list, dynamic_array* other_list);
bool dynarr_insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size);


bool dynarr_set(dynamic_array* list, const size_t index, const void* data, const size_t data_size);


bool dynarr_contains(const dynamic_array* list, const void* data, const size_t data_size);
bool dynarr_index_of(const dynamic_array* list, const void* data, const size_t data_size, size_t* index);
bool dynarr_last_index_of(const dynamic_array* list, const void* data, const size_t data_size, size_t* index);


void dynarr_clear(dynamic_array* list);
size_t dynarr_size(const dynamic_array* list);
int dynarr_is_empty(const dynamic_array* list);


bool dynarr_ensure_capacity(dynamic_array* list, size_t capacity);
void dynarr_trim_to_size(dynamic_array* list);


void dynarr_sort(dynamic_array* list, int (*compar)(const void*, const void*));

#endif //_DATASTRUCTURES_DYNAMIC_ARRAY_H
