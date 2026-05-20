#include <gtest/gtest.h>

#include "core/entities/ComponentManager.hpp"
#include "core/entities/EntityManager.hpp"

class ComponentManagerTest : public ::testing::Test {
  protected:
    parteeengine::EntityManager entityManager;
    parteeengine::ComponentManager componentManager;

    struct DataComponent {
        int value = 0;
        float data = 0.0f;
    };

    struct PositionComponent {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct VelocityComponent {
        float vx = 0.0f;
        float vy = 0.0f;
        float vz = 0.0f;
    };
};

TEST_F(ComponentManagerTest, ComponentCreation) {
    auto entity = entityManager.generateEntity();

    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));

    componentManager.addComponent<DataComponent>(entity);

    EXPECT_TRUE(componentManager.hasComponent<DataComponent>(entity));
}

TEST_F(ComponentManagerTest, ComponentDeletion) {
    auto entity = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity);

    EXPECT_TRUE(componentManager.hasComponent<DataComponent>(entity));

    componentManager.removeComponent<DataComponent>(entity);

    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));
}

TEST_F(ComponentManagerTest, ComponentData) {
    auto entity = entityManager.generateEntity();
    DataComponent data{42, 3.14f};

    auto &retrieved =
        componentManager.addComponent<DataComponent>(entity, data);

    EXPECT_EQ(retrieved.value, 42);
    EXPECT_FLOAT_EQ(retrieved.data, 3.14f);
}

TEST_F(ComponentManagerTest, GetComponent) {
    auto entity = entityManager.generateEntity();
    DataComponent data{99, 2.71f};

    componentManager.addComponent<DataComponent>(entity, data);
    auto &retrieved = componentManager.getComponent<DataComponent>(entity);

    EXPECT_EQ(retrieved.value, 99);
    EXPECT_FLOAT_EQ(retrieved.data, 2.71f);
}

TEST_F(ComponentManagerTest, GetNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_THROW(componentManager.getComponent<DataComponent>(entity),
                 std::runtime_error);
}

TEST_F(ComponentManagerTest, RemoveNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_THROW(componentManager.removeComponent<DataComponent>(entity),
                 std::runtime_error);
}

TEST_F(ComponentManagerTest, RemoveOneComponentLeavesOther) {
    auto entity = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity);
    componentManager.addComponent<PositionComponent>(entity);

    componentManager.removeComponent<DataComponent>(entity);

    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<PositionComponent>(entity));
}

TEST_F(ComponentManagerTest, MultipleEntitiesWithComponents) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();
    auto entity3 = entityManager.generateEntity();

    DataComponent data1{1, 1.0f};
    DataComponent data2{2, 2.0f};
    DataComponent data3{3, 3.0f};

    componentManager.addComponent<DataComponent>(entity1, data1);
    componentManager.addComponent<DataComponent>(entity2, data2);
    componentManager.addComponent<DataComponent>(entity3, data3);

    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity1).value, 1);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity2).value, 2);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity3).value, 3);
}

TEST_F(ComponentManagerTest, ComponentDataModification) {
    auto entity = entityManager.generateEntity();

    auto &comp = componentManager.addComponent<DataComponent>(entity);
    EXPECT_EQ(comp.value, 0);
    EXPECT_FLOAT_EQ(comp.data, 0.0f);

    comp.value = 55;
    comp.data = 5.5f;

    auto &retrieved = componentManager.getComponent<DataComponent>(entity);
    EXPECT_EQ(retrieved.value, 55);
    EXPECT_FLOAT_EQ(retrieved.data, 5.5f);
}

TEST_F(ComponentManagerTest, RemoveAndReaddComponent) {
    auto entity = entityManager.generateEntity();
    DataComponent data1{10, 1.0f};
    DataComponent data2{20, 2.0f};

    componentManager.addComponent<DataComponent>(entity, data1);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity).value, 10);

    componentManager.removeComponent<DataComponent>(entity);
    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));

    componentManager.addComponent<DataComponent>(entity, data2);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity).value, 20);
}

TEST_F(ComponentManagerTest, ThreeComponentTypes) {
    auto entity = entityManager.generateEntity();

    DataComponent data{42, 4.2f};
    PositionComponent pos{1.0f, 2.0f, 3.0f};
    VelocityComponent vel{0.1f, 0.2f, 0.3f};

    componentManager.addComponent<DataComponent>(entity, data);
    componentManager.addComponent<PositionComponent>(entity, pos);
    componentManager.addComponent<VelocityComponent>(entity, vel);

    EXPECT_TRUE(componentManager.hasComponent<DataComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<PositionComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<VelocityComponent>(entity));

    auto &retrievedData = componentManager.getComponent<DataComponent>(entity);
    auto &retrievedPos =
        componentManager.getComponent<PositionComponent>(entity);
    auto &retrievedVel =
        componentManager.getComponent<VelocityComponent>(entity);

    EXPECT_EQ(retrievedData.value, 42);
    EXPECT_FLOAT_EQ(retrievedPos.x, 1.0f);
    EXPECT_FLOAT_EQ(retrievedVel.vx, 0.1f);
}