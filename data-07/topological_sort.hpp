#ifndef __TOPOLOGICAL_SORT_H_
#define __TOPOLOGICAL_SORT_H_

#include <vector>
#include <queue>
#include "graph.hpp"
/* Feel free to add more standard library headers */

/* Return _a_ valid topologically sorted sequence of vertex descriptors */
std::vector<vertex_t> topological_sort(Graph& g) {
    std::queue<vertex_t> q;
    std::vector<vertex_t> degree = g.get_degree();
    std::vector<vertex_t> result;
    
    for(vertex_t i = 0; i<g.get_num_vertices();i++){
        if(degree[i]==0)
            q.push(i);
    }

    while(!q.empty()){
        vertex_t temp;
        temp = q.front();
        q.pop();
        result.push_back(temp);
        for(auto it = 0; it < g.adj_list[temp].size(); it++){
            degree[g.adj_list[temp][it].first]--;
            if(degree[g.adj_list[temp][it].first]==0){
                q.push(g.adj_list[temp][it].first);
            }
        }
    }
    return result;
	return {};
}

#endif // __TOPOLOGICAL_SORT_H_
