#pragma once

namespace parteeengine::rendering {

struct WindowEvent {};

struct WindowDestroyEvent : public WindowEvent {};

struct WindowCloseEvent : public WindowEvent {};

} // namespace parteeengine::rendering