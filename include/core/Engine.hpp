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

  public:
    Entity createEntity() { return entityManager.generateEntity(); }

    bool isValidEntity(const Entity entity) const { return entityManager.isValidEntity(entity); }

    void deleteEntity(const Entity entity) { entityManager.deleteEntity(entity); }

    template <typename ComponentType> void addComponent(const Entity entity) {
        componentManager.addComponent<ComponentType>(entity);
    }

    template <typename ComponentType> void addComponent(const Entity entity, ComponentType data) {
        componentManager.addComponent<ComponentType>(entity, data);
    }

    template <typename ComponentType> void removeComponent(const Entity entity) {
        componentManager.removeComponent<ComponentType>(entity);
    }

    template <typename ComponentType> ComponentType &getComponent(const Entity entity) {
        return componentManager.getComponent<ComponentType>(entity);
    }

    template <typename ComponentType> bool hasComponent(const Entity entity) const {
        return componentManager.hasComponent<ComponentType>(entity);
    }

    template <typename EventType> void subscribe(const detail::TypedSubscriber<EventType> subscriber) {
        eventManager.subscribe<EventType>(subscriber);
    }

    template <typename EventType> void emit(const EventType event) const { eventManager.emit<EventType>(event); }

    template <typename ModuleType> void addModule() { moduleManager.addModule<ModuleType>(); }

    template <typename ModuleType> void addModule(const ModuleType& module) {
        moduleManager.addModule<ModuleType>(module);
    }

    template <typename ModuleType> void replaceModule(const ModuleType &module) {
        moduleManager.replaceModule<ModuleType>(module);
    }

    template <typename ModuleType> void removeModule() { moduleManager.removeModule<ModuleType>(); }

    template <typename ModuleType> ModuleType &getModule() { return moduleManager.getModule<ModuleType>(); }
};

} // namespace parteeengine