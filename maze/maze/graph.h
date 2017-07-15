/***********************************************************************
* Component:
*    Week 13, Graph
* Author:
*    Matthew Burr
* Summary:
*    Defines a Graph class (a collection of vertices with edges between
*    them)
************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"
#include "set.h"
#include <cassert>
#include <vector>

typedef Set<Vertex> VertexSet;
typedef SetIterator<Vertex> VertexSetIterator;
typedef VertexSet* AdjList;

class Graph
{
public:
   Graph(int in_size);
   Graph(const Graph & in_source);
   ~Graph();
   int size() const { return m_size; }
   void add(Vertex & in_from, Vertex & in_to);
   void add(Vertex & in_from, VertexSet & in_to);
   void clear() { }
   bool isEdge(const Vertex & in_from, Vertex & in_to) const;
   VertexSet findEdges(const Vertex & in_from) const; 
   Graph & operator = (const Graph & in_source);
   std::vector<Vertex> findPath() const;
   std::vector<Vertex> findPath(const Vertex & in_start, const Vertex & in_end) const;

private:
   bool isValidGraph(const Graph & in_graph) const;
   bool vertexIsInBounds(const Vertex & in_vertex) const;
   void clone(const Graph & in_source);
   void destroy();

   int m_size;
   AdjList m_adjList;
};
#endif
