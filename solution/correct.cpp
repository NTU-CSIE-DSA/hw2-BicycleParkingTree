#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

enum Operation {
  PARK = 0,
  MOVE = 1,
  CLEAR = 2,
  REARRANGE = 3,
  FETCH = 4,
  REBUILD = 5
};

int64_t gcd(int64_t a, int64_t b) {
  int64_t tp = a % b;
  while(tp != 0) {
    a = b;
    b = tp;
    tp = a % b;
  }
  return b;
}

int lcm(int64_t a, int64_t b) {
  return a / gcd(a, b) * b;
}

struct rational {
  int64_t p, q;
};

void rat_simplify(struct rational *num);
struct rational rat_add(struct rational *a, struct rational *b);
struct rational rat_sub(struct rational *a, struct rational *b);
struct rational rat_mul(struct rational *a, struct rational *b);
struct rational rat_div(struct rational *a, struct rational *b);
int rat_cmp(struct rational *a, struct rational *b);

struct cds_array {
  char *data;
  size_t size, capacity, element_size;
};

struct cds_array cds_array_new(size_t element_size);
void cds_array_delete(struct cds_array *array);
int cds_array_push_back(struct cds_array *array, const void *new_element);
int cds_array_pop_back(struct cds_array *array);
void* cds_array_at(const struct cds_array *array, size_t index);
void *cds_array_get(const struct cds_array *array, size_t index);
size_t cds_array_size(const struct cds_array *array);
bool cds_array_empty(const struct cds_array *array);

struct cds_heap {
  struct cds_array data;
  int (*cmp)(const void *, const void *);
};

struct cds_heap cds_heap_new(size_t element_size, int (*cmp)(const void *, const void *));
void cds_heap_delete(struct cds_heap *heap);
int cds_heap_push(struct cds_heap *heap, const void *new_element);
int cds_heap_pop(struct cds_heap *heap);
void* cds_heap_top(const struct cds_heap *heap);
size_t cds_heap_size(const struct cds_heap *heap);
bool cds_heap_empty(const struct cds_heap *heap);

struct bicycle {
  struct rational location;
  int owner;
};

struct parking_slot {
  struct cds_array bicycles;
  size_t capacity;
};

struct parking_slot parking_slot_new(size_t capacity);
void parking_slot_delete(struct parking_slot *slot);
int parking_slot_insert(struct parking_slot *slot, size_t target_location);
struct rational parking_slot_erase(struct parking_slot *slot, int target_id);

struct bicycle_parking_tree {
  struct cds_array *edges;
  struct cds_array parking_slots;
  int64_t *dis_from_root;
  struct cds_heap chuiyuan;
};



/*
************************************
* ███╗   ███╗ █████╗ ██╗███╗   ██╗ *
* ████╗ ████║██╔══██╗██║████╗  ██║ *
* ██╔████╔██║███████║██║██╔██╗ ██║ *
* ██║╚██╔╝██║██╔══██║██║██║╚██╗██║ *
* ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║ *
* ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ *
************************************
*/
int main(void) {
  ;
}


void rat_simplify(struct rational *num) {
  if(num->q == 0) num->q = 1;
  int64_t GCD = gcd(num->p, num->q);
  num->p /= GCD;
  num->q /= GCD;
  if(num->q < 0) {
    num->p = -num->p;
    num->q = -num->q;
  }
}

struct rational rat_add(struct rational *a, struct rational *b) {
  struct rational ret = {a->p, a->q};
  int64_t LCM = lcm(ret.q, b->q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b->p *= LCM / b->q;
  ret.p += b->p;
  rat_simplify(&ret);
  return ret;
}

struct rational rat_sub(struct rational *a, struct rational *b) {
  struct rational ret = {a->p, a->q};
  int64_t LCM = lcm(ret.q, b->q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b->p *= LCM / b->q;
  ret.p -= b->p;
  rat_simplify(&ret);
  return ret;
}

struct rational rat_mul(struct rational *a, struct rational *b) {
  struct rational ret = {a->p * b->p, a->q * b->q};
  rat_simplify(&ret);
  return ret;
}

struct rational rat_div(struct rational *a, struct rational *b) {
  struct rational ret = {a->p * b->q, a->q * b->p};
  rat_simplify(&ret);
  return ret;
}

int rat_cmp(struct rational *a, struct rational *b) {
  if (a->p == b->p && a->q == b->q) {
    return 0;
  }
  if (a->p * b->q < a->q * b->p) {
    return -1;
  }
  return 1;
}


struct cds_array cds_array_new(const size_t element_size) {
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
  if (array->size == array->capacity) {
    array->capacity <<= 1;  // cap * 2
    array->data = (char*) realloc(array->data, array->capacity * array->element_size);
    if (array->data == NULL) {
      return -1;
    }
  }
  memcpy(array->data + array->size * array->element_size, new_element, array->element_size);
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

void* cds_array_at(const struct cds_array *array, const size_t index) {
  if (index < 0 || index >= array->size) {
    return NULL;
  }
  return (void*) (array->data + index * array->element_size);
}

void* cds_array_get(const struct cds_array *array, const size_t index) {
  return (void*) (array->data + index * array->element_size);
}

size_t cds_array_size(const struct cds_array *array) {
  return array->size;
}

bool cds_array_empty(const struct cds_array *array) {
  return array->size == 0;
}


struct cds_heap cds_heap_new(size_t element_size, int (*cmp)(const void *, const void *)) {
  struct cds_heap new_heap = {
    .data = cds_array_new(element_size),
    .cmp = cmp};
  char *empty = (char*) calloc(1, element_size);
  cds_array_push_back(&new_heap.data, empty);
  free(empty);
  return new_heap;
}

void cds_heap_delete(struct cds_heap *heap) {
  cds_array_delete(&heap->data);
  heap->cmp = NULL;
}

int cds_heap_increase_key(struct cds_heap *heap) {
  size_t i = heap->data.size - 1;
  while(i > 1 && heap->cmp(cds_array_at(&heap->data, i >> 1), cds_array_at(&heap->data, i)) < 0) {
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

size_t cds_heap_get_smallest(struct cds_heap *heap, size_t index) {
  size_t l = index << 1;
  size_t r = index << 1 | 1;
  size_t smallest = index;
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

void cds_heap_max_heapify(struct cds_heap *heap, size_t index) {
  size_t smallest = cds_heap_get_smallest(heap, index);
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
  cds_heap_max_heapify(heap, 1);
  return 0;
}

void* cds_heap_top(const struct cds_heap *heap) {
  return cds_array_at(&heap->data, 1);
}

size_t cds_heap_size(const struct cds_heap *heap) {
  return heap->data.size - 1;
}

bool cds_heap_empty(const struct cds_heap *heap) {
  return cds_heap_size(heap) == 0;
}


struct parking_slot parking_slot_new(size_t capacity) {
  struct parking_slot new_parking_slot = {
    .bicycles = cds_array_new(sizeof(struct bicycle)),
    .capacity = capacity};
  return new_parking_slot;
}

void parking_slot_delete(struct parking_slot *slot) {
  cds_array_delete(&slot->bicycles);
  slot->capacity = 0;
}

int parking_slot_insert(struct parking_slot *slot, size_t target_location);