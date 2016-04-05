#include "StandardInstrument.h"
#include "CompositeComponent.h"
#include "cow_ptr.h"
#include "DetectorComponent.h"
#include "Node.h"
#include "NullComponent.h"

namespace {

std::shared_ptr<CompositeComponent> make_square_bank(size_t width,
                                                     size_t height) {
  static DetectorIdType detectorId(1);
  static ComponentIdType componentId(1);
  auto bank = std::make_shared<CompositeComponent>(ComponentIdType(0));

  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < height; ++j) {
      bank->addComponent(std::unique_ptr<DetectorComponent>(
          new DetectorComponent(componentId++, detectorId++,
                                V3D{double(i), double(j), double(0)})));
    }
  }
  bank->deltaPos(V3D{-double(width) / 2, -double(height) / 2, 0}); // Center it
  return bank;
}
}

namespace std_instrument {

std::unique_ptr<Node> construct_root_node() {
  /*
          instrument
          |
   --------------------------------------------
                      |                      |
                front_trolley           rear_trolley
                          |                       |
               ________________________       ________________________
              |       |       |       |       |                     |
              N       S       E       W       l_curtain            r_curtain
  */

  const size_t width = 100;
  const size_t height = 100;
  const double width_d = double(width);
  const double height_d = double(height);

  auto N = make_square_bank(width, height);
  N->deltaPos(V3D{0, height_d, 3});
  auto E = make_square_bank(width, height);
  E->deltaPos(V3D{-width_d, 0, 3});
  auto S = make_square_bank(width, height);
  S->deltaPos(V3D{0, -height_d, 3});
  auto W = make_square_bank(width, height);
  E->deltaPos(V3D{width_d, 0, 3});

  auto l_curtain = make_square_bank(width, height);
  l_curtain->deltaPos(V3D{-width_d, 0, 6});
  auto r_curtain = make_square_bank(width, height);
  r_curtain->deltaPos(V3D{width_d, 0, 6});

  Node_uptr root(new Node(CowPtr<Component>(new NullComponent)));
  Node_uptr front_trolley(new Node(CowPtr<Component>(new NullComponent)));
  Node_uptr nodeN(new Node(front_trolley.get(), CowPtr<Component>(N)));
  Node_uptr nodeE(new Node(front_trolley.get(), CowPtr<Component>(E)));
  Node_uptr nodeS(new Node(front_trolley.get(), CowPtr<Component>(S)));
  Node_uptr nodeW(new Node(front_trolley.get(), CowPtr<Component>(W)));
  Node_uptr rear_trolley(new Node(CowPtr<Component>(new NullComponent)));
  Node_uptr nodeLCurtain(
      new Node(rear_trolley.get(), CowPtr<Component>(l_curtain)));
  Node_uptr nodeRCurtain(
      new Node(rear_trolley.get(), CowPtr<Component>(r_curtain)));
  front_trolley->addChild(std::move(nodeN));
  front_trolley->addChild(std::move(nodeE));
  front_trolley->addChild(std::move(nodeS));
  front_trolley->addChild(std::move(nodeW));
  rear_trolley->addChild(std::move(nodeLCurtain));
  front_trolley->addChild(std::move(nodeRCurtain));
  root->addChild(std::move(front_trolley));
  root->addChild(std::move(rear_trolley));

  return root;
}
}
