#ifndef COMPONENTINFO_H
#define COMPONENTINFO_H

#include <vector>
#include <cstddef>
#include "ComponentProxy.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

class Detector;
class PathComponent;
class CompositeComponent;

/**
 * Collection of ComponentProxies. Which are the flattened tree representation
 * of the InstrumentTree. Other key information also stored here in the
 *flatterned floor.
 *
 * Methods for registering components (Flattened tree construction) and
 * navigating the subtrees have been provided.
 */
class ComponentInfo {
public:
  ComponentInfo() = default;
  void clear();
  void registerDetector(Detector const *const comp);
  void registerPathComponent(PathComponent const *const comp);
  size_t registerComposite(CompositeComponent const *const comp);
  void registerDetector(Detector const *const comp, size_t parentIndex);
  void registerPathComponent(PathComponent const *const comp,
                             size_t parentIndex);
  size_t registerComposite(CompositeComponent const *const comp,
                           size_t parentIndex);

  std::vector<ComponentProxy> proxies();
  const ComponentProxy &proxyAt(size_t index) const;
  const ComponentProxy &rootProxy() const;
  size_t componentSize() const;
  size_t detectorSize() const;
  size_t pathSize() const;
  std::vector<size_t> subTreeIndexes(size_t proxyIndex) const;
  std::vector<const Detector *> detectorComponents() const;
  std::vector<const PathComponent *> pathComponents() const;
  std::vector<size_t> pathComponentIndexes() const;
  std::vector<size_t> detectorComponentIndexes() const;
  size_t detIndexToCompIndex(size_t detectorIndex) const;
  size_t pathIndexToCompIndex(size_t pathIndex) const;
  std::vector<Eigen::Vector3d> startEntryPoints() const;
  std::vector<Eigen::Vector3d> startExitPoints() const;
  std::vector<double> pathLengths() const;
  std::vector<ComponentProxy>::const_iterator begin() const;
  std::vector<ComponentProxy>::const_iterator end() const;
  std::vector<ComponentProxy>::const_iterator cbegin() const;
  std::vector<ComponentProxy>::const_iterator cend() const;

  bool operator==(const ComponentInfo &other) const;
  bool operator!=(const ComponentInfo &other) const;

  std::vector<Eigen::Vector3d> startPositions() const;
  std::vector<Eigen::Quaterniond> startRotations() const;

private:
  size_t coreUpdate(Component const *const comp);
  size_t coreUpdate(Component const *const comp, size_t previousIndex);

  /*
   These collections have the same size as the number of components. They are
   component
   type independent
   */
  std::vector<ComponentProxy> m_proxies;
  std::vector<Eigen::Vector3d> m_positions;
  std::vector<Eigen::Quaterniond> m_rotations;

  /*
    These collections are conditionally updated depending upon component type.
    The vector
    of indexes allows us to go from say detector_index -> component_index.
   */
  std::vector<const Detector *> m_detectorComponents;
  std::vector<const PathComponent *> m_pathComponents;
  std::vector<Eigen::Vector3d> m_entryPoints; // For path components
  std::vector<Eigen::Vector3d> m_exitPoints;  // For path components
  std::vector<double> m_pathLengths; // For path components
  std::vector<size_t> m_pathComponentIndexes;
  std::vector<size_t> m_detectorComponentIndexes;
};

#endif