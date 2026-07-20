#pragma once

#include <unordered_map>

#include "core/entities/Entity.hpp"

namespace parteeengine {

struct ComponentStorageBase {
    virtual ~ComponentStorageBase() = default;
};

template <typename ComponentType>
struct ComponentStorage : public ComponentStorageBase {
    std::unordered_map<Entity, size_t> sparseMap;
    std::vector<Entity> entityMap;
    std::vector<ComponentType> data;
};

} // namespace parteeengine