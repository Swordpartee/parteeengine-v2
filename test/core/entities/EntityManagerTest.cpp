#include <gtest/gtest.h>

#include "core/entities/EntityManager.hpp"

class EntityManagerTest : public ::testing::Test {
  protected:
    parteeengine::EntityManager manager;
};

TEST_F(EntityManagerTest, EntityCreation) {
    auto entity1 = manager.generateEntity();
    EXPECT_EQ(entity1.id, 0);
    EXPECT_EQ(entity1.generation, 0);
}

TEST_F(EntityManagerTest, EntityValidation) {
    auto entity1 = manager.generateEntity();
    EXPECT_EQ(entity1.id, 0);
    EXPECT_EQ(entity1.generation, 0);

    EXPECT_TRUE(manager.isValidEntity(entity1));
}

TEST_F(EntityManagerTest, EntityDeletion) {
    auto entity1 = manager.generateEntity();
    EXPECT_EQ(entity1.id, 0);
    EXPECT_EQ(entity1.generation, 0);

    EXPECT_TRUE(manager.isValidEntity(entity1));

    manager.deleteEntity(entity1);
    EXPECT_FALSE(manager.isValidEntity(entity1));
}

TEST_F(EntityManagerTest, DeleteInvalidEntity) {
    auto entity = manager.generateEntity();

    manager.deleteEntity(entity);
    EXPECT_FALSE(manager.isValidEntity(entity));

    EXPECT_ANY_THROW(manager.deleteEntity(entity));
    EXPECT_FALSE(manager.isValidEntity(entity));
}

TEST_F(EntityManagerTest, DeleteNonExistentEntity) {
    parteeengine::Entity nonExistentEntity;
    nonExistentEntity.id = 999;
    nonExistentEntity.generation = 0;

    EXPECT_ANY_THROW(manager.deleteEntity(nonExistentEntity));
    EXPECT_FALSE(manager.isValidEntity(nonExistentEntity));
}

TEST_F(EntityManagerTest, IDReuseAfterDeletion) {
    auto entity1 = manager.generateEntity();
    EXPECT_EQ(entity1.id, 0);
    EXPECT_EQ(entity1.generation, 0);

    manager.deleteEntity(entity1);
    EXPECT_FALSE(manager.isValidEntity(entity1));

    // Create a new entity - should reuse ID 0 but with incremented generation
    auto entity2 = manager.generateEntity();
    EXPECT_EQ(entity2.id, 0);
    EXPECT_GT(entity2.generation, entity1.generation);

    // Old entity should still be invalid
    EXPECT_FALSE(manager.isValidEntity(entity1));
    // New entity should be valid
    EXPECT_TRUE(manager.isValidEntity(entity2));
}
