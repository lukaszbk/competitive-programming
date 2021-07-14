#ifndef LIBRARY_GRAPH_EDGE_LIST_H_
#define LIBRARY_GRAPH_EDGE_LIST_H_

#include <memory>
#include <vector>

namespace cpl {

struct EmptyEdgeAttributes {};

// An EdgeList contains all *directed* edges of a graph in some order.
//
// This class is a convenient way to represent a graph if your algorithm
// processes all edges of the graph, but does not need to access edges that
// originate at a given vertex.
//
// Template Arguments:
//  - EdgeAttributes is a struct/class that encapsulates various edge
//    attributes, like length, weight, or capacity. The Edge class, which is
//    used for storage, inherits EdgeAttributes' public interface. By default,
//    EdgeAttributes is an empty struct.
//  - EdgeListContainerTmpl specifies the container type for storing
//    a collection of graph edges. By default, ContainerTmpl is std::vector.
//    If you need to remove edges from the graph frequently, use std::list
//    instead.
template <class _EdgeAttributes = EmptyEdgeAttributes,
          template <class T, class A = std::allocator<T>>
          class EdgeListContainerTmpl = std::vector>
class EdgeList {
 public:
  using EdgeAttributes = _EdgeAttributes;

  // Represents a *directed* edge between two vertices.
  struct Edge : public EdgeAttributes {
    Edge(int _source, int _target, const EdgeAttributes& attributes)
        : EdgeAttributes(attributes), source(_source), target(_target) {}

    int source;
    int target;
  };

  using EdgeListView = EdgeListContainerTmpl<Edge>;

  using EdgeHandle = typename EdgeListView::iterator;

  // Adds a new edge to the graph.
  void AddEdge(int source, int target,
               const EdgeAttributes& attributes = EdgeAttributes()) {
    edge_list_.emplace_back(source, target, attributes);
  }

  // Returns a reference to the collection of all edges in graph.
  EdgeListView& edges() { return edge_list_; }

  // Returns an iterator to the specified edge.
  //
  // If specified edge is not in the graph, then edges().end() is returned.
  EdgeHandle edge(int source, int target) {
    return find_if(edge_list_.begin(), edge_list_.end(),
                   [source, target](const Edge& edge) {
                     return edge.source == source && edge.target == target;
                   });
  }

  // Returns the total number of edges in the graph.
  int edge_count() const { return edge_list_.size(); }

 private:
  EdgeListContainerTmpl<Edge> edge_list_;
};

}  // namespace cpl

#endif  // LIBRARY_GRAPH_EDGE_LIST_H_