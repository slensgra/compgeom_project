#ifndef __VERTEX_H__
#define __VERTEX_H__

template<typename T>
class vertex
{
public:
    vertex() : id(-1), box_id(-1), parent_id(-1)
    {
    }

    vertex(T ix, T iy, int iid, int bid = -1)
    {
        x = ix;
        y = iy;
        id = iid;
        box_id = bid;
    }
    T x;
    T y;
    int id;
    int box_id;
    int parent_id;
};

#endif
