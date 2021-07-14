#include "library/graph/edge_list.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <ostream>
#include <tuple>

#include "library/graph/graph.h"

using ::testing::ElementsAre;
using ::testing::Eq;

namespace cpl {

class EdgesWithoutAttributes : public ::testing::Test {
 protected:
  using TestGraph = Graph<EdgeList<>>;
  using Edge = TestGraph::Edge;

  friend bool operator==(const Edge&, const Edge&);
  friend std::ostream& operator<<(std::ostream&, const Edge&);
};

// Operator == is necessary for gtest matchers to work with type Edge.
bool operator==(const EdgesWithoutAttributes::Edge& lhs,
                const EdgesWithoutAttributes::Edge& rhs) {
  return std::tie(lhs.source, lhs.target) == std::tie(rhs.source, rhs.target);
}

// Operator << is necessary for gtest to print edges nicely.
std::ostream& operator<<(std::ostream& os,
                         const EdgesWithoutAttributes::Edge& edge) {
  os << "(" << edge.source << ", " << edge.target << ")";
  return os;
}

TEST_F(EdgesWithoutAttributes, AddingAndListingEdgesWorksAsExpected) {
  TestGraph graph;
  ASSERT_EQ(0, graph.edge_count());
  graph.AddEdge(0, 1);
  ASSERT_EQ(1, graph.edge_count());
  graph.AddEdge(1, 3);
  ASSERT_EQ(2, graph.edge_count());
  graph.AddEdge(3, 0);
  ASSERT_EQ(3, graph.edge_count());
  graph.AddEdge(1, 4);
  ASSERT_EQ(4, graph.edge_count());
  graph.AddEdge(3, 4);
  ASSERT_EQ(5, graph.edge_count());
  ASSERT_THAT(graph.edges(),
              ElementsAre(Edge{0, 1, {}}, Edge{1, 3, {}}, Edge{3, 0, {}},
                          Edge{1, 4, {}}, Edge{3, 4, {}}));
}

TEST_F(EdgesWithoutAttributes, AccessEdge_Found) {
  TestGraph graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(1, 3);
  graph.AddEdge(3, 0);
  ASSERT_THAT(*graph.edge(1, 3), Eq(Edge{1, 3, {}}));
}

TEST_F(EdgesWithoutAttributes, AccessEdge_NotFound) {
  TestGraph graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(1, 3);
  graph.AddEdge(3, 0);
  ASSERT_THAT(graph.edge(1, 4), Eq(graph.edges().end()));
}

class EdgesWithAttributes : public ::testing::Test {
 protected:
  struct EdgeAttributes {
    int length;
  };

  using TestGraph = Graph<EdgeList<EdgeAttributes>>;
  using Edge = TestGraph::Edge;

  friend bool operator==(const Edge&, const Edge&);
  friend std::ostream& operator<<(std::ostream&, const Edge&);
};

// Operator == is necessary for gtest matchers to work with type Edge.
bool operator==(const EdgesWithAttributes::Edge& lhs,
                const EdgesWithAttributes::Edge& rhs) {
  return std::tie(lhs.source, lhs.target, lhs.length) ==
         std::tie(rhs.source, rhs.target, rhs.length);
}

// Operator << is necessary for gtest to print edges nicely.
std::ostream& operator<<(std::ostream& os,
                         const EdgesWithAttributes::Edge& edge) {
  os << "(" << edge.source << ", " << edge.target << ", " << edge.length << ")";
  return os;
}

TEST_F(EdgesWithAttributes, AddingAndListingEdgesWorksAsExpected) {
  TestGraph graph;
  ASSERT_EQ(0, graph.edge_count());
  graph.AddEdge(0, 1, {11});
  ASSERT_EQ(1, graph.edge_count());
  graph.AddEdge(1, 3, {12});
  ASSERT_EQ(2, graph.edge_count());
  graph.AddEdge(3, 0, {13});
  ASSERT_EQ(3, graph.edge_count());
  graph.AddEdge(1, 4, {14});
  ASSERT_EQ(4, graph.edge_count());
  graph.AddEdge(3, 4, {15});
  ASSERT_EQ(5, graph.edge_count());
  ASSERT_THAT(graph.edges(),
              ElementsAre(Edge{0, 1, {11}}, Edge{1, 3, {12}}, Edge{3, 0, {13}},
                          Edge{1, 4, {14}}, Edge{3, 4, {15}}));
}

TEST_F(EdgesWithAttributes, AccessEdge_Found) {
  TestGraph graph;
  graph.AddEdge(0, 1, {11});
  graph.AddEdge(1, 3, {12});
  graph.AddEdge(3, 0, {13});
  ASSERT_THAT(*graph.edge(1, 3), Eq(Edge{1, 3, {12}}));
}

TEST_F(EdgesWithAttributes, AccessEdge_NotFound) {
  TestGraph graph;
  graph.AddEdge(0, 1, {11});
  graph.AddEdge(1, 3, {12});
  graph.AddEdge(3, 0, {13});
  ASSERT_THAT(graph.edge(1, 4), Eq(graph.edges().end()));
}

}  // namespace cpl