#pragma once

#include "core/entities/ComponentManager.hpp"
#include "core/entities/EntityManager.hpp"
#include "core/events/EventManager.hpp"
#include "core/modules/ModuleManager.hpp"

namespace parteeengine {

class Engine {
  private:
    ModuleManager moduleManager;
    EntityManager entityManager;
    ComponentManager componentManager;
    EventManager eventManager;

    bool running = false;

  public:
    void run();

    ModuleInput buildModuleInput();

    Entity createEntity();

    bool isValidEntity(const Entity entity) const;

    void deleteEntity(const Entity entity);

    template <typename ComponentType>
    ComponentType& addComponent(const Entity entity);

    template <typename ComponentType>
    ComponentType& addComponent(const Entity entity, ComponentType data);

    template <typename ComponentType>
    void removeComponent(const Entity entity);

    template <typename ComponentType>
    ComponentType* getComponent(const Entity entity);

    template <typename ComponentType>
    bool hasComponent(const Entity entity) const;

    template <typename EventType>
    void subscribe(const detail::TypedSubscriber<EventType> subscriber);

    template <typename EventType>
    void emit(const EventType event) const;

    template <typename ModuleType>
    ModuleType& addModule();

    template <typename ModuleType>
    ModuleType& addModule(const ModuleType& module);

    template <typename ModuleType>
    void removeModule();

    template <typename ModuleType>
    ModuleType* getModule();
};

template <typename ComponentType>
ComponentType& Engine::addComponent(const Entity entity) {
    return componentManager.addComponent<ComponentType>(entity);
}

template <typename ComponentType>
ComponentType& Engine::addComponent(const Entity entity, ComponentType data) {
    return componentManager.addComponent<ComponentType>(entity, data);
}

template <typename ComponentType>
void Engine::removeComponent(const Entity entity) {
    componentManager.removeComponent<ComponentType>(entity);
}

template <typename ComponentType>
ComponentType* Engine::getComponent(const Entity entity) {
    return componentManager.getComponent<ComponentType>(entity);
}

template <typename ComponentType>
bool Engine::hasComponent(const Entity entity) const {
    return componentManager.hasComponent<ComponentType>(entity);
}

template <typename EventType>
void Engine::subscribe(const detail::TypedSubscriber<EventType> subscriber) {
    eventManager.subscribe<EventType>(subscriber);
}

template <typename EventType>
void Engine::emit(const EventType event) const {
    eventManager.emit<EventType>(event);
}

template <typename ModuleType>
ModuleType& Engine::addModule() {
    return moduleManager.addModule<ModuleType>();
}

template <typename ModuleType>
ModuleType& Engine::addModule(const ModuleType& module) {
    return moduleManager.addModule<ModuleType>(module);
}

template <typename ModuleType>
void Engine::removeModule() {
    moduleManager.removeModule<ModuleType>();
}

template <typename ModuleType>
ModuleType* Engine::getModule() {
    return moduleManager.getModule<ModuleType>();
}

} // namespace parteeengine