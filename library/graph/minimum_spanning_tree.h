#ifndef LIBRARY_GRAPH_MINIMUM_SPANNING_TREE_H_
#define LIBRARY_GRAPH_MINIMUM_SPANNING_TREE_H_

#include <algorithm>
#include <cassert>
#include <vector>

#include "library/data_structure/disjoint_sets.h"

namespace cpl {

struct KruskalAlgorithmTag {};
struct PrimAlgorithmTag {};

template <class Graph>
class KruskalHooks {
 public:
  using Edge = typename Graph::Edge;
  using ConnectedComponents = DisjointSets;

  // This hooks is called at the beginning of the algorithm to sort graph edges
  // by their weight.
  void SortEdges(Graph& graph) const {
    sort(graph.edges().begin(), graph.edges().end());
  }

  // This hook is called right before the algorithm starts iterating over
  // the sorted collection edges.
  void OnStartProcessingEdges(ConnectedComponents* components) {
    assert(components != nullptr);
    components_ = components;
  }

  // This hook is called every time a new MST edge is found.
  void OnProcessMstEdge(const Edge& edge) {}

  // This hook os called every time a non-MST edge is encountered.
  void OnProcessNonMstEdge(const Edge& edge) {}

  // This hook is called right after the algorithm finishes iterating over
  // the sorted collection edges.
  void OnFinishProcessingEdges() { components_ = nullptr; }

 protected:
  using Tag = KruskalAlgorithmTag;

  template <class G, class H>
  friend std::vector<typename G::Edge> FindMinimumSpanningTree(G&, H&);

  ConnectedComponents* components_ = nullptr;
};

template <class Graph>
class PrimHooks {
 public:
  // TODO(lukaszbk)

 private:
  using Tag = PrimAlgorithmTag;

  template <class G, class H>
  friend std::vector<typename G::Edge> FindMinimumSpanningTree(G&, H&);
};

namespace internal {

// Finds the minimum spanning tree of the specified graph using Kruskal's
// algorithm.
//
// Arguments:
//  - graph: The graph to find the minimum spanning tree for.
//  - hooks: Kruskal's algorithm hooks for customizing the procedure.
//
// Complexity:
//  - Time:  O(m log(m))
//  - Space: O(n-1)
template <class Graph, class Hooks>
std::vector<typename Graph::Edge> FindMinimumSpanningTree(Graph& graph,
                                                          Hooks& hooks,
                                                          KruskalAlgorithmTag) {
  std::vector<typename Graph::Edge> result;
  hooks.SortEdges(graph);
  typename Hooks::ConnectedComponents components(graph.vertex_count());
  hooks.OnStartProcessingEdges(&components);
  for (const auto& edge : graph.edges()) {
    if (components.Find(edge.source) != components.Find(edge.target)) {
      result.push_back(edge);
      hooks.OnProcessMstEdge(edge);
      components.MergeSets(edge.source, edge.target);
    } else {
      hooks.OnProcessNonMstEdge(edge);
    }
  }
  hooks.OnFinishProcessingEdges();
  return result;
}

// Finds the minimum spanning tree of the specified graph using Prim's
// algorithm.
//
// Arguments:
//  - graph: The graph to find the minimum spanning tree for.
//  - hooks: Prim's algorithm hooks for customizing the procedure.
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