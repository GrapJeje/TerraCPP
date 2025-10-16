package nl.grapjeje;

import net.minestom.server.MinecraftServer;
import net.minestom.server.event.Event;
import net.minestom.server.event.GlobalEventHandler;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public interface Listener {
    default void register() {
        GlobalEventHandler eventHandler = MinecraftServer.getGlobalEventHandler();
        Method[] methods = this.getClass().getDeclaredMethods();

        for (Method method : methods) {
            if (method.isAnnotationPresent(Listen.class)) {
                Class<?>[] paramTypes = method.getParameterTypes();
                if (paramTypes.length == 1 && Event.class.isAssignableFrom(paramTypes[0])) {
                    Class<?> eventToStickTo = paramTypes[0];
                    if (Event.class.isAssignableFrom(eventToStickTo)) {
                        eventHandler.addListener(eventToStickTo.asSubclass(Event.class), event -> {
                            try {
                                method.invoke(this, event);
                            } catch (IllegalAccessException | InvocationTargetException e) {
                                throw new RuntimeException(e);
                            }
                        });
                    } else {
                        throw new IllegalArgumentException("Method annotated with @Listen must have a single parameter of type Event or a subclass of Event.");
                    }
                } else {
                    throw new IllegalArgumentException("Method annotated with @Listen must have a single parameter of type Event or a subclass of Event.");
                }
            }
        }
    }
}
