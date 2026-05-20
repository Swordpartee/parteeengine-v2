#pragma once

#include "core/Entity.hpp"

#include <unordered_map>

namespace parteeengine {

struct ComponentStorageBase {
    virtual ~ComponentStorageBase() = default;
};

template <typename ComponentType> struct ComponentStorage : public ComponentStorageBase {
    std::unordered_map<Entity, size_t> sparseMap;    
    std::vector<Entity> entityMap;
    std::vector<ComponentType> data;
};

} // namespace parteeengine