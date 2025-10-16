#include "nl_grapjeje_nativechunks_NativeChunkGenerator.h"
#include <jni.h>
#include <vector>
#include <cmath>

JNIEXPORT jbyteArray JNICALL Java_nl_grapjeje_nativechunks_NativeChunkGenerator_generateChunkNative
  (JNIEnv* env, jobject, jint chunkX, jint chunkZ) {

    const int SIZE_X = 16;
    const int SIZE_Z = 16;
    const int SIZE_Y = 256;
    const int TOTAL_SIZE = SIZE_X * SIZE_Z * SIZE_Y;
    const int WATER_LEVEL = 63;

    std::vector<jbyte> blocks(TOTAL_SIZE, 0);

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

    for (int x = 0; x < SIZE_X; ++x) {
        for (int z = 0; z < SIZE_Z; ++z) {
            int worldX = chunkX * SIZE_X + x;
            int worldZ = chunkZ * SIZE_Z + z;

            double largeNoise = noise(worldX, worldZ, 0.02) * 2.0 - 1.0;
            double smallNoise = noise(worldX + 1000, worldZ + 1000, 0.08) * 2.0 - 1.0;
            
            int height = 64 + (int)(largeNoise * 6.0 + smallNoise * 2.0);
            height = std::max(56, std::min(72, height));

            for (int y = 0; y < SIZE_Y; ++y) {
                int index = x + z * SIZE_X + y * SIZE_X * SIZE_Z;
                
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
                    blocks[index] = 9
                }
            }
        }
    }

    jbyteArray result = env->NewByteArray(TOTAL_SIZE);
    env->SetByteArrayRegion(result, 0, TOTAL_SIZE, blocks.data());
    return result;
}
