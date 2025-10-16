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

    std::vector<jbyte> blocks(TOTAL_SIZE, 0);

    for (int x = 0; x < SIZE_X; ++x) {
        for (int z = 0; z < SIZE_Z; ++z) {
            int worldX = chunkX * SIZE_X + x;
            int worldZ = chunkZ * SIZE_Z + z;

            int height = 60 + static_cast<int>(8 * std::sin(worldX * 0.1) + 8 * std::cos(worldZ * 0.1));

            for (int y = 0; y < SIZE_Y; ++y) {
                int index = x + z * SIZE_X + y * SIZE_X * SIZE_Z;
                if (y < height - 1) blocks[index] = 1;
                else if (y == height - 1) blocks[index] = 2;
            }
        }
    }

    jbyteArray result = env->NewByteArray(TOTAL_SIZE);
    env->SetByteArrayRegion(result, 0, TOTAL_SIZE, blocks.data());
    return result;
}
