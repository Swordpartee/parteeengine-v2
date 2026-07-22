#pragma once

#include "core/entities/EntityManager.hpp"

namespace parteeengine {

class EntityManager;
class ComponentManager;
class EventManager;

struct ModuleInput {
    EntityManager& entityManager;
    ComponentManager& componentManager;
    EventManager& eventManager;
};

class ModuleBase {
  public:
    virtual void init(const ModuleInput&){};
    virtual void update(const ModuleInput&){};

    virtual ~ModuleBase() = default;
};

} // namespace parteeengine