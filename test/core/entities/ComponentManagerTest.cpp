#include "core/entities/ComponentManager.hpp"

#include "core/entities/EntityManager.hpp"

#include <gtest/gtest.h>

namespace parteeengine {

namespace {

class ComponentManagerTest : public ::testing::Test {
  protected:
    EntityManager entityManager;
    ComponentManager componentManager;

    struct DataComponent {
        int value = 0;
        float data = 0.0F;
    };

    struct PositionComponent {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
    };

    struct VelocityComponent {
        float vx = 0.0F;
        float vy = 0.0F;
        float vz = 0.0F;
    };
};

} // namespace

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
    DataComponent data{42, 3.14F};

    componentManager.addComponent<DataComponent>(entity, data);
    auto& retrieved = componentManager.getComponent<DataComponent>(entity);

    EXPECT_EQ(retrieved.value, 42);
    EXPECT_FLOAT_EQ(retrieved.data, 3.14F);
}

TEST_F(ComponentManagerTest, GetComponent) {
    auto entity = entityManager.generateEntity();
    DataComponent data{99, 2.71F};

    componentManager.addComponent<DataComponent>(entity, data);
    auto& retrieved = componentManager.getComponent<DataComponent>(entity);

    EXPECT_EQ(retrieved.value, 99);
    EXPECT_FLOAT_EQ(retrieved.data, 2.71F);
}

TEST_F(ComponentManagerTest, GetNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_THROW(componentManager.getComponent<DataComponent>(entity), std::runtime_error);
}

TEST_F(ComponentManagerTest, RemoveNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_THROW(componentManager.removeComponent<DataComponent>(entity), std::runtime_error);
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

    DataComponent data1{1, 1.0F};
    DataComponent data2{2, 2.0F};
    DataComponent data3{3, 3.0F};

    componentManager.addComponent<DataComponent>(entity1, data1);
    componentManager.addComponent<DataComponent>(entity2, data2);
    componentManager.addComponent<DataComponent>(entity3, data3);

    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity1).value, 1);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity2).value, 2);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity3).value, 3);
}

TEST_F(ComponentManagerTest, ComponentDataModiFication) {
    auto entity = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity);
    auto& comp = componentManager.getComponent<DataComponent>(entity);
    EXPECT_EQ(comp.value, 0);
    EXPECT_FLOAT_EQ(comp.data, 0.0F);

    comp.value = 55;
    comp.data = 5.5F;

    auto& retrieved = componentManager.getComponent<DataComponent>(entity);
    EXPECT_EQ(retrieved.value, 55);
    EXPECT_FLOAT_EQ(retrieved.data, 5.5F);
}

TEST_F(ComponentManagerTest, RemoveAndReaddComponent) {
    auto entity = entityManager.generateEntity();
    DataComponent data1{10, 1.0F};
    DataComponent data2{20, 2.0F};

    componentManager.addComponent<DataComponent>(entity, data1);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity).value, 10);

    componentManager.removeComponent<DataComponent>(entity);
    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));

    componentManager.addComponent<DataComponent>(entity, data2);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity).value, 20);
}

TEST_F(ComponentManagerTest, ThreeComponentTypes) {
    auto entity = entityManager.generateEntity();

    DataComponent data{42, 4.2F};
    PositionComponent pos{1.0F, 2.0F, 3.0F};
    VelocityComponent vel{0.1F, 0.2F, 0.3F};

    componentManager.addComponent<DataComponent>(entity, data);
    componentManager.addComponent<PositionComponent>(entity, pos);
    componentManager.addComponent<VelocityComponent>(entity, vel);

    EXPECT_TRUE(componentManager.hasComponent<DataComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<PositionComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<VelocityComponent>(entity));

    auto& retrievedData = componentManager.getComponent<DataComponent>(entity);
    auto& retrievedPos = componentManager.getComponent<PositionComponent>(entity);
    auto& retrievedVel = componentManager.getComponent<VelocityComponent>(entity);

    EXPECT_EQ(retrievedData.value, 42);
    EXPECT_FLOAT_EQ(retrievedPos.x, 1.0F);
    EXPECT_FLOAT_EQ(retrievedVel.vx, 0.1F);
}

TEST_F(ComponentManagerTest, ViewSingleComponent) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();
    auto entity3 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {1, 1.0F});
    componentManager.addComponent<DataComponent>(entity2, {2, 2.0F});
    componentManager.addComponent<DataComponent>(entity3, {3, 3.0F});

    auto view = componentManager.viewComponents<DataComponent>();

    EXPECT_EQ(view.size(), 3);
    EXPECT_EQ(std::get<0>(view[0]), entity1);
    EXPECT_EQ(std::get<1>(view[0]).value, 1);
    EXPECT_EQ(std::get<1>(view[1]).value, 2);
    EXPECT_EQ(std::get<1>(view[2]).value, 3);
}

TEST_F(ComponentManagerTest, ViewMultipleComponents) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {10, 1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {1.0F, 2.0F, 3.0F});

    componentManager.addComponent<DataComponent>(entity2, {20, 2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {4.0F, 5.0F, 6.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view[0]).value, 10);
    EXPECT_FLOAT_EQ(std::get<2>(view[0]).x, 1.0F);
    EXPECT_EQ(std::get<1>(view[1]).value, 20);
    EXPECT_FLOAT_EQ(std::get<2>(view[1]).x, 4.0F);
}

TEST_F(ComponentManagerTest, ViewFiltersEntitiesWithoutAllComponents) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();
    auto entity3 = entityManager.generateEntity();

    // entity1 has both components
    componentManager.addComponent<DataComponent>(entity1, {1, 1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {1.0F, 2.0F, 3.0F});

    // entity2 has only DataComponent
    componentManager.addComponent<DataComponent>(entity2, {2, 2.0F});

    // entity3 has both components
    componentManager.addComponent<DataComponent>(entity3, {3, 3.0F});
    componentManager.addComponent<PositionComponent>(entity3, {4.0F, 5.0F, 6.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view[0]).value, 1);
    EXPECT_EQ(std::get<1>(view[1]).value, 3);
}

TEST_F(ComponentManagerTest, ViewEmptyWhenNoEntitiesMatch) {
    auto entity1 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {1, 1.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 0);
}

TEST_F(ComponentManagerTest, ViewThreeComponentTypes) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {100, 1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {1.0F, 2.0F, 3.0F});
    componentManager.addComponent<VelocityComponent>(entity1, {0.1F, 0.2F, 0.3F});

    componentManager.addComponent<DataComponent>(entity2, {200, 2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {4.0F, 5.0F, 6.0F});
    componentManager.addComponent<VelocityComponent>(entity2, {0.4F, 0.5F, 0.6F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent, VelocityComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view[0]).value, 100);
    EXPECT_FLOAT_EQ(std::get<2>(view[0]).x, 1.0F);
    EXPECT_FLOAT_EQ(std::get<3>(view[0]).vx, 0.1F);
}

TEST_F(ComponentManagerTest, ViewAFterRemovingComponent) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {1, 1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {1.0F, 2.0F, 3.0F});

    componentManager.addComponent<DataComponent>(entity2, {2, 2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {4.0F, 5.0F, 6.0F});

    auto view1 = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(view1.size(), 2);

    componentManager.removeComponent<PositionComponent>(entity1);

    auto view2 = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(view2.size(), 1);
    EXPECT_EQ(std::get<1>(view2[0]).value, 2);
}

TEST_F(ComponentManagerTest, ViewConsistencyWithModiFications) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {1, 1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {1.0F, 2.0F, 3.0F});

    componentManager.addComponent<DataComponent>(entity2, {2, 2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {4.0F, 5.0F, 6.0F});

    // ModiFy component through getComponent (aFter all additions to avoid
    // invalidating reFerences)
    componentManager.getComponent<DataComponent>(entity1).value = 999;

    // VeriFy modiFication persists in storage
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity1).value, 999);

    // Get a Fresh view aFter modiFication
    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(std::get<1>(view[0]).value, 999);
}

} // namespace parteeengine
