#pragma once

#include <concepts>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "core/modules/ModuleBase.hpp"

namespace parteeengine {

template <typename ModuleType>
concept is_module = std::derived_from<ModuleType, ModuleBase>;

class ModuleManager {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ModuleBase>> modules;

  public:
    template <is_module ModuleType> void addModule();

    template <is_module ModuleType> void addModule(const ModuleType &module);

    template <is_module ModuleType>
    void replaceModule(const ModuleType &module);

    template <is_module ModuleType> void removeModule();

    template <is_module ModuleType> ModuleType &getModule();
};

template <is_module ModuleType> void ModuleManager::addModule() {
    modules.try_emplace(typeid(ModuleType), std::make_unique<ModuleType>());
}

template <is_module ModuleType>
void ModuleManager::addModule(const ModuleType &module) {
    modules.try_emplace(typeid(ModuleType),
                        std::make_unique<ModuleType>(module));
}

template <is_module ModuleType>
void ModuleManager::replaceModule(const ModuleType &module) {
    modules.insert_or_assign(typeid(ModuleType),
                             std::make_unique<ModuleType>(module));
}

template <is_module ModuleType> void ModuleManager::removeModule() {
    modules.erase(typeid(ModuleType));
}

template <is_module ModuleType> ModuleType &ModuleManager::getModule() {
    auto it = modules.find(typeid(ModuleType));
    if (it == modules.end()) {
        throw std::runtime_error("Module does not exist.");
    }

    return static_cast<ModuleType &>(*it->second);
}

} // namespace parteeengine