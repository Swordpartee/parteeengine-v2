#pragma once

namespace parteeengine {

using EntityID = unsigned int;
using EntityGeneration = unsigned int;

struct Entity {
    EntityID id;
    EntityGeneration generation;
};

} // namespace parteeengine