#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include "jngen.h"
#include "GraphGen.h"
using namespace std;

enum Operation { PARK = 0, MOVE = 1, CLEAR = 2, REARRANGE = 3, FETCH = 4, REBUILD = 5 };

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 1);
  // Line1
	int n = atoi(argv[1]), m = atoi(argv[2]), q = atoi(argv[3]);
  cout << n << " " << m << " " << q << "\n";
	// Line2
	int total_cap = 0;
	vector<int> cap(n);
	for (int i = 0; i < n; ++i) {
		cap[i] = rnd.next(2, 15);
	}
	for (int i = 0; i < n; ++i) {
		total_cap += cap[i];
		cout << cap[i] << " \n"[i == n - 1];
	}
	// Line3
	vector<int> fetch_delay(m);
	for (int i = 0; i < m; ++i) {
		fetch_delay[i] = rnd.next(0, 1000000);
	}
	for (int i = 0; i < m; ++i) {
		cout << fetch_delay[i] << " \n"[i == m - 1];
	}
	// Tree
	vector<edge> edges = GraphGen::GenTree(n, 100000);
	for (auto e : edges) {
		cout << e.from << " " << e.to << " " << e.dis << "\n";
	}
	// Operations
	int64_t current_time = 0;
	map<int, int> used_student;
	vector<int> used_slots(n, 0);
	int bicycle_in_tree = 0, bicycle_in_chuiyuan = 0;
	for (int i = 0; i < q; ++i) {
		int park_w = bicycle_in_tree >= total_cap ? 0 : 10;
		int move_w = bicycle_in_tree == 0 || n == 1 ? 0 : 10;
		int clear_w = bicycle_in_tree > 0 ? 5 : 0;
		int rearrange_w = 0;
		int fetch_w = bicycle_in_chuiyuan == 0 ? 0 : 2;
		int rebuild_w = 0;
		Operation op;
		op = static_cast<Operation>(rnd.nextByDistribution({park_w, move_w, clear_w, rearrange_w, fetch_w, rebuild_w}));
		cout << op << flush;
		switch (op) {
			case PARK: {
				bool find_vac = false;
				for (int i = 0; i < n; ++i) {
					if (cap[i] > used_slots[i]) {
						find_vac = true;
					}
				}
				if (!find_vac) cerr << "no vacancy\n", exit(-1);
				int s = rnd.next(0, m - 1);
				assert(s != m);
				while (used_student.count(s) && used_student[s] != -1) s = rnd.next(0, m - 1);
				size_t x = rnd.next(0, n - 1);
				while (used_slots[x] == cap[x]) x = rnd.next(0, n - 1);
				size_t p = rnd.next(1, cap[x]);
				cout << " " << s << " " << x << " " << p << "\n";
				used_slots[x]++;
				used_student[s] = x;
				bicycle_in_tree++;
				break;
			}
			case MOVE: {
				int s = rnd.next(0, m);
				while (!used_student.count(s) || used_student[s] == -1) s = rnd.next(0, m);
				size_t x = used_student[s], y = rnd.next(0, n - 1);
				while (x == y || used_slots[y] == cap[y]) y = rnd.next(0, n - 1);
				size_t p = rnd.next(1, cap[y]);
				cout << " " << s << " " << x << " " << y << " " << p << "\n";
				used_slots[x]--;
				used_slots[y]++;
				used_student[s] = y;
				break;
			}
			case CLEAR: {
				size_t x = rnd.next(0, n - 1);
				while (used_slots[x] == 0) x = rnd.next(0, n - 1);
				int64_t t = rnd.next(current_time + 1, current_time + 100000000);
				current_time = t;
				for (auto [st, sl] : used_student) {
					if (sl == x) {
						used_student[st] = -1;
						used_slots[x]--;
						bicycle_in_chuiyuan++;
						bicycle_in_tree--;
					}
				}
				used_slots[x] = 0;
				cout << " " << x << " " << t << "\n";
				break;
			}
			case REARRANGE: {
				cerr << "subtask1 does not contain rearrange, since it is not useful\n";
				exit(-1);
				break;
			}
			case FETCH: {
				int64_t t = rnd.next(current_time + 1, current_time + 100000000);
				current_time = t;
				cout << " " << t << "\n";
				bicycle_in_chuiyuan--;
				break;
			}
			case REBUILD: {
				cerr << "subtask1 does not contain rebuild\n";
				exit(-1);
				break;
			}
			default: {
				fprintf(stderr, "invalid operation type");
				exit(-1);
			}
		}
	}
  return 0;
}
