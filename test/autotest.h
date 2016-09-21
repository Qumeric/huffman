#ifndef AUTOTEST_H
#define AUTOTEST_H
#include <iostream>

enum Verdict { OK, FAIL };
enum State { running, waiting };

class Tester {
  private:
  size_t test_number;
  size_t succeed;

  protected:
  Verdict verdict;
  State state;

  public:
  Tester();
  void start_test();
  void end_test();
  void run_test(bool (*test)());
  void show_stats();
};
#endif
