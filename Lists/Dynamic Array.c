#include "Dynamic Array.h"

#define DEFAULT_CAPACITY 10

typedef struct DYNAMIC_ARRAY
{
    void* data;
    size_t size;
    size_t data_size;
    size_t capacity;
} dynamic_array;


static dynamic_array* allocate_dynamic_array(const size_t data_size, const size_t capacity)
{
    dynamic_array* list = malloc(sizeof(dynamic_array));

    if (!list)
    {
        fprintf(stderr, "No enough memory for dynamic array!\n");
        return nullptr;
    }

    list->capacity = capacity;
    list->size = 0;
    list->data_size = data_size;
    list->data = malloc(data_size * capacity);

    if (!list->data)
    {
        fprintf(stderr, "No enough memory for dynamic array data!\n");
        free(list);
        return nullptr;
    }

    return list;
}

dynamic_array* initialize_dynamic_array_empty(const size_t data_size)
{
    return allocate_dynamic_array(data_size, DEFAULT_CAPACITY);
}

dynamic_array* initialize_dynamic_array_sized(const size_t capacity, const size_t data_size)
{
    return allocate_dynamic_array(data_size, capacity);
}

dynamic_array* initialize_dynamic_array_from(const dynamic_array* list, const size_t data_size)
{
    if (!list)
    {
        fprintf(stderr, "The source list is null!\n");
        return nullptr;
    }

    if (list->data_size != data_size)
    {
        fprintf(stderr, "Cannot initialize an array of different data_size\nlist->data_size = %lu != data_size = %lu",
                list->data_size, data_size);
        return nullptr;
    }

    dynamic_array* new_list = allocate_dynamic_array(data_size, list->capacity);

    new_list->size = list->size;

    memcpy(new_list->data, list->data, new_list->size * new_list->data_size);

    return new_list;
}

static int grow_dynamic_list(dynamic_array* list)
{
    list->capacity <<= 1;

    void* dataPtr = realloc(list->data, list->capacity * list->data_size);

    if (!dataPtr)
    {
        fprintf(stderr, "No enough memory for dynamic array data resizing!\nOld dynamic data left as is!");
        return 0;
    }

    list->data = dataPtr;
    return 1;
}

int add(dynamic_array* list, const void* data, const size_t data_size)
{
    if (list->data_size != data_size)
    {
        fprintf(stderr, "Cannot add an element of different data_size\nlist->data_size = %lu != data_size = %lu",
                list->data_size, data_size);
        return 0;
    }

    // A short circuit occurs and no resizing occurs if size < capacity
    if (list->size < list->capacity || grow_dynamic_list(list))
    {
        void* dest = (char*)list->data + (list->size * data_size);
        memcpy(dest, data, data_size);
        list->size++;
        return 1;
    }

    return 0;
}

int insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size)
{
    if (list->data_size != data_size)
    {
        fprintf(stderr, "Cannot add an element of different data_size\nlist->data_size = %lu != data_size = %lu",
                list->data_size, data_size);
        return 0;
    }

    // A short circuit occurs and no resizing occurs if size < capacity
    if (list->size < list->capacity || grow_dynamic_list(list))
    {
        if (index < list->size)
        {
            void* dest = (char*)list->data + ((index + 1) * data_size);
            void* src = (char*)list->data + (index * data_size);
            size_t size = (list->size - index) * data_size;
            memmove(dest, src, size);
        }
        void* dest = (char*)list->data + (index * data_size);
        memcpy(dest, data, data_size);
        list->size++;
        return 1;
    }

    return 0;
}