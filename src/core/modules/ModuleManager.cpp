#include "core/modules/ModuleManager.hpp"

namespace parteeengine {

void ModuleManager::init(ModuleInput input) {
    for (const auto& [_, module] : modules) {
        module->init(input);
    }
}

void ModuleManager::update(ModuleInput input) {
    for (const auto& [_, module] : modules) {
        module->update(input);
    }
}

} // namespace parteeengine