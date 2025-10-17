#pragma once
#include <vector>
#include <jni.h>

class Tree {
public:
    explicit Tree(const double chance = 1.0) : chance(chance) {}
    virtual ~Tree() = default;

    [[nodiscard]] double getChance() const { return chance; }

    virtual void generate(std::vector<jbyte>& blocks,
                          int x, int y, int z,
                          int sizeX, int sizeY, int sizeZ) const = 0;

protected:
    static int indexOf(int x, int y, int z, int sizeX, int sizeZ) {
        return x + z * sizeX + y * sizeX * sizeZ;
    }

    static void placeBlock(std::vector<jbyte>& blocks,
                    int x, int y, int z, jbyte id,
                    int sizeX, int sizeY, int sizeZ) ;

    static bool isTreeNearby(const std::vector<jbyte>& blocks,
                      int x, int z, int sizeX, int sizeY, int sizeZ) ;

private:
    double chance;
};
