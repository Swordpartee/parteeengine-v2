#pragma once

#include "core/entities/ComponentStorage.hpp"
#include "core/entities/Entity.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace parteeengine {

template <typename... ComponentTypes>
using ComponentView = std::vector<std::tuple<Entity, ComponentTypes...>>;

class ComponentManager {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>>
        storages;

    template <typename ComponentType>
    ComponentStorage<ComponentType> &createStorage();

    template <typename ComponentType>
    ComponentStorage<ComponentType> &getStorage();

  public:
    // template <typename... Types> CompoentView<Types> view();

    template <typename... ComponentTypes>
    ComponentView<ComponentTypes...> viewComponent();

    template <typename ComponentType>
    ComponentType &addComponent(const Entity, ComponentType);

    template <typename ComponentType> ComponentType &addComponent(const Entity);

    template <typename ComponentType> void removeComponent(const Entity);

    template <typename ComponentType> ComponentType &getComponent(const Entity);

    template <typename ComponentType> bool hasComponent(const Entity);
};

template <typename ComponentType>
ComponentStorage<ComponentType> &ComponentManager::createStorage() {
    auto storage = std::make_unique<ComponentStorage<ComponentType>>();
    auto *rawPtr = storage.get();
    storages.emplace(typeid(ComponentType), std::move(storage));
    return *rawPtr;
}

template <typename ComponentType>
ComponentStorage<ComponentType> &ComponentManager::getStorage() {
    auto it = storages.find(typeid(ComponentType));
    if (it == storages.end()) {
        return createStorage<ComponentType>();
    } else {
        return static_cast<ComponentStorage<ComponentType> &>(*it->second);
    }
}

// template <typename... ComponentTypes>
// ComponentView<ComponentTypes...> ComponentManager::viewComponent() {

// }

template <typename ComponentType>
ComponentType &ComponentManager::addComponent(const Entity entity,
                                              ComponentType data) {
    auto &storage = getStorage<ComponentType>();

    storage.data.emplace_back(data);
    storage.entityMap.emplace_back(entity);
    storage.sparseMap.emplace(entity, storage.data.size() - 1);

    return storage.data.back();
}

template <typename ComponentType>
ComponentType &ComponentManager::addComponent(const Entity entity) {
    auto &storage = getStorage<ComponentType>();

    storage.data.emplace_back(ComponentType{});
    storage.entityMap.emplace_back(entity);
    storage.sparseMap.emplace(entity, storage.data.size() - 1);

    return storage.data.back();
}

template <typename ComponentType>
void ComponentManager::removeComponent(const Entity entity) {
    auto &storage = getStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity);
    if (it == storage.sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }

    size_t index = it->second;
    storage.sparseMap.emplace(storage.entityMap.back(), index);
    storage.sparseMap.erase(entity);
    std::swap(storage.data[index], storage.data.back());
    storage.data.pop_back();
    std::swap(storage.entityMap[index], storage.entityMap.back());
    storage.entityMap.pop_back();
}

template <typename ComponentType>
ComponentType &ComponentManager::getComponent(const Entity entity) {
    auto &storage = getStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity);
    if (it == storage.sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }
    return storage.data[it->second];
}

template <typename ComponentType>
bool ComponentManager::hasComponent(const Entity entity) {
    auto &storage = getStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity);
    if (it == storage.sparseMap.end()) {
        return false;
    }
    return true;
}

} // namespace parteeengine