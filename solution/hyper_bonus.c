#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rep(i, a, b) for (int i = a; i < b; i++)
#define rep1(i, a, b) for (int i = a; i <= b; i++)
#define int long long
#define MN 300005
#define LOGN 20
#define SQRTC 1005

// const bool debug_mode = true;
const bool debug_mode = false;

void debug(const char *fmt, ...) {
    if (!debug_mode) return;
    va_list args;
    va_start(args, fmt);
    printf("\033[0;33m");
    vprintf(fmt, args);
    printf("\033[0m");
    va_end(args);
}

/*
------------------------------
Priority queue structure and functions
------------------------------
*/

typedef struct Info {
    int s, t;  // student, available time
} Info;

typedef struct PriorityQueue {
    Info *data;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue *pq_new(int capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->capacity = capacity;
    pq->data = (Info *)malloc(capacity * sizeof(Info));
    return pq;
}

void swap(Info *a, Info *b) {
    Info t = *a;
    *a = *b;
    *b = t;
}

void pq_push(PriorityQueue *pq, Info info) {
    pq->size++;
    int i = pq->size - 1;
    pq->data[i] = info;
    while (i != 0 && pq->data[(i - 1) / 2].t > pq->data[i].t) {
        swap(&pq->data[i], &pq->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void heapify(PriorityQueue *pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < pq->size && pq->data[left].t < pq->data[smallest].t) smallest = left;
    if (right < pq->size && pq->data[right].t < pq->data[smallest].t) smallest = right;
    if (smallest != i) {
        swap(&pq->data[i], &pq->data[smallest]);
        heapify(pq, smallest);
    }
}

void pq_pop(PriorityQueue *pq) {
    if (pq->size == 1) {
        pq->size--;
        return;
    }
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heapify(pq, 0);
}

int pq_top_t(PriorityQueue *pq) {
    return pq->data[0].t;
}

int pq_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

PriorityQueue *pq;

/*
------------------------------
End of priority queue structure
------------------------------
*/

/*
------------------------------------
Fraction Data Structure and Methods
------------------------------------
*/

typedef struct Frac {
    int p, q;
} Frac;

static int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

Frac frac_new(int p, int q) {
    Frac f = {p, q};
    int r = gcd(f.p, f.q);
    f.p /= r, f.q /= r;
    if (f.q < 0) f.p = -f.p, f.q = -f.q;
    return f;
}

Frac frac_ave(Frac a, Frac b) {
    int p = a.p * b.q + b.p * a.q;
    int q = a.q * b.q * 2;
    return frac_new(p, q);
}

int frac_cmp(Frac a, Frac b) { return a.p * b.q - b.p * a.q; }
bool frac_is_int(Frac f) { return f.q == 1; }
void frac_print(Frac f) {
    if (f.q == 1) {
        printf("%lld", f.p);
    } else {
        printf("%lld/%lld", f.p, f.q);
    }
}

/*
------------------------------------
End of Fraction Structure and Methods
------------------------------------
*/

typedef struct Bike {
    int owner;
    Frac pos;
} Bike;

/*
------------------------------------
Slot Data Structure and Methods
------------------------------------
*/

typedef struct BikeNode {
    Bike bike;
    struct BikeNode *next;
    struct BikeNode *prev;
} BikeNode;

typedef struct HeadNode {
    struct HeadNode *next;
    struct HeadNode *prev;
    BikeNode *head;
    BikeNode *tail;
    int cap;
    int len;
    int int_num;
} HeadNode;

typedef struct Slot {
    int cap;
    int sqrt_cap;
    int int_num;
    HeadNode *head;
    HeadNode *tail;
} Slot;

BikeNode *bikenode_new(Bike bike) {
    BikeNode *node = (BikeNode *)malloc(sizeof(BikeNode));
    node->bike = bike;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

HeadNode *headnode_new(int cap) {
    HeadNode *node = (HeadNode *)malloc(sizeof(HeadNode));
    node->head = NULL;
    node->tail = NULL;
    node->cap = cap;
    node->len = 0;
    node->int_num = 0;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

Slot *slot_new() {
    Slot *slot = (Slot *)malloc(sizeof(Slot));
    slot->cap = 0;
    // slot->sqrt_cap = (int)sqrt((double)cap);
    slot->sqrt_cap = 500;
    slot->int_num = 0;
    slot->head = headnode_new(slot->sqrt_cap);
    slot->tail = slot->head;
    return slot;
}

// Insert a bike node to the list, if overflow, return the last node
// The inserted position is guaranteed to be unique
BikeNode *list_insert(HeadNode *h, BikeNode *b) {
    if (frac_is_int(b->bike.pos)) h->int_num++;
    h->len++;

    if (h->head == NULL) {
        h->head = b;
        h->tail = b;
        return NULL;
    }

    if (frac_cmp(h->head->bike.pos, b->bike.pos) > 0) {
        // Insert to the head
        b->next = h->head;
        h->head->prev = b;
        h->head = b;
    } else {
        // Find the position to insert
        BikeNode *cur = h->head;
        while (cur->next != NULL && frac_cmp(cur->next->bike.pos, b->bike.pos) < 0) cur = cur->next;
        // Insert after cur
        b->next = cur->next;
        b->prev = cur;
        if (cur->next != NULL)
            cur->next->prev = b;
        else
            h->tail = b;
        cur->next = b;
    }

    // Check if overflow
    if (h->len > h->cap) {
        BikeNode *last = h->tail;
        h->tail = h->tail->prev;
        h->tail->next = NULL;
        h->len--;
        if (frac_is_int(last->bike.pos)) h->int_num--;
        return last;
    } else {
        return NULL;
    }
}

// Check if the list has a bike at the position
BikeNode *list_has(HeadNode *h, Frac pos) {
    BikeNode *cur = h->head;
    while (cur != NULL) {
        if (frac_cmp(cur->bike.pos, pos) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

// Find the nearest smaller integer position before the bike node
// Given p, return the nearest integer position <= p
int find_before(int p, BikeNode *x, HeadNode *h) {
    bool found = false;

    while (x != NULL) {
        int res = frac_cmp(x->bike.pos, frac_new(p, 1));
        if (res == 0) {
            // The position is occupied, find the previous integer position
            p--;
            x = x->prev;
        } else if (res < 0) {
            // Found the nearest integer position
            found = true;
            break;
        } else {
            // Not yet found, keep searching
            x = x->prev;
        }
    }

    if (!found && h->prev != NULL && h->prev->tail->bike.pos.p == p) return -1;
    return p;
}

int find_after(int p, BikeNode *x, HeadNode *h) {
    bool found = false;

    while (x != NULL) {
        int res = frac_cmp(x->bike.pos, frac_new(p, 1));
        if (res == 0) {
            // The position is occupied, find the next integer position
            p++;
            x = x->next;
        } else if (res > 0) {
            // Found the nearest integer position
            found = true;
            break;
        } else {
            // Not yet found, keep searching
            x = x->next;
        }
    }

    if (!found && h->next != NULL && h->next->head->bike.pos.p == p) return -1;
    return p;
}

int find_nearest_smaller_int(int p, BikeNode *x, HeadNode *h, int cap) {
    p = find_before(p, x, h);
    if (p != -1) return p;

    // Not found, find the previous list that has integer positions
    h = h->prev;
    while (h != NULL && h->int_num == cap) h = h->prev;
    if (h == NULL) return -1;

    x = h->tail;
    p = x->bike.pos.p;
    p = find_before(p, x, h);
    if (p != -1) return p;

    // error
    perror("Error: find_nearest_smaller_int");
    exit(0);
}

int find_nearest_bigger_int(int p, BikeNode *x, HeadNode *h, int cap) {
    p = find_after(p, x, h);
    if (p != -1) return p;

    // Not found, find the next list that has integer positions
    h = h->next;
    while (h != NULL && h->int_num == cap) h = h->next;
    if (h == NULL) return -1;

    x = h->head;
    p = x->bike.pos.p;
    p = find_after(p, x, h);
    if (p != -1) return p;

    // error
    perror("Error: find_nearest_bigger_int");
    exit(0);
}

Frac slot_insert(Slot *slot, int p, int owner) {
    debug(">>> Insert\n");
    Bike final_bike = {owner, frac_new(p, 1)};

    debug("Hi 1\n");

    // Find which list to insert
    HeadNode *cur = slot->head;
    while (cur->next != NULL && frac_cmp(cur->next->head->bike.pos, final_bike.pos) <= 0) cur = cur->next;

    debug("Hi 2\n");
    BikeNode *exact = list_has(cur, final_bike.pos);
    // Check if the slot don't have any empty integer position
    if (slot->int_num == slot->cap) {
        debug("Hi 3\n");
        Frac another;
        if (p == 1) {
            // Special case: take the average of the cur and next position
            if (exact->next != NULL)
                another = exact->next->bike.pos;
            else
                another = cur->next->head->bike.pos;
        } else {
            // Normal case: take the average of the prev and cur position
            if (exact->prev != NULL)
                another = exact->prev->bike.pos;
            else
                another = cur->prev->tail->bike.pos;
        }
        final_bike.pos = frac_ave(final_bike.pos, another);
    } else {
        if (exact != NULL) {
            debug("Hi 4\n");
            // The position is occupied, find the nearest integer position
            int l = find_nearest_smaller_int(p - 1, exact, cur, slot->sqrt_cap);
            int r = find_nearest_bigger_int(p + 1, exact, cur, slot->sqrt_cap);

            // TODO: assert q == 1
            if (l == -1) l = slot->head->head->bike.pos.p - 1;
            if (r == -1) r = slot->tail->tail->bike.pos.p + 1;

            int pos = (p - l <= r - p) ? l : r;
            if (l == 0) pos = r;
            if (r == slot->cap + 1) pos = l;
            final_bike.pos = frac_new(pos, 1);

            debug("l = %lld, r = %lld, pos = %lld\n", l, r, pos);
        } else {
            debug("Hi 5\n");
            // The position is not occupied, insert directly
            // do nothing
        }
    }

    if (frac_is_int(final_bike.pos)) slot->int_num++;

    // insert to the list

    // Find which list to insert
    cur = slot->head;
    while (cur->next != NULL && frac_cmp(cur->next->head->bike.pos, final_bike.pos) <= 0) cur = cur->next;

    BikeNode *last = list_insert(cur, bikenode_new(final_bike));
    // If overflow, insert to the next list recursively
    while (last != NULL) {
        // If the next list is NULL, create a new list
        if (cur->next == NULL) {
            cur->next = headnode_new(slot->sqrt_cap);
            cur->next->prev = cur;
            slot->tail = cur->next;
        }
        cur = cur->next;

        last->next = last->prev = NULL;
        last = list_insert(cur, last);
    }

    return final_bike.pos;
}

void slot_erase(Slot *slot, Frac pos) {
    debug(">>> Erase\n");

    HeadNode *cur = slot->head;
    while (cur->next != NULL && frac_cmp(cur->next->head->bike.pos, pos) <= 0) cur = cur->next;

    BikeNode *x = list_has(cur, pos);
    if (x == NULL) {
        // error
        perror("Error: slot_erase");
        exit(0);
    }

    // Erase the bike node
    if (frac_is_int(x->bike.pos)) {
        slot->int_num--;
        cur->int_num--;
    }
    cur->len--;

    if (x->prev != NULL) x->prev->next = x->next;
    if (x->next != NULL) x->next->prev = x->prev;
    if (cur->head == x) cur->head = x->next;
    if (cur->tail == x) cur->tail = x->prev;

    free(x);

    // Check if the list is empty
    if (cur->len == 0) {
        if (cur->prev != NULL) cur->prev->next = cur->next;
        if (cur->next != NULL) cur->next->prev = cur->prev;
        if (slot->head == cur) slot->head = cur->next;
        if (slot->tail == cur) slot->tail = cur->prev;
        free(cur);
    }

    // Check if the slot is empty
    if (slot->head == NULL) {
        slot->head = headnode_new(slot->sqrt_cap);
        slot->tail = slot->head;
    }
}

void slot_print(Slot *slot) {
    printf("---------- Slot ----------\n");
    HeadNode *cur = slot->head;
    while (cur != NULL) {
        BikeNode *x = cur->head;
        while (x != NULL) {
            // printf("(");
            frac_print(x->bike.pos);
            printf(" ");
            // printf(", %lld) ", x->bike.owner);
            x = x->next;
        }
        cur = cur->next;
        printf("\n");
    }
    printf("--------------------------\n");
}

/*
------------------------------------
End of Slot Structure and Methods
------------------------------------
*/

/*
------------------------------
Vector structure and functions
------------------------------
*/

typedef struct pii {
    int x, y;
} pii;

typedef struct Vec {
    pii *arr;
    int size, capacity;
} Vec;

Vec *vec_new() {
    Vec *v = (Vec *)malloc(sizeof(Vec));
    v->size = 0;
    v->capacity = 1;
    v->arr = (pii *)malloc(sizeof(pii) * v->capacity);
    return v;
}

void vec_push_back(Vec *v, const pii data) {
    if (v->size == v->capacity) {
        v->capacity <<= 1;
        v->arr = (pii *)realloc(v->arr, sizeof(pii) * v->capacity);
    }
    v->arr[v->size++] = data;
}

pii vec_at(const Vec *v, const int index) {
    return v->arr[index];
}

/*
------------------------------
End of vector structure
------------------------------
*/

int par_num, stu_num, q;
int delay[MN];

Slot *slots[MN];
int prev_slot[MN];
Frac prev_pos[MN];

/*
------------------------------
Handle tree distance
------------------------------
*/

Vec *graph[MN];
int parent[MN][LOGN], depth[MN], dist[MN];

void dfs(int u, int p) {
    for (int i = 0; i < graph[u]->size; i++) {
        pii v = vec_at(graph[u], i);
        if (v.x != p) {
            parent[v.x][0] = u;
            dist[v.x] = dist[u] + v.y;
            depth[v.x] = depth[u] + 1;
            dfs(v.x, u);
        }
    }
}

void preprocess_lca() {
    dfs(0, -1);
    for (int j = 1; j < LOGN; j++) {
        for (int i = 0; i < par_num; i++) {
            if (parent[i][j - 1] != -1) {
                parent[i][j] = parent[parent[i][j - 1]][j - 1];
            }
        }
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) {
        int t = u;
        u = v;
        v = t;
    }
    for (int i = LOGN - 1; i >= 0; i--) {
        if (parent[u][i] != -1 && depth[parent[u][i]] >= depth[v]) {
            u = parent[u][i];
        }
    }
    if (u == v) return u;
    for (int i = LOGN - 1; i >= 0; i--) {
        if (parent[u][i] != parent[v][i]) {
            u = parent[u][i];
            v = parent[v][i];
        }
    }
    return parent[u][0];
}

int distance(int u, int v) {
    return dist[u] + dist[v] - 2 * dist[lca(u, v)];
}

/*
------------------------------
End of tree distance
------------------------------
*/

void park(int s, int x, int p) {
    debug(">>> Park\n");

    Slot *slot = slots[x];
    Frac pos = slot_insert(slot, p, s);

    prev_slot[s] = x;
    prev_pos[s] = pos;

    printf("%lld parked at (%lld, ", s, x);
    frac_print(pos);
    printf(").\n");
}

void move(int s, int y, int p) {
    debug(">>> Move\n");

    int x = prev_slot[s];
    if (x == y) {
        printf("%lld moved to %lld in 0 seconds.\n", s, y);
        return;
    }

    int t = distance(x, y);
    printf("%lld moved to %lld in %lld seconds.\n", s, y, t);

    slot_erase(slots[x], prev_pos[s]);
    Frac pos = slot_insert(slots[y], p, s);
    prev_slot[s] = y;
    prev_pos[s] = pos;
}

void clear(int x, int t) {
    Slot *slot = slots[x];

    HeadNode *cur = slot->head;
    while (cur != NULL) {
        BikeNode *x = cur->head;
        while (x != NULL) {
            pq_push(pq, (Info){x->bike.owner, t + delay[x->bike.owner]});
            BikeNode *tmp = x;
            x = x->next;
            free(tmp);
        }
        HeadNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }

    slot->head = headnode_new(slot->sqrt_cap);
    slot->tail = slot->head;
    slot->int_num = 0;
}

void rearrange(int x, int t) {
    Slot *slot = slots[x];
    int cnt = 0;
    HeadNode *cur = slot->head;
    while (cur != NULL) {
        HeadNode *next_block = cur->next;
        BikeNode *x = cur->head;
        while (x != NULL) {
            if (x->bike.pos.q != 1) {
                Bike b = x->bike;
                pq_push(pq, (Info){x->bike.owner, t + delay[x->bike.owner]});
                cnt++;
                x = x->next;
                slot_erase(slot, b.pos);
                continue;
            }
            x = x->next;
        }
        cur = next_block;
    }

    printf("Rearranged %lld bicycles in %lld.\n", cnt, x);
}

void fetch(int t) {
    int cnt = 0;
    while (!pq_empty(pq) && pq_top_t(pq) <= t) {
        pq_pop(pq);
        cnt++;
    }
    printf("At %lld, %lld bicycles was fetched.\n", t, cnt);
}

void rebuild(int x, int y, int d) {
}

signed main() {
    rep(i, 0, MN) slots[i] = slot_new();

    scanf("%lld%lld%lld", &par_num, &stu_num, &q);
    rep(i, 0, par_num) scanf("%lld", &slots[i]->cap);
    rep(i, 0, stu_num) scanf("%lld", &delay[i]);

    rep(i, 0, par_num) graph[i] = vec_new();
    rep(i, 0, par_num - 1) {
        int u, v, w;
        scanf("%lld%lld%lld", &u, &v, &w);
        vec_push_back(graph[u], (pii){v, w});
        vec_push_back(graph[v], (pii){u, w});
    }

    preprocess_lca();
    pq = pq_new(stu_num);

    while (q--) {
        int type;
        scanf("%lld", &type);
        if (type == 0) {
            int s, y, p;
            scanf("%lld%lld%lld", &s, &y, &p);
            park(s, y, p);
        } else if (type == 1) {
            int s, y, p;
            scanf("%lld%lld%lld", &s, &y, &p);
            move(s, y, p);
        } else if (type == 2) {
            int x, t;
            scanf("%lld%lld", &x, &t);
            clear(x, t);
        } else if (type == 3) {
            int x, t;
            scanf("%lld%lld", &x, &t);
            rearrange(x, t);
        } else if (type == 4) {
            int t;
            scanf("%lld", &t);
            fetch(t);
        } else if (type == 5) {
            int x, y, d;
            scanf("%lld%lld%lld", &x, &y, &d);
            rebuild(x, y, d);
        }
    }
}
