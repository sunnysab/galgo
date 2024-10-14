//
// Created by sunnysab on 24-9-30.
//

#include "graph.h"
#include "dijkstra.h"

#include "file.h"


Dijkstra::ContinuousDijkstra(const Graph& g, Vertex source)
    : graph(const_cast<Graph&>(g)), source(source), distance(source) {
    pq.emplace(0, source);
    distance.set(source, 0);
}

/// Throw exception if there is no path between source and target
EdgeWeight Dijkstra::get(const Vertex target) {
    if (source == target)
        return 0;
    if (const auto cached = this->distance[target]; cached != InfWeight)
        return cached;

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d > distance[v]) continue;
        auto return_flag = false;
        if (v == target) return_flag = true;

        for (const auto& [adj_v, w] : graph.get_adjacent_vertices(v)) {
            if (auto old_d = distance[adj_v], new_d = distance[v] + w; new_d < old_d) {
                distance.set(adj_v, new_d);
                pq.emplace(new_d, adj_v);
            }
        }

        if (return_flag) {
            return d;
        }
    }
    throw std::runtime_error("no path found");
}


int _main() {
    auto graph = load_graph("USA-road-t.NY-stripped-1000.gr");
    auto dijkstra = Dijkstra(graph, 810);

    auto result = dijkstra.get(1237);
    std::cout << "result = " << result << std::endl;
    return 0;
}