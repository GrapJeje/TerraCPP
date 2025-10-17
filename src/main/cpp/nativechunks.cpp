#include "nl_grapjeje_nativechunks_NativeChunkGenerator.h"
#include <jni.h>
#include <vector>
#include <cmath>
#include <random>
#include <map>
#include <tuple>

JNIEXPORT jbyteArray JNICALL Java_nl_grapjeje_nativechunks_NativeChunkGenerator_generateChunkNative
  (JNIEnv* env, jobject, jint chunkX, jint chunkZ) {

    const int SIZE_X = 16;
    const int SIZE_Z = 16;
    const int SIZE_Y = 256;
    const int TOTAL_SIZE = SIZE_X * SIZE_Z * SIZE_Y;
    const int WATER_LEVEL = 63;

    std::vector<jbyte> blocks(TOTAL_SIZE, 0);

    auto indexOf = [&](int x, int y, int z) -> int {
        return x + z * SIZE_X + y * SIZE_X * SIZE_Z;
    };

    auto hash = [](int x, int z) -> double {
        long n = x * 374761393L + z * 668265263L;
        n = (n ^ (n >> 13)) * 1274126177L;
        return ((n ^ (n >> 16)) & 0x7fffffff) / (double)0x7fffffff;
    };

    auto noise = [&](double x, double z, double scale) -> double {
        x *= scale;
        z *= scale;
        int x0 = (int)std::floor(x);
        int z0 = (int)std::floor(z);
        int x1 = x0 + 1;
        int z1 = z0 + 1;
        double fx = x - x0;
        double fz = z - z0;
        
        fx = fx * fx * (3.0 - 2.0 * fx);
        fz = fz * fz * (3.0 - 2.0 * fz);
        
        double v00 = hash(x0, z0);
        double v10 = hash(x1, z0);
        double v01 = hash(x0, z1);
        double v11 = hash(x1, z1);
        
        double v0 = v00 * (1 - fx) + v10 * fx;
        double v1 = v01 * (1 - fx) + v11 * fx;
        return v0 * (1 - fz) + v1 * fz;
    };

    auto spawnTree = [&](int startX, int startY, int startZ) {
        struct CoordCompare {
            bool operator()(const std::tuple<int,int,int>& a, const std::tuple<int,int,int>& b) const {
                if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
                if (std::get<1>(a) != std::get<1>(b)) return std::get<1>(a) < std::get<1>(b);
                return std::get<2>(a) < std::get<2>(b);
            }
        };
        using Coord = std::tuple<int,int,int>;

        std::map<Coord, jbyte, CoordCompare> treeMap;
        treeMap[{startX, startY -1, startZ}] = 3;

        treeMap[{startX, startY, startZ}] = 17;
        treeMap[{startX, startY + 1, startZ}] = 17;
        treeMap[{startX, startY + 2, startZ}] = 17;
        treeMap[{startX, startY + 3, startZ}] = 17;
        treeMap[{startX, startY + 4, startZ}] = 17;
        treeMap[{startX, startY + 5, startZ}] = 17;

        for (int dx = -2; dx <= 2; ++dx) {
            for (int dz = -2; dz <= 2; ++dz) {
                if (dx == 0 && dz == 0) continue;
                treeMap[{startX + dx, startY + 3, startZ + dz}] = 18; 
                if (dx == -2 && dz == -2) continue; 
                treeMap[{startX + dx, startY + 4, startZ + dz}] = 18; 
            }
        }

        treeMap[{startX + 1, startY + 5, startZ}] = 18;
        treeMap[{startX - 1, startY + 5, startZ}] = 18;
        treeMap[{startX, startY + 5, startZ - 1}] = 18;
        treeMap[{startX, startY + 5, startZ + 1}] = 18;
        treeMap[{startX + 1, startY + 5, startZ - 1}] = 18;

        treeMap[{startX, startY + 6, startZ}] = 18;
        treeMap[{startX + 1, startY + 6, startZ}] = 18;
        treeMap[{startX - 1, startY + 6, startZ}] = 18;
        treeMap[{startX, startY + 6, startZ - 1}] = 18;
        treeMap[{startX, startY + 6, startZ + 1}] = 18;

        for (const auto& [coord, blockId] : treeMap) {
            auto [x, y, z] = coord;
            if (x < 0 || x >= SIZE_X || z < 0 || z >= SIZE_Z || y < 0 || y >= SIZE_Y)
                continue;
            int idx = indexOf(x, y, z);
            if (blocks[idx] == 17) continue;
            blocks[idx] = blockId;
        }
    };

    auto isTreeNearby = [&](int x, int z) -> bool {
        for (int dx = -3; dx <= 3; ++dx) {
            for (int dz = -3; dz <= 3; ++dz) {
                int nx = x + dx;
                int nz = z + dz;
                if (nx < 0 || nx >= SIZE_X || nz < 0 || nz >= SIZE_Z)
                    continue;

                for (int y = 0; y < SIZE_Y; ++y) {
                    if (blocks[indexOf(nx, y, nz)] == 17)
                        return true;
                }
            }
        }
        return false;
    };

    auto treeChance = [&](int x, int y, int z) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 99);
        int randomPercent = distr(gen);
        if (randomPercent < 1) {
            if (x >= 2 && x < SIZE_X - 2 && z >= 2 && z < SIZE_Z - 2)
                if (!isTreeNearby(x, z))
                    spawnTree(x, y + 1, z);
        }
    };

    for (int x = 0; x < SIZE_X; ++x) {
        for (int z = 0; z < SIZE_Z; ++z) {
            int worldX = chunkX * SIZE_X + x;
            int worldZ = chunkZ * SIZE_Z + z;

            double largeNoise = noise(worldX, worldZ, 0.02) * 2.0 - 1.0;
            double smallNoise = noise(worldX + 1000, worldZ + 1000, 0.08) * 2.0 - 1.0;
            
            int height = 64 + (int)(largeNoise * 6.0 + smallNoise * 2.0);
            height = std::max(56, std::min(72, height));

            for (int y = 0; y < SIZE_Y; ++y) {
                int index = indexOf(x, y, z);
                
                if (y == 0) {
                    blocks[index] = 7;
                } else if (y < height - 4) {
                    blocks[index] = 1;
                } else if (y < height - 1) {
                    blocks[index] = 3;
                } else if (y == height - 1 && height > WATER_LEVEL) {
                    blocks[index] = 2;
                } else if (y == height - 1 && height <= WATER_LEVEL) {
                    blocks[index] = 3;
                } else if (y >= height && y <= WATER_LEVEL) {
                    blocks[index] = 9;
                }
            }

            if (height > WATER_LEVEL)
                treeChance(x, height - 1, z);
        }
    }

    jbyteArray result = env->NewByteArray(TOTAL_SIZE);
    env->SetByteArrayRegion(result, 0, TOTAL_SIZE, blocks.data());
    return result;
}
