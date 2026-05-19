#pragma once

#include "core/ComponentStorage.hpp"
#include "core/Entity.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace parteeengine {

template<typename... ComponentTypes>
using ComponentView = std::vector<std::tuple<Entity, ComponentTypes...>>;

class ComponentManager {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentStorageBase>>
        storages;

    template <typename ComponentType>
    ComponentStorage<ComponentType>& createStorage();

    template <typename ComponentType>
    ComponentStorage<ComponentType>& getStorage();

  public:
    // template <typename... Types> CompoentView<Types> view();

    template <typename... ComponentTypes>
    ComponentView<ComponentTypes...> viewComponent();

    template <typename ComponentType>
    ComponentType &addComponent(const Entity, ComponentType);

    template <typename ComponentType>
    ComponentType &createComponent(const Entity);

    template <typename ComponentType> void removeComponent(const Entity);

    template <typename ComponentType> ComponentType* getComponent(const Entity);


};

template <typename ComponentType>
ComponentStorage<ComponentType>& ComponentManager::createStorage() {
    auto storage = std::make_unique<ComponentStorage<ComponentType>>();
    auto* rawPtr = storage.get();
    storages.emplace(typeid(ComponentType), std::move(storage));
    return *rawPtr;
}

template <typename ComponentType>
ComponentStorage<ComponentType>& ComponentManager::getStorage() {
    auto it = storages.find(typeid(ComponentType));
    if (it == storages.end()) {
        return createStorage<ComponentType>();
    } else {
        return static_cast<ComponentStorage<ComponentType>&>(*it->second);
    }
}

template <typename... ComponentTypes>
ComponentView<ComponentTypes...> ComponentManager::viewComponent() {

}

template <typename ComponentType>
ComponentType &ComponentManager::addComponent(const Entity entity, ComponentType data) {
    ComponentStorage<ComponentType> storage = getStorage<ComponentType>();

    storage.data.emplace_back(data);
    storage.sparseMap.emplace(entity, storage.data.size());
}

template <typename ComponentType>
ComponentType &ComponentManager::createComponent(const Entity entity) {
    ComponentStorage<ComponentType> storage = getStorage<ComponentType>();

    storage.data.emplace_back({});
    storage.sparseMap.emplace(entity, storage.data.size());
}

template <typename ComponentType>
void ComponentManager::removeComponent(const Entity entity) {
    ComponentStorage<ComponentType> storage = getStorage<ComponentType>();

    auto it = storage.sparseMap.find(entity) 
    if (it == storage.sparseMap.end()) {
        throw std::runtime_error("Entity does not have component");
    }

    size_t index = it->second;
    storage.data.swap(storage.data[index], storage.back());
    storage.data.pop_back();
    storage.sparseMap
}

template <typename ComponentType>
ComponentType* ComponentManager::getComponent(const Entity entity) {
    
}

} // namespace parteeengine