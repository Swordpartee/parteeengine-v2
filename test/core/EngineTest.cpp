
#include "core/Engine.hpp"

#include "core/events/QuitEvent.hpp"
#include "core/modules/ModuleBase.hpp"

#include <future>
#include <gtest/gtest.h>

namespace parteeengine {
namespace {

class TestModule : public ModuleBase {
  public:
    int value = 0;
    bool initalized = false;
    int updated = 0;

    void init(const ModuleInput&) {
        initalized = true;
    }

    void update(const ModuleInput&) {
        updated++;
    }
};

class QuitModule : public ModuleBase {
    public:
        int timer = 0;

    void update(const ModuleInput& input) {
        timer++;

        if (timer > 4) {
            input.eventManager.emit(QuitEvent{});
        }
    }
};

class SystemModule : public ModuleBase {
public:
    bool entityCreatedAndVerified = false;
    bool componentAddedAndVerified = false;
    bool componentRemovedAndVerified = false;

    void update(const ModuleInput& input) {
        // 1. Test Entity Creation via ModuleInput
        auto entity = input.entityManager.generateEntity();
        if (input.entityManager.isValidEntity(entity)) {
            entityCreatedAndVerified = true;
        }

        // 2. Test Component Addition and Access via ModuleInput
        input.componentManager.addComponent(entity, 100);
        if (input.componentManager.hasComponent<int>(entity) && 
            input.componentManager.getComponent<int>(entity) == 100) {
            componentAddedAndVerified = true;
        }

        // 3. Test Component Removal via ModuleInput
        input.componentManager.removeComponent<int>(entity);
        if (!input.componentManager.hasComponent<int>(entity)) {
            componentRemovedAndVerified = true;
        }
    }
};

class EngineTest : public ::testing::Test {
    protected:
        Engine engine;
};

} // namespace

TEST_F(EngineTest, EngineRoutesEntityAndComponentOperations) {
    auto entity = engine.createEntity();
    ASSERT_TRUE(engine.isValidEntity(entity));

    engine.addComponent(entity, 42);
    EXPECT_TRUE(engine.hasComponent<int>(entity));
    EXPECT_EQ(engine.getComponent<int>(entity), 42);

    engine.removeComponent<int>(entity);
    EXPECT_FALSE(engine.hasComponent<int>(entity));
}

TEST_F(EngineTest, EngineRoutesEventsAndModules) {
    bool called = false;
    int received = 0;

    engine.subscribe<int>([&](const int value) {
        called = true;
        received = value;
    });

    engine.emit<int>(7);

    EXPECT_TRUE(called);
    EXPECT_EQ(received, 7);

    engine.addModule<TestModule>();
    auto& module = engine.getModule<TestModule>();
    module.value = 11;

    EXPECT_EQ(module.value, 11);
}

TEST_F(EngineTest, EngineExitAndEventManager) {
    engine.addModule<QuitModule>();

    auto future = std::async(std::launch::async, [&]() {
        engine.run();
    });

    auto status = future.wait_for(std::chrono::milliseconds(100));

    EXPECT_NE(status, std::future_status::timeout) << "Test timed out! Possible infinite loop.";

}

TEST_F(EngineTest, ModuleInitAndUpdate) {
    engine.addModule<QuitModule>();
    engine.addModule<TestModule>();

    engine.run();

    auto& testModule = engine.getModule<TestModule>();

    EXPECT_TRUE(testModule.initalized);
    EXPECT_TRUE(testModule.updated > 2);
}

TEST_F(EngineTest, ModuleManagerAccess) {
    engine.addModule<QuitModule>();
    engine.addModule<SystemModule>();

    // Run the engine loop to trigger SystemModule::update()
    engine.run();

    auto& systemModule = engine.getModule<SystemModule>();

    // Assert that the module successfully used the managers inside its update phase
    EXPECT_TRUE(systemModule.entityCreatedAndVerified);
    EXPECT_TRUE(systemModule.componentAddedAndVerified);
    EXPECT_TRUE(systemModule.componentRemovedAndVerified);
}

} // namespace parteeengine
