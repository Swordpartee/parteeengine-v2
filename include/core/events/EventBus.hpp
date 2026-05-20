#pragma once

#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace parteeengine {

using Subscriber = std::function<void(const std::any)>;
template <typename EventType>
using TypedSubscriber = std::function<void(const EventType)>;

class EventBus {
  private:
    mutable std::unordered_map<std::type_index, std::vector<Subscriber>> subscriberMap;

    template <typename EventType> std::vector<Subscriber>& getSubscribers() const;

  public:
    template <typename EventType>
    void subscribe(const TypedSubscriber<EventType>);

    template <typename EventType> void emit(const EventType) const;
};

template <typename EventType>
std::vector<Subscriber>& EventBus::getSubscribers() const {
    return subscriberMap.try_emplace(typeid(EventType)).first->second;
}

template <typename EventType>
void EventBus::subscribe(const TypedSubscriber<EventType> subscriber) {
    getSubscribers<EventType>().emplace_back([subscriber](const std::any e) {
        auto event = std::any_cast<EventType>(e);

        subscriber(event);
    });
}

template <typename EventType> void EventBus::emit(const EventType event) const {
    for (auto subscriber : getSubscribers<EventType>()) {
        subscriber(event);
    }
}

} // namespace parteeengine