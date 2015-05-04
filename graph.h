#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "./vertex.h"
#include <list>
#include "./Geometry/point.h"
#include <vector>
#include <utility>

template<typename T>
class graph
{
public:
    int id;
    //edges have weights, need a pair first: id, second:cost
    void add_vertex(T x, T y, std::list<std::pair<int,T> > neighbors)
    {
        vertices.emplace_back(x, y,id++);
        adj_list.emplace_back(neighbors);
    };
   
    graph()
    {
        id = 0;
        vertices = std::vector<vertex<T>>();
        adj_list = std::vector<std::list<std::pair<int,T>>>();
    }

    void add_vertex(T x, T y)
    {
        vertices.emplace_back(x, y, id++);
    };

    void set_neighbors(int id, std::list<std::pair<int,T> > neighbors)
    {
        adj_list[id] = neighbors;
    };

    //get_path applies a graph search to get a 
    //path between two vertices
    std::list<point<T,2>> get_path(int s, int e);

private:
        std::vector<vertex<T>> vertices;
        std::vector<std::list<std::pair<int,T> >> adj_list;
};

#endif
