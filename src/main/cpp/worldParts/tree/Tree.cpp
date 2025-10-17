#include "Tree.h"
#include "../../material/material.h"

void Tree::placeBlock(std::vector<jbyte>& blocks,
                      int x, int y, int z, jbyte id,
                      int sizeX, int sizeY, int sizeZ) {
    if (x < 0 || x >= sizeX || z < 0 || z >= sizeZ || y < 0 || y >= sizeY)
        return;

    int idx = indexOf(x, y, z, sizeX, sizeZ);
    if (blocks[idx] != 17)
        blocks[idx] = id;
}

bool Tree::isTreeNearby(const std::vector<jbyte>& blocks,
                        const int x, const int z,
                        const int sizeX, const int sizeY, const int sizeZ) {
    for (int dx = -3; dx <= 3; ++dx) {
        for (int dz = -3; dz <= 3; ++dz) {
            const int nx = x + dx;
            const int nz = z + dz;
            if (nx < 0 || nx >= sizeX || nz < 0 || nz >= sizeZ)
                continue;
            for (int y = 0; y < sizeY; ++y) {
                if (blocks[indexOf(nx, y, nz, sizeX, sizeZ)] == static_cast<jbyte>(Material::OAK_LOG))
                    return true;
            }
        }
    }
    return false;
}
