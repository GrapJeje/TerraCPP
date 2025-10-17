#ifndef NATIVECHUNKS_MEDIUMOAKTREE_H
#define NATIVECHUNKS_MEDIUMOAKTREE_H
#include "../Tree.h"


class MediumOakTree : public Tree {
public:
    MediumOakTree() : Tree(0.01) {}

    void generate(std::vector<jbyte>& blocks,
                  int x, int y, int z,
                  int sizeX, int sizeY, int sizeZ) const override;
};

#endif