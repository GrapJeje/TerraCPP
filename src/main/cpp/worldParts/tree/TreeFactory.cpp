#include "TreeFactory.h"
#include "types/OakTree.h"
#include <vector>

std::unique_ptr<Tree> TreeFactory::createRandom(int64_t seed, int worldX, int worldZ) {
    std::vector<std::unique_ptr<Tree>> treeTypes;
    treeTypes.push_back(std::make_unique<OakTree>());

    int64_t hash = seed + static_cast<int64_t>(worldX) * 341873128712LL + static_cast<int64_t>(worldZ) * 132897987541LL;
    hash = (hash ^ (hash >> 13)) * 1274126177LL;
    hash = (hash ^ (hash >> 16));

    const double randomValue = ((hash & 0x7fffffff) / static_cast<double>(0x7fffffff));

    double cumulative = 0.0;
    for (const auto& t : treeTypes) {
        cumulative += t->getChance();
        if (randomValue <= cumulative)
            return std::make_unique<OakTree>();
    }

    return nullptr;
}
