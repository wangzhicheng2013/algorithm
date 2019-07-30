// https://www.cnblogs.com/lyfruit/articles/3081600.html
//https://www.cnblogs.com/LJB00125/p/LG-p3385-tijie.html
#include <iostream>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/depth_first_search.hpp"
using namespace std;
using namespace boost;
const int N = 6;
enum { A, B, C, D, E, F, G, H };

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int>> mygraph;

class DFSmyv :public boost::default_dfs_visitor {
public:
    template<typename vertex, typename Graph>
    void discover_vertex(vertex v, Graph g) const {
        cout << "at vertex " << v << endl;
    }
    template<typename Edge,typename Graph> const
    void examine_edge(Edge e, Graph g) {
        cout << "edge = " << e << endl;
    }
public:
    static int sum;
};
int DFSmyv::sum = 0;
int main() {
    mygraph myg;
    add_edge(A, B, 13, myg);
    add_edge(B, C, 23, myg);
    DFSmyv myd;
    depth_first_search(myg, visitor(myd));
    
    return 0;
}
