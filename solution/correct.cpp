#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>

enum Operation {
  PARK = 0,
  MOVE = 1,
  CLEAR = 2,
  REARRANGE = 3,
  FETCH = 4,
  REBUILD = 5
};

int64_t gcd(int64_t a, int64_t b);
int64_t lcm(int64_t a, int64_t b);

struct rational {
  int64_t p, q;
};

struct rational rational_new(int64_t p, int64_t q);
struct rational rational_from(int64_t p);
void rational_simplify(struct rational *num);
struct rational rational_add(struct rational *a, struct rational *b);
struct rational rational_sub(struct rational *a, struct rational *b);
struct rational rational_mul(struct rational *a, struct rational *b);
struct rational rational_div(struct rational *a, struct rational *b);
int rational_cmp(struct rational *a, struct rational *b);

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

int bicycle_cmp(const void *a, const void *b);

struct parking_slot {
  struct cds_array bicycles;
  size_t capacity;
};

struct parking_slot parking_slot_new(size_t capacity);
void parking_slot_delete(struct parking_slot *slot);
struct rational parking_slot_insert(struct parking_slot *slot, size_t target_location);
int parking_slot_erase(struct parking_slot *slot, int target_id);

struct edge {
  size_t to;
  int64_t dis;
};

struct bicycle_parking_tree {
  size_t n, m;
  struct parking_slot *parking_slots;
  struct cds_array *edges;
  int64_t *fetch_delay;
  int64_t *dis_from_root;
  struct cds_heap chuiyuan;
};

struct bicycle_parking_tree bicycle_parking_tree_new(size_t n, size_t m);
void bicycle_parking_tree_delete(struct bicycle_parking_tree *parking_tree);

void park(struct bicycle_parking_tree *parking_tree, size_t x, size_t p);
void move(struct bicycle_parking_tree *parking_tree, size_t x, size_t y, size_t p);
void clear(struct bicycle_parking_tree *parking_tree, size_t x, int64_t t);
void rearrange(struct bicycle_parking_tree *parking_tree, size_t x, int64_t t);
void fetch(struct bicycle_parking_tree *parking_tree, int64_t t);
void rebuild(struct bicycle_parking_tree *parking_tree, size_t x, size_t y, int64_t d);
void handle_commands(struct bicycle_parking_tree *parking_tree, size_t q);

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
  // Read first line: scale
  size_t n, m, q;
  assert(scanf("%zu%zu%zu", &n, &m, &q) == 3);
  struct bicycle_parking_tree parking_tree = bicycle_parking_tree_new(n, m);
  // Read second line: capacity for each slot
  for (int i = 0; i < n; ++i) {
    size_t capacity;
    assert(scanf("%zu", &capacity) == 1);
    parking_tree.parking_slots[i] = parking_slot_new(capacity);
  }
  // Read third line: fetch delay for each student
  for (int i = 0; i < m; ++i) {
    assert(scanf("%" SCNd64, &parking_tree.fetch_delay[i]) == 1);
  }
  // Read tree
  for (int i = 0; i < n - 1; ++i) {
    size_t x, y;
    int64_t w;
    assert(scanf("%zu%zu%" SCNd64, &x, &y, &w) == 3);
    struct edge toy = {.to = y, .dis = w};
    cds_array_push_back(&parking_tree.edges[x], (void*) &toy);
    struct edge tox = {.to = x, .dis = w};
    cds_array_push_back(&parking_tree.edges[y], (void*) &tox);
  }

  handle_commands(&parking_tree, q);
  bicycle_parking_tree_delete(&parking_tree);
}


int64_t gcd(int64_t a, int64_t b) {
  int64_t tp = a % b;
  while(tp != 0) {
    a = b;
    b = tp;
    tp = a % b;
  }
  return b;
}

int64_t lcm(int64_t a, int64_t b) {
  return a / gcd(a, b) * b;
}


struct rational rational_new(int64_t p, int64_t q) {
  struct rational new_rational = {p, q};
  return new_rational;
}

struct rational rational_from(int64_t p) {
  struct rational new_rational = {p, 1};
  return new_rational;
}

void rational_simplify(struct rational *num) {
  if(num->q == 0) num->q = 1;
  int64_t GCD = gcd(num->p, num->q);
  num->p /= GCD;
  num->q /= GCD;
  if(num->q < 0) {
    num->p = -num->p;
    num->q = -num->q;
  }
}

struct rational rational_add(struct rational *a, struct rational *b) {
  struct rational ret = {a->p, a->q};
  int64_t LCM = lcm(ret.q, b->q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b->p *= LCM / b->q;
  ret.p += b->p;
  rational_simplify(&ret);
  return ret;
}

struct rational rational_sub(struct rational *a, struct rational *b) {
  struct rational ret = {a->p, a->q};
  int64_t LCM = lcm(ret.q, b->q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b->p *= LCM / b->q;
  ret.p -= b->p;
  rational_simplify(&ret);
  return ret;
}

struct rational rational_mul(struct rational *a, struct rational *b) {
  struct rational ret = {a->p * b->p, a->q * b->q};
  rational_simplify(&ret);
  return ret;
}

struct rational rational_div(struct rational *a, struct rational *b) {
  struct rational ret = {a->p * b->q, a->q * b->p};
  rational_simplify(&ret);
  return ret;
}

int rational_cmp(struct rational *a, struct rational *b) {
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


int bicycle_cmp(const void *a, const void *b) {
  struct bicycle *ba = (struct bicycle*) a;
  struct bicycle *bb = (struct bicycle*) b;
  int location_cmp_ret = rational_cmp(&ba->location, &bb->location);
  if (location_cmp_ret != 0) {
    return location_cmp_ret;
  }
  if (ba->owner == bb->owner) {
    fprintf(stderr, "there should not be two bicycles that owned by a person\n");
    exit(-1);
  }
  return ba->owner < bb->owner ? -1 : 1;
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

struct rational parking_slot_insert(struct parking_slot *slot, size_t target_location) {
  struct rational r_target = { .p = target_location, .q = 1 };
  int occupied[24] = {0};
  int min_index = -1;
  struct rational min_dis = { .p = 1000000000, .q = 1 };
  for (int i = 0; i < cds_array_size(&slot->bicycles); ++i) {
    struct bicycle *b = (struct bicycle*) cds_array_at(&slot->bicycles, i);
    if (b->location.q == 1) {
      occupied[b->location.p] = 1;
      struct rational dis = rational_sub(&b->location, &r_target);
      if (rational_cmp(&b->location, &r_target) != 0 && rational_cmp(&dis, &min_dis) < 0) {
        min_index = i;
        min_dis = dis;
      }
    }
  }
  if (!occupied[target_location]) {
    for (int i = 0; i < cds_array_size(&slot->bicycles); ++i) {
      const struct bicycle *b = (struct bicycle*) cds_array_at(&slot->bicycles, i);
      if (b->location.q == 1) {
        occupied[b->location.p] = 1;
      }
    }
  }
  bool find_vacancy = false;
  for (int i = 1; i <= slot->capacity; ++i) {
    if (!occupied[i]) {
      find_vacancy = true;
      break;
    }
  }
  if (find_vacancy) {
    ;
  } else {
    ;
  }
}

int parking_slot_erase(struct parking_slot *slot, int target_id);


struct bicycle_parking_tree bicycle_parking_tree_new(size_t n, size_t m) {
  struct bicycle_parking_tree new_parking_tree = {
    .n = n,
    .m = m,
    .parking_slots = (struct parking_slot*) malloc(sizeof(struct parking_slot) * n),
    .edges = (struct cds_array*) malloc(sizeof(struct cds_array) * n),
    .fetch_delay = (int64_t*) malloc(sizeof(int64_t) * m),
    .dis_from_root = (int64_t*) malloc(sizeof(int64_t) * m),
    .chuiyuan = cds_heap_new(sizeof(struct bicycle), bicycle_cmp)};
  for (int i = 0; i < n; ++i) {
    new_parking_tree.edges[i] = cds_array_new(sizeof(struct edge));
  }
  return new_parking_tree;
}

void bicycle_parking_tree_delete(struct bicycle_parking_tree *parking_tree) {
  for (int i = 0; i < parking_tree->n; ++i) {
    parking_slot_delete(&parking_tree->parking_slots[i]);
  }
  free(parking_tree->parking_slots);
  for (int i = 0; i < parking_tree->n; ++i) {
    cds_array_delete(&parking_tree->edges[i]);
  }
  free(parking_tree->edges);
  free(parking_tree->fetch_delay);
  free(parking_tree->dis_from_root);
  cds_heap_delete(&parking_tree->chuiyuan);
}


void park(struct bicycle_parking_tree *parking_tree, size_t x, size_t p);
void move(struct bicycle_parking_tree *parking_tree, size_t x, size_t y, size_t p);
void clear(struct bicycle_parking_tree *parking_tree, size_t x, int64_t t);
void rearrange(struct bicycle_parking_tree *parking_tree, size_t x, int64_t t);
void fetch(struct bicycle_parking_tree *parking_tree, int64_t t);
void rebuild(struct bicycle_parking_tree *parking_tree, size_t x, size_t y, int64_t d);
void handle_commands(struct bicycle_parking_tree *parking_tree, size_t q);

void handle_commands(struct bicycle_parking_tree *parking_tree, size_t q) {
  for (int i = 0; i < q; ++i) {
    Operation op;
    assert(scanf("%u", &op) == 1);
    switch (op) {
      case PARK: {
        size_t x, p;
        assert(scanf("%zu%zu", &x, &p) == 2);
        park(parking_tree, x, p);
        break;
      }
      case MOVE: {
        size_t x, y, p;
        assert(scanf("%zu%zu%zu", &x, &y, &p) == 3);
        move(parking_tree, x, y, p);
        break;
      }
      case CLEAR: {
        size_t x;
        int64_t t;
        assert(scanf("%zu%" SCNd64, &x, &t) == 2);
        clear(parking_tree, x, t);
        break;
      }
      case REARRANGE: {
        size_t x;
        int64_t t;
        assert(scanf("%zu%" SCNd64, &x, &t) == 2);
        rearrange(parking_tree, x, t);
        break;
      }
      case FETCH: {
        int64_t t;
        assert(scanf("%" SCNd64, &t) == 1);
        fetch(parking_tree, t);
        break;
      }
      case REBUILD: {
        size_t x, y;
        assert(scanf("%zu%zu", &x, &y) == 2);
        break;
      }
      default: {
        fprintf(stderr, "invalid operation type");
        exit(-1);
      }
    }
  }
}