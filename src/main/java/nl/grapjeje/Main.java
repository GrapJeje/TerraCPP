package nl.grapjeje;

import lombok.Getter;
import lombok.Setter;
import net.minestom.server.MinecraftServer;
import net.minestom.server.instance.InstanceContainer;
import net.minestom.server.instance.InstanceManager;
import net.minestom.server.world.DimensionType;
import nl.grapjeje.nativechunks.NativeGenerator;

import java.util.Random;

public class Main {
    @Getter
    @Setter
    private static InstanceContainer instanceContainer;

    public static void main(String[] args) {
        // Initialize server
        MinecraftServer server = MinecraftServer.init();
        MinecraftServer.setBrandName("TerraCPP");

        // Initialize instance
        InstanceManager instanceManager = MinecraftServer.getInstanceManager();
        InstanceContainer instance = instanceManager.createInstanceContainer(DimensionType.OVERWORLD);

        instance.setGenerator(new NativeGenerator());

        setInstanceContainer(instance);

        // Register Listeners
        new ListenManager().init();

        // Start
        server.start("0.0.0.0", 25565);
    }
}
