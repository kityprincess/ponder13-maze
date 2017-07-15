#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"
#include "set.h"

class Graph
{
public:
   Graph(int in_value) { }
   int size() const { return 0; }
   void add(const Vertex & in_left, const Vertex & in_right) { }
   void add(const Vertex & in_left, const Set<Vertex> & in_others) { }
   void clear() { }
   bool isEdge(const Vertex & in_from, const Vertex & in_to) const
   {
      return false;
   }
   Set<Vertex> findEdges(const Vertex & in_from) const
   {
      return Set<Vertex>();
   }
};
#endif
