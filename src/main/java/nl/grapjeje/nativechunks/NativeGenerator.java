package nl.grapjeje.nativechunks;

import net.minestom.server.instance.block.Block;
import net.minestom.server.instance.generator.GenerationUnit;
import net.minestom.server.instance.generator.Generator;
import org.jetbrains.annotations.NotNull;

public class NativeGenerator implements Generator {
    private final NativeChunkGenerator nativeGen = new NativeChunkGenerator();

    @Override
    public void generate(@NotNull GenerationUnit unit) {
        int startX = unit.absoluteStart().blockX();
        int startY = unit.absoluteStart().blockY();
        int startZ = unit.absoluteStart().blockZ();
        byte[] blocks = nativeGen.generateChunk(startX / 16, startZ / 16);

        int SIZE_X = 16;
        int SIZE_Z = 16;
        int SIZE_Y = 256;
        int index = 0;
        for (int y = 0; y < SIZE_Y; y++) {
            for (int z = 0; z < SIZE_Z; z++) {
                for (int x = 0; x < SIZE_X; x++) {
                    byte block = blocks[index++];
                    if (block == 1) unit.modifier().setBlock(startX + x, startY + y, startZ + z, Block.STONE);
                    else if (block == 2) unit.modifier().setBlock(startX + x, startY + y, startZ + z, Block.GRASS_BLOCK);
                }
            }
        }
    }
}
