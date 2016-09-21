#include "autotest.h"
#include <cstdio>
#include <iostream>

using namespace std;

Tester::Tester()
{
  succeed = 0;
  test_number = 1;
  state = waiting;
}

void Tester::start_test()
{
  if (state != waiting) {
    cerr << "Trying to start test while not ready" << endl;
    throw;
  }
  cout << "Run test #" << test_number << endl;
  state = running;
}

void Tester::end_test()
{
  if (state != running) {
    cerr << "Trying to end test while there are no test running" << endl;
    throw;
  }
  cout << "Test #" << test_number << " have finished with verdict " << (verdict == OK ? "OK" : "FAIL") << endl;
  if (verdict == OK) {
    ++succeed;
  }
  state = waiting;
  ++test_number;
}

void Tester::show_stats()
{
  cout << "Tests stats: " << succeed << '/' << test_number - 1 << endl;
  if (succeed == test_number - 1) {
    cout << "All OK!" << endl;
  }
}

void Tester::run_test(bool (*test)())
{
  start_test();
  verdict = (test() ? OK : FAIL);
  end_test();
}
