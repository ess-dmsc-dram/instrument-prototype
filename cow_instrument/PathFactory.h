#ifndef PATHFACTORY_H
#define PATHFACTORY_H

#include "Path.h"

class InstrumentTree;

/**
 * FactoryMethod. Makes all flight paths for an InstrumentTree.
 *
 */
class PathFactory {
public:
  virtual Paths create(const InstrumentTree &instrument) = 0;
  virtual ~PathFactory() {}
};

#endif
