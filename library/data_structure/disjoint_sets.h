#ifndef LIBRARY_DATA_STRUCTURE_DISJOINT_SETS_H_
#define LIBRARY_DATA_STRUCTURE_DISJOINT_SETS_H_

#include <algorithm>
#include <vector>

namespace cpl {

// A DisjointSets represents a partition of {0, 1, 2, ..., n} set.
//
// Partition of a set is grouping of its elements into, non-empty,
// non-overlapping subsets, in such a way that every element is included in
// exactly one subset.
//
// This data structure uses a forest of trees to represent partition of a set.
// Each tree in the forest corresponds to a separate subset. The root of each
// tree is the subset representative returned by the Find() operation.
// The subset representative can change over time, namely when the MergeSets()
// operation is called.
//
// All operations supported by DisjointSets are fast thanks to the fact that the
// following optimizations are used:
// 1) Path Compression
//    When we call Find(a) for element a, we actally find the representative r
//    for all elements on the path between a and r. The trick is to make the
//    paths for all those elements shorter, by setting their parents to r.
// 2) Merge by Size
//    When we call MergeSets(a, b), we always attach the representative of the
//    smaller subset to the representative of the larger subset. This guarantees
//    that height of all trees in the forest is at most O(log n).
//
// If both optimizations are enabled, then the amortized cost of each operation
// is O(alpha(n)). alpha(n) is the inverse Ackermann function, which grows very
// slowly (it doesn't exceed 4 for all n < 10^600).
// If only one optimization is enabled, then the amortized cost of each
// operation is at most O(log n).
//
// This class uses CRTP (Curiously Recuring Template Pattern) to implement
// static polymorphism.
template <class Implementation>
class DisjointSetsCrtp {
 public:
  // Creates a new singleton subset in the partition.
  //
  // Return Value:
  //  Returns the element that belongs to the created singleton.
  //
  // Complexity:
  //  - Time:  Refer to the class description for details
  //  - Space: O(1)
  int MakeSingleton() {
    hooks()->OnMakeSingleton();
    parent_or_size_.push_back(-1);
    return union_size() - 1;
  }

  // Merges the two specified subsets: the subset in which element a is located,
  // and the subset in which the element b is located.
  //
  // Return Value:
  //  Returns the representative of the merged subset.
  //
  // Complexity:
  //  - Time:  Refer to the class description for details
  //  - Space: O(1)
  int MergeSets(int a, int b) {
    if (kEnableMergeBySize_ && size(a) > size(b)) {
      std::swap(a, b);
    }
    hooks()->OnMergeSets(a, b);
    a = Find(a);
    b = Find(b);
    parent_or_size_[b] += parent_or_size_[a];
    parent_or_size_[a] = b;
    return b;
  }

  // Finds and returns the representative of the subset that contains element a.
  //
  // Complexity:
  //  - Time:  Refer to the class description for details
  //  - Space: O(1)
  int Find(int element) {
    int root = element;
    while (parent_or_size_[root] >= 0) {
      root = parent_or_size_[root];
    }
    if (root == element || root == parent_or_size_[element] ||
        !kEnablePathCompression_) {
      return root;
    }
    hooks()->OnCompressPath(element, root);
    while (element != root) {
      const int parent = parent_or_size_[element];
      parent_or_size_[element] = root;
      element = parent;
    }
    return root;
  }

  // Returns the parent of the specified element in the forest.
  int parent(int element) const {
    if (parent_or_size_[element] >= 0) {
      return parent_or_size_[element];
    } else {
      return element;
    }
  }

  // Returns the size of the subset which the specified element is located.
  int size(int element) { return -parent_or_size_[Find(element)]; }

  // Returns the total number of elements in the union of the partition.
  int union_size() const { return parent_or_size_.size(); }

 protected:
  DisjointSetsCrtp(int n = 0, bool enable_merge_by_size = true,
                   bool enable_path_compression = true)
      : kEnableMergeBySize_(enable_merge_by_size),
        kEnablePathCompression_(enable_path_compression),
        parent_or_size_(n, -1) {}

  // This hooks is called right before a new singleton subset is created.
  void OnMakeSingleton() {}

  // This hook is called right before two specified subsets are merged.
  void OnMergeSets(int element, int parent) {}

  // This hook is called right before a path from element to root is compressed.
  void OnCompressPath(int element, int root) {}

  const bool kEnableMergeBySize_;
  const bool kEnablePathCompression_;

  // If parent_or_size_[a] >= 0, then parent_or_size_[a] represents the parent
  // of element a in the forest.
  // If parent_or_size_[a] < 0, then element a is the root of some tree in the
  // forest and -parent_or_size_[a] represents the size of that tree.
  std::vector<int> parent_or_size_;

 private:
  Implementation* hooks() { return static_cast<Implementation*>(this); }
};

class DisjointSets final : public DisjointSetsCrtp<DisjointSets> {
 public:
  DisjointSets(int n = 0, bool enable_merge_by_size = true,
               bool enable_path_compression = true)
      : DisjointSetsCrtp(n, enable_merge_by_size, enable_path_compression) {}
};

}  // namespace cpl

#endif  // LIBRARY_DATA_STRUCTURE_DISJOINT_SETS_H_