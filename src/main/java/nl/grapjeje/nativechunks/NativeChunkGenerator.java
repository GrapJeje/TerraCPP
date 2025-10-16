package nl.grapjeje.nativechunks;

public class NativeChunkGenerator {
    private static boolean loaded = false;

    public NativeChunkGenerator() {
        if (!loaded) {
            System.load("C:\\Users\\johan\\Java\\TerraCPP\\src\\main\\cpp\\build\\Debug\\nativechunks.dll");
            loaded = true;
        }
    }

    public byte[] generateChunk(int chunkX, int chunkZ) {
        return generateChunkNative(chunkX, chunkZ);
    }

    private native byte[] generateChunkNative(int chunkX, int chunkZ);
}
