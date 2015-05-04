#ifndef __VERTEX_H__
#define __VERTEX_H__

template<typename T>
class vertex
{
public:
    vertex(T ix, T iy, int iid)
    {
        x = ix;
        y = iy;
        id = iid;
    }
    T x;
    T y;
    int id;
};

#endif
