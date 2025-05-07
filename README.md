# Solution

## Introduction
This repository contains a C implementation of a Bicycle Parking Tree data structure, supporting operations to park, move, clear, rearrange, fetch, and rebuild bicycles across slots arranged in a tree. It uses:
- Heavy-Light Decomposition for fast path queries
- Binary Indexed Tree (Fenwick) for dynamic edge-weight updates
- Dynamic arrays and binary heaps for slot and delayed‐fetch management
- Rational arithmetic for fractional parking positions

## Compilation & Usage
```bash
cd public/hw2-sol
make        # builds `answer` executable
./answer    # runs solution, reads input as specified in problem statement
```

## File Structure
```
public/hw2-sol
├── answer.c/answer.h      # main solution entry point
├── cds.h/cds.c            # container data structures (array, heap)
├── rational.h/rational.c  # rational arithmetic utilities
├── bicycle.h              # bicycle class and operations
└── Makefile               # makefile for building the solution
```

## Approach
1. Build the tree of parking slots and initialize heavy‐light decomposition.
2. Maintain for each slot a sorted dynamic list of bicycles (by position) with rational coordinates.
3. Use Fenwick tree across decomposition to answer travel‐time queries in $O(\log^2 n)$.
4. Handle slot operations:
   - **PARK**: binary search for nearest free or fractional midpoint.
   - **MOVE**: erase and re‐insert with distance query.
   - **CLEAR/REARRANGE**: flush bicycles into a min‐heap by ready‐time.
   - **FETCH**: pop all heap entries up to current time.
5. Update edge weights dynamically via Fenwick updates for **REBUILD**.

## Complexity
- Preprocessing (decomposition + BIT build): $O(n \log n)$
- Each operation: at most $O(\log^2 n + \log m)$, where $m$ is total delayed‐fetch events.

## Additional Practice

1. The capacity of a single slot can be very large. How do we handle this? (Hint: Consider two-leveled linked list)
2. Consider a case that when the bicycle is moving, it will not be moved to Shuiyuan. How do we handle this? (Hint: Consider a RB-tree)