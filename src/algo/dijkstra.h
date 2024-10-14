//
// Created by sunnysab on 24-9-30.
//

#pragma once

#include <queue>
#include "graph.h"
#include "distance.h"

struct ContinuousDijkstra
{
    using PQ_PAIR = std::pair<EdgeWeight, Vertex>;
private:
    std::unordered_set<Vertex> visited;
    priority_queue<PQ_PAIR, std::vector<PQ_PAIR>, std::greater<>> pq;

public:
    const Graph &graph;
    const Vertex source;

    DistanceVector<EdgeWeight>  distance;

    ContinuousDijkstra(const Graph &g, Vertex source);

    /// 获取两点之间的权重，如果不存在路径则返回无穷大
    EdgeWeight get(Vertex target);

    /// 获取两点之间的权重，如果不存在路径则返回无穷大
    EdgeWeight operator[](const Vertex target) {
        return get(target);
    }
};

using Dijkstra = ContinuousDijkstra;
