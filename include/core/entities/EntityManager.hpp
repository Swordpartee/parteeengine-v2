#pragma once

#include "core/entities/Entity.hpp"

#include <vector>

namespace parteeengine {

class EntityManager {
  private:
    EntityID nextID{};

    std::vector<EntityID> availableIDs;
    std::vector<EntityGeneration> currentGenerations;

  public:
    Entity generateEntity();

    [[nodiscard]] bool isValidEntity(const Entity entity) const;

    // Invalidates an entity;
    bool deleteEntity(const Entity entity);
};

} // namespace parteeengine