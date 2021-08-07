#include "library/graph/minimum_spanning_tree.h"

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ostream>
#include <tuple>
#include <vector>

#include "library/graph/edge_list.h"
#include "library/graph/graph.h"

using ::testing::_;
using ::testing::InSequence;
using ::testing::UnorderedElementsAreArray;

namespace cpl {

class FindMstTest : public ::testing::Test {
 public:
  struct EdgeAttributes {
    int weight;

    bool operator<(const EdgeAttributes& rhs) const {
      return weight < rhs.weight;
    }
  };

  using TestGraph = Graph<EdgeList<EdgeAttributes>>;
  using Edge = TestGraph::Edge;

 protected:
  void SetUp() override {
    graph_.AddEdge(1, 2, {weight : 3});
    graph_.AddEdge(2, 5, {weight : 6});
    graph_.AddEdge(5, 1, {weight : 5});
    graph_.AddEdge(2, 3, {weight : 5});
    graph_.AddEdge(3, 4, {weight : 9});
    graph_.AddEdge(4, 6, {weight : 7});
    graph_.AddEdge(6, 5, {weight : 2});
    graph_.AddEdge(3, 6, {weight : 3});
    mst_ = {Edge{6, 5, {2}}, Edge{1, 2, {3}}, Edge{3, 6, {3}}, Edge{5, 1, {5}},
            Edge{4, 6, {7}}};
  }

  TestGraph graph_;
  std::vector<Edge> mst_;
};

// Operator == is necessary for gtest matchers to work with type Edge.
bool operator==(const FindMstTest::Edge& lhs, const FindMstTest::Edge& rhs) {
  return std::tie(lhs.source, lhs.target, lhs.weight) ==
         std::tie(rhs.source, rhs.target, rhs.weight);
}

// Operator << is necessary for gtest to print edges nicely.
std::ostream& operator<<(std::ostream& os, const FindMstTest::Edge& edge) {
  os << "(" << edge.source << ", " << edge.target << ", " << edge.weight << ")";
  return os;
}

TEST_F(FindMstTest, KruskalAlgorithmWorks) {
  ASSERT_THAT(FindMinimumSpanningTree(graph_), UnorderedElementsAreArray(mst_));
}

TEST_F(FindMstTest, KruskalHooksWork) {
  class TestKruskalHooks : public KruskalHooks<TestGraph> {
   public:
    void SortEdges(TestGraph& graph) const {
      stable_sort(graph.edges().begin(), graph.edges().end());
    }

    MOCK_METHOD(void, OnStartProcessingEdges, (ConnectedComponents*));
    MOCK_METHOD(void, OnProcessMstEdge, (const Edge&));
    MOCK_METHOD(void, OnProcessNonMstEdge, (const Edge&));
    MOCK_METHOD(void, OnFinishProcessingEdges, ());
  };

  {
    InSequence s;
    TestKruskalHooks hooks;
    EXPECT_CALL(hooks, OnStartProcessingEdges(_)).Times(1);
    EXPECT_CALL(hooks, OnProcessMstEdge(Edge{6, 5, {2}}));
    EXPECT_CALL(hooks, OnProcessMstEdge(Edge{1, 2, {3}}));
    EXPECT_CALL(hooks, OnProcessMstEdge(Edge{3, 6, {3}}));
    EXPECT_CALL(hooks, OnProcessMstEdge(Edge{5, 1, {5}}));
    EXPECT_CALL(hooks, OnProcessNonMstEdge(Edge{2, 3, {5}}));
    EXPECT_CALL(hooks, OnProcessNonMstEdge(Edge{2, 5, {6}}));
    EXPECT_CALL(hooks, OnProcessMstEdge(Edge{4, 6, {7}}));
    EXPECT_CALL(hooks, OnProcessNonMstEdge(Edge{3, 4, {9}}));
    EXPECT_CALL(hooks, OnFinishProcessingEdges()).Times(1);
    ASSERT_THAT(FindMinimumSpanningTree(graph_, hooks),
                UnorderedElementsAreArray(mst_));
  }
}

class CustomLessThanOperatorTest : public ::testing::Test {
 public:
  struct EdgeAttributes {
    int weight;
  };

  using TestGraph = Graph<EdgeList<EdgeAttributes>>;
  using Edge = TestGraph::Edge;
};

// Operator == is necessary for gtest matchers to work with type Edge.
bool operator==(const CustomLessThanOperatorTest::Edge& lhs,
                const CustomLessThanOperatorTest::Edge& rhs) {
  return std::tie(lhs.source, lhs.target, lhs.weight) ==
         std::tie(rhs.source, rhs.target, rhs.weight);
}

// Operator << is necessary for gtest to print edges nicely.
std::ostream& operator<<(std::ostream& os,
                         const CustomLessThanOperatorTest::Edge& edge) {
  os << "(" << edge.source << ", " << edge.target << ", " << edge.weight << ")";
  return os;
}

TEST_F(CustomLessThanOperatorTest, KruskalAlgorithmWorks) {
  TestGraph graph;
  graph.AddEdge(1, 2, {weight : 3});
  graph.AddEdge(2, 5, {weight : 6});
  graph.AddEdge(5, 1, {weight : 5});

  struct Hooks : public KruskalHooks<TestGraph> {
    void SortEdges(TestGraph& graph) const {
      stable_sort(graph.edges().begin(), graph.edges().end(),
                  [](const EdgeAttributes& lhs, const EdgeAttributes& rhs) {
                    return lhs.weight < rhs.weight;
                  });
    }
  };

  ASSERT_THAT(FindMinimumSpanningTree(graph, Hooks()),
              UnorderedElementsAreArray({Edge{1, 2, {3}}, Edge{5, 1, {5}}}));
}

}  // namespace cpl