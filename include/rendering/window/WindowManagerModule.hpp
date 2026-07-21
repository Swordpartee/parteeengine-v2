#pragma once

#include "core/modules/ModuleBase.hpp"
#include "rendering/window/WindowConfig.hpp"

#include <vector>

namespace parteeengine::rendering {

    class WindowManagerModule {
        private:
            std::vector<Window> windows;

        public:

            Window& createWindow(WindowConfig config);


    };

} // namespace parteeengine::rendering