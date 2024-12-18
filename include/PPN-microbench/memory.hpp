#ifndef PPN_MICROBENCH_CACHE_HPP
#define PPN_MICROBENCH_CACHE_HPP

// Include necessary headers
#include <PPN-microbench/constants.hpp>
#include <PPN-microbench/microbench.hpp>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <numeric>
#include <vector>

// Memory class inherits from Microbench
class Memory : public Microbench {
  private:
    // Vectors to store memory sizes and times
    std::vector<u64> mem_sizes = std::vector<u64>(21);
    std::vector<u64> mem_times;

    // Function to execute the benchmark, overrides the base class method
    void executeBench() override;

  public:
    // Constructor
    Memory();

    // Destructor
    ~Memory();

    // Function to run the benchmark, overrides the base class method
    void run() override;

    // Function to get the results in JSON format, overrides the base class
    // method
    json getJson() override;
};

#endif