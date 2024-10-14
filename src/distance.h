/// 本文件实现了一个距离矩阵的存储.
/// 由于距离矩阵是对称的，因此只需要存储一半的距离即可.

#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include "graph.h"


template <typename Weight>
class DistanceMatrix {
    std::unordered_map<Vertex, Weight> diagonal;
    std::vector<Weight> distances;
    std::unordered_map<Vertex, size_t> vertex_to_index;
    size_t last_index = 0;

    size_t get_index(Vertex vertex) const {
        return vertex_to_index.at(vertex);
    }

    optional<size_t> get_index_opt(Vertex vertex) const {
        if (!vertex_to_index.contains(vertex)) {
            return nullopt;
        }
        return vertex_to_index.at(vertex);
    }

    size_t get_index_mut(Vertex vertex) {
        if (!vertex_to_index.contains(vertex)) {
            vertex_to_index[vertex] = last_index++;
        }
        return vertex_to_index[vertex];
    }

    // 计算两点距离在距离矩阵中的位置
    static size_t calc_pos(size_t i, size_t j) {
        if (i > j) std::swap(i, j);
        return j * (j - 1) / 2 + i;
    }

public:
    const Weight inf = ~0;
    const bool self_circle = false;

    using T = Weight;

    DistanceMatrix(size_t n, const bool self_circle = false, const Weight inf = ~0) : distances(n * (n - 1) / 2, inf), self_circle(self_circle), inf(inf) {}

    void set(Vertex i, Vertex j, Weight distance) {
        if (i == j) {
            if (self_circle) {
                // 给顶点 i 在矩阵上申请一个位置，占位
                const auto _ = get_index_mut(i);
                // 对角线元素放到对角线的 map 中
                diagonal[i] = distance;
            } else {
                throw std::invalid_argument("self circle is not allowed.");
            }
        } else {
            size_t pos = calc_pos(get_index_mut(i), get_index_mut(j));
            distances[pos] = distance;
        }
    }

    Weight get(Vertex i, Vertex j) const {
        if (i == j) {
            if (!self_circle) return 0;
            return diagonal.at(i); /* 可能产生异常 */
        }
        size_t pos = calc_pos(get_index(i), get_index(j));
        return distances[pos];
    }

    Weight get_or_inf(Vertex i, Vertex j) const {
        if (i == j) {
            if (!self_circle) return 0;
            if (const auto it = diagonal.cfind(i); it != diagonal.cend()) return it->second;
            return inf;
        }

        optional<size_t> pos_i, pos_j;
        if ((pos_i = get_index_opt(i)) == nullopt || (pos_j = get_index_opt(j)) == nullopt) {
            return this->inf;
        }
        const auto pos = calc_pos(*pos_i, *pos_j);
        return distances[pos];
    }

    Weight operator()(Vertex i, Vertex j) const {
        return get(i, j);
    }

    size_t size() const {
        return distances.size();
    }
};

template <typename W>
void print_distance_matrix(const Graph &g, const DistanceMatrix<W> &matrix) {
    // print header line
    std::cout << "    ";
    for (const Vertex i: g.vertices) {
        std::cout << i << " ";
    }
    std::cout << std::endl << "--------------------------------" << std::endl;

    for (const Vertex i: g.vertices) {
        std::cout << i << " | ";
        for (const Vertex j: g.vertices) {
            std::cout << matrix.get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}


template <typename W>
struct DistanceVector
{
    const W inf = ~0;

    Vertex source;
    std::unordered_map<Vertex, W> distances;

    explicit DistanceVector(const Vertex source): source(source) {}

    W operator[](const Vertex target) const {
        return get(target);
    }

    void set(const Vertex target, W weight) {
        this->distances[target] = weight;
    }

    W get(const Vertex target) const {
        if (auto it = distances.find(target); it != distances.end()) {
            return it->second;
        }
        return inf;
    }
};