#pragma once
#include <stddef.h>
#include <stdbool.h>

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