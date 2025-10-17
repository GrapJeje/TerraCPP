#include "TreeFactory.h"
#include <vector>
#include <memory>
#include <functional>

#include "types/OakTree.h"
#include "types/MediumOakTree.h"

std::unique_ptr<Tree> TreeFactory::createRandom(int64_t seed, int worldX, int worldZ) {
    std::vector<std::function<std::unique_ptr<Tree>()>> treeFactories = {
        []() { return std::make_unique<OakTree>(); },
        []() { return std::make_unique<MediumOakTree>(); }
    };

    int64_t hash = seed + static_cast<int64_t>(worldX) * 341873128712LL + static_cast<int64_t>(worldZ) * 132897987541LL;
    hash = (hash ^ (hash >> 13)) * 1274126177LL;
    hash = (hash ^ (hash >> 16));
    double randomValue = (hash & 0x7fffffff) / static_cast<double>(0x7fffffff);

    double totalChance = 0.0;
    for (const auto& factory : treeFactories) {
        auto tree = factory();
        totalChance += tree->getChance();
    }

    double cumulative = 0.0;
    for (const auto& factory : treeFactories) {
        auto tree = factory();
        cumulative += tree->getChance() / totalChance;
        if (randomValue <= cumulative) {
            return factory();
        }
    }
    return treeFactories.front()();
}
