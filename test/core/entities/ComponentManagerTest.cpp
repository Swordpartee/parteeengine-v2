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
    const DataComponent data{.value=42, .data=3.14F};

    auto& component = componentManager.addComponent<DataComponent>(entity, data);

    EXPECT_EQ(component.value, 42);
    EXPECT_FLOAT_EQ(component.data, 3.14F);
}

TEST_F(ComponentManagerTest, GetComponent) {
    auto entity = entityManager.generateEntity();
    const DataComponent data{.value=99, .data=2.71F};

    auto& component = componentManager.addComponent<DataComponent>(entity, data);

    EXPECT_EQ(component.value, 99);
    EXPECT_FLOAT_EQ(component.data, 2.71F);
}

TEST_F(ComponentManagerTest, GetNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity), nullptr);
}

TEST_F(ComponentManagerTest, RemoveNonexistentCompoent) {
    auto entity = entityManager.generateEntity();

    EXPECT_FALSE(componentManager.removeComponent<DataComponent>(entity));
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

    const DataComponent data1{.value=1, .data=1.0F};
    const DataComponent data2{.value=2, .data=2.0F};
    const DataComponent data3{.value=3, .data=3.0F};

    componentManager.addComponent<DataComponent>(entity1, data1);
    componentManager.addComponent<DataComponent>(entity2, data2);
    componentManager.addComponent<DataComponent>(entity3, data3);

    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity1)->value, 1);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity2)->value, 2);
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity3)->value, 3);
}

TEST_F(ComponentManagerTest, ComponentDataModiFication) {
    auto entity = entityManager.generateEntity();

    auto& component = componentManager.addComponent<DataComponent>(entity);

    EXPECT_EQ(component.value, 0);
    EXPECT_FLOAT_EQ(component.data, 0.0F);

    component.value = 55;
    component.data = 5.5F;

    EXPECT_EQ(component.value, 55);
    EXPECT_FLOAT_EQ(component.data, 5.5F);
}

TEST_F(ComponentManagerTest, RemoveAndReaddComponent) {
    auto entity = entityManager.generateEntity();
    const DataComponent data1{.value=10, .data=1.0F};
    const DataComponent data2{.value=20, .data=2.0F};

    auto& component = componentManager.addComponent<DataComponent>(entity, data1);
    EXPECT_EQ(component.value, 10);

    componentManager.removeComponent<DataComponent>(entity);
    EXPECT_FALSE(componentManager.hasComponent<DataComponent>(entity));

    component = componentManager.addComponent<DataComponent>(entity, data2);
    EXPECT_EQ(component.value, 20);
}

TEST_F(ComponentManagerTest, ThreeComponentTypes) {
    auto entity = entityManager.generateEntity();

    const DataComponent data{.value=42, .data=4.2F};
    const PositionComponent pos{.x=1.0F, .y=2.0F, .z=3.0F};
    const VelocityComponent vel{.vx=0.1F, .vy=0.2F, .vz=0.3F};

    auto& dataComponent = componentManager.addComponent<DataComponent>(entity, data);
    auto& posComponent = componentManager.addComponent<PositionComponent>(entity, pos);
    auto& velComponent = componentManager.addComponent<VelocityComponent>(entity, vel);

    EXPECT_TRUE(componentManager.hasComponent<DataComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<PositionComponent>(entity));
    EXPECT_TRUE(componentManager.hasComponent<VelocityComponent>(entity));

    EXPECT_EQ(dataComponent.value, 42);
    EXPECT_FLOAT_EQ(posComponent.x, 1.0F);
    EXPECT_FLOAT_EQ(velComponent.vx, 0.1F);
}

TEST_F(ComponentManagerTest, ViewSingleComponent) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();
    auto entity3 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=1, .data=1.0F});
    componentManager.addComponent<DataComponent>(entity2, {.value=2, .data=2.0F});
    componentManager.addComponent<DataComponent>(entity3, {.value=3, .data=3.0F});

    auto view = componentManager.viewComponents<DataComponent>();

    //NOLINTBEIng

    EXPECT_EQ(view.size(), 3);
    EXPECT_EQ(std::get<0>(view.at(0)), entity1);
    EXPECT_EQ(std::get<1>(view.at(0)).value, 1);
    EXPECT_EQ(std::get<1>(view.at(1)).value, 2);
    EXPECT_EQ(std::get<1>(view.at(2)).value, 3);
}

TEST_F(ComponentManagerTest, ViewMultipleComponents) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=10, .data=1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {.x=1.0F, .y=2.0F, .z=3.0F});

    componentManager.addComponent<DataComponent>(entity2, {.value=20, .data=2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {.x=4.0F, .y=5.0F, .z=6.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view.at(0)).value, 10);
    EXPECT_FLOAT_EQ(std::get<2>(view.at(0)).x, 1.0F);
    EXPECT_EQ(std::get<1>(view.at(1)).value, 20);
    EXPECT_FLOAT_EQ(std::get<2>(view.at(1)).x, 4.0F);
}

TEST_F(ComponentManagerTest, ViewFiltersEntitiesWithoutAllComponents) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();
    auto entity3 = entityManager.generateEntity();

    // entity1 has both components
    componentManager.addComponent<DataComponent>(entity1, {.value=1, .data=1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {.x=1.0F, .y=2.0F, .z=3.0F});

    // entity2 has only DataComponent
    componentManager.addComponent<DataComponent>(entity2, {.value=2, .data=2.0F});

    // entity3 has both components
    componentManager.addComponent<DataComponent>(entity3, {.value=3, .data=3.0F});
    componentManager.addComponent<PositionComponent>(entity3, {.x=4.0F, .y=5.0F, .z=6.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view.at(0)).value, 1);
    EXPECT_EQ(std::get<1>(view.at(1)).value, 3);
}

TEST_F(ComponentManagerTest, ViewEmptyWhenNoEntitiesMatch) {
    auto entity1 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=1, .data=1.0F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();

    EXPECT_EQ(view.size(), 0);
}

TEST_F(ComponentManagerTest, ViewThreeComponentTypes) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=100, .data=1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {.x=1.0F, .y=2.0F, .z=3.0F});
    componentManager.addComponent<VelocityComponent>(entity1, {.vx=0.1F, .vy=0.2F, .vz=0.3F});

    componentManager.addComponent<DataComponent>(entity2, {.value=200, .data=2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {.x=4.0F, .y=5.0F, .z=6.0F});
    componentManager.addComponent<VelocityComponent>(entity2, {.vx=0.4F, .vy=0.5F, .vz=0.6F});

    auto view = componentManager.viewComponents<DataComponent, PositionComponent, VelocityComponent>();

    EXPECT_EQ(view.size(), 2);
    EXPECT_EQ(std::get<1>(view.at(0)).value, 100);
    EXPECT_FLOAT_EQ(std::get<2>(view.at(0)).x, 1.0F);
    EXPECT_FLOAT_EQ(std::get<3>(view.at(0)).vx, 0.1F);
}

TEST_F(ComponentManagerTest, ViewAFterRemovingComponent) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=1, .data=1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {.x=1.0F, .y=2.0F, .z=3.0F});

    componentManager.addComponent<DataComponent>(entity2, {.value=2, .data=2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {.x=4.0F, .y=5.0F, .z=6.0F});

    auto view1 = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(view1.size(), 2);

    componentManager.removeComponent<PositionComponent>(entity1);

    auto view2 = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(view2.size(), 1);
    EXPECT_EQ(std::get<1>(view2.at(0)).value, 2);
}

TEST_F(ComponentManagerTest, ViewConsistencyWithModiFications) {
    auto entity1 = entityManager.generateEntity();
    auto entity2 = entityManager.generateEntity();

    componentManager.addComponent<DataComponent>(entity1, {.value=1, .data=1.0F});
    componentManager.addComponent<PositionComponent>(entity1, {.x=1.0F, .y=2.0F, .z=3.0F});

    componentManager.addComponent<DataComponent>(entity2, {.value=2, .data=2.0F});
    componentManager.addComponent<PositionComponent>(entity2, {.x=4.0F, .y=5.0F, .z=6.0F});

    // ModiFy component through getComponent (aFter all additions to avoid
    // invalidating reFerences)
    componentManager.getComponent<DataComponent>(entity1)->value = 999;

    // VeriFy modiFication persists in storage
    EXPECT_EQ(componentManager.getComponent<DataComponent>(entity1)->value, 999);

    // Get a Fresh view aFter modiFication
    auto view = componentManager.viewComponents<DataComponent, PositionComponent>();
    EXPECT_EQ(std::get<1>(view.at(0)).value, 999);
}

} // namespace parteeengine
