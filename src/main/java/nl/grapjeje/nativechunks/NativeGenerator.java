package nl.grapjeje.nativechunks;

import net.minestom.server.instance.block.Block;
import net.minestom.server.instance.generator.GenerationUnit;
import net.minestom.server.instance.generator.Generator;
import org.jetbrains.annotations.NotNull;

import java.util.Random;

public class NativeGenerator implements Generator {
    private final NativeChunkGenerator nativeGen = new NativeChunkGenerator(new Random().nextLong());

    @Override
    public void generate(@NotNull GenerationUnit unit) {
        int startX = unit.absoluteStart().blockX();
        int startY = unit.absoluteStart().blockY();
        int startZ = unit.absoluteStart().blockZ();
        byte[] blocks = nativeGen.generateChunk(startX / 16, startZ / 16);

        final int SIZE_X = 16;
        final int SIZE_Z = 16;
        final int SIZE_Y = 256;
        int index = 0;

        for (int y = 0; y < SIZE_Y; y++) {
            for (int z = 0; z < SIZE_Z; z++) {
                for (int x = 0; x < SIZE_X; x++) {
                    byte blockId = blocks[index++];

                    Block block = switch (blockId) {
                        case 1 -> Block.STONE;
                        case 2 -> Block.GRASS_BLOCK;
                        case 3 -> Block.DIRT;
                        case 4 -> Block.BEDROCK;
                        case 5 -> Block.WATER;
                        case 6 -> Block.OAK_LOG;
                        case 7 -> Block.OAK_LEAVES;
                        case 8 -> Block.SHORT_GRASS;
                        case 9 -> Block.SEAGRASS;
                        default -> Block.AIR;
                    };
                    if (block != Block.AIR) unit.modifier().setBlock(startX + x, startY + y, startZ + z, block);
                }
            }
        }
    }
}
