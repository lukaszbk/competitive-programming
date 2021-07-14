#ifndef LIBRARY_GRAPH_GRAPH_H_
#define LIBRARY_GRAPH_GRAPH_H_

#include "library/graph/vertex_list.h"

namespace cpl {

// A Graph contains collections of *directed* edges and vertices.
//
// This class assumes that vertices are numbered 0, 1, 2, ..., n-1, where n is
// some positive integer.
template <class EdgeCollection, class VertexCollection = VertexList<>>
class Graph {
 public:
  using EdgeAttributes = typename EdgeCollection::EdgeAttributes;
  using Edge = typename EdgeCollection::Edge;

  using EdgeListView = typename EdgeCollection::EdgeListView;

  using EdgeHandle = typename EdgeCollection::EdgeHandle;

  using VertexAttributes = typename VertexCollection::VertexAttributes;
  using VertexListContainer = typename VertexCollection::VertexListContainer;

  // -- Methods for manipulating edges. ---------------------------------------

  // Adds a new edge to the graph.
  void AddEdge(int source, int target,
               const EdgeAttributes& attributes = EdgeAttributes()) {
    vertex_collection_.UpdateVertexCount(source);
    vertex_collection_.UpdateVertexCount(target);
    edge_collection_.AddEdge(source, target, attributes);
  }

  // Returns a reference to the collection of all edges in graph.
  EdgeListView& edges() { return edge_collection_.edges(); }

  // Returns an iterator to the specified edge.
  //
  // If specified edge is not in the graph, then edges().end() is returned.
  EdgeHandle edge(int source, int target) {
    return edge_collection_.edge(source, target);
  }

  // Returns the total number of edges in the graph.
  int edge_count() const { return edge_collection_.edge_count(); }

  // -- Methods for manipulating vertices. ------------------------------------

  // Adds a new vertex to the graph.
  void AddVertex(int vertex_id,
                 const VertexAttributes& attributes = VertexAttributes()) {
    vertex_collection_.AddVertex(vertex_id, attributes);
  }

  // Returns a reference to the collection of all vertices in the graph.
  VertexListContainer& vertices() { return vertex_collection_.vertices(); }

  // Returns a reference to the specified vertex in the graph.
  VertexAttributes& vertex(int vertex_id) {
    return vertex_collection_.vertex(vertex_id);
  }

  // Returns the total number of vertices in the graph.
  int vertex_count() const { return vertex_collection_.vertex_count(); }

 private:
  EdgeCollection edge_collection_;
  VertexCollection vertex_collection_;
};

}  // namespace cpl

#endif  // LIBRARY_GRAPH_GRAPH_H_