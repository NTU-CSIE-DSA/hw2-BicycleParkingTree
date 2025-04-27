#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cds.h"

struct cds_array ca_new(const size_t element_size) {
  struct cds_array new_arr = {
    .data = (char*) malloc(element_size),
    .size = 0,
    .capacity = 1,
    .element_size = element_size};
  return new_arr;
}
  
void ca_delete(struct cds_array *arr) {
  if (arr->data != NULL) free(arr->data);
  arr->data = NULL;
  arr->size = arr->capacity = 0;
  arr->element_size = 0;
}

int ca_push_back(struct cds_array *arr, const void *new_element) {
  if (arr->size == arr->capacity) {
    arr->capacity <<= 1;  // cap * 2
    arr->data = (char*) realloc(arr->data, arr->capacity * arr->element_size);
    if (arr->data == NULL) {
      return -1;
    }
  }
  memmove(arr->data + arr->size * arr->element_size, new_element, arr->element_size);
  arr->size++;
  return 0;
}

int ca_pop_back(struct cds_array *arr) {
  if (arr->size == 0) {
    return -1;
  }
  arr->size--;
  return 0;
}

void* ca_at(const struct cds_array *arr, const size_t index) {
  if (index < 0 || index >= arr->size) {
    return NULL;
  }
  return (void*) (arr->data + index * arr->element_size);
}

void* ca_get(const struct cds_array *arr, const size_t index) {
  return (void*) (arr->data + index * arr->element_size);
}

size_t ca_size(const struct cds_array *arr) {
  return arr->size;
}

bool ca_empty(const struct cds_array *arr) {
  return arr->size == 0;
}

int ca_insert(struct cds_array *arr, const size_t at_index, const void *new_element) {
  if (arr->size == arr->capacity) {
    arr->capacity <<= 1;  // cap * 2
    arr->data = (char*) realloc(arr->data, arr->capacity * arr->element_size);
    if (arr->data == NULL) {
      return -1;
    }
  }
  memmove(arr->data + arr->element_size * (at_index + 1),
    arr->data + arr->element_size * at_index,
    arr->element_size * (arr->size - at_index));
  memmove(arr->data + arr->element_size * at_index, new_element, arr->element_size);
  arr->size++;
  return 0;
}

int ca_erase(struct cds_array *arr, const size_t at_index) {
  if (arr->size == 0) {
    return -1;
  }
  memmove(arr->data + arr->element_size * at_index,
    arr->data + arr->element_size * (at_index + 1),
    arr->element_size * (arr->size - at_index - 1));
  arr->size--;
  return 0;
}


struct cds_heap ch_new(size_t element_size, int (*cmp)(const void *, const void *)) {
  struct cds_heap new_h = {
    .data = ca_new(element_size),
    .cmp = cmp};
  new_h.data.size = 1;
  return new_h;
}

void ch_delete(struct cds_heap *h) {
  ca_delete(&h->data);
  h->cmp = NULL;
}

int ch_increase_key(struct cds_heap *h) {
  size_t i = h->data.size - 1;
  while(i > 1 && h->cmp(ca_at(&h->data, i >> 1), ca_at(&h->data, i)) > 0) {
    static char tp[1024] = {0};
    if (h->data.element_size < 1024) {
      memmove(tp, ca_at(&h->data, i >> 1), h->data.element_size);
      memmove(ca_at(&h->data, i >> 1), ca_at(&h->data, i),
        h->data.element_size);
      memmove(ca_at(&h->data, i), tp, h->data.element_size);
    } else {
      fprintf(stderr, "buffer size not enough");
      exit(-1);
    }
    i >>= 1;
  }
  return 0;
}

int ch_push(struct cds_heap *h, const void *new_element) {
  if (ca_push_back(&h->data, new_element) != 0) {
    return -1;
  }
  if (ch_increase_key(h) != 0) {
    return -1;
  }
  return 0;
}

size_t ch_get_smallest(struct cds_heap *h, size_t index) {
  size_t l = index << 1;
  size_t r = index << 1 | 1;
  size_t smallest = index;
  if (l <= ch_size(h) &&
      h->cmp(ca_at(&h->data, l), ca_at(&h->data, smallest)) < 0) {
    smallest = l;
  }
  if (r <= ch_size(h) &&
      h->cmp(ca_at(&h->data, r), ca_at(&h->data, smallest)) < 0) {
    smallest = r;
  }
  return smallest;
}

void ch_min_hify(struct cds_heap *h, size_t index) {
  size_t smallest = ch_get_smallest(h, index);
  while(smallest != index) {
    static char tp[1024] = {0};
    memmove(tp, ca_at(&h->data, index), h->data.element_size);
    memmove(ca_at(&h->data, index), ca_at(&h->data, smallest),
      h->data.element_size);
    memmove(ca_at(&h->data, smallest), tp, h->data.element_size);
    index = smallest;
    smallest = ch_get_smallest(h, smallest);
  }
}

int ch_pop(struct cds_heap *h) {
  memmove(ca_at(&h->data, 1),
    ca_at(&h->data, h->data.size - 1),
    h->data.element_size);
  if (ca_pop_back(&h->data) != 0) {
    return -1;
  }
  ch_min_hify(h, 1);
  return 0;
}

void* ch_top(const struct cds_heap *h) {
  return ca_at(&h->data, 1);
}

size_t ch_size(const struct cds_heap *h) {
  return h->data.size - 1;
}

bool ch_empty(const struct cds_heap *h) {
  return ch_size(h) == 0;
}