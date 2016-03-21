#ifndef INSTRUMENT_TREE_H
#define INSTRUMENT_TREE_H

#include <memory>
#include <map>

class Node;
class NodeIterator;
class Detector;

/*
 The instrument is nothing more than syntatic sugar over the root Node.
 */
class InstrumentTree {
public:

    InstrumentTree(std::unique_ptr<const Node>&& root);

    std::unique_ptr<NodeIterator> iterator() const;

    const Node& root() const;

    const Detector& getDetector(size_t detectorId) const;

    unsigned int version() const;

private:

    std::map<size_t, Detector const *> m_detectorMap;

    std::unique_ptr<const Node> m_root;

};

#endif
