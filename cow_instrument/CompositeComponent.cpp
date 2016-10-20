#include "CompositeComponent.h"
#include "ComponentVisitor.h"
#include "Detector.h"

CompositeComponent::CompositeComponent(ComponentIdType componentId,
                                       std::string name)
    : m_componentId(componentId), m_name(name) {}

CompositeComponent::CompositeComponent(const CompositeComponent &other)
    : m_componentId(other.componentId()), m_name(other.name()) {
  for (auto &child : other.m_children) {
    m_children.emplace_back(child->clone());
  }
}

CompositeComponent &CompositeComponent::
operator=(const CompositeComponent &other) {
  m_componentId = other.componentId();
  m_name = other.name();
  m_children.clear();
  for (auto &child : other.m_children) {
    m_children.emplace_back(child->clone());
  }
  return *this;
}

Eigen::Vector3d CompositeComponent::getPos() const {

  /*
   An expensive operation on a composite component, but I don't think this
   is commonly
   */
  Eigen::Vector3d pos{0, 0, 0};
  for (size_t i = 0; i < m_children.size(); ++i) {
    auto childPos = m_children[i]->getPos();
    pos += childPos;
  }
  pos /= m_children.size();
  return pos;
}

void CompositeComponent::shiftPositionBy(const Eigen::Vector3d &delta) {
  for (size_t i = 0; i < m_children.size(); ++i) {
    m_children[i]->shiftPositionBy(delta);
  }
}

std::vector<std::shared_ptr<Component>> CompositeComponent::children() const {
  return m_children;
}

void CompositeComponent::rotate(const Eigen::Vector3d &axis,
                                const double &theta,
                                const Eigen::Vector3d &center) {
  using namespace Eigen;
  Affine3d transform =
      Translation3d(center) * AngleAxisd(theta, axis) * Translation3d(-center);
  Quaterniond rotationPart(transform.rotation());
  for (size_t i = 0; i < m_children.size(); ++i) {

    m_children[i]->rotate(transform, rotationPart);
  }
}

void CompositeComponent::rotate(const Eigen::Affine3d &transform,
                                const Eigen::Quaterniond &rotationPart) {
  for (size_t i = 0; i < m_children.size(); ++i) {

    m_children[i]->rotate(transform, rotationPart);
  }
}

CompositeComponent *CompositeComponent::clone() const {
  CompositeComponent *product = new CompositeComponent{m_componentId};
  for (size_t i = 0; i < this->size(); ++i) {
    product->addComponent(std::unique_ptr<Component>(m_children[i]->clone()));
  }
  return product;
}

bool CompositeComponent::equals(const Component &other) const {
  if (auto *otherComposite = dynamic_cast<const CompositeComponent *>(&other)) {
    if (otherComposite->size() != this->size()) {
      return false;
    }
    for (size_t i = 0; i < m_children.size(); ++i) {
      if (!m_children[i]->equals(*otherComposite->m_children[i]))
        return false;
    }
    return true;
  }
  return false;
}

void CompositeComponent::addComponent(std::unique_ptr<Component> &&child) {
  if (dynamic_cast<const CompositeComponent *>(child.get())) {
    throw std::invalid_argument(
        "Cannot add a composite component to a composite component");
  }
  m_children.emplace_back(std::move(child));
}

const Component &CompositeComponent::getChild(size_t index) const {
  if (index >= m_children.size()) {
    throw std::invalid_argument(
        "index out of range in CompositeComponent::getChild");
  } else {
    return *m_children[index];
  }
}

void CompositeComponent::registerContents(
    std::vector<const Detector *> &lookupDetectors,
    std::vector<const PathComponent *> &lookupPathComponents,
    std::vector<size_t> &detectorIndexes, std::vector<size_t> &pathIndexes) {
  for (auto &child : m_children) {
    child->registerContents(lookupDetectors, lookupPathComponents,
                            detectorIndexes, pathIndexes);
  }
}

ComponentIdType CompositeComponent::componentId() const {
  return m_componentId;
}

std::string CompositeComponent::name() const { return m_name; }

bool CompositeComponent::accept(ComponentVisitor *visitor) const {
  return visitor->visit(this);
}

Eigen::Quaterniond CompositeComponent::getRotation() const {
  throw std::runtime_error("Not implemented");
}
