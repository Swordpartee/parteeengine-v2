#pragma once

#include "core/Entity.hpp"

#include <vector>

namespace parteeengine {

class EntityManager {
  private:
    EntityID nextID;

    std::vector<EntityID> availableIDs;
    std::vector<EntityGeneration> currentGenerations;

  public:
    Entity generateEntity();

    bool isValidEntity(const Entity) const;

    // Invalidates an entity;
    void deleteEntity(const Entity);
};

} // namespace parteeengine