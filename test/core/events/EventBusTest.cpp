#include <gtest/gtest.h>

#include "core/events/EventBus.hpp"

class EventBusTest : public ::testing::Test {
  protected:
    parteeengine::EventBus eventBus;

    struct TestEvent {};
    struct TestEventWithData {
        int value;
        std::string message;
    };
    struct AnotherEvent {
        double data;
    };
};

// Basic subscription and emission test
TEST_F(EventBusTest, Subscribe) {
    bool called = false;

    eventBus.subscribe<TestEvent>([&called](const TestEvent) {
        called = true;
    });

    eventBus.emit<TestEvent>({});

    EXPECT_TRUE(called);
}

// Multiple subscribers to the same event type
TEST_F(EventBusTest, MultipleSubscribers) {
    int callCount = 0;

    eventBus.subscribe<TestEvent>([&callCount](const TestEvent) {
        callCount++;
    });
    eventBus.subscribe<TestEvent>([&callCount](const TestEvent) {
        callCount++;
    });
    eventBus.subscribe<TestEvent>([&callCount](const TestEvent) {
        callCount++;
    });

    eventBus.emit<TestEvent>({});

    EXPECT_EQ(callCount, 3);
}

// Different event types don't interfere
TEST_F(EventBusTest, DifferentEventTypes) {
    bool testEventCalled = false;
    bool anotherEventCalled = false;

    eventBus.subscribe<TestEvent>([&testEventCalled](const TestEvent) {
        testEventCalled = true;
    });
    eventBus.subscribe<AnotherEvent>([&anotherEventCalled](const AnotherEvent) {
        anotherEventCalled = true;
    });

    eventBus.emit<TestEvent>({});

    EXPECT_TRUE(testEventCalled);
    EXPECT_FALSE(anotherEventCalled);
}

// Events with data are properly transmitted
TEST_F(EventBusTest, EventWithData) {
    TestEventWithData receivedEvent = {0, ""};

    eventBus.subscribe<TestEventWithData>([&receivedEvent](const TestEventWithData event) {
        receivedEvent = event;
    });

    TestEventWithData emittedEvent = {42, "test message"};
    eventBus.emit<TestEventWithData>(emittedEvent);

    EXPECT_EQ(receivedEvent.value, 42);
    EXPECT_EQ(receivedEvent.message, "test message");
}

// Emitting multiple times to same subscribers
TEST_F(EventBusTest, EmitMultipleTimes) {
    int callCount = 0;

    eventBus.subscribe<TestEvent>([&callCount](const TestEvent) {
        callCount++;
    });

    eventBus.emit<TestEvent>({});
    eventBus.emit<TestEvent>({});
    eventBus.emit<TestEvent>({});

    EXPECT_EQ(callCount, 3);
}

// Subscribers can be added after emission
TEST_F(EventBusTest, SubscribeAfterEmit) {
    int callCount = 0;

    eventBus.emit<TestEvent>({});

    eventBus.subscribe<TestEvent>([&callCount](const TestEvent) {
        callCount++;
    });

    eventBus.emit<TestEvent>({});

    EXPECT_EQ(callCount, 1);
}

// Multiple different event types with different data
TEST_F(EventBusTest, MultipleEventTypesWithData) {
    TestEventWithData receivedTestEvent = {0, ""};
    AnotherEvent receivedAnotherEvent = {0.0};

    eventBus.subscribe<TestEventWithData>([&receivedTestEvent](const TestEventWithData event) {
        receivedTestEvent = event;
    });
    eventBus.subscribe<AnotherEvent>([&receivedAnotherEvent](const AnotherEvent event) {
        receivedAnotherEvent = event;
    });

    TestEventWithData testEvent = {99, "multi-event test"};
    AnotherEvent anotherEvent = {3.14159};

    eventBus.emit<TestEventWithData>(testEvent);
    eventBus.emit<AnotherEvent>(anotherEvent);

    EXPECT_EQ(receivedTestEvent.value, 99);
    EXPECT_EQ(receivedTestEvent.message, "multi-event test");
    EXPECT_DOUBLE_EQ(receivedAnotherEvent.data, 3.14159);
}

// Emitting with no subscribers doesn't cause issues
TEST_F(EventBusTest, EmitWithNoSubscribers) {
    // Should not throw or crash
    EXPECT_NO_THROW(eventBus.emit<TestEvent>({}));
    EXPECT_NO_THROW(eventBus.emit<TestEventWithData>({0, ""}));
}

// Multiple subscribers receive the same event data
TEST_F(EventBusTest, MultipleSubscribersReceiveSameData) {
    int value1 = 0, value2 = 0, value3 = 0;

    eventBus.subscribe<TestEventWithData>([&value1](const TestEventWithData event) {
        value1 = event.value;
    });
    eventBus.subscribe<TestEventWithData>([&value2](const TestEventWithData event) {
        value2 = event.value;
    });
    eventBus.subscribe<TestEventWithData>([&value3](const TestEventWithData event) {
        value3 = event.value;
    });

    eventBus.emit<TestEventWithData>({123, "shared data"});

    EXPECT_EQ(value1, 123);
    EXPECT_EQ(value2, 123);
    EXPECT_EQ(value3, 123);
}