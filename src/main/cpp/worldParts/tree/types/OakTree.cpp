#include "OakTree.h"
#include <tuple>
#include <map>
#include <algorithm>
#include <random>

#include "../../../material/material.h"

void OakTree::generate(std::vector<jbyte>& blocks,
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
    std::uniform_int_distribution<> dist(4, 6);
    int height = dist(gen);
    for (int i = 0; i < height; ++i) {
        treeMap[{x, y + i, z}] = static_cast<jbyte>(Material::OAK_LOG);
    }
    const int top = height + y;

    for (int dx = -2; dx <= 2; ++dx) {
        for (int dz = -2; dz <= 2; ++dz) {
            if (dx == 0 && dz == 0) continue;
            treeMap[{x + dx, top - 3, z + dz}] = static_cast<jbyte>(Material::OAK_LEAVES);
            if (dx == -2 && dz == -2) continue;
            treeMap[{x + dx, top - 2, z + dz}] = static_cast<jbyte>(Material::OAK_LEAVES);
        }
    }

    treeMap[{x + 1, top - 1, z}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x - 1, top - 1, z}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x, top - 1, z - 1}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x, top - 1, z + 1}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x + 1, top - 1, z - 1}] = static_cast<jbyte>(Material::OAK_LEAVES);

    treeMap[{x, top, z}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x + 1, top, z}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x - 1, top, z}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x, top, z - 1}] = static_cast<jbyte>(Material::OAK_LEAVES);
    treeMap[{x, top, z + 1}] = static_cast<jbyte>(Material::OAK_LEAVES);

    for (const auto& [coord, blockId] : treeMap) {
        auto [bx, by, bz] = coord;
        if (bx < 0 || bx >= sizeX || bz < 0 || bz >= sizeZ || by < 0 || by >= sizeY)
            continue;

        if (int idx = indexOf(bx, by, bz, sizeX, sizeZ); blocks[idx] != static_cast<jbyte>(Material::OAK_LOG))
            blocks[idx] = blockId;
    }
}

