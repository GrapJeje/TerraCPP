package nl.grapjeje.nativechunks;

import lombok.Getter;

@Getter
public class NativeChunkGenerator {
    private static boolean loaded = false;
    private final long seed;

    public NativeChunkGenerator(long seed) {
        this.seed = seed;
        if (!loaded) {
            System.load("C:\\Users\\johan\\Java\\TerraCPP\\src\\main\\cpp\\build\\Debug\\nativechunks.dll");
            loaded = true;
        }
    }

    public byte[] generateChunk(int chunkX, int chunkZ) {
        return generateChunkNative(chunkX, chunkZ, this.getSeed());
    }

    private native byte[] generateChunkNative(int chunkX, int chunkZ, long seed);
    public static native String getMaterialName(byte id);
}
