#ifndef __GRAPH_H_
#define __GRAPH_H__
#include "./vertex.h"
#include <list>
#include <queue>
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
        adj_list.emplace_back();
    };

    void set_neighbors(int id, std::list<std::pair<int,T> > neighbors)
    {
        adj_list.at(id) = neighbors;
    };

    //get_path applies a graph search to get a 
    //path between two vertices
    std::list<point<GLfloat,2>> path(int aid, int bid)
    {
        //BFS!!!
        //start at aid, bfs til we get to bid, store parents
        //along the way, then we can back track
        std::list<int> visited;
        std::queue<int> Q;
        visited.push_back(aid);
        Q.push(aid);

        while(!Q.empty())
        {
            auto c = Q.front();
            Q.pop();
            for(auto i : adj_list[c])
            {
                bool visit = false;

                for(auto j : visited)
                {
                    if(j == i.first)
                        visit = true;
                }
                if(!visit)
                {
                    Q.push(i.first);
                    vertices[i.first].parent_id = c;
                    visited.push_back(i.first);
                }
            }
        }

        std::list<point<GLfloat,2>> ret;
        //now back track and build the path.
        int current = bid;
        ret.push_back(point<GLfloat,2>({vertices[bid].x,vertices[bid].y}));

        while(current != aid)
        {
            ret.push_back(point<GLfloat,2>({vertices[current].x,vertices[current].y}));
            current = vertices[current].parent_id;
        }

        ret.push_back(point<GLfloat,2>({vertices[aid].x,vertices[aid].y}));


        for(auto i : ret)
        {
            std::cout << i << std::endl;
        }
        return ret;
    };

private:
    std::vector<std::list<std::pair<int, GLfloat>>> adj_list;
    std::vector<vertex<GLfloat>> vertices;
};

#endif
