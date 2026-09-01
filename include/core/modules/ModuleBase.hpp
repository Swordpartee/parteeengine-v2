#pragma once

#include "core/entities/EntityManager.hpp"

namespace parteeengine {

class EntityManager;
class ComponentManager;
class EventManager;

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct ModuleInput {
    EntityManager& entityManager;
    ComponentManager& componentManager;
    EventManager& eventManager;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

class ModuleBase {
  public:
    virtual void init(const ModuleInput& /*input*/) {};
    virtual void update(const ModuleInput& /*input*/) {};

    virtual ~ModuleBase() = default;
};

} // namespace parteeengine