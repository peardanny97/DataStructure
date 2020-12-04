#ifndef __PRIM_MINIMUM_SPANNING_TREE_H_
#define __PRIM_MINIMUM_SPANNING_TREE_H_

#include <optional>
#include <vector>
#include <queue>
#include <algorithm>
#include "graph.hpp"
/* Feel free to add more standard library headers */

/* Returns the vector of edges in the MST, or std::nullopt if MST does
 * not exist */

std::optional<edges_t> prim_minimum_spanning_tree(Graph& g, vertex_t src) {
    bool p_visited_list[g.num_vertices];
    std::priority_queue<edge_t,edges_t,compare> pq;
    edges_t mst;
    for(auto i=0; i<g.num_vertices; i++){
        p_visited_list[i] = false;
    }
    for(auto i=0; i<g.adj_list[src].size();i++){
        vertex_t s, d;
        edge_weight_t w;
        s = src; // start point
        d = g.adj_list[src][i].first; // destination point
        w = g.adj_list[src][i].second; // weight
        pq.push({s,d,w});
    }
    while(!pq.empty()){
        edge_t cur = pq.top();
        pq.pop();
        if(p_visited_list[std::get<0>(cur)]&&p_visited_list[std::get<1>(cur)]){
            // they are already connected
            continue;
        }
        vertex_t s = std::get<0>(cur); // start point
        vertex_t d = std::get<1>(cur); // destination point
        p_visited_list[s] = true;
        p_visited_list[d] = true;
        mst.push_back({s,d,std::get<2>(cur)});
        for(auto i=0; i<g.adj_list[d].size();i++){
            // Let's make pq for next vertex
            vertex_t next_s, next_d;
            edge_weight_t next_w;
            next_s = d; // next start point is destination
            next_d = g.adj_list[next_s][i].first; // destination point
            if(next_d == s) continue; //it's same case
            next_w = g.adj_list[next_s][i].second; // weight
            pq.push({next_s,next_d,next_w});
        }
    }
    for(auto i=0; i<g.num_vertices; i++){
        if(!p_visited_list[i]){
            return {};
        }
    }
    return mst;
}
#endif // __PRIM_MINIMUM_SPANNING_TREE_H_
