#include "core/modules/ModuleManager.hpp"

#include "core/modules/ModuleBase.hpp"

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace {

struct TestModuleA : public parteeengine::ModuleBase {
    int value{0};

    TestModuleA() = default;
    explicit TestModuleA(int v) : value(v) {}
};

struct TestModuleB : public parteeengine::ModuleBase {
    std::string name{"default"};

    TestModuleB() = default;
    explicit TestModuleB(std::string n) : name(std::move(n)) {}
};

class ModuleManagerTest : public ::testing::Test {
  protected:
    parteeengine::ModuleManager moduleManager;
};

TEST_F(ModuleManagerTest, AddModuleDefaultConstructsModule) {
    moduleManager.addModule<TestModuleA>();

    auto& module = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(module.value, 0);
}

TEST_F(ModuleManagerTest, AddModuleWithValueStoresCopy) {
    TestModuleA input{42};
    moduleManager.addModule<TestModuleA>(input);

    auto& module = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(module.value, 42);
}

TEST_F(ModuleManagerTest, AddModuleDuplicateDoesNotOverwriteExisting) {
    moduleManager.addModule<TestModuleA>(TestModuleA{10});
    moduleManager.addModule<TestModuleA>(TestModuleA{99}); // try_emplace should ignore

    auto& module = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(module.value, 10);
}

TEST_F(ModuleManagerTest, ReplaceModuleOverwritesExisting) {
    moduleManager.addModule<TestModuleA>(TestModuleA{10});
    moduleManager.replaceModule<TestModuleA>(TestModuleA{55});

    auto& module = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(module.value, 55);
}

TEST_F(ModuleManagerTest, ReplaceModuleInsertsWhenMissing) {
    moduleManager.replaceModule<TestModuleA>(TestModuleA{77});

    auto& module = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(module.value, 77);
}

TEST_F(ModuleManagerTest, RemoveModuleRemovesExisting) {
    moduleManager.addModule<TestModuleA>(TestModuleA{5});
    moduleManager.removeModule<TestModuleA>();

    EXPECT_THROW((void)moduleManager.getModule<TestModuleA>(), std::runtime_error);
}

TEST_F(ModuleManagerTest, RemoveModuleWhenMissingDoesNotThrow) {
    EXPECT_NO_THROW(moduleManager.removeModule<TestModuleA>());
}

TEST_F(ModuleManagerTest, GetModuleWhenMissingThrowsRuntimeError) {
    EXPECT_THROW((void)moduleManager.getModule<TestModuleA>(), std::runtime_error);
}

TEST_F(ModuleManagerTest, GetModuleReturnsReferenceToStoredInstance) {
    moduleManager.addModule<TestModuleA>(TestModuleA{1});

    auto& module = moduleManager.getModule<TestModuleA>();
    module.value = 123;

    auto& moduleAgain = moduleManager.getModule<TestModuleA>();
    EXPECT_EQ(moduleAgain.value, 123);
}

TEST_F(ModuleManagerTest, DifferentModuleTypesAreStoredIndependently) {
    moduleManager.addModule<TestModuleA>(TestModuleA{11});
    moduleManager.addModule<TestModuleB>(TestModuleB{"audio"});

    EXPECT_EQ(moduleManager.getModule<TestModuleA>().value, 11);
    EXPECT_EQ(moduleManager.getModule<TestModuleB>().name, "audio");
}

} // namespace