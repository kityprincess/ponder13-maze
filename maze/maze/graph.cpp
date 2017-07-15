#include "graph.h"
#include <vector>
#include <queue>
using namespace std;

/******************************************************************************
 * GRAPH CONSTRUCTOR
 * Creates a new instance of Graph that contains in_size vertices
 ******************************************************************************/
Graph::Graph(int in_size)
   : m_size(in_size), m_adjList(NULL)
{
   assert(m_size > 0);
   m_adjList = new VertexSet[m_size];
   assert(isValidGraph(*this));
}

/******************************************************************************
* GRAPH COPY CONSTRUCTOR
* Creates a new instance of Graph that is a copy of an existing instance
******************************************************************************/
Graph::Graph(const Graph & in_source)
   : m_size(0), m_adjList(NULL)
{
   clone(in_source);
}

/******************************************************************************
* GRAPH DESTRUCTOR
* Destroys a graph
******************************************************************************/
Graph::~Graph()
{
   destroy();
}

/******************************************************************************
* GRAPH ADD
* Adds a new edge from in_from to in_to to the Graph
******************************************************************************/
void Graph::add(Vertex & in_from, Vertex & in_to)
{
   assert(in_from.index() >= 0 && in_from.index() < size());
   m_adjList[in_from.index()].insert(in_to);
}

/******************************************************************************
* GRAPH ADD
* Adds new edges from in_from to each vertex in in_to to the Graph
******************************************************************************/
void Graph::add(Vertex & in_from, VertexSet & in_to)
{
   for (VertexSetIterator it = in_to.begin(); it != in_to.end(); ++it)
      add(in_from, *it);
}

/******************************************************************************
* GRAPH IS EDGE
* Returns true if there is an edge from in_from to in_to in the Graph
******************************************************************************/
bool Graph::isEdge(const Vertex & in_from, Vertex & in_to) const
{
   assert(vertexIsInBounds(in_from));
   return m_adjList[in_from.index()].find(in_to) != 
      m_adjList[in_from.index()].end();

}

/******************************************************************************
* GRAPH FIND EDGES
* Returns the set of vertices that have an edge from in_from in the Graph
******************************************************************************/
VertexSet Graph::findEdges(const Vertex & in_from) const
{
   assert(vertexIsInBounds(in_from));
   return m_adjList[in_from.index()];
}

/******************************************************************************
* GRAPH ASSIGNMENT OPERATOR
* Sets this instance of Graph to be a copy of the in_source instance of Graph
******************************************************************************/
Graph & Graph::operator=(const Graph & in_source)
{
   destroy();
   clone(in_source);
   return *this;
}

/******************************************************************************
* GRAPH FIND PATH
* Finds the shortest path from the first Vertex in the Graph to the last
******************************************************************************/
vector<Vertex> Graph::findPath() const
{
   return findPath(Vertex(0), Vertex(size() - 1));
}

/******************************************************************************
* GRAPH FIND PATH
* Finds the shortest path from in_start to in_end in the Graph
* Uses the algorithm proposed by Bro. Helfrich in his text
******************************************************************************/
vector<Vertex> Graph::findPath(const Vertex & in_start, const Vertex & in_end) const
{
   assert(vertexIsInBounds(in_start));
   assert(vertexIsInBounds(in_end));

   int distance = 0;
   queue<Vertex> toVisit;
   toVisit.push(in_start);
   
   Vertex* predecessor = new Vertex[size()];
   int* distances = new int[size()];
   for (int i = 0; i < size(); ++i)
      distances[i] = -1;

   while (!toVisit.empty() && distances[in_end.index()] == -1)
   {
      Vertex v = toVisit.front();
      toVisit.pop();

      if (distances[v.index()] > distance)
         distance++;

      VertexSet s = findEdges(v);
      for (VertexSetIterator it = s.begin(); it != s.end(); ++it)
      {
         int index = (*it).index();

         if (distances[index] == -1)
         {
            distances[index] = distance + 1;
            predecessor[index] = v;
            toVisit.push(*it);
         }
      }
   }
   distance++;

   if (distances[in_end.index()] == -1)
      throw "ERROR: No path from source to destination.";

   vector<Vertex> path;
   path.push_back(in_end);

   for (int i = 1; i <= distance; ++i)
   {
      path.push_back(predecessor[path[i - 1].index()]);
   }

   delete[] predecessor;
   delete[] distances;
   return path;
}

/******************************************************************************
* GRAPH IS VALID GRAPH
* Checks to ensure that the structure of the Graph looks valid
******************************************************************************/
bool Graph::isValidGraph(const Graph & in_graph) const
{
   return in_graph.m_size > 0;
}

/******************************************************************************
* GRAPH VERTEX IS IN BOUNDS
* Returns true if the vertex fits within the bounds of the Graph
******************************************************************************/
inline bool Graph::vertexIsInBounds(const Vertex & in_vertex) const
{
   return in_vertex.index() >= 0 && in_vertex.index() < size();
}

/******************************************************************************
* GRAPH CLONE
* Sets this Graph to copy the structure of the in_source Graph
******************************************************************************/
void Graph::clone(const Graph & in_source)
{
   m_size = in_source.m_size;
   m_adjList = new VertexSet[m_size];
   for (int i = 0; i < m_size; ++i)
      m_adjList[i] = in_source.m_adjList[i];
   
   assert(isValidGraph(*this));
}

/******************************************************************************
* GRAPH DESTROY
* Destroys the internal structure of the Graph
******************************************************************************/
void Graph::destroy()
{
   delete[] m_adjList;
   m_adjList = NULL;
}
