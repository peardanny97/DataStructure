#ifndef __DIJKSTRA_SHORTEST_PATHS_H_
#define __DIJKSTRA_SHORTEST_PATHS_H_

#include <unordered_map>
#include <optional>
#include <tuple>
#include <queue>
#include <algorithm>

/* Feel free to add more standard library headers */

#include "graph.hpp"



/* Given a vertex `v`, `dijkstra_shortest_path` is:
 * - `nullopt` if `v` is not reacheble from `src`.
 * - {`u`, `d`} where `u` is the predecessor of `v` and `d` is the distance
 *   from `src`
 */

std::unordered_map<vertex_t, std::optional<std::tuple<vertex_t, edge_weight_t>>>
dijkstra_shortest_path(Graph& g, vertex_t src) {
    bool d_visited_list[g.num_vertices];
    std::priority_queue<edge_t,edges_t,compare> pq;
    std::unordered_map<vertex_t,std::optional
    <std::tuple<vertex_t,edge_weight_t>>> d_path;
    for(auto i=0; i<g.num_vertices; i++){
        d_visited_list[i] = false;
    }
    for(auto i=0; i<g.adj_list[src].size();i++){
        vertex_t s, d;
        edge_weight_t w;
        s = src; // start point
        d = g.adj_list[src][i].first; // destination point
        w = g.adj_list[src][i].second; // weight
        pq.push({s,d,w});
    }
    d_path[src] = {src,0.0f};
    while(!pq.empty()){
        edge_t cur = pq.top();
        pq.pop();
        if(d_visited_list[std::get<0>(cur)]&&d_visited_list[std::get<1>(cur)]){
            // they are already connected
            continue;
        }
        vertex_t s = std::get<0>(cur); // start point
        vertex_t d = std::get<1>(cur); // destination point
        edge_weight_t w = std::get<2>(cur);
        d_visited_list[s] = true;
        d_visited_list[d] = true;
        d_path[d] = {s, std::get<2>(cur)};
        for(auto i=0; i<g.adj_list[d].size();i++){
            // Let's make pq for next vertex
            vertex_t next_s, next_d;
            edge_weight_t next_w;
            next_s = d; // next start point is destination
            next_d = g.adj_list[next_s][i].first; // destination point
            if(next_d == s) continue; //it's same case
            next_w = g.adj_list[next_s][i].second; // weight
            pq.push({next_s,next_d,next_w + w});
        }
    }
    for(auto i=0; i<g.num_vertices; i++){
        if(!d_visited_list[i]){
            return {};
        }
    }
    return d_path;
}

#endif // __DIJKSTRA_SHORTEST_PATHS_H_
