#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "./point.h"
#include <initializer_list>
#include <array>

template<typename ptype> class polygon
{
public:
    polygon(const std::initializer_list<ptype> & il) : vals(il)
    {};

    polygon()
    {};

    ~polygon()
    {};

    ptype operator[] (int i)
    {
        return vals[i];
    };

    typename std::vector<ptype>::iterator begin()
    {
        return vals.begin();
    };

    typename std::vector<ptype>::iterator end()
    {
        return vals.end();
    };

    int size()
    {
        return vals.size();
    };

    template<typename optype>
    friend std::ostream& operator << (std::ostream& stream, polygon<optype> p)
    {
        stream << "poly:{ ";
        int i = 0;
        for(i = 0; i < p.size()-1; ++i)
            stream << p[i] << "; ";
        stream << p[i] << " }";
        return stream;
    };

protected:
    std::vector<ptype> vals;
};


#endif
