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
        fprintf(stderr, "Cannot initialize an array of different data_size\nlist->data_size = %lu != data_size = %lu\n",
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

    void* data_ptr = realloc(list->data, list->capacity * list->data_size);

    if (!data_ptr)
    {
        fprintf(stderr, "No enough memory for dynamic array data resizing!\nOld dynamic data left as is!\n");
        return 0;
    }

    list->data = data_ptr;
    return 1;
}

int add(dynamic_array* list, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to add()\n");
        return 0;
    }

    if (list->data_size != data_size)
    {
        fprintf(stderr, "Cannot add an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
                list->data_size, data_size);
        return 0;
    }

    // A short circuit occurs and no resizing occurs if size < capacity
    if (list->size < list->capacity || grow_dynamic_list(list))
    {
        void* dest = (char*)(list->data) + (list->size * data_size);
        memcpy(dest, data, data_size);
        list->size++;
        return 1;
    }

    return 0;
}

int insert(dynamic_array* list, const size_t index, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to insert()\n");
        return 0;
    }

    if (list->data_size != data_size)
    {
        fprintf(stderr, "Cannot add an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
                list->data_size, data_size);
        return 0;
    }

    // A short circuit occurs and no resizing occurs if size < capacity
    if (list->size < list->capacity || grow_dynamic_list(list))
    {
        if (index < list->size)
        {
            void* dest = (char*)(list->data) + ((index + 1) * data_size);
            void* src = (char*)(list->data) + (index * data_size);
            size_t size = (list->size - index) * data_size;
            memmove(dest, src, size);
        }
        void* dest = (char*)(list->data) + (index * data_size);
        memcpy(dest, data, data_size);
        list->size++;
        return 1;
    }

    return 0;
}

int add_all(dynamic_array* list, dynamic_array* other_list)
{
    if (!list || !other_list)
    {
        fprintf(stderr, "Null pointer passed to add_all()\n");
        return 0;
    }

    if (list->data_size != other_list->data_size)
    {
        fprintf(stderr, "Cannot add an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
                list->data_size, other_list->data_size);
        return 0;
    }

    const size_t old_capacity = list->capacity;

    while (list->capacity < (other_list->size + list->size))
    {
        list->capacity <<= 1;
    }

    void* data_ptr = realloc(list->data, list->capacity * list->data_size);

    if (!data_ptr)
    {
        fprintf(stderr, "Memory allocation failed in add_all(). Unable to add elements from other_list\n");
        list->capacity = old_capacity;
        return 0;
    }

    list->data = data_ptr;

    const void* src = (char*)(other_list->data);
    void* dest = (char*)(list->data) + (list->size * list->data_size);
    memcpy(dest, src, other_list->data_size * other_list->size);
    list->size += other_list->size;
    return 1;
}

// Note: If the list stores pointers or other lists they should be freed individually before calling clear()
// As that would cause a memory leak.
void clear(dynamic_array* list)
{
    if (!list)
    {
        return;
    }

    free(list->data);
    void* data_ptr = malloc(DEFAULT_CAPACITY * list->data_size);

    if (!data_ptr)
    {
        fprintf(stderr, "Failed to allocate memory in clear()\n");
        list->data = NULL;
        return;
    }

    list->data = data_ptr;
    list->size = 0;
    list->capacity = DEFAULT_CAPACITY;
}

dynamic_array* clone(const dynamic_array* list)
{
    return initialize_dynamic_array_from(list, list->data_size);
}

int contains(const dynamic_array* list, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to contains()\n");
        return 0;
    }

    if (list->data_size != data_size)
    {
        fprintf(
            stderr, "Cannot search for an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
            list->data_size, data_size);
        return 0;
    }

    for (size_t i = 0; i < list->size; ++i)
    {
        void* src = (char*)(list->data) + (i * data_size);
        if (!memcmp(data, src, data_size))
        {
            return 1;
        }
    }
    return 0;
}

void ensure_capacity(dynamic_array* list, size_t capacity)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to ensure_capacity()\n");
        return;
    }
    if (list->size > capacity)
    {
        fprintf(stderr, "Can not change capacity elements will be lost, since capacity < list->size: %lu\n",
                list->size);
        return;
    }
    if (list->capacity != capacity)
    {
        void* data_ptr = realloc(list->data, list->data_size * capacity);

        if (!data_ptr)
        {
            fprintf(stderr, "No enough memory for dynamic array data resizing!\nOld dynamic data left as is!\n");
            return;
        }

        list->data = data_ptr;
        list->capacity = capacity;
    }
}

void* get(dynamic_array* list, const size_t index)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to get()\n");
        return nullptr;
    }

    if (index >= list->size)
    {
        fprintf(stderr, "Index Out of Bounds! %lu out of bounds of %lu\n", index, list->size - 1);
        return nullptr;
    }
    return (char*)(list->data) + (index * list->data_size);
}

long long int index_of(const dynamic_array* list, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to index_of()\n");
        return -1;
    }

    if (list->data_size != data_size)
    {
        fprintf(
            stderr, "Cannot search for an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
            list->data_size, data_size);
        return -1;
    }

    for (size_t i = 0; i < list->size; ++i)
    {
        const void* src = (char*)(list->data) + (i * data_size);
        if (!memcmp(data, src, data_size))
        {
            return (long long int)i;
        }
    }
    return -1;
}

int is_empty(const dynamic_array* list)
{
    return list->size == 0;
}

long long int last_index_of(dynamic_array* list, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to last_index_of()\n");
        return -1;
    }

    if (list->data_size != data_size)
    {
        fprintf(
            stderr, "Cannot search for an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
            list->data_size, data_size);
        return -1;
    }

    for (long long int i = (long long int)list->size - 1; i >= 0; --i)
    {
        const void* src = (char*)(list->data) + (i * data_size);
        if (!memcmp(data, src, data_size))
        {
            return i;
        }
    }
    return -1;
}

void* remove_at(dynamic_array* list, const size_t index)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to remove_at()\n");
        return nullptr;
    }

    if (index >= list->size)
    {
        fprintf(stderr, "Index Out of Bounds! %lu out of bounds of %lu\n", index, list->size - 1);
        return nullptr;
    }

    void* res = malloc(list->data_size);

    if (!res)
    {
        fprintf(stderr, "Failed to allocate memory for removed element\n");
        return nullptr;
    }

    const void* element_src = (char*)(list->data) + ((index) * list->data_size);
    memcpy(res, element_src, list->data_size);

    if (index < list->size)
    {
        const void* src = (char*)(list->data) + ((index + 1) * list->data_size);
        void* dest = (char*)(list->data) + (index * list->data_size);
        const size_t buffer_size = (list->size - index - 1) * list->data_size;
        memmove(dest, src, buffer_size);
    }

    list->size--;
    return res;
}

int remove_element(dynamic_array* list, const void* data, const size_t data_size)
{
    if (!list || !data)
    {
        fprintf(stderr, "Null pointer passed to remove_at()\n");
        return 0;
    }

    if (list->data_size != data_size)
    {
        fprintf(
            stderr, "Cannot search for an element of different data_size\nlist->data_size = %lu != data_size = %lu\n",
            list->data_size, data_size);
        return 0;
    }

    const long long int i = index_of(list, data, data_size);

    if (i < 0)
    {
        fprintf(stderr, "Element not found!\n");
        return 0;
    }

    void* removed = remove_at(list, i);

    if (!removed)
    {
        return 0;
    }

    const int res = memcmp(removed, data, data_size) == 0;

    return res;
}

int remove_all(dynamic_array* list, dynamic_array* other_list)
{
    if (!list || !other_list)
    {
        fprintf(stderr, "Null pointer passed to remove_all()\n");
        return 0;
    }

    if (list->data_size != other_list->data_size)
    {
        fprintf(
            stderr,
            "Cannot remove an element of different data_size\nlist->data_size = %lu != other_list->data_size = %lu\n",
            list->data_size, other_list->data_size);
        return 0;
    }

    for (size_t i = 0; i < other_list->size; ++i)
    {
        const void* to_be_removed = (char*)(other_list->data) + (i * other_list->data_size);
        remove_element(list, to_be_removed, list->data_size);
    }

    return 1;
}

void remove_range(dynamic_array* list, const size_t start, const size_t end)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to remove_range()\n");
        return;
    }

    if (start >= list->size)
    {
        fprintf(stderr, "Index out of bounds: start (%lu) can not be larger than list->size (%lu)\n", start
                , list->size);
        return;
    }

    if (start > end)
    {
        fprintf(stderr, "Invalid range: start %lu > end %lu\n", start, end);
        return;
    }

    if (start == end)
    {
        void* removed = remove_at(list, end);
        if (removed)
        {
            free(removed);
        }
        return;
    }

    long long int c = end < list->size ? (long long int)(end - 1) : (long long int)list->size - 1;

    while (c >= start)
    {
        void* removed = remove_at(list, c);
        if (removed)
        {
            free(removed);
        }
        c--;
    }
}

size_t size(dynamic_array* list)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to size()\n");
        return 0;
    }

    return list->size;
}

void sort(dynamic_array* list, int (*compar)(const void*, const void*))
{
    if (!list || !compar)
    {
        fprintf(stderr, "Null pointer passed to sort()\n");
        return;
    }

    if (list->size == 0)
    {
        fprintf(stderr, "Cannot sort an empty array.\n");
        return;
    }

    qsort(list->data, list->size, list->data_size, compar);
}

dynamic_array* get_sub_list(dynamic_array* list, const size_t start, const size_t end)
{
    if (!list)
    {
        fprintf(stderr, "Null pointer passed to get_sub_list()\n");
        return nullptr;
    }

    if (start >= list->size)
    {
        fprintf(stderr, "Index out of bounds: start (%lu) can not be larger than list->size (%lu)\n", start
                , list->size);
        return nullptr;
    }

    if (start > end)
    {
        fprintf(stderr, "Invalid range: start %lu > end %lu\n", start, end);
        return nullptr;
    }

    if (start == end)
    {
        dynamic_array* res = initialize_dynamic_array_empty(list->data_size);
        if (!res)
        {
            fprintf(stderr, "Failed to allocate memory for Sublist");
            return nullptr;
        }
        const void* src = (char*)(list->data) + (end * list->data_size);
        memcpy(res->data, src, list->data_size);
        return res;
    }

    const long long int c = end < list->size ? (long long int)end : (long long int)list->size - 1;
    const size_t sub_list_size = c - start;


    dynamic_array* res = initialize_dynamic_array_sized(sub_list_size, list->data_size);

    if (!res)
    {
        fprintf(stderr, "Failed to allocate memory for Sublist");
        return nullptr;
    }

    const void* src = (char*)(list->data) + (start * list->data_size);
    memcpy(res->data, src, sub_list_size * res->data_size);

    res->size = sub_list_size;

    return res;
}
