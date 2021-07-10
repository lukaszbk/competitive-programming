#ifndef LIBRARY_GRAPH_VERTEX_LIST_H_
#define LIBRARY_GRAPH_VERTEX_LIST_H_

#include <algorithm>
#include <memory>
#include <vector>

namespace cpl {
namespace graph {

struct EmptyVertexAttributes {};

// A VertexList contains vertices of a graph keyed by their ids.
//
// This class assumes that vertices are numbered 0, 1, 2, ..., n-1, where n is
// some positive integer.
//
// Template Arguments:
//  - VertexAttributes is a struct/class that encapsulates various vertex
//    attributes. This type is used for storage and by default is an empty
//    struct.
//  - VertexListContainerTmpl specifies the container type for storing
//    a collection of vertices. By default, ContainerTmpl is std::vector.
template <class _VertexAttributes = EmptyVertexAttributes,
          template <class T, class A = std::allocator<T>>
          class VertexListContainerTmpl = std::vector>
class VertexList {
 public:
  using VertexAttributes = _VertexAttributes;
  using VertexListContainer = VertexListContainerTmpl<VertexAttributes>;

  VertexList() : vertex_count_{0} {}

  // Adds a new vertex to the graph.
  void AddVertex(int vertex_id,
                 const VertexAttributes& attributes = VertexAttributes()) {
    UpdateVertexCount(vertex_id);
    vertices_.resize(vertex_count_);
    vertices_[vertex_id] = attributes;
  }

  // Returns a reference to the collection of all vertices in the graph.
  VertexListContainer& vertices() { return vertices_; }

  // Returns a reference to the specified vertex in the graph.
  //
  // No bounds checking is performed.
  VertexAttributes& vertex(int vertex_id) { return vertices_[vertex_id]; }

  // Returns the total number of vertices in the graph.
  int vertex_count() const { return vertex_count_; }

  // Updates the total number of vertices in the graph.
  //
  // This method should be called whenever a new vertex, or a new edge is added
  // to the graph. We assume that vertices are numbered 0, 1, 2, ..., n-1, where
  // n is some positive integer. Thus, when we see a given vertex_id, we know
  // that the total number of vertices in the graph is at least vertex_id + 1.
  void UpdateVertexCount(int vertex_id) {
    vertex_count_ = std::max(vertex_count_, vertex_id + 1);
  }

 private:
  VertexListContainer vertices_;
  int vertex_count_;
};

}  // namespace graph
}  // namespace cpl

#endif  // LIBRARY_GRAPH_VERTEX_LIST_H_