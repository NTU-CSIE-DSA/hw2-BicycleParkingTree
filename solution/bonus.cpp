#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

enum Operation {
  PARK = 0,
  MOVE = 1,
  CLEAR = 2,
  REARRANGE = 3,
  FETCH = 4,
  REBUILD = 5
};

/*
 *********************************************************************************************************
 *
 *                                                 GCD
 * 
 * Description: Computes the greatest common divisor of two integers using Euclidean algorithm.
 * 
 * Arguments: a, b   The two integers for which to find the GCD.
 *
 * Returns: The greatest common divisor of a and b.
 * 
 * Notes: Assumes that b is not 0 when the function is called.
 *********************************************************************************************************
 */
long long gcd(long long a, long long b);

/*
 *********************************************************************************************************
 *
 *                                                 LCM
 * 
 * Description: Computes the least common multiple of two integers.
 * 
 * Arguments: a, b   The two integers for which to find the LCM.
 *
 * Returns: The least common multiple of a and b.
 * 
 * Notes: Uses the GCD function to calculate LCM using the formula lcm(a,b) = a*b/gcd(a,b).
 *********************************************************************************************************
 */
long long lcm(long long a, long long b);

struct rational {
  long long p, q;
};

/*
 *********************************************************************************************************
 *
 *                                           RATIONAL NEW
 * 
 * Description: Creates a new rational number with the specified numerator and denominator.
 * 
 * Arguments: p   The numerator of the rational number.
 *            q   The denominator of the rational number.
 *
 * Returns: A rational number struct representing p/q.
 * 
 * Notes: The rational number is not automatically simplified.
 *********************************************************************************************************
 */
struct rational r_new(long long p, long long q);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL FROM
 * 
 * Description: Creates a new rational number from an integer.
 * 
 * Arguments: p   The integer to convert to a rational number.
 *
 * Returns: A rational number struct representing p/1.
 * 
 * Notes: This is a convenience function for creating rational numbers from integers.
 *********************************************************************************************************
 */
struct rational r_from(long long p);

/*
 *********************************************************************************************************
 *
 *                                       RATIONAL SIMPLIFY
 * 
 * Description: Simplifies a rational number to its lowest terms.
 * 
 * Arguments: num   Pointer to the rational number to simplify.
 *
 * Returns: void
 * 
 * Notes: The function modifies the rational number in-place. If the denominator is negative,
 *        the signs of both numerator and denominator are flipped.
 *********************************************************************************************************
 */
void r_simplify(struct rational *num);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL ADD
 * 
 * Description: Adds two rational numbers.
 * 
 * Arguments: a, b   The two rational numbers to add.
 *
 * Returns: A rational number representing the sum a + b.
 * 
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_add(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL SUB
 * 
 * Description: Subtracts one rational number from another.
 * 
 * Arguments: a, b   Calculate a - b.
 *
 * Returns: A rational number representing the difference a - b.
 * 
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_sub(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL MUL
 * 
 * Description: Multiplies two rational numbers.
 * 
 * Arguments: a, b   The two rational numbers to multiply.
 *
 * Returns: A rational number representing the product a * b.
 * 
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_mul(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL DIV
 * 
 * Description: Divides one rational number by another.
 * 
 * Arguments: a, b   Calculate a / b.
 *
 * Returns: A rational number representing the quotient a / b.
 * 
 * Notes: The result is automatically simplified. Assumes b is not zero.
 *********************************************************************************************************
 */
struct rational r_div(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL CMP
 * 
 * Description: Compares two rational numbers.
 * 
 * Arguments: a, b   Pointers to the rational numbers to compare.
 *
 * Returns: -1 if a < b, 0 if a = b, 1 if a > b.
 * 
 * Notes: Comparison is done by cross multiplication to avoid precision issues.
 *********************************************************************************************************
 */
int r_cmp(struct rational *a, struct rational *b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL ABS
 * 
 * Description: Computes the absolute value of a rational number.
 * 
 * Arguments: a   The rational number to find the absolute value of.
 *
 * Returns: A rational number representing |a|.
 * 
 * Notes: Takes the absolute value of both numerator and denominator.
 *********************************************************************************************************
 */
struct rational r_abs(struct rational a);

struct cds_array {
  char *data;
  size_t size, capacity, element_size;
};

/*
 *********************************************************************************************************
 *
 *                                          CDS arr NEW
 * 
 * Description: Creates a new dynamic arr with the specified element size.
 * 
 * Arguments: element_size   The size of each element in the arr.
 *
 * Returns: A newly created struct cds_array instance.
 * 
 * Notes: The caller is responsible for freeing the memory using cds_array_delete.
 *        The initial capacity is 1, and will grow as needed.
 *********************************************************************************************************
 */
struct cds_array ca_new(size_t element_size);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr DELETE
 * 
 * Description: Frees the memory allocated for a dynamic arr.
 * 
 * Arguments: arr   Pointer to the arr to delete.
 *
 * Returns: void
 * 
 * Notes: Sets all fields of the arr to 0 or NULL after freeing the memory.
 *********************************************************************************************************
 */
void ca_delete(struct cds_array *arr);

/*
 *********************************************************************************************************
 *
 *                                      CDS arr PUSH BACK
 * 
 * Description: Adds an element to the end of a dynamic arr.
 * 
 * Arguments: arr         Pointer to the arr.
 *            new_element   Pointer to the element to add.
 *
 * Returns: 0 on success, -1 on memory allocation failure.
 * 
 * Notes: The capacity of the arr is doubled if needed.
 *********************************************************************************************************
 */
int ca_push_back(struct cds_array *arr, const void *new_element);

/*
 *********************************************************************************************************
 *
 *                                      CDS arr POP BACK
 * 
 * Description: Removes the last element from a dynamic arr.
 * 
 * Arguments: arr   Pointer to the arr.
 *
 * Returns: 0 on success, -1 if the arr is empty.
 * 
 * Notes: The capacity of the arr is not decreased.
 *********************************************************************************************************
 */
int ca_pop_back(struct cds_array *arr);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr AT
 * 
 * Description: Retrieves an element from a dynamic arr with bounds checking.
 * 
 * Arguments: arr   Pointer to the arr.
 *            index   Index of the element to retrieve.
 *
 * Returns: Pointer to the element, or NULL if the index is out of bounds.
 * 
 * Notes: Use this function for safe access to elements.
 *********************************************************************************************************
 */
void* ca_at(const struct cds_array *arr, size_t index);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr GET
 * 
 * Description: Retrieves an element from a dynamic arr without bounds checking.
 * 
 * Arguments: arr   Pointer to the arr.
 *            index   Index of the element to retrieve.
 *
 * Returns: Pointer to the element.
 * 
 * Notes: Does not check if the index is valid. Use with caution.
 *********************************************************************************************************
 */
void* ca_get(const struct cds_array *arr, size_t index);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr SIZE
 * 
 * Description: Returns the number of elements in a dynamic arr.
 * 
 * Arguments: arr   Pointer to the arr.
 *
 * Returns: The number of elements in the arr.
 * 
 * Notes: None.
 *********************************************************************************************************
 */
size_t ca_size(const struct cds_array *arr);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr EMPTY
 * 
 * Description: Checks if a dynamic arr is empty.
 * 
 * Arguments: arr   Pointer to the arr.
 *
 * Returns: true if the arr is empty, false otherwise.
 * 
 * Notes: None.
 *********************************************************************************************************
 */
bool ca_empty(const struct cds_array *arr);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr INSERT
 * 
 * Description: Inserts an element at a specific position in a dynamic arr.
 * 
 * Arguments: arr         Pointer to the arr.
 *            at_index      Index at which to insert the element.
 *            new_element   Pointer to the element to insert.
 *
 * Returns: 0 on success, -1 on memory allocation failure.
 * 
 * Notes: The capacity of the arr is doubled if needed.
 *********************************************************************************************************
 */
int ca_insert(struct cds_array *arr, const size_t at_index, const void *new_element);

/*
 *********************************************************************************************************
 *
 *                                        CDS arr ERASE
 * 
 * Description: Removes an element at a specific position from a dynamic arr.
 * 
 * Arguments: arr      Pointer to the arr.
 *            at_index   Index of the element to remove.
 *
 * Returns: 0 on success, -1 if the arr is empty.
 * 
 * Notes: The elements after the removed one are shifted to fill the gap.
 *********************************************************************************************************
 */
int ca_erase(struct cds_array *arr, const size_t at_index);

struct cds_heap {
  struct cds_array data;
  int (*cmp)(const void *, const void *);
};

/*
 *********************************************************************************************************
 *
 *                                           CDS h NEW
 * 
 * Description: Creates a new binary min-h with the specified element size and comparison function.
 * 
 * Arguments: element_size   The size of each element in the h.
 *            cmp            Function to compare two elements.
 *
 * Returns: A newly created struct cds_heap instance.
 * 
 * Notes: The h is implemented as a 1-indexed arr (first element at index 1).
 *        The comparison function should return negative if first arg is less than second,
 *        zero if equal, and positive if greater.
 *********************************************************************************************************
 */
struct cds_heap ch_new(size_t element_size, int (*cmp)(const void *, const void *));

/*
 *********************************************************************************************************
 *
 *                                         CDS h DELETE
 * 
 * Description: Frees the memory allocated for a binary h.
 * 
 * Arguments: h   Pointer to the h to delete.
 *
 * Returns: void
 * 
 * Notes: Sets the comparison function to NULL after freeing the memory.
 *********************************************************************************************************
 */
void ch_delete(struct cds_heap *h);

/*
 *********************************************************************************************************
 *
 *                                          CDS h PUSH
 * 
 * Description: Adds an element to a binary h.
 * 
 * Arguments: h          Pointer to the h.
 *            new_element   Pointer to the element to add.
 *
 * Returns: 0 on success, -1 on failure.
 * 
 * Notes: The element is added at the end of the h and then moved up to maintain the h property.
 *********************************************************************************************************
 */
int ch_push(struct cds_heap *h, const void *new_element);

/*
 *********************************************************************************************************
 *
 *                                          CDS h POP
 * 
 * Description: Removes the top element from a binary h.
 * 
 * Arguments: h   Pointer to the h.
 *
 * Returns: 0 on success, -1 on failure.
 * 
 * Notes: The last element is moved to the top and then moved down to maintain the h property.
 *********************************************************************************************************
 */
int ch_pop(struct cds_heap *h);

/*
 *********************************************************************************************************
 *
 *                                          CDS h TOP
 * 
 * Description: Retrieves the top element from a binary h without removing it.
 * 
 * Arguments: h   Pointer to the h.
 *
 * Returns: Pointer to the top element, or NULL if the h is empty.
 * 
 * Notes: Does not modify the h.
 *********************************************************************************************************
 */
void* ch_top(const struct cds_heap *h);

/*
 *********************************************************************************************************
 *
 *                                         CDS h SIZE
 * 
 * Description: Returns the number of elements in a binary h.
 * 
 * Arguments: h   Pointer to the h.
 *
 * Returns: The number of elements in the h.
 * 
 * Notes: The size is adjusted to account for the 1-indexing of the h.
 *********************************************************************************************************
 */
size_t ch_size(const struct cds_heap *h);

/*
 *********************************************************************************************************
 *
 *                                        CDS h EMPTY
 * 
 * Description: Checks if a binary h is empty.
 * 
 * Arguments: h   Pointer to the h.
 *
 * Returns: true if the h is empty, false otherwise.
 * 
 * Notes: None.
 *********************************************************************************************************
 */
bool ch_empty(const struct cds_heap *h);

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


long long gcd(long long a, long long b) {
  long long tp = a % b;
  while(tp != 0) {
    a = b;
    b = tp;
    tp = a % b;
  }
  return b;
}

long long lcm(long long a, long long b) {
  return a / gcd(a, b) * b;
}


struct rational r_new(long long p, long long q) {
  struct rational new_rational = {p, q};
  return new_rational;
}

struct rational r_from(long long p) {
  struct rational new_rational = {p, 1};
  return new_rational;
}

void r_simplify(struct rational *num) {
  if(num->q == 0) num->q = 1;
  long long GCD = gcd(num->p, num->q);
  num->p /= GCD;
  num->q /= GCD;
  if(num->q < 0) {
    num->p = -num->p;
    num->q = -num->q;
  }
}

struct rational r_add(struct rational a, struct rational b) {
  struct rational ret = {a.p, a.q};
  long long LCM = lcm(ret.q, b.q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b.p *= LCM / b.q;
  ret.p += b.p;
  r_simplify(&ret);
  return ret;
}

struct rational r_sub(struct rational a, struct rational b) {
  struct rational ret = {a.p, a.q};
  long long LCM = lcm(ret.q, b.q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b.p *= LCM / b.q;
  ret.p -= b.p;
  r_simplify(&ret);
  return ret;
}

struct rational r_mul(struct rational a, struct rational b) {
  struct rational ret = {a.p * b.p, a.q * b.q};
  r_simplify(&ret);
  return ret;
}

struct rational r_div(struct rational a, struct rational b) {
  struct rational ret = {a.p * b.q, a.q * b.p};
  r_simplify(&ret);
  return ret;
}

int r_cmp(struct rational *a, struct rational *b) {
  if (a->p == b->p && a->q == b->q) {
    return 0;
  }
  if (a->p * b->q < a->q * b->p) {
    return -1;
  }
  return 1;
}

struct rational r_abs(struct rational a) {
  struct rational ret = { .p = abs(a.p), .q = abs(a.q) };
  return ret;
}


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


int si_cmp(const void *a, const void *b) {
  struct sy_info *ca = (struct sy_info*) a;
  struct sy_info *cb = (struct sy_info*) b;
  if (ca->t < cb->t) return -1;
  if (ca->t > cb->t) return 1;
  if (ca->owner < cb->owner) return -1;
  if (ca->owner > cb->owner) return 1;
  return 0;
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