#include "core/Engine.hpp"

#include "core/entities/Entity.hpp"
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

ModuleInput Engine::buildModuleInput() { return {.entityManager = entityManager, .componentManager = componentManager, .eventManager = eventManager}; }

Entity Engine::createEntity() { return entityManager.generateEntity(); }

bool Engine::isValidEntity(const Entity entity) const { return entityManager.isValidEntity(entity); }

void Engine::deleteEntity(const Entity entity) { entityManager.deleteEntity(entity); }

} // namespace parteeengine