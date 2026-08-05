#pragma once

namespace parteeengine::rendering {

struct WindowEvent {
};

struct WindowQuitEvent : public WindowEvent {

};

struct WindowResizeEvent : public WindowEvent {
    
};

}