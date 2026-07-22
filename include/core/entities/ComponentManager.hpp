#pragma once

#include "core/entities/ComponentStorage.hpp"
#include "core/entities/Entity.hpp"

#include <memory>
#include <stdexcept>
#include <typeindex>

#include <unordered_map>

namespace parteeengine {

template <typename... ComponentTypes>
using ComponentView = std::vector<std::tuple<Entity, ComponentTypes...>>;

class ComponentManager {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>> storages;

    template <typename ComponentType>
    ComponentStorage<ComponentType>& getOrCreateStorage();

    template <typename ComponentType>
    const ComponentStorage<ComponentType>* findStorage() const;

  public:
    template <typename... ComponentTypes>
    ComponentView<ComponentTypes...> viewComponents() const;

    template <typename ComponentType>
    void addComponent(const Entity, ComponentType);

    template <typename ComponentType>
    void addComponent(const Entity);

    template <typename ComponentType>
    void removeComponent(const Entity);

    template <typename ComponentType>
    ComponentType& getComponent(const Entity);

    template <typename ComponentType>
    ComponentType const& getComponent(const Entity) const;

    template <typename ComponentType>
    bool hasComponent(const Entity) const;
};

template <typename ComponentType>
ComponentStorage<ComponentType>& ComponentManager::getOrCreateStorage() {
    auto [it, inserted] = storages.try_emplace(std::type_index(typeid(ComponentType)),
                                               std::make_unique<ComponentStorage<ComponentType>>());
    return static_cast<ComponentStorage<ComponentType>&>(*it->second);
}

template <typename ComponentType>
const ComponentStorage<ComponentType>* ComponentManager::findStorage() const {
    auto it = storages.find(std::type_index(typeid(ComponentType)));
    if (it == storages.end()) {
        return nullptr;
    }

    return static_cast<const ComponentStorage<ComponentType>*>(it->second.get());
}

template <typename... ComponentTypes>
ComponentView<ComponentTypes...> ComponentManager::viewComponents() const {
    ComponentView<ComponentTypes...> result;

    if constexpr (sizeof...(ComponentTypes) == 0) {
        return result;
    }

    using FirstComponent = std::tuple_element_t<0, std::tuple<ComponentTypes...>>;
    const auto* firstStorage = findStorage<FirstComponent>();
    if (firstStorage == nullptr) {
        return result;
    }

    for (const auto& entity : firstStorage->entityMap) {
        if ((hasComponent<ComponentTypes>(entity) && ...)) {
            result.emplace_back(entity, getComponent<ComponentTypes>(entity)...);
        }
    }

    return result;
}

template <typename ComponentType>
void ComponentManager::addComponent(const Entity entity, ComponentType data) {
    auto& storage = getOrCreateStorage<ComponentType>();

    storage.data.emplace_back(data);
    storage.entityMap.emplace_back(entity);
    storage.sparseMap.emplace(entity, storage.data.size() - 1);
}

template <typename ComponentType>
void ComponentManager::addComponent(const Entity entity) {
    auto& storage = getOrCreateStorage<ComponentType>();

    storage.data.emplace_back(ComponentType{});
    storage.entityMap.emplace_back(entity);
    storage.sparseMap.emplace(entity, storage.data.size() - 1);
}

template <typename ComponentType>
void ComponentManager::removeComponent(const Entity entity) {
    auto& storage = getOrCreateStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity);
    if (it == storage.sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }

    size_t index = it->second;
    storage.sparseMap[storage.entityMap.back()] = index;
    storage.sparseMap.erase(entity);
    std::swap(storage.data[index], storage.data.back());
    storage.data.pop_back();
    std::swap(storage.entityMap[index], storage.entityMap.back());
    storage.entityMap.pop_back();
}

template <typename ComponentType>
ComponentType& ComponentManager::getComponent(const Entity entity) {
    auto& storage = getOrCreateStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity);
    if (it == storage.sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }
    return storage.data[it->second];
}

template <typename ComponentType>
ComponentType const& ComponentManager::getComponent(const Entity entity) const {
    const auto* storage = findStorage<ComponentType>();

    if (storage == nullptr) {
        throw std::runtime_error("Entity does not have component");
    }

    auto it = storage->sparseMap.find(entity);
    if (it == storage->sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }
    return storage->data[it->second];
}

template <typename ComponentType>
bool ComponentManager::hasComponent(const Entity entity) const {
    const auto* storage = findStorage<ComponentType>();
    if (storage == nullptr) {
        return false;
    }

    auto it = storage->sparseMap.find(entity);
    if (it == storage->sparseMap.end()) {
        return false;
    }
    return true;
}

} // namespace parteeengine