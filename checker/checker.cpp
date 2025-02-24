#include "testlib.h"
using namespace std;

int main(int argc, char* argv[]) {
	registerChecker("bicycletree", argc, argv);
	compareRemainingLines();
}
