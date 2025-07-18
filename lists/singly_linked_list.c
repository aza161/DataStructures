/**************************************************************************
 *   singly_linked_list.c  --                                             *
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

#include "singly_linked_list.h"


/* Singly linked node */
typedef struct s_node s_node;

typedef struct s_node
{
    /* The data stored in the node */
    void* data;
    /* The pointer to the next node */
    s_node* next;
} s_node;

/* Singly linked list */
typedef struct singly_linked_list
{
    /* The first element of the list */
    s_node* head;
    /* The last element of the list */
    s_node* tail;
    /* The number of elements in the list */
    size_t size;
    /* The size of a single data element in bytes */
    size_t data_size;
} singly_linked_list;

/* Local functions */
static s_node* s_node_initialize(const void* data, const size_t data_size);
static bool s_node_destroy(s_node* node);
static bool s_node_has_next(const s_node* node);
static s_node* s_node_get_kth(s_node* node, const size_t k);


static s_node* s_node_initialize(const void* data, const size_t data_size)
{
    s_node* node = calloc(1, sizeof(s_node));

    if (!node)
    {
        return nullptr;
    }

    memcpy(node->data, data, data_size);

    return node;
}

static bool s_node_destroy(s_node* node)
{
    if (!node)
    {
        return false;
    }

    free(node->data);
    free(node);
    return true;
}

static bool s_node_has_next(const s_node* node)
{
    if (!node)
    {
        return false;
    }

    return node->next != nullptr;
}

static s_node* s_node_get_kth(s_node* node, const size_t k)
{
    if (!node)
    {
        return nullptr;
    }

    size_t i = 0;
    while (i != k)
    {
        node = node->next;
        if (!node)
        {
            return nullptr;
        }
        i++;
    }
    return node;
}

singly_linked_list* slist_initialize(const size_t data_size)
{
    singly_linked_list* list = calloc(1, sizeof(singly_linked_list));
    if (!list)
    {
        return nullptr;
    }
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    list->data_size = data_size;
    return list;
}

singly_linked_list* slist_initialize_from(const singly_linked_list* list, const size_t data_size)
{
    if ((!list) || (data_size != list->data_size) || (!list->head))
    {
        return nullptr;
    }

    singly_linked_list* new_list = slist_initialize(data_size);

    if (!new_list)
    {
        return nullptr;
    }

    new_list->head = s_node_initialize(nullptr, data_size);

    if (!new_list->head)
    {
        free(new_list);
        return nullptr;
    }

    s_node* new_list_current = new_list->head;
    const s_node* list_current = list->head;

    memcpy(new_list_current->data, list_current->data, data_size);
    new_list->tail = new_list_current;

    while (s_node_has_next(list_current))
    {
        new_list_current->next = s_node_initialize(list_current->next->data, data_size);
        if (!new_list_current->next)
        {
            slist_destroy(new_list);
            return nullptr;
        }

        new_list_current = new_list_current->next;

        list_current = list_current->next;
    }

    new_list->tail = new_list_current;
    new_list->size = list->size;

    return new_list;
}

bool slist_destroy(singly_linked_list* list)
{
    if (!list)
    {
        return false;
    }

    s_node* current = list->head;
    while (current)
    {
        s_node* next = current->next;
        s_node_destroy(current);
        current = next;
    }

    free(list);
    return true;
}


void* slist_get_first(const singly_linked_list* list)
{
    if ((!list) || (!list->size) || (!list->head))
    {
        return nullptr;
    }

    return list->head->data;
}

void* slist_get_last(const singly_linked_list* list)
{
    if ((!list) || (!list->size) || (!list->tail))
    {
        return nullptr;
    }

    return list->tail->data;
}

void* slist_get_at(const singly_linked_list* list, const size_t index)
{
    if ((!list) || (index >= list->size) || (!list->head))
    {
        return nullptr;
    }

    size_t i = 0;
    const s_node* current = list->head;

    while (i != index)
    {
        current = current->next;
        i++;
    }

    return current->data;
}

singly_linked_list* slist_get_sub_list(const singly_linked_list* list, const size_t start, const size_t end)
{
    if ((!list) || (start >= list->size) || (end > list->size) || (start >= end) || (!list->head))
    {
        return nullptr;
    }

    const size_t data_size = list->data_size;

    singly_linked_list* sub_list = slist_initialize(data_size);

    if (!sub_list)
    {
        return nullptr;
    }

    sub_list->head = s_node_initialize(list->head->data, data_size);

    if (!sub_list->head)
    {
        slist_destroy(sub_list);
        return nullptr;
    }

    sub_list->tail = sub_list->head;
    sub_list->size++;

    s_node* sub_list_current = sub_list->head;
    const s_node* list_current = s_node_get_kth(list->head, start);

    if (!list_current)
    {
        slist_destroy(sub_list);
        return nullptr;
    }

    const size_t total_elements = end - start;

    while (sub_list->size < total_elements)
    {
        sub_list_current->next = s_node_initialize(list_current->next->data, data_size);

        if (!sub_list_current->next)
        {
            slist_destroy(sub_list);
            return nullptr;
        }

        sub_list_current = sub_list_current->next;

        list_current = list_current->next;

        sub_list->size++;
    }

    sub_list->tail = sub_list_current;

    return sub_list;
}