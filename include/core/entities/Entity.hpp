#pragma once

#include <functional>

namespace parteeengine {

using EntityID = unsigned int;
using EntityGeneration = unsigned int;

struct Entity {
    EntityID id;
    EntityGeneration generation;

    bool operator==(const Entity &other) const { return id == other.id && generation == other.generation; }
};

} // namespace parteeengine

// hash function for unordered_maps in component storage
namespace std {
template <> struct hash<parteeengine::Entity> {
    size_t operator()(const parteeengine::Entity &entity) const {
        return hash<unsigned int>()(entity.id) ^ (hash<unsigned int>()(entity.generation) << 1);
    }
};
} // namespace std