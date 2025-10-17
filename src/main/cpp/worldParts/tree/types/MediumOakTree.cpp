#include "MediumOakTree.h"
#include <tuple>
#include <map>
#include <algorithm>
#include <random>

void MediumOakTree::generate(std::vector<jbyte> &blocks,
    int x, int y, int z,
    int sizeX, int sizeY, int sizeZ) const {

    struct CoordCompare {
        bool operator()(const std::tuple<int,int,int>& a, const std::tuple<int,int,int>& b) const {
            if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
            if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
            return std::get<2>(a) < std::get<2>(b);
        }
    };
    using Coord = std::tuple<int,int,int>;
    std::map<Coord, jbyte, CoordCompare> treeMap;

    treeMap[{x, y -1, z}] = 3;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(3, 5);
    int height = dist(gen);
    for (int i = 0; i < height; ++i) {
        treeMap[{x, y + i, z}] = 17;
    }
    const int top = height + y;

    int layers[3] = {top, top - 1, top - 2};

    for (int y : layers) {
        for (int dx = -2; dx <= 2; dx++) {
            for (int dz = -2; dz <= 2; dz++) {
                if (std::abs(dx) == 2 && std::abs(dz) == 2) continue;

                std::tuple<int,int,int> pos = {x + dx, y, z + dz};
                if (treeMap.count(pos) && treeMap[pos] == 17) continue;
                treeMap[pos] = 18;
            }
        }
    }

    treeMap[{x, top + 1, z}] = 18;
    treeMap[{x + 1, top + 1, z}] = 18;
    treeMap[{x - 1, top + 1, z}] = 18;
    treeMap[{x, top + 1, z + 1}] = 18;
    treeMap[{x, top + 1, z - 1}] = 18;

    for (const auto& [coord, blockId] : treeMap) {
        auto [bx, by, bz] = coord;
        if (bx < 0 || bx >= sizeX || bz < 0 || bz >= sizeZ || by < 0 || by >= sizeY)
            continue;

        if (int idx = indexOf(bx, by, bz, sizeX, sizeZ); blocks[idx] != 17)
            blocks[idx] = blockId;
    }
}
