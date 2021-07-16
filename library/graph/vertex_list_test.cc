#include "library/graph/vertex_list.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <ostream>

#include "library/graph/edge_list.h"
#include "library/graph/graph.h"

using ::testing::ElementsAre;

namespace cpl {

struct V {
  int weight;
};

// Operator == is necessary for gtest matchers to work with type Vertex.
bool operator==(const V& lhs, const V& rhs) { return lhs.weight == rhs.weight; }

// Operator << is necessary for gtest to print verties nicely.
std::ostream& operator<<(std::ostream& os, const V& vertex) {
  os << "{.weight = " << vertex.weight << "}";
  return os;
}

TEST(VertexList, AddingAndListingVerticesWorksAsExpected) {
  Graph<EdgeList<>, VertexList<V>> graph;
  ASSERT_EQ(0, graph.vertex_count());
  graph.AddVertex(4, {.weight = 14});
  ASSERT_EQ(5, graph.vertex_count());
  ASSERT_THAT(graph.vertices(),
              ElementsAre(V{}, V{}, V{}, V{}, V{.weight = 14}));
  graph.AddVertex(1, {.weight = 11});
  ASSERT_EQ(5, graph.vertex_count());
  ASSERT_THAT(graph.vertices(),
              ElementsAre(V{}, V{.weight = 11}, V{}, V{}, V{.weight = 14}));
  graph.AddVertex(0, {.weight = 10});
  ASSERT_EQ(5, graph.vertex_count());
  ASSERT_THAT(graph.vertices(), ElementsAre(V{.weight = 10}, V{.weight = 11},
                                            V{}, V{}, V{.weight = 14}));
}

TEST(VertexList, AccessingVerticesWorksAsExpected) {
  Graph<EdgeList<>, VertexList<V>> graph;
  ASSERT_EQ(0, graph.vertex_count());
  graph.AddVertex(4, {.weight = 14});
  ASSERT_EQ(V{.weight = 14}, graph.vertex(4));
}

TEST(VertexList, UpdatingVertexCountWorksAsExpected) {
  VertexList<> vertex_list;
  ASSERT_EQ(0, vertex_list.vertex_count());
  vertex_list.UpdateVertexCount(2);
  ASSERT_EQ(3, vertex_list.vertex_count());
  vertex_list.UpdateVertexCount(1);
  ASSERT_EQ(3, vertex_list.vertex_count());
  vertex_list.UpdateVertexCount(5);
  ASSERT_EQ(6, vertex_list.vertex_count());
  ASSERT_EQ(0, vertex_list.vertices().size());
}

}  // namespace cpl