#pragma once

namespace parteeengine::rendering {

struct WindowEvent {
    virtual ~WindowEvent() = default;
};

struct WindowCloseEvent : public WindowEvent {};

} // namespace parteeengine::rendering