#include <bits/stdc++.h>
using namespace std;

#define hyper                     \
    ios_base::sync_with_stdio(0); \
    cin.tie(0)
#define rep(i, a, b) for (int i = a; i < b; i++)
#define rep1(i, a, b) for (int i = a; i <= b; i++)
#define int long long
#define pii pair<int, int>
#define MN 300005
#define LOGN 20

class Fraction {
   public:
    long long p, q;  // p/q

    void reduce() {
        long long r = __gcd(p, q);
        p /= r, q /= r;
        if (q < 0) p = -p, q = -q;
    }

    Fraction(long long numerator = 0, long long denominator = 1) : p(numerator), q(denominator) {
        if (q == 0) {
            throw invalid_argument("Denominator cannot be zero.");
        }
        reduce();
    }

    Fraction operator+(const Fraction &other) const {
        long long num = p * other.q + other.p * q;
        long long den = q * other.q;
        return Fraction(num, den);
    }

    Fraction operator-(const Fraction &other) const {
        long long num = p * other.q - other.p * q;
        long long den = q * other.q;
        return Fraction(num, den);
    }

    Fraction operator*(const Fraction &other) const {
        long long num = p * other.p;
        long long den = q * other.q;
        return Fraction(num, den);
    }

    Fraction operator/(const Fraction &other) const {
        if (other.p == 0) {
            throw invalid_argument("Cannot divide by zero.");
        }
        long long num = p * other.q;
        long long den = q * other.p;
        return Fraction(num, den);
    }

    bool operator<(const Fraction &other) const {
        return p * other.q < other.p * q;
    }

    bool operator>(const Fraction &other) const {
        return p * other.q > other.p * q;
    }

    friend ostream &operator<<(ostream &os, const Fraction &frac) {
        os << frac.p;
        if (frac.q != 1) {
            os << "/" << frac.q;
        }
        return os;
    }
};

struct Bike {
    int s;
    Fraction pos;
    Bike(int s, Fraction pos) : s(s), pos(pos) {}
};

struct Slot {
    int cap;
    bool occupied[32];
    vector<Bike> bikes;
    void insert_bike(int s, Fraction pos) {
        int i = 0;
        while (i < bikes.size() && bikes[i].pos < pos) i++;
        bikes.insert(bikes.begin() + i, Bike(s, pos));
    }
};

int par_num, stu_num, q;
Slot slots[MN];
int delay[MN];
vector<pii> graph[MN];

int parent[MN][LOGN];
int depth[MN];
int dist[MN];

int prev_slot[MN];

#ifdef DEBUG
template <typename... Args>
void debug(Args... args) {
    cout << "\033[1;31m";
    (cout << ... << args);
    cout << "\033[0m" << endl;
}
#else
template <typename... Args>
void debug(Args... args) {}
#endif

void dfs(int u, int p) {
    for (auto &[v, w] : graph[u]) {
        if (v != p) {
            parent[v][0] = u;
            dist[v] = dist[u] + w;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

void preprocess_LCA() {
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
    if (depth[u] < depth[v]) swap(u, v);

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

int distance(int u, int v) { return dist[u] + dist[v] - 2 * dist[lca(u, v)]; }

void print_park(int s, int x, Fraction p) {
    cout << s << " parked at (" << x << ", " << p << ").\n";
}

void print_move(int s, int y, int t) {
    cout << s << " moved to " << y << " in " << t << " seconds.\n";
}

void print_rearrange(int x, int t) {
    cout << "Rearranged " << x << " bicycles in " << t << ".\n";
}

void print_fetch(int t, int n) {
    cout << "At " << t << ", " << n << " bicycles was fetched.\n";
}

Fraction park_bike(int s, int x, int p) {
    debug("slot ", x, " has ", slots[x].bikes.size(), " bikes");
    for (auto &bike : slots[x].bikes) {
        debug("bike ", bike.s, " at position ", bike.pos);
    }
    // print occupied debug
    debug("occupied: ");
    for (int i = 1; i <= slots[x].cap; i++) {
        debug(i, " = ", slots[x].occupied[i]);
    }

    Slot &slot = slots[x];
    prev_slot[s] = x;

    // has empty space at p
    if (!slot.occupied[p]) {
        slot.insert_bike(s, Fraction(p));
        slot.occupied[p] = true;
        debug("insert at empty space ", p);
        return Fraction(p);
    }

    // find nearest empty space
    int l = p, r = p;
    while (l >= 1 && slot.occupied[l]) l--;
    while (r <= slot.cap && slot.occupied[r]) r++;

    // no integer empty space
    if (l == 0 && r == slot.cap + 1) {
        // find the bike index at position p
        int idx = 0;
        while (idx < slot.bikes.size() && slot.bikes[idx].pos < Fraction(p)) idx++;
        debug("idx = ", idx);
        debug("p = ", p);
        Fraction m = (slot.bikes[idx - 1].pos + slot.bikes[idx].pos) / 2;
        debug("m = ", m);
        if (idx == 0) m = (slot.bikes[idx].pos + slot.bikes[idx + 1].pos) / 2;
        slot.insert_bike(s, m);
        debug("insert at middle ", m);
        return m;
    }

    // insert at the nearest empty space
    int pos = (p - l <= r - p) ? l : r;
    if (l == 0) pos = r;
    if (r == slot.cap + 1) pos = l;
    slot.insert_bike(s, Fraction(pos));
    slot.occupied[pos] = true;
    debug("insert at nearest empty space ", pos);
    return pos;
}

void park(int s, int x, int p) {
    debug("parking bike ", s, " at slot ", x, " at position ", p);

    Fraction pos = park_bike(s, x, p);
    print_park(s, x, pos);
}

void move(int s, int y, int p) {
    debug("moving bike ", s, " to slot ", y, " at position ", p);
    int x = prev_slot[s];
    if (x == y) {
        print_move(s, y, 0);
        return;
    }

    // remove from previous slot
    Slot &slot = slots[x];
    for (int i = 0; i < slot.bikes.size(); i++) {
        if (slot.bikes[i].s == s) {
            if (slot.bikes[i].pos.q == 1) {
                slot.occupied[slot.bikes[i].pos.p] = false;
            }
            slot.bikes.erase(slot.bikes.begin() + i);
            break;
        }
    }

    int t = distance(x, y);
    print_move(s, y, t);

    // insert to new slot
    park_bike(s, y, p);
}

void clear(int x, int t) {
}

void rearrange(int x, int t) {
}

void fetch(int t) {
}

void rebuild(int x, int y, int d) {
}

signed main() {
    // hyper;
    cin >> par_num >> stu_num >> q;
    rep(i, 0, par_num) cin >> slots[i].cap;
    rep(i, 0, stu_num) cin >> delay[i];

    rep(i, 0, par_num - 1) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    preprocess_LCA();

    while (q--) {
        int type;
        cin >> type;
        debug("type = ", type);
        if (type == 0) {
            int s, y, p;
            cin >> s >> y >> p;
            park(s, y, p);
        } else if (type == 1) {
            int s, y, p;
            cin >> s >> y >> p;
            move(s, y, p);
        } else if (type == 2) {
            int x, t;
            cin >> x >> t;
            clear(x, t);
        } else if (type == 3) {
            int x, t;
            cin >> x >> t;
            rearrange(x, t);
        } else if (type == 4) {
            int t;
            cin >> t;
            fetch(t);
        } else if (type == 5) {
            int x, y, d;
            cin >> x >> y >> d;
            rebuild(x, y, d);
        }
    }
}
