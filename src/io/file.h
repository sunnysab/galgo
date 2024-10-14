//
// Created by sunnysab on 24-9-17.
//

#pragma once

#include <string>
#include <climits>
#include "poi.h"

/// 加载图
Graph load_graph(const std::string &path, unsigned int limit = UINT_MAX);

/// 加载 POI
PoiSet load_poi(std::string path);

/// 保存 POI
void save_poi(const PoiSet &poi_set, const std::string &path);

