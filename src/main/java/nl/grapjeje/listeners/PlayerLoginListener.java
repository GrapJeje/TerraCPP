package nl.grapjeje.listeners;

import net.minestom.server.coordinate.Pos;
import net.minestom.server.entity.GameMode;
import net.minestom.server.entity.Player;
import net.minestom.server.event.player.AsyncPlayerConfigurationEvent;
import net.minestom.server.event.player.PlayerSpawnEvent;
import net.minestom.server.instance.Instance;
import net.minestom.server.instance.block.Block;
import nl.grapjeje.Listen;
import nl.grapjeje.Listener;
import nl.grapjeje.Main;

public class PlayerLoginListener implements Listener {

    @Listen
    public void onPlayerLogin(AsyncPlayerConfigurationEvent e) {
        final Player player = e.getPlayer();

        final Instance in = Main.getInstanceContainer();
        e.setSpawningInstance(in);
    }

    @Listen
    public void onPlayerSpawn(PlayerSpawnEvent e) {
        final Player player = e.getPlayer();
        final int spawnX = 0;
        final int spawnZ = 0;

        int highestY = -1;
        for (int y = 0; y < 400; y++) {
            if (e.getInstance().getBlock(spawnX & 15, y, spawnZ & 15) == Block.GRASS_BLOCK) {
                highestY = y;
                break;
            }
        }

        Pos teleportPos;
        if (highestY != -1) {
            teleportPos = new Pos(spawnX + 0.5, highestY + 1, spawnZ + 0.5);
        } else {
            int airY = 0;
            for (int y = 0; y < 400; y++) {
                if (e.getInstance().getBlock(spawnX, y, spawnZ).isAir()) {
                    airY = y;
                    break;
                }
            }
            teleportPos = new Pos(spawnX + 0.5, airY, spawnZ + 0.5);
        }

        player.teleport(teleportPos);
        player.setGameMode(GameMode.CREATIVE);
    }
}
