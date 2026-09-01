#include "core/entities/EntityManager.hpp"

#include "core/entities/Entity.hpp"

#include <cassert>
#include <limits>

namespace parteeengine {

Entity EntityManager::generateEntity() {
    if (availableIDs.empty()) {
        assert(nextID != std::numeric_limits<EntityID>::max());

        currentGenerations.emplace_back(0);

        return {.id = nextID++, .generation = 0};
    } else {
        auto id = availableIDs.back();
        availableIDs.pop_back();

        return {.id = id, .generation = currentGenerations.at(id)};
    }
}

bool EntityManager::deleteEntity(const Entity entity) {
    if (!isValidEntity(entity)) {
        return false;
    }
    currentGenerations.at(entity.id)++;

    if (currentGenerations.at(entity.id) == std::numeric_limits<EntityGeneration>::max()) {
        return false;
    }

    availableIDs.emplace_back(entity.id);

    return true;
}

bool EntityManager::isValidEntity(const Entity entity) const {
    return entity.id < nextID && entity.generation == currentGenerations.at(entity.id);
}

} // namespace parteeengine