#include "test_benchmark_instance.hpp"

#include <vector>

#include <cs/benchmark/benchmark.hpp>

void voidTest() {}
bool boolTest() { return true; }
std::vector<int> vectorTest() { return std::vector<int>{}; }

cs::details::TestBenchmarkInstance::TestBenchmarkInstance() {
    cs::Benchmark benchmark;
    benchmark.execute(&voidTest);
    benchmark.execute(&boolTest);
    benchmark.execute(&vectorTest);
}
