#pragma once
#include "../Tree.h"

class OakTree : public Tree {
public:
    OakTree() : Tree(0.8) {}

    void generate(std::vector<jbyte>& blocks,
                  int x, int y, int z,
                  int sizeX, int sizeY, int sizeZ) const override;
};
