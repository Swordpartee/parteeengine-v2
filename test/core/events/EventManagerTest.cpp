#include "core/events/EventManager.hpp"

#include <gtest/gtest.h>

namespace parteeengine {

namespace {

class EventManagerTest : public ::testing::Test {
  protected:
    EventManager eventManager;

    struct TestEvent {};
    struct TestEventWithData {
        int value;
        std::string message;
    };
    struct AnotherEvent {
        double data;
    };
};

} // namespace

// Basic subscription and emission test
TEST_F(EventManagerTest, Subscribe) {
    bool called = false;

    eventManager.subscribe<TestEvent>([&called](const TestEvent) { called = true; });

    eventManager.emit<TestEvent>({});

    EXPECT_TRUE(called);
}

// Multiple subscribers to the same event type
TEST_F(EventManagerTest, MultipleSubscribers) {
    int callCount = 0;

    eventManager.subscribe<TestEvent>([&callCount](const TestEvent) { callCount++; });
    eventManager.subscribe<TestEvent>([&callCount](const TestEvent) { callCount++; });
    eventManager.subscribe<TestEvent>([&callCount](const TestEvent) { callCount++; });

    eventManager.emit<TestEvent>({});

    EXPECT_EQ(callCount, 3);
}

// Different event types don't interfere
TEST_F(EventManagerTest, DifferentEventTypes) {
    bool testEventCalled = false;
    bool anotherEventCalled = false;

    eventManager.subscribe<TestEvent>([&testEventCalled](const TestEvent) { testEventCalled = true; });
    eventManager.subscribe<AnotherEvent>([&anotherEventCalled](const AnotherEvent) { anotherEventCalled = true; });

    eventManager.emit<TestEvent>({});

    EXPECT_TRUE(testEventCalled);
    EXPECT_FALSE(anotherEventCalled);
}

// Events with data are properly transmitted
TEST_F(EventManagerTest, EventWithData) {
    TestEventWithData receivedEvent = {0, ""};

    eventManager.subscribe<TestEventWithData>(
        [&receivedEvent](const TestEventWithData event) { receivedEvent = event; });

    TestEventWithData emittedEvent = {42, "test message"};
    eventManager.emit<TestEventWithData>(emittedEvent);

    EXPECT_EQ(receivedEvent.value, 42);
    EXPECT_EQ(receivedEvent.message, "test message");
}

// Emitting multiple times to same subscribers
TEST_F(EventManagerTest, EmitMultipleTimes) {
    int callCount = 0;

    eventManager.subscribe<TestEvent>([&callCount](const TestEvent) { callCount++; });

    eventManager.emit<TestEvent>({});
    eventManager.emit<TestEvent>({});
    eventManager.emit<TestEvent>({});

    EXPECT_EQ(callCount, 3);
}

// Subscribers can be added after emission
TEST_F(EventManagerTest, SubscribeAfterEmit) {
    int callCount = 0;

    eventManager.emit<TestEvent>({});

    eventManager.subscribe<TestEvent>([&callCount](const TestEvent) { callCount++; });

    eventManager.emit<TestEvent>({});

    EXPECT_EQ(callCount, 1);
}

// Multiple different event types with different data
TEST_F(EventManagerTest, MultipleEventTypesWithData) {
    TestEventWithData receivedTestEvent = {0, ""};
    AnotherEvent receivedAnotherEvent = {0.0};

    eventManager.subscribe<TestEventWithData>(
        [&receivedTestEvent](const TestEventWithData event) { receivedTestEvent = event; });
    eventManager.subscribe<AnotherEvent>(
        [&receivedAnotherEvent](const AnotherEvent event) { receivedAnotherEvent = event; });

    TestEventWithData testEvent = {99, "multi-event test"};
    AnotherEvent anotherEvent = {3.14159};

    eventManager.emit<TestEventWithData>(testEvent);
    eventManager.emit<AnotherEvent>(anotherEvent);

    EXPECT_EQ(receivedTestEvent.value, 99);
    EXPECT_EQ(receivedTestEvent.message, "multi-event test");
    EXPECT_DOUBLE_EQ(receivedAnotherEvent.data, 3.14159);
}

// Emitting with no subscribers doesn't cause issues
TEST_F(EventManagerTest, EmitWithNoSubscribers) {
    // Should not throw or crash
    EXPECT_NO_THROW(eventManager.emit<TestEvent>({}));
    EXPECT_NO_THROW(eventManager.emit<TestEventWithData>({0, ""}));
}

// Multiple subscribers receive the same event data
TEST_F(EventManagerTest, MultipleSubscribersReceiveSameData) {
    int value1 = 0, value2 = 0, value3 = 0;

    eventManager.subscribe<TestEventWithData>([&value1](const TestEventWithData event) { value1 = event.value; });
    eventManager.subscribe<TestEventWithData>([&value2](const TestEventWithData event) { value2 = event.value; });
    eventManager.subscribe<TestEventWithData>([&value3](const TestEventWithData event) { value3 = event.value; });

    eventManager.emit<TestEventWithData>({123, "shared data"});

    EXPECT_EQ(value1, 123);
    EXPECT_EQ(value2, 123);
    EXPECT_EQ(value3, 123);
}

} // namespace parteeengine
