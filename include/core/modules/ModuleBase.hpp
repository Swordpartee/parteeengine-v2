#pragma once

namespace parteeengine {

class ModuleBase {
public:
    virtual void update() = 0;
    virtual ~ModuleBase() = default;

};

} // namespace parteeengine