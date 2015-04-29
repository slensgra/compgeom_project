#ifndef __DCEL_NODE_H__
#define __DCEL_NODE_H__

template<class ptype>
class dcel_node
{
public:
//next vertex
//prev vertex
//next_poly (polygons come in a linked list) from polygon to next, same vertex
//intersect
//entry_exit
//neighbor
//alpha

    dcel_node* next;
    dcel_node* prev;
    dcel_node* neighbor;

    polygon* next_poly;

    //if the point was generated as an intersect, use these. 
    bool intersect;
    float alpha;
    bool entry_exit;
    
    decltype operator[] (int i)
    {
        return vert[i];
    };

private:
    ptype vert;
};

#endif
