#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include <tuple>

#include "graph.hpp"
#include "topological_sort.hpp"
#include "dijkstra_shortest_paths.hpp"
#include "prim_minimum_spanning_tree.hpp"

int main() {
//
//    edges_t dijkstra_edges1 = {
//            {0, 1, 20.0f},
//            {0, 3, 21.0f},
//            {1, 2, 7.0f},
//            {1, 3, 13.0f},
//            {1, 4, 5.0f},
//            {2, 4, 1.0f},
//            {3, 4, 10.0f},
//            {3, 5, 14.0f},
//            {4, 5, 22.0f},
//            {4, 6, 15.0f},
//            {4, 7, 6.0f},
//            {5, 6, 4.0f},
//            {6, 7, 11.0f},
//            {6, 8, 3.0f},
//            {6, 9, 19.0f},
//            {7, 8, 2.0f},
//            {7, 10, 8.0f},
//            {7, 11, 9.0f},
//            {8, 9, 17.0f},
//            {8, 10, 12.0f},
//            {9, 10, 17.0f},
//            {10, 11, 16.0f},
//            {12,13,17.0f},
//            {13,14,1.0f}
//        };
//
//        Graph g1(15, dijkstra_edges1, GraphType::UNDIRECTED);
//        std::unordered_map<vertex_t,
//        std::optional<std::tuple<vertex_t,
//        edge_weight_t>>> dijkstra_result
//        = dijkstra_shortest_path(g1, 10);
//
//        // We do not check the source vertex's parent and distance
//        std::vector<vertex_t> parent_answer = {1, 4, 4, 4, 7, 6, 8, 10, 7, 10, (vertex_t)-1, 10};
//        std::vector<edge_weight_t> dist_answer = {39.0f, 19.0f, 15.0f, 24.0f, 14.0f, 17.0f, 13.0f, 8.0f, 10.0f, 17.0f, 0, 16.0f};
//    if(dijkstra_result[14]==std::nullopt){
//        std::cout<<"hi"<<std::endl;
//    }
//    else std::cout<<"bye";
//        for(size_t i = 0 ; i < 12 ; ++i) {
//            if(i == 10)
//                continue;
//            if(std::get<0>(dijkstra_result[i].value()) != parent_answer[i])
//                std::cout<<"1 wrong!" <<std::endl;
//            else std::cout<<"1 good" <<std::endl;
//
//            // Use epsilon to compare floating-point values.  Check
//            // https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
//            // for more details.
//            if(
//                    fabs(std::get<1>(dijkstra_result[i].value()) - dist_answer[i])
//                    >= std::numeric_limits<edge_weight_t>::epsilon())
//                std:: cout<< "2 wrong!"<<std::endl;
//            else std::cout<<"good!"<<std::endl;
//        }


    edges_t prim_edges = {
        {0, 1, 4.0f},
        {0, 7, 8.0f},
        {1, 2, 8.0f},
        {1, 7, 11.0f},
        {2, 3, 7.0f},
        {2, 8, 2.0f},
        {2, 5, 4.0f},
        {3, 4, 9.0f},
        {3, 5, 14.0f},
        {4, 5, 10.0f},
        {5, 6, 2.0f},
        {6, 7, 1.0f},
        {6, 8, 6.0f},
        {9,10,3.0f},
        {7, 8, 7.0f}};
    
    Graph g(11, prim_edges, GraphType::UNDIRECTED);
    std::optional<edges_t> prim_result
    = prim_minimum_spanning_tree(g, 0);
    
    // 1. This graph has MST.
    if(prim_result == std::nullopt)
        std::cout<< "first test case is failed"<<std::endl;
    else std::cout<<"first test case is passed"<<std::endl;
//
//    std::unordered_map<vertex_t, bool> visited;
//    edge_weight_t weight = 0;
//    for(size_t i = 0 ; i < 9 ; ++i)
//    visited[i] = false;
//
//
//    // 2. Check if the returned results are a spanning tree
//    for(auto edge : prim_result.value()) {
//        vertex_t src = std::get<0>(edge);
//        vertex_t dst = std::get<1>(edge);
//
//        // If both src and dst vertices were already visisted, the
//        // returned edges are not MST
//        if((visited[src] && !visited[dst]))
//            std::cout<< "second test case is failed"<<std::endl;
//        else std::cout<<"second test case is passed"<<std::endl;
//
//        if(!visited[src])
//            visited[src] = true;
//        if(!visited[dst])
//            visited[dst] = true;
//        weight += std::get<2>(edge);
//    }
//
//    // 2. Every node should be visited to be MST
//    for(size_t i = 0 ; i < 9 ; ++i)
//    if(visited[i] != true)
//        std::cout<< "third test case is failed"<<std::endl;
//    else std::cout<<"third test case is passed"<<std::endl;
//
//
//    // 3. Weight should be minimum to be MST
//    if(fabs(weight - 37.0f) > std::numeric_limits<edge_weight_t>::epsilon())
//        std::cout<< "final test case is failed"<<std::endl;
//    else std::cout<<"final test case is passed"<<std::endl;

    
//    edges_t topological_edges = {{0, 1, 1.0f},
//        {3, 0, 2.0f},{3, 4, 2.0f},{3, 5, 2.0f},{2, 3, 2.0f},
//        {5, 4, 2.0f},{5, 6, 2.0f},{6, 4, 2.0f},{6, 11, 2.0f},
//        {7, 3, 2.0f},{7, 8, 2.0f},{8, 9, 2.0f},{9, 5, 2.0f},
//        {5, 10, 2.0f},{5, 4, 2.0f},{10, 11, 2.0f}};
//
//    Graph g(12, topological_edges, GraphType::DIRECTED);
//
//    std::vector<vertex_t> topological_result
//        = topological_sort(g);
//    for(int i=0; i<topological_result.size();i++){
//        std::cout<< topological_result[i] <<std::endl;
//    }
//    std::vector<vertex_t> answer = {0, 1, 2, 3, 4, 5};
    
//    edges_t topological_edges = {{0, 1, 1.0f},
//                                 {1, 2, 2.0f},
//                                 {2, 3, 2.0f},
//                                 {3, 4, 2.0f},
//                                 {4, 5, 2.0f}};
//
//    Graph g(6, topological_edges, GraphType::DIRECTED);
//    std::vector<vertex_t> topological_result
//        = topological_sort(g);
//    for(int i=0; i<topological_result.size();i++){
//        std::cout<< topological_result[i] <<std::endl;
//    }
    
//
//    edges_t dijkstra_edges1 = {{0, 1, 3.0f},
//                               {0, 2, 1.0f},
//                               {1, 2, 7.0f},
//                               {1, 3, 5.0f},
//                               {1, 4, 1.0f},
//                               {2, 3, 2.0f},
//                               {3, 4, 7.0f}};
//
//    Graph g1(5, dijkstra_edges1, GraphType::UNDIRECTED);
    return 0;
}
