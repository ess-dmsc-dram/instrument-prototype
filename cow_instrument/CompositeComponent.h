#ifndef COMPOSITE_COMPONENT_H
#define COMPOSITE_COMPONENT_H

#include "Component.h"
#include <vector>
#include <memory>
#include <string>

class Detector;

class CompositeComponent : public Component {
public:
  CompositeComponent(ComponentIdType componentId,
                     std::string name = std::string(""));
  ~CompositeComponent() = default;
  CompositeComponent(const CompositeComponent &other);
  CompositeComponent &operator=(const CompositeComponent &other);
  virtual Eigen::Vector3d getPos() const override;
  virtual Eigen::Quaterniond getRotation() const override;
  void shiftPositionBy(const Eigen::Vector3d &pos) override;
  void rotate(const Eigen::Vector3d& axis, const double& theta, const Eigen::Vector3d& center) override;
  void rotate(const Eigen::Affine3d &transform,
              const Eigen::Quaterniond &rotationPart) override;
  CompositeComponent *clone() const override;
  bool equals(const Component &other) const override;
  void addComponent(std::unique_ptr<Component>&& child);
  size_t size() const {return m_children.size();}
  const Component& getChild(size_t index) const;
  void registerContents(
      std::vector<const Detector *> &lookup,
      std::vector<const PathComponent *> &,
      std::vector<size_t> &detectorIndexes, std::vector<size_t> &pathIndexes,
      size_t previousIndex,
      std::vector<ComponentProxy> &componentProxies) const override;
  ComponentIdType componentId() const override;
  std::string name() const override;
  virtual bool accept(class ComponentVisitor *visitor) const override;
  std::vector<std::shared_ptr<Component>> children() const;
private:
  ComponentIdType m_componentId;
  std::vector<std::shared_ptr<Component>> m_children;
  std::string m_name;
};

using CompositeComponent_uptr = std::unique_ptr<const CompositeComponent>;

#endif
