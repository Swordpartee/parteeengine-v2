#include "core/EntityManager.hpp"

#include <stdexcept>
#include <iostream>
#include <limits>

namespace parteeengine {

Entity EntityManager::generateEntity() {
    if (availableIDs.empty()) {
        if (nextID == std::numeric_limits<EntityID>::max()) {
            throw std::runtime_error("EntityManager: Entity ID space exhausted");
        }

        currentGenerations.emplace_back(0);

        return {nextID++, 0};
    } else {
        auto id = availableIDs.back();
        availableIDs.pop_back();

        return {id, currentGenerations[id]};
    }
}

void EntityManager::deleteEntity(const Entity entity) {
    currentGenerations[entity.id]++;

    if (currentGenerations[entity.id] ==
        std::numeric_limits<EntityGeneration>::max()) {
        return;
    }

    availableIDs.emplace_back(entity.id);
}

bool EntityManager::isValidEntity(const Entity entity) const {
    return entity.id < nextID &&
           entity.generation == currentGenerations[entity.id];
}

} // namespace parteeengine