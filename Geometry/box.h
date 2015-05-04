#ifndef __BOX_H__
#define __BOX_H__

#include "./point.h"
#include "./polygon.h"

template <typename T>
class box : public polygon<point<T, 2>>
{
public:
    //returns true if the intersection is nonempty
    //
    //MUST be ccw:
    //  0: xmin,ymin
    //  1: xmax,ymin
    //  2: xmax,ymax
    //  3: xmin,ymax
    //  4: xmin,ymin 
    box( T xmin, T xmax, T ymin, T ymax)
    {
        this->vals = std::vector<point<T, 2>>(5, point<T, 2>());
        this->vals[0] = point<T, 2>({xmin,ymin});
        this->vals[1] = point<T, 2>({xmax,ymin});
        this->vals[2] = point<T, 2>({xmax,ymax});
        this->vals[3] = point<T, 2>({xmin,ymax});
        this->vals[4] = point<T, 2>({xmin,ymin});
    };

    glm::vec4 color;
 
    box() : polygon<point<T,2>>()
    {};

    ~box()
    {};

    bool intersects(box b)
    {
        return interval_intersects(this->vals[0][0], this->vals[1][0], b[0][0], b[1][0]) &&
            interval_intersects(this->vals[0][1],this->vals[2][1], b[0][1], b[2][1]);
    };

 private:
    bool interval_intersects(T x1,T y1,T x2,T y2)
    {
        T intersection_min = std::max(x1,x2);

        T intersection_max = std::min(y1,y2);

        return intersection_min <= intersection_max;
    };
};


#endif
