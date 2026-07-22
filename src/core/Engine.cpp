#include "core/Engine.hpp"

#include "core/modules/ModuleBase.hpp"

namespace parteeengine {

void Engine::run() {
    moduleManager.init(buildModuleInput());

    while (true) {
        moduleManager.update(buildModuleInput());
    }
};

ModuleInput Engine::buildModuleInput() { return {entityManager, componentManager, eventManager}; }

Entity Engine::createEntity() { return entityManager.generateEntity(); }

bool Engine::isValidEntity(const Entity entity) const { return entityManager.isValidEntity(entity); }

void Engine::deleteEntity(const Entity entity) { entityManager.deleteEntity(entity); }

} // namespace parteeengine