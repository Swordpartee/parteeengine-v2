#pragma once

namespace parteeengine::rendering {

struct WindowEvent {
    virtual ~WindowEvent() = default;
};

struct WindowDestroyEvent : public WindowEvent {};

struct WindowCloseEvent : public WindowEvent {};

} // namespace parteeengine::rendering