#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>
#include <utility>
/* Feel free to add more standard library headers */

// A vertex is typed as `vertex_t`. An edge is typed as `edge_t`,
// which is associated with a source vertex, a destination vertex, and
// its weight. A list of edges is typed as `edges_t`, which is a
// vector of `edge_t`.

using vertex_t = std::size_t;
using edge_weight_t = double;
using edge_t = std::tuple<vertex_t,
                          vertex_t,
                          edge_weight_t>;
using edges_t = std::vector<edge_t>;

enum class GraphType {
UNDIRECTED,
DIRECTED
};

/*
 * We do not enforce a specific design constraints other than the
 * consturtor. You may introduce your own changes as you
 * want---implementing your own member functions or variables. You
 * will still need to ensure that your constructor works properly,
 * such that we can run the test for grading. The provided test will
 * check if the constructor is correctly designed.
 */
class compare{
public:
    bool operator()(const edge_t& a, const edge_t& b) const{
        if (std::get<2>(a) > std::get<2>(b))
            return true;
        else return false;
      }
};

class Graph {
    
        
    public:
        GraphType Type = GraphType::UNDIRECTED;
        vertex_t num_vertices;
        edges_t edges;
        std::vector <std::pair<vertex_t,edge_weight_t>> *adj_list;
        std::vector<vertex_t> degree_list;
        Graph() = delete;
        Graph(const Graph&) = delete;
        Graph(Graph&&) = delete;
        ~Graph(){
            delete[] adj_list;
        }
        /* We assume that if num_vertices is n, a graph contains n vertices
           from 0 to n-1. */
        Graph(size_t num_vertices,
              const edges_t& edges,
              GraphType Type) {
            this->num_vertices = num_vertices;
            this->Type = Type;
            this->edges = edges;
            adj_list = new std::vector <std::pair<vertex_t,edge_weight_t>>[num_vertices];
            for(size_t i = 0; i<edges.size();i++){
                add_Edge(std::get<0>(edges[i]), std::get<1>(edges[i]), std::get<2>(edges[i]));
                // this will make adjacency list
            }
            
            // TODO: Implement the constructor
        }
    
        void set_directional(){
            Type = GraphType::DIRECTED;
        }
        /* make directional I don't know this will be used*/
        void add_Edge(vertex_t a, vertex_t b, edge_weight_t w){
            
            adj_list[a].push_back(std::make_pair(b, w));
            if(Type == GraphType::UNDIRECTED){
                adj_list[b].push_back(std::make_pair(a,w));
            }
        }
        /* this function will push every vertex connected with edges, if it is dirrectional it will push just once
        but if it's not it will do it reversely too */
    
        void set_degree(){
            degree_list.resize(num_vertices,0);
            for(vertex_t i = 0; i<num_vertices; i++){
                for(auto j=0; j<adj_list[i].size(); j++ ){
                    degree_list[adj_list[i][j].first]++;
                }
            }
        }
    
        std::vector<vertex_t> get_degree(){
            set_degree();
            return degree_list;
        }
        
        vertex_t get_num_vertices(){
            return num_vertices;
        }
    
        void get_adj_list(std::vector <std::pair<vertex_t,edge_weight_t>> *list){
            list = adj_list;
        }
    
        void print_all(){
            for(auto i=0;i<num_vertices;i++)
            {
                std::cout<< i <<" is connected with" <<std::endl;
                for(int j=0; j<adj_list[i].size(); j++){
                    std::cout<<adj_list[i][j].first<<" by weight : "<<adj_list[i][j].second<<std::endl;
                }
            }
        }//for test
        
        //destructor???
        
        // TODO(optional): Define helper functions, e.g., out_deges(v)
		
    private:
        
        // TODO: Roll out your own data structures
};

#endif // __GRAPH_H_
