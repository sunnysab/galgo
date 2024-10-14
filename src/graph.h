//
// Created by sunnysab on 24-9-13.
//

#pragma once


#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <optional>
#include <stdexcept>

using namespace std;


using Vertex = unsigned int;
using EdgeWeight = unsigned int;

constexpr EdgeWeight InfWeight = ~0;
constexpr Vertex InvalidVertex = 0;


class EdgeIterator: public iterator_traits<pair<Vertex, EdgeWeight>> {
    optional<unordered_map<Vertex, EdgeWeight>::const_iterator> it;

public:
    explicit EdgeIterator() = default;
    explicit EdgeIterator(unordered_map<Vertex, EdgeWeight>::const_iterator it): it(it) {}

    static EdgeIterator invalid() {
        return EdgeIterator();
    }

    bool operator==(const EdgeIterator &other) const {
        return *it == *other.it;
    }

    bool operator!=(const EdgeIterator &other) const {
        return *it != *other.it;
    }

    EdgeIterator &operator++() {
        ++(*it);
        return *this;
    }

    std::pair<Vertex, EdgeWeight> operator*() const {
        return **it;
    }
};


class EdgeSet {
    optional<const unordered_map<Vertex, EdgeWeight>*> map;

public:
    EdgeSet(const unordered_map<Vertex, EdgeWeight>  *map) {
        this->map = map;
    }

    static EdgeSet empty() {
        return {nullptr};
    }

    EdgeIterator begin() const {
        if (nullptr == this->map) {
            return EdgeIterator::invalid();
        }

        return EdgeIterator{(*map)->begin()};
    }

    EdgeIterator end() const {
        if (nullptr == this->map) {
            return EdgeIterator::invalid();
        }

        return EdgeIterator{(*map)->end()};
    }
};

struct GraphByAdjacencyList {
    unsigned int  vertex_count = 0;

    // 顶点集
    unordered_set<Vertex>  vertices;
    // 将两个顶点打包为一个pair，建立一对顶点到边的映射
    unordered_map<Vertex, unordered_map<Vertex, EdgeWeight>> edges;

    GraphByAdjacencyList() = default;

    // 包含顶点
    bool contains(Vertex v) const;

    // 判断两个顶点是否相邻
    bool is_connected(Vertex v1, Vertex v2) const;

    // 判断顶点的插入是否成功
    bool insert_vertex(Vertex v);

    // 添加有向边
    void add_directional_edge(Vertex v1, Vertex v2, EdgeWeight weight);

    // 连接两个顶点
    void connect(Vertex v1, Vertex v2, EdgeWeight weight);

    // 获得两点之间的边权
    EdgeWeight get_weight(Vertex v1, Vertex v2) const;

    // 获得相邻边迭代器
    EdgeSet get_adjacent_vertices(Vertex src) const;
};

using Graph = GraphByAdjacencyList;


template <class T>
class SymmetricMatrix {
    std::vector<T> data;
    size_t size;

    [[nodiscard]] size_t getIndex(int row, int col) const {
        if (row > col) {
            std::swap(row, col);
        }
        return row * size - (row - 1) * row / 2 + col - row;
    }

public:
    explicit SymmetricMatrix(size_t  n, T default_value) : size(n) {
        data.resize(n * (n + 1) / 2, default_value);
        for (auto i = 0; i < n; ++i) {
            this->set(i, i, 0);
        }
    }

    void set(size_t row, size_t col, T value) {
        if (row >= size || col >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[getIndex(row, col)] = value;
    }

    [[nodiscard]] T get(const size_t row, const size_t col) const {
        if (row >= size || col >= size) {
            throw std::out_of_range("Index out of range");
        }
        if (row == col) {
            return 0;
        }
        return data[getIndex(row, col)];
    }
};
