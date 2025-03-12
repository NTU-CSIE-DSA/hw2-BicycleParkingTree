## Problem Description

We provide two versions of the problem statement below: one with a story and one in a formal
representation. Feel free to read either or both, depending on your preference.

### Story Version

There are numerous bikes in NTU, and the parking lots are scarce. Thus, students end up pushing other students' bikes into tight places, and the area gets crowded. Not only are the students unable to easily locate the bikes when leaving, but the bikes are also at greater risks of theft. In a bid to fully understand the problem, the NTU's General Affairs Office has decided to model students' parking trends.

To simplify the analysis, each parking slot is considered a vertex, and these slots together form a tree structure. Each slot can accommodate one or more bicycles. Students are allowed to park their bicycles at any slot and move them between slots as needed. However, to maintain order and cleanliness, the university periodically clears and rearranges the parking areas. Bicycles that violate regulations will be relocated to Shuiyuan. If students wish to retrieve their bicycles, they must go to Shuiyuan and pay an additional fee.

There are $5 + 1$ types of operations:

1. Park $s, x, p$: Student $s$ park a bicycle at a parking slot $x$ with relative position $p$. The parking policy is as follows.
    - If the parking slot has empty space at $p$, the student park zir bicycle at $p$.
    - If there's another vacancy in the same parking slot. Ze will select the nearest empty space in $x$. If there's two nearest vacancy, ze will choose the one with smaller corrdinate.
    - Otherwise, ze will insert zir bicycle at the middle of two bicycles that at right of $p$ and $p$. For example, if the parking slot with capacity $3$ have `1 2 3` occupied, and one wants to park at 3, ze will park at `5/2`. However, if ze want to park at 1, ze will park at `1/2` instead. That is, we always consider the border of the parking slot to be a occupied space.
2. Move $s, y, p$: Student $s$ moves zir bicycle from the parking slot $x$ ze parked to another parking slot $y$ with relative position $p$. You have to calculate traval time from $x$ to $y$. The parking policy is the same as the first operation. If $x = y$, let the traval time be $0$ and **do not** move the bicycle.
3. Clear $x, t$: At time $t$, clear a parking slot $x$ by relocate all bicycles to Shuiyuan. Those owners $O$ of the relocated bicycle will be notified after $l_i, i \in O$
4. Rearrange $x, t$: At time $t$, relocate all rule violating bicycles from the parking slot $x$ to Shuiyuan. Those owners $O$ of the relocated bicycle will be notified after $l_i, i \in O$
5. Fetch $t$: At time $t$, there's a school bus from the NTU to Shuiyuan. Students who are informed to fetch their bicycles will all go to Shuiyuan by this bus together. Those bicycle may be parked later. (This behavior is actually different from the reality. Thus, please read the operation description carefully.)
6. Bonus: rebuild $x, y, d$: Sometimes, the Dwellers and Student Affairs want to adjust the road so that the students' traval time between $x$ and $y$ becomes $d$, it is garenteed that $x$ and $y$ is connected before this type of operation.

## Formal Version

Given a weighted tree. There is a coordinate system in each vertex. The coordinate system is limited to $[1, c_x]$ You should support the following operations. We denote the undirected edge weight between $x$ and $y$ to be $w(x, y)$.

There are $5 + 1$ types of operations:

1. $s, x, p$: Insert $s$ in vertex $x$ at $p$. The insertion policy is as follows.
    - If $p$ is not occupied, insert $s$ at $p$.
    - If there exists an integer $p'$ such that $p'$ is not occupied in $x$. Insert $s$ at the nearest empty space in $x$.
    - Otherwise, let $p'$ be the maximum element less than $p$ in $x$. For example, if $x$ with capacity $3$ have `1 2 3` occupied, and there is an insertion at 3, insert $s$ at `5/2`. However, if inserting at 1, you will insert $s$ at `1/2` instead. That is, we always consider the border of the coordinate system to be an occupied space.
2. $s, y, p$: Move an element $s$ from $x$ (last inserted vertex) to $y$ at $p$ and print the total weight $\displaystyle\sum_{e\in \text{path(x, y)}}{w(x, y)}$. The insertion policy is the same as the first operation. If $x = y$, the path weight should be $0$, and **do not** move the element.
3. Clear $x, t$: At time $t$, clear $x$ by relocate all elements to a special vertex $S$ with infinite capacity. An element $i$ can be removed by fetch operation after timepoint $t + l_i$.
4. Rearrange $x, t$: At time $t$, relocate all elements with non-integer $p$ from $x$ to $S$. An element $i$ can be removed by fetch operation after timepoint $t + l_i$.
5. Fetch $t$: At time $t$, some $s$'s satisified certain condition will fetch elements from $S$. The condition is that $t_b + l_i < t, \forall i \in S$, where $t_b$ is the time when $i$ be relocated to $S$. Those elements may be inserted into tree later.
6. Bonus: $x, y, d$: Adjust the $w(x, y)$ becomes $d$, it is garenteed that $x$ and $y$ is connected before this type of operation.

## Input

- The first line contains three integers $n$, $m$, and $q$ representing the number of parking slots, the number of students, and the number of operations, respectively.
- The second line contains $n$ integers representing the capacity for each parking slot $c_x$. The id of the parking slot $x \in [0, n-1]$
- The third line contains $m$ integers representing the fetch time for each student. That is, the $i$-th student will fetch the bicycle at least $l_i$ minutes after zir bicycle being relocated.
- The next $n-1$ lines contain three integers $u$, $v$, and $d$ representing the parking slot $u$ and $v$ are connected with travel time $w$.
- The next $q$ lines contain the operations.

For each operations:

1. Park: `0 s x p`
2. Move: `1 s y p`
3. Clear: `2 x t`
4. Rearrange: `3 x t`
5. Fetch: `4 t`
6. Rebuild: `5 x y d`

Note that the unit of $t$ is always second(s).

## Output

For each operation:

1. Park: output the relative position of the bicycle in the parking slot.
    ```
    [s] parked at ([x], [fp]).
    ```
    where $fp$ is the final location of the bicycle/element. The rational number $\frac{p}{q}$ is represented as `p/q`. If the denomitor $q$ is $1$, simply output $p$.
2. Move: output the relative position and the travel time/total weight $t$ of the bicycle in the parking slot $y$.
    ```
    [s] moved to [y] in [t] seconds.
    ```
3. Clear: nothing
4. Rearrange: output the number of bicycles/elements $n$ that are relocated.
    ```
    Rearranged [n] bicycles in [x].
    ```
5. Fetch: output the number of bicycles/elements that are fetched.
    ```
    At [t], [n] bicycles was fetched.
    ``` 
6. Rebuild: nothing

## Constraint

- $1 \leq n, m \leq 3 \times 10^5$
- $1 \leq q \leq 10^5$
- $1 \leq s \leq m$
- $1 \leq x, y \leq n$
- $0 \leq d \leq 10^{5}$
- $0 \leq t \leq 10^{15}$
- $1 \leq p \leq c_x$
- $\forall i \in [0, m), 0 \leq l_i \leq 10^6$
- $\forall x \in [0, n), 2 \leq c_x \leq 15$
- At any time, the number of bicycles/elements in a single (parking slot/vertex) $x$ will not exceed $2 \times c_x$.
- The time $t$ in all operations are given in accending order.

For each operation:

1. Park: It is guaranteed that the bicycle of student $s$ is not in any of the parking slots.
2. Move: It is guaranteed that the bicycle of student $s$ is in a parking slot.
3. Clear: It is guaranteed that the parking slot $x$ is not empty.
4. Rearrange: It is guaranteed that the parking slot $x$ is not empty.
5. Fetch: It is guaranteed that the fetch time is in ascending order.
6. Rebuild: It is guaranteed that the parking slot $x$ and $y$ are connected.

## Subtask

### Subtask 1 (10pt)

- $1 \leq n, m \leq 3 \times 10^2$
- $1 \leq q \leq 3 \times 10^2$
- Only operations 1 and 2 are present.

Expected solution, simple simulation without Shuiyuan.

### Subtask 2 (20pt)

- $1 \leq n, m \leq 3 \times 10^2$
- $1 \leq q \leq 10^3$
- Only operations 1, 2, 3, 4, and 5 are present.

Expected solution, harder simulation.

### Subtask 3 (20pt)

- $1 \leq n \leq 3 \times 10^2$
- Only operations 1, 2, 3, 4, and 5 are present.

Expected solution, use min heap for operation 5. 

### Subtask 4 (50pt)

- Only operations 1, 2, 3, 4, and 5 are present.

Expected solution, use prefix sum on the tree for operation 2.

### Subtask 5 (Bonus: free snacks | beverages)

- All operations are present.

Expected solution, use heavy-light decomposition on the tree for operation 2.

### Subtask 6 (Bonus: planning)

- $\forall x \in [0, n), 2 \leq c_x \leq 10 ^ 6$
- Only operations 1, 2, 3, 4, and 5 are valid.
<!-- - Invalid operations present. If you found an invalid solution, you should ignore it. -->

## Sample Testcases

### Sample Input 1

```
1 5 3
3
0 0 0 0 0
0 0 0 2
0 1 0 2
0 2 0 2

```

### Sample Output 1

```
0 parked at (0, 2).
1 parked at (0, 1).
2 parked at (0, 3).

```

### Sample Input 2

```
1 5 6
3
0 0 0 0 0
0 0 0 1
0 1 0 2
0 2 0 3
0 3 0 3
3 0 1
0 3 0 3

```

### Sample Output 2

```
0 parked at (0, 1).
1 parked at (0, 2).
2 parked at (0, 3).
3 parked at (0, 5/2).
Rearranged 1 bicycles in 0.
3 parked at (0, 5/2).

```

### Sample Input 3

```
1 5 6
3
3 4 5 6 7
0 0 0 1
0 1 0 1
0 2 0 1
0 3 0 1
2 0 1
4 6

```

### Sample Output 3

```
0 parked at (0, 1).
1 parked at (0, 2).
2 parked at (0, 3).
3 parked at (0, 3/2).
At 6, 3 bicycles was fetched.

```

### Sample Input 4

```
6 5 6
3 3 3 4 4 4
3 4 5 6 7
0 2 1
0 5 4
1 5 7
2 3 2
4 5 3
0 0 0 1
1 0 1 1
1 0 4 1
1 0 2 1
1 0 3 1
1 0 2 1

```

### Sample Output 4

```
0 parked at (0, 1).
0 moved to 1 in 11 seconds.
0 moved to 4 in 10 seconds.
0 moved to 2 in 8 seconds.
0 moved to 3 in 2 seconds.
0 moved to 2 in 2 seconds.

```

<!-- ### Sample Input 5

```

```

### Sample Output 5

```

``` -->

## Hints

- You may want to use enum to represent the operations.
    ```c
    enum Operation {
      PARK = 0,
      MOVE = 1,
      CLEAR = 2,
      REARRANGE = 3,
      FETCH = 4,
      REBUILD = 5
    };
    ```
- If you want to calculate the weight sum of the path, you can try prefix sum on tree.