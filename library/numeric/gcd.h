#ifndef LIBRARY_NUMERIC_GCD_H_
#define LIBRARY_NUMERIC_GCD_H_

#include <algorithm>

namespace cpl {

// Computes the greatest common divisor of two integers.
//
// Assumptions:
//  - If a >= 0 && b >= 0, then the result is positive.
//  - If a < 0 || b < 0, then the result can be negative.
//
// Complexity:
//  - Time:  O(log(min(a,b)))
//  - Space: O(1)
template <typename T>
T gcd(T a, T b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

}  // namespace cpl

#endif  // LIBRARY_NUMERIC_GCD_H_