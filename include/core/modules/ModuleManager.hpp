#pragma once

#include "core/modules/ModuleBase.hpp"

#include <concepts>
#include <memory>
#include <typeindex>

#include <unordered_map>
#include <cassert>

namespace parteeengine {

template <typename ModuleType>
concept is_module = std::derived_from<ModuleType, ModuleBase>;

class ModuleManager {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ModuleBase>> modules;

  public:
    void init(ModuleInput input);

    void update(ModuleInput input);

    template <is_module ModuleType>
    ModuleType& addModule();

    template <is_module ModuleType>
    ModuleType& addModule(const ModuleType& module);

    template <is_module ModuleType>
    void removeModule();

    template <is_module ModuleType>
    ModuleType* getModule();
};

template <is_module ModuleType>
ModuleType& ModuleManager::addModule() {
    modules.try_emplace(typeid(ModuleType), std::make_unique<ModuleType>());

    auto* modulePtr = getModule<ModuleType>();
    assert(modulePtr != nullptr);
    return *modulePtr;
}

template <is_module ModuleType>
ModuleType& ModuleManager::addModule(const ModuleType& module) {
    modules.try_emplace(typeid(ModuleType), std::make_unique<ModuleType>(module));

    auto* modulePtr = getModule<ModuleType>();
    assert(modulePtr != nullptr);
    return *modulePtr;
}

template <is_module ModuleType>
void ModuleManager::removeModule() {
    modules.erase(typeid(ModuleType));
}

template <is_module ModuleType>
ModuleType* ModuleManager::getModule() {
    auto it = modules.find(typeid(ModuleType));
    if (it == modules.end()) {
        return nullptr;
    }
    return static_cast<ModuleType*>(it->second.get());
}

} // namespace parteeengine