#include "gtest/gtest.h"
#include "CompositeComponent.h"
#include "MockTypes.h"

using namespace testing;

namespace {

TEST(composite_component_test, test_construction){
    CompositeComponent composite{ComponentIdType(1)};
    EXPECT_EQ(0, composite.size());
}

TEST(composite_component_test, test_provide_name) {
  const std::string name = "bank_x";
  CompositeComponent composite{ComponentIdType(1), name};
  EXPECT_EQ(composite.name(), name);
}

TEST(composite_component_test, test_clone){
    CompositeComponent composite{ComponentIdType(1)};

    MockComponent* childA = new MockComponent;
    EXPECT_CALL(*childA, clone()).WillRepeatedly(Return(new MockComponent));
    MockComponent* childB = new MockComponent;
    EXPECT_CALL(*childB, clone()).WillRepeatedly(Return(new MockComponent));

    composite.addComponent(std::unique_ptr<MockComponent>(childA));
    composite.addComponent(std::unique_ptr<MockComponent>(childB));
    auto* clone = composite.clone();
    EXPECT_EQ(clone->size(), composite.size());
    delete clone;
    EXPECT_TRUE(Mock::VerifyAndClearExpectations(childA));
    EXPECT_TRUE(Mock::VerifyAndClearExpectations(childB));
}

TEST(composite_component_test, test_get_pos){

    MockComponent* childA = new MockComponent;
    EXPECT_CALL(*childA, getPos()).WillRepeatedly(Return(Eigen::Vector3d{1,1,1}));
    MockComponent* childB = new MockComponent;
    EXPECT_CALL(*childB, getPos()).WillRepeatedly(Return(Eigen::Vector3d{2,2,2}));
    MockComponent* childC = new MockComponent;
    EXPECT_CALL(*childC, getPos()).WillRepeatedly(Return(Eigen::Vector3d{3,3,3}));

    CompositeComponent composite{ComponentIdType(1)};
    composite.addComponent(std::unique_ptr<MockComponent>(childA));
    composite.addComponent(std::unique_ptr<MockComponent>(childB));
    composite.addComponent(std::unique_ptr<MockComponent>(childC));

    Eigen::Vector3d pos = composite.getPos();
    EXPECT_EQ(pos[0],2);
    EXPECT_EQ(pos[1],2);
    EXPECT_EQ(pos[2],2);
}

TEST(composite_component_test, test_shiftPos) {
  using namespace testing;
  MockComponent *child = new MockComponent;
  EXPECT_CALL(*child, shiftPositionBy(_)).Times(1);

  CompositeComponent composite{ComponentIdType(1)};
  composite.addComponent(std::unique_ptr<MockComponent>(child));

  composite.shiftPositionBy(Eigen::Vector3d{1, 1, 1});
  EXPECT_TRUE(Mock::VerifyAndClearExpectations(child));
}

TEST(composite_component_test, test_rotate) {
  using namespace testing;
  MockComponent *child = new MockComponent;
  EXPECT_CALL(*child, rotate(_, _)).Times(1);

  CompositeComponent composite{ComponentIdType(1)};
  composite.addComponent(std::unique_ptr<MockComponent>(child));

  const Eigen::Vector3d axis{1, 1, 1};
  const auto theta = M_PI / 2;
  const Eigen::Vector3d center{1, 1, 1};

  composite.rotate(axis, theta, center);
  EXPECT_TRUE(Mock::VerifyAndClearExpectations(child));
}

TEST(composite_component_test, test_rotate_fast) {
  using namespace testing;
  MockComponent *child = new MockComponent;
  EXPECT_CALL(*child, rotate(_, _)).Times(1);

  CompositeComponent composite{ComponentIdType(1)};
  composite.addComponent(std::unique_ptr<MockComponent>(child));

  Eigen::Affine3d transform = Eigen::Affine3d::Identity();
  Eigen::Quaterniond rotation(transform.rotation());
  // Overload for fast rotations
  composite.rotate(transform, rotation);
  EXPECT_TRUE(Mock::VerifyAndClearExpectations(child));
}

} // namespace
