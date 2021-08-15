#include "util/solution_test/solution_test.h"

#include <gtest/gtest.h>

namespace sol {

INSTANTIATE_TEST_SUITE_P(
    Suite, SolutionTest, testing::ValuesIn(SolutionTest::GetTestFilenames()),
    [](const testing::TestParamInfo<SolutionTest::ParamType>& info) {
      return SolutionTest::GetTestName(info.param);
    });

}  // namespace sol