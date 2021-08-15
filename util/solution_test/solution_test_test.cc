#include "util/solution_test/solution_test.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <istream>
#include <tuple>

using ::testing::Eq;
using ::testing::UnorderedElementsAre;

namespace sol {

void Solve(std::istream& in, std::ostream& out) {}

// Operator == is necessary for gtest matchers to work with this type.
bool operator==(const TestFilenames& lhs, const TestFilenames& rhs) {
  return std::make_tuple(lhs.in_filename, lhs.out_filename) ==
         std::make_tuple(rhs.in_filename, rhs.out_filename);
}

TEST(SolutionTest, GetTestFilenames) {
  ASSERT_THAT(SolutionTest::GetTestFilenames(),
              UnorderedElementsAre(
                  TestFilenames{"./util/solution_test/testdata/in00.txt",
                                "./util/solution_test/testdata/out00.txt"},
                  TestFilenames{"./util/solution_test/testdata/in01.txt",
                                "./util/solution_test/testdata/out01.txt"}));
}

TEST(SolutionTest, GetTestName) {
  TestFilenames test_filenames;

  test_filenames.in_filename = "./problems/contest/1234/testdata/in0.txt";
  ASSERT_THAT(SolutionTest::GetTestName(test_filenames), Eq("in0"));

  test_filenames.in_filename = "./problems/contest/1234/testdata/in00.txt";
  ASSERT_THAT(SolutionTest::GetTestName(test_filenames), Eq("in00"));

  test_filenames.in_filename = "./problems/contest/1234/testdata/in01.txt";
  ASSERT_THAT(SolutionTest::GetTestName(test_filenames), Eq("in01"));

  test_filenames.in_filename = "./problems/contest/1234/testdata/in123.txt";
  ASSERT_THAT(SolutionTest::GetTestName(test_filenames), Eq("in123"));
}

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(SolutionTest);

}  // namespace sol