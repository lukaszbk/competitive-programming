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
// 2. For any two maximum spanning tree components U and V that get merged by
//    Kruskal's algorithm, and for any tour belt candidate A, exactly one of
//    the following statements is true:
//      (1) A ⊆ U
//      (2) A ⊆ V
//      (3) U ∪ V ⊆ A

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
    graph.AddEdge(u - 1, v - 1, {.synergy = k});
  }
  return graph;
}

class KruskalHooks : public cpl::KruskalHooksCrtp<Graph, KruskalHooks> {
 public:
  void OnSetUp() {
    const int n = graph_->vertex_count();
    min_synergy_.resize(n, std::vector<int>(n, kMaxSynergy_));
    max_synergy_.resize(n, std::vector<int>(n, kMinSynergy_));
    for (const auto& edge : graph_->edges()) {
      const int u = edge.source, v = edge.target;
      min_synergy_[u][v] = min_synergy_[v][u] = edge.synergy;
      max_synergy_[u][v] = max_synergy_[v][u] = edge.synergy;
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
    int max_border_synergy = kMinSynergy_;
    int min_inside_synergy = kMaxSynergy_;
    for (int i = 0; i < graph_->vertex_count(); ++i) {
      min_synergy_[v][i] = std::min(min_synergy_[v][i], min_synergy_[u][i]);
      max_synergy_[v][i] = std::max(max_synergy_[v][i], max_synergy_[u][i]);
      if (components_->Find(i) == u || components_->Find(i) == v) {
        min_inside_synergy = std::min(min_inside_synergy, min_synergy_[v][i]);
      } else {
        max_border_synergy = std::max(max_border_synergy, max_synergy_[v][i]);
      }
    }
    if (max_border_synergy < min_inside_synergy) {
      answer += components_->size(u) + components_->size(v);
    }
  }

  int answer = 0;

 private:
  const int kMinSynergy_ = 0;
  const int kMaxSynergy_ = 1e5 + 1;

  std::vector<std::vector<int>> min_synergy_;
  std::vector<std::vector<int>> max_synergy_;
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