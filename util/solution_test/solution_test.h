#ifndef UTIL_SOLUTION_TEST_SOLUTION_TEST_H_
#define UTIL_SOLUTION_TEST_SOLUTION_TEST_H_

#include <gtest/gtest.h>

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace sol {

struct TestFilenames {
  std::string in_filename;
  std::string out_filename;
};

// Operator << is necessary for gtest to print this type nicely.
std::ostream& operator<<(std::ostream&, const TestFilenames&);

class SolutionTest : public testing::TestWithParam<TestFilenames> {
 public:
  static std::vector<TestFilenames> GetTestFilenames();
  static std::string GetTestName(const TestFilenames&);
};

void AssertOutputsEqual(std::istream& expected_out, std::istream& actual_out);

}  // namespace sol

#endif  // UTIL_SOLUTION_TEST_SOLUTION_TEST_H_
