#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "cds.h"
#include "rational.h"
#include "bicycle.h"
#include "answer.h"

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
  struct bicycle_pt pt = bpt_new(n, m);
  // Read second line: capacity for each slot
  for (int i = 0; i < n; ++i) {
    size_t capacity;
    assert(scanf("%zu", &capacity) == 1);
    pt.pss[i] = ps_new(capacity);
  }
  // Read third line: fetch delay for each student
  for (int i = 0; i < m; ++i) {
    assert(scanf("%lld", &pt.delay[i]) == 1);
  }
  // Read tree
  for (int i = 0; i < (int) n - 1; ++i) {
    size_t x, y;
    long long w;
    assert(scanf("%zu%zu%lld", &x, &y, &w) == 3);
    struct edge toy = { .to = y, .dis = w };
    ca_push_back(&pt.edges[x], (void*) &toy);
    struct edge tox = { .to = x, .dis = w };
    ca_push_back(&pt.edges[y], (void*) &tox);
  }
  bpt_find_parent(&pt, 0, 0);
  bpt_build_chain(&pt, 0, 0);
  bpt_build_bit(&pt, 0, 0);

  handle_commands(&pt, q);
  bpt_delete(&pt);
}


struct rational ps_insert(struct ps *slot, int owner, size_t target_location) {
  // Convert target location to rational form
  struct rational r_target = { .p = (long long) target_location, .q = 1 };
  
  // Allocate and check for memory allocation failure
  int *occupied = (int*) calloc(slot->capacity + 1, sizeof(int));
  
  // Find important positions:
  // 1. Is the target location already occupied?
  // 2. Which bicycle is closest to the target?
  // 3. What's the first bicycle to the right of target?
  int closest_bicycle_idx = -1;
  int target_occupied_idx = -1;
  int first_right_bicycle_idx = -1;
  struct rational min_distance = { .p = LLONG_MAX, .q = 1 };
  
  for (int i = 0; i < ca_size(&slot->bicycles); ++i) {
    struct bicycle *b = (struct bicycle*) ca_at(&slot->bicycles, i);
    
    // Mark integer positions as occupied
    if (b->location.q == 1) {
      occupied[b->location.p] = 1;
      
      // Check if this bicycle is at the target position
      if (r_cmp(&b->location, &r_target) == 0) {
        target_occupied_idx = i;
      }
      else {
        // Calculate distance to find closest bicycle
        struct rational distance = r_abs(r_sub(b->location, r_target));
        if (r_cmp(&distance, &min_distance) < 0) {
          closest_bicycle_idx = i;
          min_distance = distance;
        }
      }
    }
    
    // Find the first bicycle to the right of target
    if (r_cmp(&b->location, &r_target) > 0 && 
        (first_right_bicycle_idx == -1 || 
         r_cmp(&b->location, &((struct bicycle*)ca_at(&slot->bicycles, first_right_bicycle_idx))->location) < 0)) {
      first_right_bicycle_idx = i;
    }
  }
  
  // Case 1: Target position is vacant
  if (!occupied[target_location]) {
    free(occupied);
    struct bicycle new_bicycle = {
      .location = {(long long) target_location, 1},
      .owner = owner
    };
    
    if (first_right_bicycle_idx == -1) {
      // Insert at the end if no bicycles to the right
      ca_push_back(&slot->bicycles, (void*) &new_bicycle);
    } else {
      // Insert before the first bicycle to the right
      ca_insert(&slot->bicycles, first_right_bicycle_idx, (void*) &new_bicycle);
    }
    return new_bicycle.location;
  }
  
  // Case 2: Find nearest vacant position if one exists
  int nearest_vacant_position = -1;
  struct rational nearest_vacant_distance = { .p = LLONG_MAX, .q = 1 };
  bool found_vacancy = false;
  
  for (int i = 1; i <= slot->capacity; ++i) {
    if (!occupied[i]) {
      struct rational current_position = { .p = i, .q = 1 };
      struct rational current_distance = r_abs(r_sub(current_position, r_target));
      
      if (r_cmp(&current_distance, &nearest_vacant_distance) < 0) {
        nearest_vacant_position = i;
        nearest_vacant_distance = current_distance;
        found_vacancy = true;
      }
    }
  }
  
  free(occupied); // Done with the occupied arr
  
  if (found_vacancy) {
    // Case 2: Park at nearest vacant position
    struct rational nearest_position = { .p = nearest_vacant_position, .q = 1 };
    
    // Find the right position in the bicycles arr
    int insert_idx = ca_size(&slot->bicycles);
    for (int i = 0; i < ca_size(&slot->bicycles); ++i) {
      struct bicycle *b = (struct bicycle*) ca_at(&slot->bicycles, i);
      if (r_cmp(&b->location, &nearest_position) > 0) {
        insert_idx = i;
        break;
      }
    }
    
    struct bicycle new_bicycle = {
      .location = nearest_position,
      .owner = owner
    };
    
    if (insert_idx == ca_size(&slot->bicycles)) {
      ca_push_back(&slot->bicycles, (void*) &new_bicycle);
    } else {
      ca_insert(&slot->bicycles, insert_idx, (void*) &new_bicycle);
    }
    return new_bicycle.location;
  }
  
  // Case 3: No vacancies, insert at middle position
  int left_idx, right_idx;
  
  if (target_occupied_idx == -1) {
    fprintf(stderr, "Logic error: target position should be occupied\n");
    exit(EXIT_FAILURE);
  }
  
  // Find positions for midpoint calculation
  if (target_occupied_idx > 0) {
    left_idx = target_occupied_idx - 1;
    right_idx = target_occupied_idx;
  } else if (ca_size(&slot->bicycles) > 1) {
    left_idx = target_occupied_idx;
    right_idx = target_occupied_idx + 1;
  } else {
    // Only one bicycle exists and it's at the target position
    // Create position at target + 0.5
    struct rational mid_location = r_div(
      r_add(r_target, r_new(r_target.p + 1, r_target.q)),
      r_from(2)
    );
    struct bicycle new_bicycle = {
      .location = mid_location,
      .owner = owner
    };
    ca_push_back(&slot->bicycles, (void*) &new_bicycle);
    return new_bicycle.location;
  }
  
  // Calculate midpoint between two positions
  struct rational left_pos = ((struct bicycle*)ca_at(&slot->bicycles, left_idx))->location;
  struct rational right_pos = ((struct bicycle*)ca_at(&slot->bicycles, right_idx))->location;
  struct rational mid_location = r_div(r_add(left_pos, right_pos), r_from(2));
  
  // Insert the new bicycle
  struct bicycle new_bicycle = {
    .location = mid_location,
    .owner = owner
  };
  
  int insert_idx = (left_idx < right_idx) ? right_idx : left_idx + 1;
  ca_insert(&slot->bicycles, insert_idx, (void*) &new_bicycle);
  
  return new_bicycle.location;
}

int ps_erase(struct ps *slot, int target_id) {
  size_t target_index = -1;
  for (size_t i = 0; i < ca_size(&slot->bicycles); ++i) {
    struct bicycle *b = (struct bicycle*) ca_at(&slot->bicycles, i);
    if (b->owner == target_id) {
      target_index = i;
      break;
    }
  }
  if (target_index != (size_t) -1) {
    return ca_erase(&slot->bicycles, target_index);
  }
  return -1;
}


void bit_update(struct bicycle_pt *pt, int index, long long value) {
  long long original_value = bit_range_query(pt, index, index);
  long long difference = value - original_value;
  for (int i = index; i <= pt->n; i += (i & -i)) {
    pt->binary_index_tree[i] += difference;
  }
}

long long bit_prefix_sum(struct bicycle_pt *pt, int index) {
  long long ret = 0;
  for (int i = index; i > 0; i -= (i & -i)) {
    ret += pt->binary_index_tree[i];
  }
  return ret;
}

long long bit_range_query(struct bicycle_pt *pt, int left, int right) {
  return bit_prefix_sum(pt, right) - bit_prefix_sum(pt, left - 1);
}

void bpt_find_parent(struct bicycle_pt *pt, int now, int parent) {
  pt->parent[now] = parent;
  pt->ssz[now] = 1;
  pt->link[now] = -1;
  pt->dep[now] = pt->dep[parent] + 1;
  int max_ssz = 0;
  for (size_t i = 0; i < ca_size(&pt->edges[now]); ++i) {
    struct edge *next = (struct edge*) ca_at(&pt->edges[now], i);
    if (next->to == parent) continue;
    bpt_find_parent(pt, next->to, now);
    pt->ssz[now] += pt->ssz[next->to];
    if (pt->ssz[next->to] > max_ssz) {
      max_ssz = pt->ssz[next->to];
      pt->link[now] = next->to;
    }
  }
}

void bpt_build_chain(struct bicycle_pt *pt, int now, int top) {
  static int stamp = 1;
  pt->order[now] = stamp++;
  pt->top[now] = top;
  if (pt->link[now] != -1) {
    bpt_build_chain(pt, pt->link[now], top);
  }
  for (size_t i = 0; i < ca_size(&pt->edges[now]); ++i) {
    struct edge *next = (struct edge*) ca_at(&pt->edges[now], i);
    if (pt->order[next->to] == 0) {
      bpt_build_chain(pt, next->to, next->to);
    }
  }
}

void bpt_build_bit(struct bicycle_pt *pt, int now, int parent) {
  for (size_t i = 0; i < ca_size(&pt->edges[now]); ++i) {
    struct edge *next = (struct edge*) ca_at(&pt->edges[now], i);
    if (next->to == parent) continue;
    bit_update(pt, pt->order[next->to], next->dis);
    bpt_build_bit(pt, next->to, now);
  }
}

long long bpt_find_dis(struct bicycle_pt *pt, size_t from, size_t to) {
  long long ret = 0;
  while (pt->top[from] != pt->top[to]) {
    if (pt->dep[pt->top[from]] < pt->dep[pt->top[to]]) {
      int tp = from;
      from = to;
      to = tp;
    }
    ret += bit_range_query(pt, pt->order[pt->top[from]],
      pt->order[from]);
    from = pt->parent[pt->top[from]];
  }
  if (pt->dep[from] > pt->dep[to]) {
    int tp = from;
    from = to;
    to = tp;
  }
  ret += bit_range_query(pt, pt->order[from], pt->order[to]);
  ret -= bit_range_query(pt, pt->order[from], pt->order[from]);
  return ret;
}


void park(struct bicycle_pt *pt, int s, size_t x, size_t p) {
  struct rational final_position = ps_insert(&pt->pss[x], s, p);
  pt->previous_slot[s] = x;
  printf("%d parked at (%zu, ", s, x);
  if (final_position.q == 1) {
    printf("%lld", final_position.p);
  } else {
    printf("%lld" "/%lld" , final_position.p, final_position.q);
  }
  printf(").\n");
}

void move(struct bicycle_pt *pt, int s, size_t y, size_t p) {
  const size_t x = pt->previous_slot[s];
  if (x == y) {
    printf("%d moved to %zu in 0 seconds.\n", s, y);
    return;
  }
  ps_erase(&pt->pss[x], s);
  const long long t = bpt_find_dis(pt, x, y);
  printf("%d moved to %zu in %lld" " seconds.\n", s, y, t);
  ps_insert(&pt->pss[y], s, p);
  pt->previous_slot[s] = y;
}

void clear(struct bicycle_pt *pt, size_t x, long long t) {
  for (size_t i = 0; i < ca_size(&pt->pss[x].bicycles); ++i) {
    struct bicycle *b = (struct bicycle*) ca_at(&pt->pss[x].bicycles, i);
    struct sy_info info = {
      .owner = b->owner,
      .t = t + pt->delay[b->owner]
    };
    ch_push(&pt->sy, &info);
  }
  ca_delete(&pt->pss[x].bicycles);
  pt->pss[x].bicycles = ca_new(sizeof(struct bicycle));
}

void rearrange(struct bicycle_pt *pt, size_t x, long long t) {
  struct cds_array *bicycles = &pt->pss[x].bicycles;
  size_t new_size = 0;
  for (size_t i = 0; i < ca_size(bicycles); ++i) {
    struct bicycle *b = (struct bicycle*) ca_at(bicycles, i);
    if (b->location.q != 1) {
      struct sy_info info = {
        .owner = b->owner,
        .t = t + pt->delay[b->owner]
      };
      ch_push(&pt->sy, &info);
    } else {
      if (new_size != i) {
        memmove(ca_at(bicycles, new_size), b, sizeof(struct bicycle));
      }
      new_size++;
    }
  }
  printf("Rearranged %zu bicycles in %zu.\n", bicycles->size - new_size, x);
  bicycles->size = new_size;
}

void fetch(struct bicycle_pt *pt, long long t) {
  int fetched = 0;
  while (ch_size(&pt->sy) > 0 && 
      ((struct sy_info*) ch_top(&pt->sy))->t <= t) {
    fetched++;
    ch_pop(&pt->sy);
  }
  printf("At %lld" ", %d bikes was fetched.\n", t, fetched);
}

void rebuild(struct bicycle_pt *pt, size_t x, size_t y, long long d) {
  if (pt->dep[x] > pt->dep[y]) {
    size_t tp = x;
    x = y;
    y = tp;
  }
  bit_update(pt, pt->order[y], d);
}

void handle_commands(struct bicycle_pt *pt, size_t q) {
  for (int i = 0; i < q; ++i) {
    int op;
    assert(scanf("%u", &op) == 1);
    switch (op) {
      case PARK: {
        int s;
        size_t x, p;
        assert(scanf("%d%zu%zu", &s, &x, &p) == 3);
        park(pt, s, x, p);
        break;
      }
      case MOVE: {
        int s;
        size_t y, p;
        assert(scanf("%d%zu%zu", &s, &y, &p) == 3);
        move(pt, s, y, p);
        break;
      }
      case CLEAR: {
        size_t x;
        long long t;
        assert(scanf("%zu%lld", &x, &t) == 2);
        clear(pt, x, t);
        break;
      }
      case REARRANGE: {
        size_t x;
        long long t;
        assert(scanf("%zu%lld", &x, &t) == 2);
        rearrange(pt, x, t);
        break;
      }
      case FETCH: {
        long long t;
        assert(scanf("%lld", &t) == 1);
        fetch(pt, t);
        break;
      }
      case REBUILD: {
        size_t x, y;
        long long d;
        assert(scanf("%zu%zu%lld", &x, &y, &d) == 3);
        rebuild(pt, x, y, d);
        break;
      }
      default: {
        fprintf(stderr, "invalid operation type");
        exit(-1);
      }
    }
  }
}