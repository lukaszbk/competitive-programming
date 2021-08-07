#include "library/data_structure/disjoint_sets.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using ::testing::ElementsAre;
using ::testing::InSequence;

namespace cpl {

template <class DisjointSets>
std::vector<int> sizes(DisjointSets& partition) {
  std::vector<int> result;
  for (int i = 0; i < partition.union_size(); ++i) {
    result.push_back(partition.size(i));
  }
  return result;
}

template <class DisjointSets>
std::vector<int> parents(const DisjointSets& partition) {
  std::vector<int> result;
  for (int i = 0; i < partition.union_size(); ++i) {
    result.push_back(partition.parent(i));
  }
  return result;
}

template <class DisjointSets>
std::vector<int> representatives(DisjointSets& partition) {
  std::vector<int> result;
  for (int i = 0; i < partition.union_size(); ++i) {
    result.push_back(partition.Find(i));
  }
  return result;
}

TEST(DisjointSets, EmptyInitialization) {
  DisjointSets partition;
  ASSERT_EQ(0, partition.union_size());
}

TEST(DisjointSets, IterativeInitialization) {
  DisjointSets partition;
  ASSERT_EQ(0, partition.union_size());

  ASSERT_EQ(0, partition.MakeSingleton());
  ASSERT_THAT(representatives(partition), ElementsAre(0));
  ASSERT_THAT(parents(partition), ElementsAre(0));
  ASSERT_THAT(sizes(partition), ElementsAre(1));

  ASSERT_EQ(1, partition.MakeSingleton());
  ASSERT_THAT(representatives(partition), ElementsAre(0, 1));
  ASSERT_THAT(parents(partition), ElementsAre(0, 1));
  ASSERT_THAT(sizes(partition), ElementsAre(1, 1));

  ASSERT_EQ(2, partition.MakeSingleton());
  ASSERT_THAT(representatives(partition), ElementsAre(0, 1, 2));
  ASSERT_THAT(parents(partition), ElementsAre(0, 1, 2));
  ASSERT_THAT(sizes(partition), ElementsAre(1, 1, 1));
}

TEST(DisjointSets, NoOptimizations) {
  DisjointSets partition(6, false, false);

  partition.MergeSets(3, 1);
  ASSERT_THAT(representatives(partition), ElementsAre(0, 1, 2, 1, 4, 5));
  ASSERT_THAT(parents(partition), ElementsAre(0, 1, 2, 1, 4, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(1, 2, 1, 2, 1, 1));

  partition.MergeSets(1, 0);
  ASSERT_THAT(representatives(partition), ElementsAre(0, 0, 2, 0, 4, 5));
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 2, 1, 4, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(3, 3, 1, 3, 1, 1));

  partition.MergeSets(4, 2);
  ASSERT_THAT(representatives(partition), ElementsAre(0, 0, 2, 0, 2, 5));
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 2, 1, 2, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(3, 3, 2, 3, 2, 1));

  partition.MergeSets(2, 0);
  ASSERT_THAT(representatives(partition), ElementsAre(0, 0, 0, 0, 0, 5));
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 0, 1, 2, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(5, 5, 5, 5, 5, 1));
}

TEST(DisjointSets, JustMergeByRankOptimization) {
  DisjointSets partition(6, true, false);

  partition.MergeSets(3, 1);
  ASSERT_THAT(representatives(partition), ElementsAre(0, 1, 2, 1, 4, 5));
  ASSERT_THAT(parents(partition), ElementsAre(0, 1, 2, 1, 4, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(1, 2, 1, 2, 1, 1));

  partition.MergeSets(1, 0);
  ASSERT_THAT(representatives(partition), ElementsAre(1, 1, 2, 1, 4, 5));
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 2, 1, 4, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(3, 3, 1, 3, 1, 1));

  partition.MergeSets(4, 2);
  ASSERT_THAT(representatives(partition), ElementsAre(1, 1, 2, 1, 2, 5));
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 2, 1, 2, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(3, 3, 2, 3, 2, 1));

  partition.MergeSets(2, 0);
  ASSERT_THAT(representatives(partition), ElementsAre(1, 1, 1, 1, 1, 5));
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 1, 1, 2, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(5, 5, 5, 5, 5, 1));
}

TEST(DisjointSets, JustPathCompressionOptimization) {
  DisjointSets partition(6, false, true);

  partition.MergeSets(3, 1);
  ASSERT_THAT(parents(partition), ElementsAre(0, 1, 2, 1, 4, 5));

  partition.MergeSets(1, 0);
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 2, 1, 4, 5));

  partition.MergeSets(4, 2);
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 2, 1, 2, 5));

  partition.MergeSets(2, 0);
  ASSERT_THAT(parents(partition), ElementsAre(0, 0, 0, 1, 2, 5));

  ASSERT_THAT(representatives(partition), ElementsAre(0, 0, 0, 0, 0, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(5, 5, 5, 5, 5, 1));
}

TEST(DisjointSets, BothOptimizations) {
  DisjointSets partition(6);

  partition.MergeSets(3, 1);
  ASSERT_THAT(parents(partition), ElementsAre(0, 1, 2, 1, 4, 5));

  partition.MergeSets(1, 0);
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 2, 1, 4, 5));

  partition.MergeSets(4, 2);
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 2, 1, 2, 5));

  partition.MergeSets(0, 2);
  ASSERT_THAT(parents(partition), ElementsAre(1, 1, 1, 1, 2, 5));

  ASSERT_THAT(representatives(partition), ElementsAre(1, 1, 1, 1, 1, 5));
  ASSERT_THAT(sizes(partition), ElementsAre(5, 5, 5, 5, 5, 1));
}

TEST(DisjointSets, HooksAreCalledAsExpected) {
  class Partition : public DisjointSetsCrtp<Partition> {
   public:
    Partition(int n = 0) : DisjointSetsCrtp(n) {}

    MOCK_METHOD(void, OnMakeSingleton, ());
    MOCK_METHOD(void, OnMergeSets, (int, int, bool));
    MOCK_METHOD(void, OnCompressPath, (int, int));
  };

  {
    InSequence s;

    Partition partition(3);

    EXPECT_CALL(partition, OnMakeSingleton()).Times(1);
    ASSERT_EQ(3, partition.MakeSingleton());

    EXPECT_CALL(partition, OnMergeSets(0, 1, false)).Times(1);
    ASSERT_EQ(1, partition.MergeSets(0, 1));

    EXPECT_CALL(partition, OnMergeSets(2, 3, false)).Times(1);
    ASSERT_EQ(3, partition.MergeSets(2, 3));

    EXPECT_CALL(partition, OnMakeSingleton()).Times(1);
    ASSERT_EQ(4, partition.MakeSingleton());

    EXPECT_CALL(partition, OnMergeSets(4, 3, true)).Times(1);
    ASSERT_EQ(3, partition.MergeSets(3, 4));

    EXPECT_CALL(partition, OnMergeSets(0, 2, true)).Times(1);
    ASSERT_EQ(3, partition.MergeSets(2, 0));

    EXPECT_CALL(partition, OnCompressPath(0, 3));
    ASSERT_EQ(3, partition.Find(0));
  }
}

}  // namespace cpl