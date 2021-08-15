#include "util/solution_test/solution_test.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <istream>
#include <ostream>
#include <regex>
#include <sstream>
#include <vector>

namespace sol {

void Solve(std::istream&, std::ostream&);

namespace {

testing::AssertionResult AssertLinesEqual(const char*, const char*, const char*,
                                          std::size_t line_idx,
                                          const std::string& expected_line,
                                          const std::string& actual_line) {
  if (expected_line == actual_line) {
    return testing::AssertionSuccess();
  }
  return testing::AssertionFailure()
         << "Wrong Answer. Outputs differ in line " << line_idx + 1 << "."
         << "\n  Actual: " << (actual_line.empty() ? "EOF" : actual_line)
         << "\nExpected: " << (expected_line.empty() ? "EOF" : expected_line);
}

std::vector<std::string> GetLines(std::istream& in) {
  std::vector<std::string> lines;
  for (std::string line; std::getline(in, line);) {
    lines.push_back(line);
  }
  return lines;
}

}  // namespace

std::ostream& operator<<(std::ostream& os, const TestFilenames& param) {
  os << "{" << param.in_filename << ", " << param.out_filename << "}";
  return os;
}

std::vector<TestFilenames> SolutionTest::GetTestFilenames() {
  std::vector<TestFilenames> test_files;
  for (auto& entry : std::filesystem::recursive_directory_iterator(".")) {
    if (entry.is_regular_file() &&
        regex_match(entry.path().string(),
                    std::regex(".*/testdata/in\\d+.txt"))) {
      test_files.push_back(TestFilenames{
          entry.path().string(),
          regex_replace(entry.path().string(), std::regex("/in"), "/out")});
    }
  }
  sort(test_files.begin(), test_files.end(),
       [](const TestFilenames& lhs, const TestFilenames& rhs) {
         return lhs.in_filename < rhs.in_filename;
       });
  return test_files;
}

std::string SolutionTest::GetTestName(const TestFilenames& param) {
  const std::string& filename = param.in_filename;
  return filename.substr(
      filename.find_last_of("/") + 1,
      filename.find(".txt") - filename.find_last_of("/") - 1);
}

void AssertOutputsEqual(std::istream& expected_out, std::istream& actual_out) {
  auto expected_answer = GetLines(expected_out);
  auto actual_answer = GetLines(actual_out);
  std::size_t line_idx = 0;
  while (line_idx < std::min(expected_answer.size(), actual_answer.size())) {
    ASSERT_PRED_FORMAT3(AssertLinesEqual, line_idx, expected_answer[line_idx],
                        actual_answer[line_idx]);
    ++line_idx;
  }
  if (line_idx < expected_answer.size()) {
    ASSERT_PRED_FORMAT3(AssertLinesEqual, line_idx, expected_answer[line_idx],
                        "");
  }
  if (line_idx < actual_answer.size()) {
    ASSERT_PRED_FORMAT3(AssertLinesEqual, line_idx, "",
                        actual_answer[line_idx]);
  }
}

TEST_P(SolutionTest, SampleInput) {
  std::fstream in(GetParam().in_filename, std::fstream::in);
  std::fstream expected_out(GetParam().out_filename, std::fstream::in);
  std::stringstream actual_out;
  Solve(in, actual_out);
  AssertOutputsEqual(expected_out, actual_out);
}

}  // namespace sol