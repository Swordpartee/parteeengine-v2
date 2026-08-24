#pragma once

namespace parteeengine::rendering {

struct WindowEvent {
};

struct WindowCloseEvent : public WindowEvent {

};

struct WindowResizeEvent : public WindowEvent {
    
};

struct WindowMoveEvent : public WindowEvent {
    
};

}