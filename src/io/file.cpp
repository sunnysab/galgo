//
// Created by sunnysab on 24-9-13.
//

#include <cstring>
#include <format>
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

#include "graph.h"
#include "poi.h"


Graph load_graph(const std::string &path, unsigned int limit /* = UINT_MAX */) {
    Graph g;
    ifstream file;

    file.open(path, std::ios::in);
    if (!file) {
        throw std::runtime_error(std::format("can't open file {}: {}\n", path, std::strerror(errno)));
    }

    for (string line; std::getline(file, line);) {
        stringstream ss(line);

        char operation;
        ss >> operation;

        switch (operation) {
        case'a': {
            Vertex start, end;
            EdgeWeight weight;
            ss >> start >> end >> weight;
            /* 由于 limit 的存在，我们可能只能加载部分图，尽可能地希望恰好加载 limit 个顶点.
             * 约束：可以缺少一个，但不能多出一个. */
            if (limit == UINT_MAX
                || g.vertex_count + static_cast<int>(!g.contains(start)) + static_cast<int>(!g.contains(end)) <= limit) {
                for (auto v: {start, end}) {
                    if (!g.contains(v)) {
                        g.insert_vertex(v);
                    }
                }

                g.add_directional_edge(start, end, weight);
            }
            break;
        }
        case'p': {
            unsigned int edge_count, vertex_count;
            string sp;
            ss >> sp >> vertex_count >> edge_count;
            cout << "In complete graph, |V| = " << vertex_count << ", " << "|E| = " << edge_count << endl;
            break;
        }
        case 'c': {
            //auto声明变量是自动类型，注意：使用auto必须要进行初始化！
            auto comment = line.substr(0);
            cout << comment << endl;
            break;
        }
        default: {
            cerr << "unknown operation: " << operation << endl;
            break;
            }
        }
    }
    return g;
}


PoiSet load_poi(std::string path) {
    PoiSet pois;
    ifstream file;

    file.open(path, std::ios::in);
    if (!file) {
        throw std::runtime_error(std::format("can't open file {}: {}\n", path, std::strerror(errno)));
    }

    // poi 文件的格式如下：
    // 0 56987 41 1 5
    // 0 56988 10 1 33
    // 0 61184 41 1 25
    // 第 0 列是所属的子图，忽略。第1列是对应顶点编号，第2列是POI类型，第3列是boundary（忽略），第4列是POI兴趣度（权重）
    for (string line; getline(file, line);) {
        stringstream ss(line);

        unsigned int _unused;
        Vertex v;
        PoiType type;
        Interest interest;
        ss >> _unused >> v >> type >> _unused >> interest;
        pois.add(Poi{v, type, interest});
    }

    return pois;
}


void save_poi(const PoiSet& poi_set, const std::string& path) {
    std::ofstream file(path, std::ios::out);
    if (!file) {
        throw std::runtime_error(std::format("can't open file {}: {}\n", path, std::strerror(errno)));
    }

    for (const auto& [v, poi] : poi_set.pois_map) {
        // 使用默认子图编号 0 和 boundary 1 来保持与 load_poi 函数的一致性
        file << std::format("0 {} {} 1 {}\n", poi.v, poi.type, poi.interest);
    }
    file.close();
}