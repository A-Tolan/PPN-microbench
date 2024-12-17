#include <PPN-microbench/context.hpp>
#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/ops/flops.hpp>
#include <PPN-microbench/ops/iops.hpp>
#include <PPN-microbench/memory.hpp>

#include <nlohmann/json.hpp>
#include <vector>

int main() {
    Memory memoryBenchmark;
    memoryBenchmark.run();
    json results = memoryBenchmark.getJson();
    std::cout << results.dump(4) << std::endl;
    return 0;
}