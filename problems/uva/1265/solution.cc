#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <vector>

#include "library/graph/edge_list.h"
#include "library/graph/graph.h"
#include "library/graph/minimum_spanning_tree.h"

// Observations:
// 1. For any two tour belt candidates A and B, exactly one of the following
//    statements is true:
//      (1) A ⊆ B
//      (2) B ⊆ A
//      (3) A ∩ B = ∅
// 2. For any two maximum spanning tree components U and V that get merged while
//    Kruskal's algorithm is running, and for any tour belt candidate A, exactly
//    one of the following statements is true:
//      (1) A ⊆ U
//      (2) A ⊆ V
//      (2) U ∪ V ⊆ A

namespace sol {

struct EdgeAttributes {
  int synergy;
};

using Graph = cpl::Graph<cpl::EdgeList<EdgeAttributes>>;

Graph ReadGraph(std::istream& in) {
  Graph graph;
  int n, m;
  in >> n >> m;
  while (m--) {
    int u, v, k;
    in >> u >> v >> k;
    graph.AddEdge(u, v, {.synergy = k});
  }
  return graph;
}

class KruskalHooks : public cpl::KruskalHooksCrtp<Graph, KruskalHooks> {
 public:
  void OnSetUp() {
    const int n = graph_->vertex_count();
    border_synergy_.resize(n, std::vector<int>(n, -1));
    inside_synergy_.resize(n, std::vector<int>(n, -1));
    for (const auto& edge : graph_->edges()) {
      UpdateBorderSynergy(edge.source, edge.target, edge.synergy);
      UpdateBorderSynergy(edge.target, edge.source, edge.synergy);
    }
  }

  void SortEdges() {
    sort(graph_->edges().begin(), graph_->edges().end(),
         [](const Graph::Edge& lhs, const Graph::Edge& rhs) {
           return lhs.synergy > rhs.synergy;
         });
  }

  void OnProcessMstEdge(const Graph::Edge& edge) {
    int u = components_->Find(edge.source);
    int v = components_->Find(edge.target);
    if (components_->size(u) > components_->size(v)) {
      std::swap(u, v);
    }
    // Vertex v will be the representative of the newly merged component.
    UpdateInsideSynergy(v, u, edge.synergy);
    for (int i = 0; i < graph_->vertex_count(); ++i) {
      if (i == v) continue;
      if (BorderEdgeExists(u, i)) {
        if (components_->Find(i) == v) {
          UpdateInsideSynergy(v, i, border_synergy_[u][i]);
        } else {
          UpdateBorderSynergy(v, i, border_synergy_[u][i]);
        }
      } else if (InsideEdgeExists(u, i)) {
        UpdateInsideSynergy(v, i, inside_synergy_[u][i]);
      }
    }
    if (MinBorderSynergy(v) < MaxInsideSynergy(v)) {
      answer += components_->size(u) + components_->size(v);
    }
  }

  int answer = 0;

 private:
  bool BorderEdgeExists(int u, int v) const {
    return border_synergy_[u][v] >= 0;
  }

  void UpdateBorderSynergy(int u, int v, int synergy) {
    if (!BorderEdgeExists(u, v) || border_synergy_[u][v] > synergy) {
      border_synergy_[u][v] = synergy;
    }
  }

  bool InsideEdgeExists(int u, int v) const {
    return inside_synergy_[u][v] >= 0;
  }

  void UpdateInsideSynergy(int u, int v, int synergy) {
    if (!InsideEdgeExists(u, v) || inside_synergy_[u][v] < synergy) {
      inside_synergy_[u][v] = synergy;
    }
    border_synergy_[u][v] = -1;
  }

  int MinBorderSynergy(int u) const {
    int result = -1;
    for (int i = 0; i < graph_->vertex_count(); ++i) {
      result = std::max(result, border_synergy_[u][i]);
    }
    return result;
  }

  int MaxInsideSynergy(int u) const {
    int result = 1e5;
    for (int i = 0; i < graph_->vertex_count(); ++i) {
      if (!InsideEdgeExists(u, i)) continue;
      result = std::min(result, inside_synergy_[u][i]);
    }
    return result;
  }

  std::vector<std::vector<int>> border_synergy_;
  std::vector<std::vector<int>> inside_synergy_;
};

void ProcessTestCase(std::istream& in, std::ostream& out) {
  Graph graph = ReadGraph(in);
  KruskalHooks hooks;
  FindMinimumSpanningTree(graph, hooks);
  out << hooks.answer << std::endl;
}

void Solve(std::istream& in, std::ostream& out) {
  int T;
  in >> T;
  while (T--) {
    ProcessTestCase(in, out);
  }
}

}  // namespace sol