#include "StandardInstrument.h"
#include "InstrumentTree.h"
#include "Node.h"
#include "Detector.h"

#include <benchmark/benchmark_api.h>

namespace {

class ReadFixture : public benchmark::Fixture {
public:
  InstrumentTree m_instrument;

  ReadFixture()
      : benchmark::Fixture(),
        m_instrument(std_instrument::construct_root_node()) {}
};

void BM_InstrumentTreeConstruction(benchmark::State &state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto flattenedNodeTree = std_instrument::construct_root_node();
    state.ResumeTiming();

    InstrumentTree instrument(std::move(flattenedNodeTree));
  }
  state.SetItemsProcessed(state.iterations() * 1);
}
BENCHMARK(BM_InstrumentTreeConstruction);

BENCHMARK_F(ReadFixture, BM_SingleAccessMetric)(benchmark::State &state) {
  while (state.KeepRunning()) {
    size_t max = 100 * 100 * 6;
    double pos_x = 0;
    for (size_t i = 1; i < max; ++i) {
      const auto &det = m_instrument.getDetector(i);
      benchmark::DoNotOptimize(pos_x += det.getPos()[0]);
    }
  }
  state.SetItemsProcessed(state.iterations() * m_instrument.nDetectors());
}

} // namespace

BENCHMARK_MAIN()
