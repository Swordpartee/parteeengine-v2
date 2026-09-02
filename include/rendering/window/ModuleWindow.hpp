// ModuleWindow.hpp
#pragma once

#include "WindowEventHandler.hpp"
#include "rendering/window/NativeWindow.hpp"
#include "rendering/window/WindowEvent.hpp"
#include "rendering/window/WindowEventHandler.hpp"

#include <functional>
#include <memory>
#include <typeindex>
#include <vector>

#include <unordered_map>

namespace parteeengine::rendering {

struct WindowConfig;
class ModuleWindow;

using WindowSubscriber = std::function<void(WindowEvent&, ModuleWindow&)>;
template <is_window_event EventType>
using TypedWindowSubscriber = std::function<void(const EventType&, ModuleWindow&)>;

class ModuleWindow : public WindowEventHandler {

  private:
    std::unordered_map<std::type_index, std::vector<WindowSubscriber>> subscriberMap;
    NativeWindow* nativeWindow;

  public:
    explicit ModuleWindow(NativeWindow* native) : nativeWindow(native) {}
    ModuleWindow() = delete;

    template <is_window_event EventType>
    std::vector<WindowSubscriber>& getSubscribers() {
        return subscriberMap[typeid(EventType)];
    }

    template <is_window_event EventType>
    void subscribe(const TypedWindowSubscriber<EventType>& subscriber);

    void emit(std::unique_ptr<WindowEvent> event) override;

    [[nodiscard]] bool shouldForward(const WindowEvent& event) const override;

    void configure(const WindowConfig& config);
    void close();
    void poll();
    void config(const WindowConfig& config);
};

template <is_window_event EventType>
void ModuleWindow::subscribe(const TypedWindowSubscriber<EventType>& subscriber) {
    getSubscribers<EventType>().emplace_back([subscriber](WindowEvent& event, ModuleWindow& window) {
        subscriber(static_cast<const EventType&>(event), window);
    });
}

} // namespace parteeengine::rendering