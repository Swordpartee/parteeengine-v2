#include "core/Engine.hpp"

#include "core/events/QuitEvent.hpp"
#include "core/modules/ModuleBase.hpp"

namespace parteeengine {

void Engine::run() {
    running = true;
    eventManager.subscribe<QuitEvent>([this](const QuitEvent&) { this->running = false; });

    moduleManager.init(buildModuleInput());

    while (running) {
        moduleManager.update(buildModuleInput());
    }
};

ModuleInput Engine::buildModuleInput() { return {entityManager, componentManager, eventManager}; }

Entity Engine::createEntity() { return entityManager.generateEntity(); }

bool Engine::isValidEntity(const Entity entity) const { return entityManager.isValidEntity(entity); }

void Engine::deleteEntity(const Entity entity) { entityManager.deleteEntity(entity); }

} // namespace parteeengine