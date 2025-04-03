#ifndef CDS_H
#define CDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct cds_array {
  char *data;
  long long size, capacity, element_size;
};

struct cds_array cds_array_new(long long element_size);
void cds_array_delete(struct cds_array *array);
int cds_array_push_back(struct cds_array *array, const void *new_element);
int cds_array_pop_back(struct cds_array *array);
void* cds_array_at(const struct cds_array *array, long long index);
void *cds_array_get(const struct cds_array *array, long long index);
long long cds_array_size(const struct cds_array *array);
bool cds_array_empty(const struct cds_array *array);
int cds_array_insert(struct cds_array *array, const long long at_index, const void *new_element);
int cds_array_erase(struct cds_array *array, const long long at_index);

struct cds_heap {
  struct cds_array data;
  int (*cmp)(const void *, const void *);
};

struct cds_heap cds_heap_new(long long element_size, int (*cmp)(const void *, const void *));
void cds_heap_delete(struct cds_heap *heap);
int cds_heap_push(struct cds_heap *heap, const void *new_element);
int cds_heap_pop(struct cds_heap *heap);
void* cds_heap_top(const struct cds_heap *heap);
long long cds_heap_size(const struct cds_heap *heap);
bool cds_heap_empty(const struct cds_heap *heap);

struct cds_array cds_array_new(const long long element_size) {
  struct cds_array new_array = {
    .data = (char*) malloc(element_size),
    .size = 0,
    .capacity = 1,
    .element_size = element_size};
  return new_array;
}
  
void cds_array_delete(struct cds_array *array) {
  if (array->data != NULL) free(array->data);
  array->data = NULL;
  array->size = array->capacity = 0;
  array->element_size = 0;
}

int cds_array_push_back(struct cds_array *array, const void *new_element) {
  // printf("sz: %zu, cap: %zu\n", array->size, array->capacity);
  if (array->size == array->capacity) {
    array->capacity <<= 1;  // cap * 2
    array->data = (char*) realloc(array->data, array->capacity * array->element_size);
    if (array->data == NULL) {
      return -1;
    }
  }
  memmove(array->data + array->size * array->element_size, new_element, array->element_size);
  array->size++;
  return 0;
}

int cds_array_pop_back(struct cds_array *array) {
  if (array->size == 0) {
    return -1;
  }
  array->size--;
  return 0;
}

void* cds_array_at(const struct cds_array *array, const long long index) {
  if (index < 0 || index >= array->size) {
    return NULL;
  }
  return (void*) (array->data + index * array->element_size);
}

void* cds_array_get(const struct cds_array *array, const long long index) {
  return (void*) (array->data + index * array->element_size);
}

long long cds_array_size(const struct cds_array *array) {
  return array->size;
}

bool cds_array_empty(const struct cds_array *array) {
  return array->size == 0;
}

int cds_array_insert(struct cds_array *array, const long long at_index, const void *new_element) {
  if (array->size == array->capacity) {
    array->capacity <<= 1;  // cap * 2
    array->data = (char*) realloc(array->data, array->capacity * array->element_size);
    if (array->data == NULL) {
      return -1;
    }
  }
  memmove(array->data + array->element_size * (at_index + 1),
    array->data + array->element_size * at_index,
    array->element_size * (array->size - at_index));
  memmove(array->data + array->element_size * at_index, new_element, array->element_size);
  array->size++;
  return 0;
}

int cds_array_erase(struct cds_array *array, const long long at_index) {
  if (array->size == 0) {
    return -1;
  }
  memmove(array->data + array->element_size * at_index,
    array->data + array->element_size * (at_index + 1),
    array->element_size * (array->size - at_index - 1));
  array->size--;
  return 0;
}


struct cds_heap cds_heap_new(long long element_size, int (*cmp)(const void *, const void *)) {
  struct cds_heap new_heap = {
    .data = cds_array_new(element_size),
    .cmp = cmp};
  new_heap.data.size = 1;
  return new_heap;
}

void cds_heap_delete(struct cds_heap *heap) {
  cds_array_delete(&heap->data);
  heap->cmp = NULL;
}

int cds_heap_increase_key(struct cds_heap *heap) {
  long long i = heap->data.size - 1;
  while(i > 1 && heap->cmp(cds_array_at(&heap->data, i >> 1), cds_array_at(&heap->data, i)) > 0) {
    static char tp[1024] = {0};
    if (heap->data.element_size < 1024) {
      memmove(tp, cds_array_at(&heap->data, i >> 1), heap->data.element_size);
      memmove(cds_array_at(&heap->data, i >> 1), cds_array_at(&heap->data, i),
        heap->data.element_size);
      memmove(cds_array_at(&heap->data, i), tp, heap->data.element_size);
    } else {
      fprintf(stderr, "buffer size not enough");
      exit(-1);
    }
    i >>= 1;
  }
  return 0;
}

int cds_heap_push(struct cds_heap *heap, const void *new_element) {
  if (cds_array_push_back(&heap->data, new_element) != 0) {
    return -1;
  }
  if (cds_heap_increase_key(heap) != 0) {
    return -1;
  }
  return 0;
}

long long cds_heap_get_smallest(struct cds_heap *heap, long long index) {
  long long l = index << 1;
  long long r = index << 1 | 1;
  long long smallest = index;
  if (l <= cds_heap_size(heap) &&
      heap->cmp(cds_array_at(&heap->data, l), cds_array_at(&heap->data, smallest)) < 0) {
    smallest = l;
  }
  if (r <= cds_heap_size(heap) &&
      heap->cmp(cds_array_at(&heap->data, r), cds_array_at(&heap->data, smallest)) < 0) {
    smallest = r;
  }
  return smallest;
}

void cds_heap_min_heapify(struct cds_heap *heap, long long index) {
  long long smallest = cds_heap_get_smallest(heap, index);
  while(smallest != index) {
    static char tp[1024] = {0};
    memmove(tp, cds_array_at(&heap->data, index), heap->data.element_size);
    memmove(cds_array_at(&heap->data, index), cds_array_at(&heap->data, smallest),
      heap->data.element_size);
    memmove(cds_array_at(&heap->data, smallest), tp, heap->data.element_size);
    index = smallest;
    smallest = cds_heap_get_smallest(heap, smallest);
  }
}

int cds_heap_pop(struct cds_heap *heap) {
  memmove(cds_array_at(&heap->data, 1),
    cds_array_at(&heap->data, heap->data.size - 1),
    heap->data.element_size);
  if (cds_array_pop_back(&heap->data) != 0) {
    return -1;
  }
  cds_heap_min_heapify(heap, 1);
  return 0;
}

void* cds_heap_top(const struct cds_heap *heap) {
  return cds_array_at(&heap->data, 1);
}

long long cds_heap_size(const struct cds_heap *heap) {
  return heap->data.size - 1;
}

bool cds_heap_empty(const struct cds_heap *heap) {
  return cds_heap_size(heap) == 0;
}

#endif