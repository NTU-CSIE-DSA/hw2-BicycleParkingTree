#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cds.h"
#include "rational.h"

enum Operation {
  PARK = 0,
  MOVE = 1,
  CLEAR = 2,
  REARRANGE = 3,
  FETCH = 4,
  REBUILD = 5
};

struct bicycle {
  struct rational location;
  int owner;
};

/*
 *********************************************************************************************************
 *
 *                                          BICYCLE CMP
 * 
 * Description: Compares two bicycles based on their locations and owners.
 * 
 * Arguments: a, b   Pointers to the bicycles to compare.
 *
 * Returns: Negative if a is before b, 0 if equal, positive if a is after b.
 * 
 * Notes: Primary key is location, secondary key is owner. Errors if owners are the same.
 *********************************************************************************************************
 */
int b_cmp(const void *a, const void *b);

struct ps {
  struct cds_array bicycles;
  size_t capacity;
};

/*
 *********************************************************************************************************
 *
 *                                      PARKING SLOT NEW
 * 
 * Description: Creates a new parking slot with the specified capacity.
 * 
 * Arguments: capacity   The maximum number of bicycles that can be parked at integer positions.
 *
 * Returns: A newly created struct ps instance.
 * 
 * Notes: The bicycles arr is initialized as empty.
 *********************************************************************************************************
 */
struct ps ps_new(size_t capacity);

/*
 *********************************************************************************************************
 *
 *                                    PARKING SLOT DELETE
 * 
 * Description: Frees the memory allocated for a parking slot.
 * 
 * Arguments: slot   Pointer to the parking slot to delete.
 *
 * Returns: void
 * 
 * Notes: Sets the capacity to 0 after freeing the memory.
 *********************************************************************************************************
 */
void ps_delete(struct ps *slot);

/*
 *********************************************************************************************************
 *
 *                                    PARKING SLOT INSERT
 * 
 * Description: Inserts a bicycle into a parking slot based on the parking policy.
 * 
 * Arguments: slot             Pointer to the parking slot.
 *            owner            ID of the bicycle owner.
 *            target_location  The intended parking position.
 *
 * Returns: The final position of the bicycle as a rational number.
 * 
 * Notes: Follows the parking policy described in the problem statement:
 *        1. If target_location is vacant, park there.
 *        2. If there's another vacancy in the slot, park at the nearest one.
 *        3. Otherwise, park at the middle position between appropriate bicycles.
 *********************************************************************************************************
 */
struct rational ps_insert(struct ps *slot, int owner, size_t target_location);

/*
 *********************************************************************************************************
 *
 *                                    PARKING SLOT ERASE
 * 
 * Description: Removes a bicycle with the specified owner ID from a parking slot.
 * 
 * Arguments: slot       Pointer to the parking slot.
 *            target_id  The ID of the bicycle owner to remove.
 *
 * Returns: 0 on success, -1 if the bicycle is not found.
 * 
 * Notes: Searches for the bicycle and removes it if found.
 *********************************************************************************************************
 */
int ps_erase(struct ps *slot, int target_id);

struct edge {
  size_t to;
  long long dis;
};

struct sy_info {
  int owner;
  long long t;
};

/*
 *********************************************************************************************************
 *
 *                                     sy INFO CMP
 * 
 * Description: Compares two sy_info structures based on time and owner.
 * 
 * Arguments: a, b   Pointers to the sy_info structures to compare.
 *
 * Returns: Negative if a is before b, 0 if equal, positive if a is after b.
 * 
 * Notes: Primary key is time (t), secondary key is owner.
 *********************************************************************************************************
 */
int si_cmp(const void *a, const void *b);

struct bicycle_pt {
  size_t n, m;
  struct ps *pss;
  struct cds_array *edges;
  long long *delay;
  int *top;
  int *order;
  int *parent;
  int *ssz;
  int *link;
  int *dep;
  long long *binary_index_tree;
  size_t *previous_slot;
  struct cds_heap sy;
};

/*
 *********************************************************************************************************
 *
 *                                 BINARY INDEX TREE UPDATE
 * 
 * Description: Updates a value in the Binary Indexed Tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            index          The index to update.
 *            value          The new value.
 *
 * Returns: void
 * 
 * Notes: The difference between the new value and the old value is propagated through the tree.
 *********************************************************************************************************
 */
void bit_update(struct bicycle_pt *pt, int index, long long value);

/*
 *********************************************************************************************************
 *
 *                               BINARY INDEX TREE PREFIX SUM
 * 
 * Description: Calculates the prefix sum up to the given index in the Binary Indexed Tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            index          The index up to which to calculate the prefix sum.
 *
 * Returns: The prefix sum up to the given index.
 * 
 * Notes: Useful for range queries in the Binary Indexed Tree.
 *********************************************************************************************************
 */
long long bit_prefix_sum(struct bicycle_pt *pt, int index);

/*
 *********************************************************************************************************
 *
 *                              BINARY INDEX TREE RANGE QUERY
 * 
 * Description: Calculates the sum of values in a range in the Binary Indexed Tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            left           The start index of the range (inclusive).
 *            right          The end index of the range (inclusive).
 *
 * Returns: The sum of values in the specified range.
 * 
 * Notes: Uses prefix sums to calculate the range sum.
 *********************************************************************************************************
 */
long long bit_range_query(struct bicycle_pt *pt, int left, int right);

/*
 *********************************************************************************************************
 *
 *                              BICYCLE PARKING TREE NEW
 * 
 * Description: Creates a new bicycle parking tree with the specified number of nodes and students.
 * 
 * Arguments: n   The number of parking slots (nodes).
 *            m   The number of students (users).
 *
 * Returns: A newly created struct bicycle_pt instance.
 * 
 * Notes: Allocates memory for all necessary data structures.
 *********************************************************************************************************
 */
struct bicycle_pt bpt_new(size_t n, size_t m);

/*
 *********************************************************************************************************
 *
 *                             BICYCLE PARKING TREE DELETE
 * 
 * Description: Frees the memory allocated for a bicycle parking tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree to delete.
 *
 * Returns: void
 * 
 * Notes: Recursively frees all allocated memory.
 *********************************************************************************************************
 */
void bpt_delete(struct bicycle_pt *pt);

/*
 *********************************************************************************************************
 *
 *                          BICYCLE PARKING TREE FIND PARENT
 * 
 * Description: Builds the parent-child relationships in the tree and calculates subtree sizes.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            now            The current node being processed.
 *            parent         The parent of the current node.
 *
 * Returns: void
 * 
 * Notes: Also sets the heavy edge for each node (used for heavy-light decomposition).
 *********************************************************************************************************
 */
void bpt_find_parent(struct bicycle_pt *pt, int now, int parent);

/*
 *********************************************************************************************************
 *
 *                          BICYCLE PARKING TREE BUILD CHAIN
 * 
 * Description: Builds chains for heavy-light decomposition of the tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            now            The current node being processed.
 *            parent         The chain top for the current node.
 *
 * Returns: void
 * 
 * Notes: Assigns DFS order to nodes and identifies chain tops.
 *********************************************************************************************************
 */
void bpt_build_chain(struct bicycle_pt *pt, int now, int parent);

/*
 *********************************************************************************************************
 *
 *                           BICYCLE PARKING TREE BUILD BIT
 * 
 * Description: Builds the Binary Indexed Tree for efficient path queries.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            now            The current node being processed.
 *            parent         The parent of the current node.
 *
 * Returns: void
 * 
 * Notes: Initializes the Binary Indexed Tree with edge weights.
 *********************************************************************************************************
 */
void bpt_build_bit(struct bicycle_pt *pt, int now, int parent);

/*
 *********************************************************************************************************
 *
 *                          BICYCLE PARKING TREE FIND DIS
 * 
 * Description: Calculates the distance (travel time) between two nodes in the tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            from           The source node.
 *            to             The destination node.
 *
 * Returns: The distance (travel time) from the source to the destination.
 * 
 * Notes: Uses heavy-light decomposition and Binary Indexed Tree for efficient queries.
 *********************************************************************************************************
 */
long long bpt_find_dis(struct bicycle_pt *pt, size_t from, size_t to);

/*
 *********************************************************************************************************
 *
 *                                             PARK
 * 
 * Description: Handles a PARK operation - parking a bicycle at a specific slot.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            s              The student ID.
 *            x              The parking slot.
 *            p              The intended parking position.
 *
 * Returns: void
 * 
 * Notes: Updates the previous_slot for the student and prints the final parking position.
 *********************************************************************************************************
 */
void park(struct bicycle_pt *pt, int s, size_t x, size_t p);

/*
 *********************************************************************************************************
 *
 *                                             MOVE
 * 
 * Description: Handles a MOVE operation - moving a bicycle from one slot to another.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            s              The student ID.
 *            y              The destination parking slot.
 *            p              The intended parking position at the destination.
 *
 * Returns: void
 * 
 * Notes: Calculates the travel time, updates the previous_slot, and prints the result.
 *********************************************************************************************************
 */
void move(struct bicycle_pt *pt, int s, size_t y, size_t p);

/*
 *********************************************************************************************************
 *
 *                                             CLEAR
 * 
 * Description: Handles a CLEAR operation - clearing all bicycles from a parking slot.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            x              The parking slot to clear.
 *            t              The current time.
 *
 * Returns: void
 * 
 * Notes: Adds all cleared bicycles to the sy h with appropriate fetch times.
 *********************************************************************************************************
 */
void clear(struct bicycle_pt *pt, size_t x, long long t);

/*
 *********************************************************************************************************
 *
 *                                          REARRANGE
 * 
 * Description: Handles a REARRANGE operation - removing non-integer positioned bicycles.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            x              The parking slot to rearrange.
 *            t              The current time.
 *
 * Returns: void
 * 
 * Notes: Adds removed bicycles to the sy h and prints the number of bicycles removed.
 *********************************************************************************************************
 */
void rearrange(struct bicycle_pt *pt, size_t x, long long t);

/*
 *********************************************************************************************************
 *
 *                                             FETCH
 * 
 * Description: Handles a FETCH operation - retrieving bicycles from sy.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            t              The current time.
 *
 * Returns: void
 * 
 * Notes: Removes bicycles from the sy h if their fetch time is <= t.
 *********************************************************************************************************
 */
void fetch(struct bicycle_pt *pt, long long t);

/*
 *********************************************************************************************************
 *
 *                                           REBUILD
 * 
 * Description: Handles a REBUILD operation - changing the weight of an edge in the tree.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            x, y           The endpoints of the edge to modify.
 *            d              The new weight for the edge.
 *
 * Returns: void
 * 
 * Notes: Updates the Binary Indexed Tree to reflect the changed edge weight.
 *********************************************************************************************************
 */
void rebuild(struct bicycle_pt *pt, size_t x, size_t y, long long d);

/*
 *********************************************************************************************************
 *
 *                                       HANDLE COMMANDS
 * 
 * Description: Processes operation commands for the bicycle parking system.
 * 
 * Arguments: pt   Pointer to the bicycle parking tree.
 *            q              The number of operations to process.
 *
 * Returns: void
 * 
 * Notes: Reads operations from standard input and calls appropriate handler functions.
 *********************************************************************************************************
 */
void handle_commands(struct bicycle_pt *pt, size_t q);


int si_cmp(const void *a, const void *b) {
  struct sy_info *ca = (struct sy_info*) a;
  struct sy_info *cb = (struct sy_info*) b;
  if (ca->t < cb->t) return -1;
  if (ca->t > cb->t) return 1;
  if (ca->owner < cb->owner) return -1;
  if (ca->owner > cb->owner) return 1;
  return 0;
}

int b_cmp(const void *a, const void *b) {
  struct bicycle *ba = (struct bicycle*) a;
  struct bicycle *bb = (struct bicycle*) b;
  int location_cmp_ret = r_cmp(&ba->location, &bb->location);
  if (location_cmp_ret != 0) {
    return location_cmp_ret;
  }
  if (ba->owner == bb->owner) {
    fprintf(stderr, "there should not be two bicycles that owned by a person\n");
    exit(-1);
  }
  return ba->owner < bb->owner ? -1 : 1;
}

struct ps ps_new(size_t capacity) {
  struct ps new_ps = {
    .bicycles = ca_new(sizeof(struct bicycle)),
    .capacity = capacity};
  return new_ps;
}

void ps_delete(struct ps *slot) {
  ca_delete(&slot->bicycles);
  slot->capacity = 0;
}


struct bicycle_pt bpt_new(size_t n, size_t m) {
  struct bicycle_pt new_pt = {
    .n = n,
    .m = m,
    .pss = (struct ps*) malloc(sizeof(struct ps) * n),
    .edges = (struct cds_array*) malloc(sizeof(struct cds_array) * n),
    .delay = (long long*) malloc(sizeof(long long) * m),
    .top = (int*) malloc(sizeof(int) * n),
    .order = (int*) calloc(n, sizeof(int)),
    .parent = (int*) malloc(sizeof(int) * n),
    .ssz = (int*) calloc(n, sizeof(int)),
    .link = (int*) malloc(sizeof(int) * n),
    .dep = (int*) malloc(sizeof(int) * n),
    .binary_index_tree = (long long*) calloc(n + 1, sizeof(long long)),
    .previous_slot = (size_t*) calloc(m, sizeof(size_t)),
    .sy = ch_new(sizeof(struct sy_info), si_cmp)};
  for (int i = 0; i < n; ++i) {
    new_pt.edges[i] = ca_new(sizeof(struct edge));
  }
  return new_pt;
}

void bpt_delete(struct bicycle_pt *pt) {
  for (int i = 0; i < pt->n; ++i) {
    ps_delete(&pt->pss[i]);
  }
  free(pt->pss);
  for (int i = 0; i < pt->n; ++i) {
    ca_delete(&pt->edges[i]);
  }
  free(pt->edges);
  free(pt->delay);
  free(pt->top);
  free(pt->binary_index_tree);
  free(pt->order);
  free(pt->parent);
  free(pt->dep);
  free(pt->previous_slot);
  ch_delete(&pt->sy);
}