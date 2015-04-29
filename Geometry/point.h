//simple templated point class
#ifndef __POINT_H__
#define __POINT_H__

#include <array>
#include <iostream>
#include <initializer_list>
#include <string>


template <typename data_t, int dim> class point 
{
public:
    point()
    {};

    const int dimension() const
    {
        return dim;
    };

    point(const std::initializer_list<data_t> & il)
    {
        int j = 0;

        if(il.size() != dim) 
        {
            std::cerr << __PRETTY_FUNCTION__ << " initializer list, point dimension mismatch." << std::endl;
        }

        for(auto i : il)
        {
            vals[j++] = i;
        }
    };

    template <typename pdata_t, int pdim>
    point& operator= (const point<pdata_t, pdim> & p)
    {
        if(p.dimension() != dim) 
        {
            std::cerr << __PRETTY_FUNCTION__ << " point dimension mismatch." << std::endl;
        }
        
        if(! std::is_same<pdata_t, data_t>::value ) 
        {
            std::cerr << __PRETTY_FUNCTION__ << " point type mismatch." << std::endl;
        }

        int j = 0;
        for(auto i : p)
        {
            vals[j++] = i;
        }
    };

    data_t operator[] (int i)
    {
        if(i >= dim)
            std::cerr << __PRETTY_FUNCTION__ << " point subscript " << i << " too large." << std::endl;

        return vals[i];
    };


    template<typename pdata_t, int pdim>
    friend std::ostream& operator<< (std::ostream& stream, point<pdata_t, pdim> p)
    {
        stream << "(";
        int i = 0;
        for(i = 0; i < pdim-1; ++i)
            stream << p[i] << ",";

        stream << p[i] << ")";
        return stream;
    };

    
    typename std::array<data_t, dim>::iterator begin()
    {
        return vals.begin();
    };

    typename std::array<data_t, dim>::iterator end()
    {
        return vals.end();
    };

private:
    std::array<data_t, dim> vals;

};

#endif
