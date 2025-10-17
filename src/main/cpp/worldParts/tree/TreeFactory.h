#pragma once
#include <memory>
#include "Tree.h"
#include <cstdint>

class TreeFactory {
public:
    static std::unique_ptr<Tree> createRandom(int64_t seed, int worldX, int worldZ);
};
