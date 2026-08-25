#pragma once

namespace parteeengine::rendering {

struct WindowEvent {};

struct WindowDestroyEvent : public WindowEvent {};

struct WindowCloseEvent : public WindowEvent {};

struct WindowResizeEvent : public WindowEvent {};

struct WindowMoveEvent : public WindowEvent {};

} // namespace parteeengine::rendering