
// #include "core/Engine.hpp"

// #include "core/modules/ModuleBase.hpp"

// #include <gtest/gtest.h>

// namespace parteeengine {
// namespace {

// class TestModule : public ModuleBase {
//   public:
//     int value = 0;
// };

// } // namespace

// TEST(EngineTest, FacadeRoutesEntityAndComponentOperations) {
//     Engine engine;

//     auto entity = engine.createEntity();
//     ASSERT_TRUE(engine.isValidEntity(entity));

//     engine.addComponent(entity, 42);
//     EXPECT_TRUE(engine.hasComponent<int>(entity));
//     EXPECT_EQ(engine.getComponent<int>(entity), 42);

//     engine.removeComponent<int>(entity);
//     EXPECT_FALSE(engine.hasComponent<int>(entity));
// }

// TEST(EngineTest, FacadeRoutesEventsAndModules) {
//     Engine engine;

//     bool called = false;
//     int received = 0;

//     engine.subscribe<int>([&](const int value) {
//         called = true;
//         received = value;
//     });

//     engine.emit<int>(7);

//     EXPECT_TRUE(called);
//     EXPECT_EQ(received, 7);

//     engine.addModule<TestModule>();
//     auto& module = engine.getModule<TestModule>();
//     module.value = 11;

//     EXPECT_EQ(module.value, 11);
// }

// } // namespace parteeengine
