#include "nl_grapjeje_nativechunks_NativeChunkGenerator.h"
#include <jni.h>
#include <vector>
#include <cmath>
#include <algorithm>

#include "worldParts/tree/TreeFactory.h"

JNIEXPORT jbyteArray JNICALL Java_nl_grapjeje_nativechunks_NativeChunkGenerator_generateChunkNative
(JNIEnv* env, jobject, const jint chunkX, const jint chunkZ, const jlong seed) {
    constexpr int SIZE_X = 16;
    constexpr int SIZE_Z = 16;
    constexpr int SIZE_Y = 256;
    constexpr int TOTAL_SIZE = SIZE_X * SIZE_Z * SIZE_Y;

    std::vector<jbyte> blocks(TOTAL_SIZE, 0);

    auto indexOf = [&](const int x, const int y, const int z) -> int {
        return x + z * SIZE_X + y * SIZE_X * SIZE_Z;
    };

    auto hash = [&](const int x, const int z) -> double {
        long n = seed + x * 374761393L + z * 668265263L;
        n = (n ^ (n >> 13)) * 1274126177L;
        return ((n ^ (n >> 16)) & 0x7fffffff) / static_cast<double>(0x7fffffff);
    };

    auto noise = [&](double x, double z, const double scale) -> double {
        x *= scale;
        z *= scale;
        const int x0 = static_cast<int>(std::floor(x));
        const int z0 = static_cast<int>(std::floor(z));
        const int x1 = x0 + 1;
        const int z1 = z0 + 1;
        double fx = x - x0;
        double fz = z - z0;

        fx = fx * fx * (3.0 - 2.0 * fx);
        fz = fz * fz * (3.0 - 2.0 * fz);

        const double v00 = hash(x0, z0);
        const double v10 = hash(x1, z0);
        const double v01 = hash(x0, z1);
        const double v11 = hash(x1, z1);

        const double v0 = v00 * (1 - fx) + v10 * fx;
        const double v1 = v01 * (1 - fx) + v11 * fx;
        return v0 * (1 - fz) + v1 * fz;
    };

    auto isTreeNearby = [&](const int x, const int z) -> bool {
        for (int dx = -3; dx <= 3; ++dx) {
            for (int dz = -3; dz <= 3; ++dz) {
                const int nx = x + dx;
                const int nz = z + dz;
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

    constexpr int EDGE_MARGIN = 3;
    constexpr int WATER_LEVEL = 63;

    for (int x = 0; x < SIZE_X; ++x) {
        for (int z = 0; z < SIZE_Z; ++z) {
            const int worldX = chunkX * SIZE_X + x;
            const int worldZ = chunkZ * SIZE_Z + z;

            const double largeNoise = noise(worldX, worldZ, 0.02) * 2.0 - 1.0;
            const double smallNoise = noise(worldX + 1000, worldZ + 1000, 0.08) * 2.0 - 1.0;

            int height = 64 + static_cast<int>(largeNoise * 6.0 + smallNoise * 2.0);
            height = std::clamp(height, 56, 72);

            for (int y = 0; y < SIZE_Y; ++y) {
                const int index = indexOf(x, y, z);

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

            if (height > WATER_LEVEL &&
                !isTreeNearby(x, z) &&
                x >= EDGE_MARGIN && x < SIZE_X - EDGE_MARGIN &&
                z >= EDGE_MARGIN && z < SIZE_Z - EDGE_MARGIN) {

                long posHash = seed + worldX * 341873128712L + worldZ * 132897987541L;
                posHash = (posHash ^ (posHash >> 13)) * 1274126177L;
                posHash = (posHash ^ (posHash >> 16));

                if (const int randomValue = static_cast<int>(posHash & 0x7fffffff) % 100; randomValue < 3) {
                    if (const auto tree = TreeFactory::createRandom(seed, worldX, worldZ)) {
                        tree->generate(blocks, x, height, z, SIZE_X, SIZE_Y, SIZE_Z);
                    }
                }
            }
        }
    }

    jbyteArray result = env->NewByteArray(TOTAL_SIZE);
    env->SetByteArrayRegion(result, 0, TOTAL_SIZE, blocks.data());
    return result;
}
