#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;
using ll = long long;

// Custom GCD implementation
ll custom_gcd(ll a, ll b) {
    a = a < 0 ? -a : a;  // Take absolute value
    b = b < 0 ? -b : b;  // Take absolute value
    
    while (b) {
        ll temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Custom LCM implementation
ll custom_lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    return (a / custom_gcd(a, b)) * b;
}

// Custom absolute value implementation
ll custom_abs(ll a) {
    return a < 0 ? -a : a;
}

enum Operation {
    PARK = 0,
    MOVE = 1,
    CLEAR = 2,
    REARRANGE = 3,
    FETCH = 4,
    REBUILD = 5
};

// Rational number class using 64-bit integers
struct Rational {
    ll numerator;
    ll denominator;
    
    Rational(ll num = 0, ll den = 1) {
        if (den == 0) {
            throw runtime_error("Denominator cannot be zero");
        }
        
        // Normalize the fraction
        if (den < 0) {
            num = -num;
            den = -den;
        }
        
        ll g = custom_gcd(custom_abs(num), den);
        numerator = num / g;
        denominator = den / g;
    }
    
    // Create a rational from an integer
    static Rational fromInt(ll value) {
        return Rational(value, 1);
    }
    
    // Arithmetic operations
    Rational operator+(const Rational& other) const {
        ll lcm_val = custom_lcm(denominator, other.denominator);
        ll num = numerator * (lcm_val / denominator) + other.numerator * (lcm_val / other.denominator);
        return Rational(num, lcm_val);
    }
    
    Rational operator-(const Rational& other) const {
        ll lcm_val = custom_lcm(denominator, other.denominator);
        ll num = numerator * (lcm_val / denominator) - other.numerator * (lcm_val / other.denominator);
        return Rational(num, lcm_val);
    }
    
    Rational operator*(const Rational& other) const {
        return Rational(numerator * other.numerator, denominator * other.denominator);
    }
    
    Rational operator/(const Rational& other) const {
        if (other.numerator == 0) {
            throw runtime_error("Division by zero");
        }
        return Rational(numerator * other.denominator, denominator * other.numerator);
    }
    
    // Comparison operators
    bool operator<(const Rational& other) const {
        ll lcm_val = custom_lcm(denominator, other.denominator);
        return numerator * (lcm_val / denominator) < other.numerator * (lcm_val / other.denominator);
    }
    
    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }
    
    bool operator!=(const Rational& other) const {
        return !(*this == other);
    }
    
    bool operator>(const Rational& other) const {
        return other < *this;
    }
    
    bool operator<=(const Rational& other) const {
        return *this < other || *this == other;
    }
    
    bool operator>=(const Rational& other) const {
        return *this > other || *this == other;
    }
    
    // Check if the rational is an integer
    bool isInteger() const {
        return denominator == 1;
    }
    
    // Absolute value
    Rational abs() const {
        return Rational(custom_abs(numerator), denominator);
    }
    
    // Convert to string
    string toString() const {
        if (denominator == 1) {
            return to_string(numerator);
        } else {
            return to_string(numerator) + "/" + to_string(denominator);
        }
    }
};

// Structure to represent a bicycle
struct Bicycle {
    int studentId;
    Rational position;
    ll relocatedTime;
    
    Bicycle(int id, const Rational& pos) : studentId(id), position(pos), relocatedTime(0) {}
};

// Structure for a parking slot
struct ParkingSlot {
    int capacity;
    vector<Bicycle> bicycles;
    
    bool isPositionOccupied(const Rational& pos) {
        for (const auto& bike : bicycles) {
            if (bike.position == pos) {
                return true;
            }
        }
        return false;
    }
    
    Rational findNearestEmptySpace(const Rational& pos) {
        set<Rational> occupied;
        for (const auto& bike : bicycles) {
            occupied.insert(bike.position);
        }
        
        Rational minDiff = Rational::fromInt(capacity + 2);
        Rational nearestPos = Rational(-1);
        
        for (int i = 1; i <= capacity; i++) {
            Rational intPos = Rational::fromInt(i);
            if (occupied.find(intPos) == occupied.end()) {
                Rational diff = (pos - intPos).abs();
                if (diff < minDiff || (diff == minDiff && intPos < nearestPos)) {
                    minDiff = diff;
                    nearestPos = intPos;
                }
            }
        }
        
        return nearestPos;
    }
    
    Rational insertBicycle(int studentId, const Rational& pos) {
        // Check if position is already occupied
        if (!isPositionOccupied(pos)) {
            bicycles.push_back(Bicycle(studentId, pos));
            return pos;
        }
        
        // Try to find nearest empty integer position
        Rational nearestPos = findNearestEmptySpace(pos);
        if (nearestPos.numerator > 0) {
            bicycles.push_back(Bicycle(studentId, nearestPos));
            return nearestPos;
        }
        
        // Insert between two bicycles or at boundary
        vector<Rational> positions;
        for (const auto& bike : bicycles) {
            positions.push_back(bike.position);
        }
        sort(positions.begin(), positions.end());
        
        // Always consider boundary as occupied
        positions.insert(positions.begin(), Rational(0, 1));
        positions.push_back(Rational(capacity + 1, 1));
        
        // Find the position to insert
        for (size_t i = 0; i < positions.size() - 1; i++) {
            if (positions[i] < pos && pos < positions[i + 1]) {
                Rational insertPos = (positions[i] + positions[i + 1]) / Rational(2, 1);
                bicycles.push_back(Bicycle(studentId, insertPos));
                return insertPos;
            }
        }
        
        // Fallback (should not reach here with given constraints)
        return Rational(-1);
    }
    
    void removeBicycle(int studentId) {
        for (auto it = bicycles.begin(); it != bicycles.end(); ++it) {
            if (it->studentId == studentId) {
                bicycles.erase(it);
                return;
            }
        }
    }
};

class BicycleParkingSystem {
private:
    int n, m;  // n: number of parking slots, m: number of students
    vector<ParkingSlot> parkingSlots;
    vector<int> fetchTimes;  // Time for each student to fetch their bicycle
    vector<vector<pair<int, int>>> adjList;  // Adjacency list for the tree
    unordered_map<int, int> studentParkingSlot;  // Maps student ID to parking slot
    
    // Shuiyuan special vertex
    vector<Bicycle> shuiyuan;
    
    // Find shortest path between two vertices
    int findPathWeight(int from, int to) {
        if (from == to) return 0;
        
        vector<int> dist(n, INT_MAX);
        vector<bool> visited(n, false);
        dist[from] = 0;
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, from});
        
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            
            if (u == to) return dist[u];
            if (visited[u]) continue;
            visited[u] = true;
            
            for (const auto& edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;
                
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        
        return dist[to];
    }
    
    // Update edge weight (for rebuild operation)
    void updateEdgeWeight(int u, int v, int newWeight) {
        for (auto& edge : adjList[u]) {
            if (edge.first == v) {
                edge.second = newWeight;
                break;
            }
        }
        
        for (auto& edge : adjList[v]) {
            if (edge.first == u) {
                edge.second = newWeight;
                break;
            }
        }
    }

public:
    BicycleParkingSystem(int numSlots, int numStudents) 
        : n(numSlots), m(numStudents), parkingSlots(numSlots), adjList(numSlots), fetchTimes(numStudents) {}
    
    void setCapacity(int slotId, int capacity) {
        parkingSlots[slotId].capacity = capacity;
    }
    
    void setFetchTime(int studentId, int time) {
        fetchTimes[studentId - 1] = time;  // Adjust for 0-based indexing
    }
    
    void addEdge(int u, int v, int weight) {
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
    }
    
    string parkBicycle(int studentId, int slotId, int position) {
        Rational posRational = Rational::fromInt(position);
        Rational finalPos = parkingSlots[slotId].insertBicycle(studentId, posRational);
        studentParkingSlot[studentId] = slotId;
        return to_string(studentId) + " parked at (" + to_string(slotId) + ", " + finalPos.toString() + ").";
    }
    
    string moveBicycle(int studentId, int newSlotId, int position) {
        int currentSlotId = studentParkingSlot[studentId];
        
        if (currentSlotId == newSlotId) {
            return to_string(studentId) + " moved to " + to_string(newSlotId) + " in 0 seconds.";
        }
        
        int travelTime = findPathWeight(currentSlotId, newSlotId);
        
        // Remove bicycle from current slot
        parkingSlots[currentSlotId].removeBicycle(studentId);
        
        // Park bicycle in new slot
        Rational posRational = Rational::fromInt(position);
        Rational finalPos = parkingSlots[newSlotId].insertBicycle(studentId, posRational);
        studentParkingSlot[studentId] = newSlotId;
        
        return to_string(studentId) + " moved to " + to_string(newSlotId) + " in " + to_string(travelTime) + " seconds.";
    }
    
    void clearSlot(int slotId, ll currentTime) {
        for (auto& bike : parkingSlots[slotId].bicycles) {
            bike.relocatedTime = currentTime;
            shuiyuan.push_back(bike);
            studentParkingSlot.erase(bike.studentId);
        }
        parkingSlots[slotId].bicycles.clear();
    }
    
    string rearrangeSlot(int slotId, ll currentTime) {
        int count = 0;
        vector<Bicycle> remaining;
        
        for (auto& bike : parkingSlots[slotId].bicycles) {
            // If position is not an integer, relocate to Shuiyuan
            if (!bike.position.isInteger()) {
                bike.relocatedTime = currentTime;
                shuiyuan.push_back(bike);
                studentParkingSlot.erase(bike.studentId);
                count++;
            } else {
                remaining.push_back(bike);
            }
        }
        
        parkingSlots[slotId].bicycles = remaining;
        return "Rearranged " + to_string(count) + " bicycles in " + to_string(slotId) + ".";
    }
    
    string fetchBicycles(ll currentTime) {
        int count = 0;
        vector<Bicycle> stillInShuiyuan;
        
        for (auto& bike : shuiyuan) {
            int studentId = bike.studentId;
            ll notificationTime = bike.relocatedTime + fetchTimes[studentId - 1];
            
            if (notificationTime < currentTime) {
                count++;
                // Bicycle is fetched, student can park it later
                // (we don't need to track this for the problem)
            } else {
                stillInShuiyuan.push_back(bike);
            }
        }
        
        shuiyuan = stillInShuiyuan;
        return "At " + to_string(currentTime) + ", " + to_string(count) + " bicycles was fetched.";
    }
    
    void rebuildRoad(int u, int v, int newWeight) {
        updateEdgeWeight(u, v, newWeight);
    }
};

int main() {
    int n, m, q;
    cin >> n >> m >> q;
    
    BicycleParkingSystem system(n, m);
    
    // Read capacities
    for (int i = 0; i < n; i++) {
        int capacity;
        cin >> capacity;
        system.setCapacity(i, capacity);
    }
    
    // Read fetch times
    for (int i = 1; i <= m; i++) {
        int fetchTime;
        cin >> fetchTime;
        system.setFetchTime(i, fetchTime);
    }
    
    // Read tree edges
    for (int i = 0; i < n - 1; i++) {
        int u, v, d;
        cin >> u >> v >> d;
        system.addEdge(u, v, d);
    }
    
    // Process operations
    for (int i = 0; i < q; i++) {
        int opType;
        cin >> opType;
        
        switch (opType) {
            case PARK: {
                int s, x, p;
                cin >> s >> x >> p;
                cout << system.parkBicycle(s, x, p) << endl;
                break;
            }
            case MOVE: {
                int s, y, p;
                cin >> s >> y >> p;
                cout << system.moveBicycle(s, y, p) << endl;
                break;
            }
            case CLEAR: {
                int x;
                ll t;
                cin >> x >> t;
                system.clearSlot(x, t);
                break;
            }
            case REARRANGE: {
                int x;
                ll t;
                cin >> x >> t;
                cout << system.rearrangeSlot(x, t) << endl;
                break;
            }
            case FETCH: {
                ll t;
                cin >> t;
                cout << system.fetchBicycles(t) << endl;
                break;
            }
            case REBUILD: {
                int x, y, d;
                cin >> x >> y >> d;
                system.rebuildRoad(x, y, d);
                break;
            }
        }
    }
    
    return 0;
}