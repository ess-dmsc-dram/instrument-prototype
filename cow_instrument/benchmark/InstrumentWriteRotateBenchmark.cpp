#include "StandardInstrument.h"
#include "InstrumentTree.h"
#include "Node.h"
#include "Detector.h"
#include "RotateCommand.h"
#include <benchmark/benchmark_api.h>

namespace {

class WriteRotateFixture : public StandardInstrumentFixture {

public:
  void rotateOnNode(size_t nodeIndex, benchmark::State &state) {
    RotateCommand rotateIt{Eigen::Vector3d{0, 0, 1}, M_PI};
    while (state.KeepRunning()) {
      // Then modify that node
      m_instrument.modify(nodeIndex, rotateIt);
    }
    // For statistics. Mark the number of itertions
    state.SetItemsProcessed(state.iterations() * 1);
  }

  void rotateOnNodeWithCopy(size_t nodeIndex, benchmark::State &state) {
    RotateCommand rotateIt{Eigen::Vector3d{0, 0, 1}, M_PI};
    while (state.KeepRunning()) {
          // increase reference count on components
      auto temp = m_instrument;
      // Then modify that node
      temp.modify(nodeIndex, rotateIt);
    }
    // For statistics. Mark the number of itertions
    state.SetItemsProcessed(state.iterations() * 1);
  }
};

BENCHMARK_F(WriteRotateFixture,
            BM_cow_rotate_root)(benchmark::State &state) {
  this->rotateOnNodeWithCopy(0, state);
}

BENCHMARK_F(WriteRotateFixture,
            BM_cow_rotate_one_trolley)(benchmark::State &state) {
  this->rotateOnNodeWithCopy(1, state);
}

BENCHMARK_F(WriteRotateFixture,
            BM_cow_rotate_one_bank)(benchmark::State &state) {
  this->rotateOnNodeWithCopy(2, state);
}

BENCHMARK_F(WriteRotateFixture, BM_rotate_root)(benchmark::State &state) {
  this->rotateOnNode(0, state);
}

BENCHMARK_F(WriteRotateFixture,
            BM_rotate_one_trolley)(benchmark::State &state) {
  this->rotateOnNode(1, state);
}

BENCHMARK_F(WriteRotateFixture,
            BM_rotate_one_bank)(benchmark::State &state) {
  this->rotateOnNode(2, state);
}

} // namespace