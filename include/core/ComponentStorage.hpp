#pragma once

#include "core/Entity.hpp"

#include <unordered_map>

namespace parteeengine {

struct ComponentStorageBase {
    virtual ~ComponentStorageBase() = default;
};

template <typename ComponentType> struct ComponentStorage {
    std::unordered_map<EntityID, size_t> sparseMap;    
    std::vector<ComponentType> data;
};

} // namespace parteeengine