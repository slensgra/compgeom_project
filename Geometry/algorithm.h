#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

//union for a list of polygons!!!!!
/*polygon<point<long long int, 2> >
union_all(std::vector<polygon<point<long long int, 2>>> in_list)
{
    std::vector<dcel_poly<point<long long int, 2> > > polys;

    for(auto i : in_list)
        polys.push_back(dcel_poly<point<long long int,2> >(i));

    return dcel_to_poly(_union_all()); 
};

dcel_poly<point<long long int, 2> >
_union_all(std::vector<dcel_poly<point<long long int, 2> > > polys)
{
        //split halves, merge with _dcel_poly_union
        dcel_poly<point<long long int, 2> > A = 
            _union_all(std::vector<dcel_poly<point<long long int, 2> > >(polys.first(), polys.end() - (polys.size() / 2)));

        dcel_poly B = _union_all(std::vector<dcel_poly<point<long long int, 2> > >(polys.end() - (pols.size() / 2), polys.end()));

        return _dcel_union(A,B);
}

dcel_poly<point<long long int, 2> >
_dcel_poly_union(dcel_poly<point<long long int, 2> > A, dcel_poly<point<long long int, 2> > B) 
{
    //now the hard part.
    //basic flow is: we detect intersections, sort them into the polygons, then trace around.     
}*/

//visible_vertices sweepline algo.
//visible_vertices

#endif
