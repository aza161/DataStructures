/**************************************************************************
 *   singly_linked_list.h  --                                             *
 *   This file is part of Data Structures Library.                        *
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

#ifndef _DATASTRUCTURES_SINGLY_LINKED_LIST_H
#define _DATASTRUCTURES_SINGLY_LINKED_LIST_H


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct singly_linked_list singly_linked_list;


singly_linked_list* slist_initialize(const size_t data_size);
singly_linked_list* slist_initialize_from(const singly_linked_list* list, const size_t data_size);
bool slist_destroy(singly_linked_list* list);


void* slist_get_first(const singly_linked_list* list);
void* slist_get_last(const singly_linked_list* list);
void* slist_get_at(const singly_linked_list* list, const size_t index);
singly_linked_list* slist_get_sub_list(const singly_linked_list* list, const size_t start, const size_t end);


void* slist_remove_first(singly_linked_list* list);
void* slist_remove_last(singly_linked_list* list);
bool slist_remove_at(singly_linked_list* list, const size_t index);
bool slist_remove_element(singly_linked_list* list, const void* data, const size_t data_size);
bool slist_remove_all(singly_linked_list* list, const singly_linked_list* other_list);
void slist_remove_range(singly_linked_list* list, const size_t start, const size_t end);


bool slist_add_first(singly_linked_list* list, const void* data, const size_t data_size);
bool slist_add_last(singly_linked_list* list, const void* data, const size_t data_size);
bool slist_add_all(singly_linked_list* list, const singly_linked_list* other_list);
bool slist_add_all_at(singly_linked_list* list, const size_t index, const singly_linked_list* other_list);
bool slist_insert(singly_linked_list* list, const size_t index, const void* data, const size_t data_size);


void* slist_set(singly_linked_list* list, const size_t index, const void* data, const size_t data_size);


bool slist_contains(const singly_linked_list* list, const void* data, const size_t data_size);
bool slist_index_of(const singly_linked_list* list, const void* data, const size_t data_size, size_t* index);
bool slist_last_index_of(const singly_linked_list* list, const void* data, const size_t data_size, size_t* index);


void slist_clear(singly_linked_list* list);
size_t slist_size(const singly_linked_list* list);
bool slist_is_empty(const singly_linked_list* list);


void slist_reverse(singly_linked_list* list);
void slist_sort(singly_linked_list* list, int (compar)(const void*, const void*));


#endif //_DATASTRUCTURES_SINGLY_LINKED_LIST_H
