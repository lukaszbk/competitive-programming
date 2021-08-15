#ifndef LIBRARY_GRAPH_MINIMUM_SPANNING_TREE_H_
#define LIBRARY_GRAPH_MINIMUM_SPANNING_TREE_H_

#include <algorithm>
#include <cassert>
#include <vector>

#include "library/data_structure/disjoint_sets.h"

namespace cpl {

namespace internal {

struct KruskalAlgorithmTag {};

// Finds the minimum spanning tree of the specified *undirected* graph using
// Kruskal's algorithm.
//
// Arguments:
//  - graph: The graph to find the minimum spanning tree for.
//           Note: Conceptually, Kruskal's algorithm works only for undirected
//           graph. However, this implementation works fine even if the given
//           graph is directed.
//  - hooks: Kruskal's algorithm hooks.
//
// Complexity:
//  - Time:  O(m log(m))
//  - Space: O(n-1)
template <class Graph, class Hooks>
std::vector<typename Graph::Edge> FindMinimumSpanningTree(Graph& graph,
                                                          Hooks& hooks,
                                                          KruskalAlgorithmTag) {
  std::vector<typename Graph::Edge> result;
  auto components = hooks.MakeDisjointSets(graph.vertex_count());
  hooks.SetUp(&graph, &components);
  hooks.SortEdges();
  for (const auto& edge : graph.edges()) {
    if (components.Find(edge.source) != components.Find(edge.target)) {
      result.push_back(edge);
      hooks.OnProcessMstEdge(edge);
      components.MergeSets(edge.source, edge.target);
    } else {
      hooks.OnProcessNonMstEdge(edge);
    }
  }
  hooks.TearDown();
  return result;
}

}  // namespace internal

// KruskalHooks represents hooks for customizing Kruskal's algorithm execution.
//
// This class uses CRTP (Curiously Recuring Template Pattern) to implement
// static polymorphism.
template <class Graph, class Implementation>
class KruskalHooksCrtp {
 protected:
  using Edge = typename Graph::Edge;

  DisjointSets MakeDisjointSets(int vertex_count) const {
    return DisjointSets(vertex_count);
  }

  // This hook is called right before the algorithm starts (after the setup).
  void OnSetUp() {}

  // This hooks is called at the beginning of the algorithm to sort graph edges.
  void SortEdges() { sort(graph_->edges().begin(), graph_->edges().end()); }

  // This hook is called every time a new MST edge is found.
  void OnProcessMstEdge(const Edge& edge) {}

  // This hook is called every time a non-MST edge is encountered.
  void OnProcessNonMstEdge(const Edge& edge) {}

  // This hook is called right after the algorithm finishes.
  void OnTearDown() {}

  Graph* graph_ = nullptr;
  DisjointSets* components_ = nullptr;

 private:
  using Tag = internal::KruskalAlgorithmTag;

  void SetUp(Graph* graph, DisjointSets* components) {
    assert(graph != nullptr);
    assert(components != nullptr);
    graph_ = graph;
    components_ = components;
    hooks()->OnSetUp();
  }

  void TearDown() {
    hooks()->OnTearDown();
    components_ = nullptr;
  }

  Implementation* hooks() { return static_cast<Implementation*>(this); }

  template <class G, class H>
  friend std::vector<typename G::Edge> FindMinimumSpanningTree(G&, H&);
  template <class G, class H>
  friend std::vector<typename G::Edge> internal::FindMinimumSpanningTree(
      G&, H&, internal::KruskalAlgorithmTag);
};

template <class G>
class KruskalHooks final : public KruskalHooksCrtp<G, KruskalHooks<G>> {};

namespace internal {

struct PrimAlgorithmTag {};

// Finds the minimum spanning tree of the specified *undirected* graph using
// Prim's algorithm.
//
// Arguments:
//  - graph: The *undirected* graph to find the minimum spanning tree for.
//  - hooks: Prim's algorithm hooks.
//
// Complexity:
//  - Time:  O(m log(n))
//  - Space: O(m)
template <class Graph, class Hooks>
std::vector<typename Graph::Edge> FindMinimumSpanningTree(Graph& graph,
                                                          Hooks& hooks,
                                                          PrimAlgorithmTag) {
  // TODO(lukaszbk)
  return {};
}

}  // namespace internal

// PrimHooks represents hooks for customizing Prim's algorithm execution.
template <class Graph>
class PrimHooks {
 public:
  // TODO(lukaszbk)

 private:
  using Tag = internal::PrimAlgorithmTag;

  template <class G, class H>
  friend std::vector<typename G::Edge> FindMinimumSpanningTree(G&, H&);
  template <class G, class H>
  friend std::vector<typename G::Edge> internal::FindMinimumSpanningTree(
      G&, H&, internal::PrimAlgorithmTag);
};

template <class Graph, class Hooks = KruskalHooks<Graph>>
std::vector<typename Graph::Edge> FindMinimumSpanningTree(Graph& graph,
                                                          Hooks& hooks) {
  return internal::FindMinimumSpanningTree(graph, hooks, typename Hooks::Tag());
}

template <class Graph, class Hooks = KruskalHooks<Graph>>
std::vector<typename Graph::Edge> FindMinimumSpanningTree(Graph& graph) {
  Hooks hooks;
  return FindMinimumSpanningTree(graph, hooks);
}

}  // namespace cpl

#endif  // LIBRARY_GRAPH_MINIMUM_SPANNING_TREE_H_