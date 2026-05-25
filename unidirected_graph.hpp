#pragma once

#include <iostream>
#include <list>
#include <set>
#include <map>
#include <unordered_map>


class unidirected_edge {
    
    private:
        
        int u;
        int v;

    public:
        
    unidirected_edge(int n1, int n2) {
            u = std::min(n1,n2);
            v = std::max(n1, n2);
        }
    
    int from() const {
        return u;
    }
    
    int to() const {
        return v;
    }
    
    bool operator<(const unidirected_edge& other) const {
        if (u != other.u) {
            return u < other.u;
        }
        return v < other.v;
    }
    
    bool operator==(const unidirected_edge& other) const {
        return (u == other.u) && (v == other.v);
    }

};

std::ostream& operator<<(std::ostream& os, const unidirected_edge& e) {
        os << "(" << e.from() << ", " << e.to() << ")";
        return os;
};

class unidirected_graph {

    private:

        std::map<int, std::set<int>> adj_list;
        std::map<unidirected_edge, int> edge_to_id;
        std::map<int, unidirected_edge> id_to_edge;
        int next_edge_id;

    public:
        
    unidirected_graph() : next_edge_id(0) {}

        unidirected_graph(const unidirected_graph& other) 
            : adj_list(other.adj_list), edge_to_id(other.edge_to_id), id_to_edge(other.id_to_edge), next_edge_id(other.next_edge_id) {}

    std::set<int> neighbours(int node) const {
        auto n = adj_list.find(node);
        if (n != adj_list.end()) {
            return n->second;
        }
        return std::set<int>();
    }

    void add_edge(int n1, int n2) {
        unidirected_edge e(n1, n2);
        if (edge_to_id.find(e) == edge_to_id.end()) {
            edge_to_id[e] = next_edge_id;
            id_to_edge.emplace(next_edge_id, e);
            next_edge_id++;
            adj_list[n1].insert(n2);
            adj_list[n2].insert(n1);
        }
    }

    std::set<unidirected_edge> all_edges() const {
        std::set<unidirected_edge> edges;
        for (const auto& pair : edge_to_id) {
            edges.insert(pair.first);
        }
        return edges;
    }

    std::set<int> all_nodes() const {
        std::set<int> nodes;
        for (const auto& pair : adj_list) {
            nodes.insert(pair.first);
        }
        return nodes;
    }

    int edge_number(const unidirected_edge& e) const {
        auto it = edge_to_id.find(e);
        if (it != edge_to_id.end()) {
            return it->second;
        }
        return -1; 
    }

    unidirected_edge edge_at(int id) const {
        return id_to_edge.at(id);
    }

    unidirected_graph operator-(const unidirected_graph& other) const {
        unidirected_graph result;
        for (const auto& pair : edge_to_id) {
            const unidirected_edge& e = pair.first;
            if (other.edge_to_id.find(e) == other.edge_to_id.end()) {
                result.add_edge(e.from(), e.to());
            }
        }
        return result;
    }

};