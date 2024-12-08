#ifndef PPN_MICROBENCH_CACHE_HPP
#define PPN_MICROBENCH_CACHE_HPP

// Include necessary headers
#include <PPN-microbench/constants.hpp>
#include <PPN-microbench/microbench.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Alias for nlohmann::ordered_json
using json = nlohmann::ordered_json;

// Memory class inherits from Microbench
class Memory : public Microbench {
  private:
    // Vectors to store memory sizes and times
    std::vector<u64> mem_sizes;
    std::vector<u64> mem_times;

    // Function to execute the benchmark, overrides the base class method
    void executeBench() override;

  public:
    // Constructor to initialize the Memory object with a name and number of iterations
    Memory(std::string name, int nbIterations);
    
    // Destructor
    ~Memory();

    // Function to run the benchmark, overrides the base class method
    void run() override;
    
    // Function to get the results in JSON format, overrides the base class method
    json getJson() override;
};

#endif 