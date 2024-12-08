#include <PPN-microbench/memory.hpp>


// Function to generate a random index within a given threshold
inline u64 random_index(u64 threshold) { return rand() % threshold; }

// Function to measure cache/memory latency using pointer chasing
double measure_latency(u64 size, u64 nbIterations) {
    std::vector<void *> memblock(size);

    // Initialize the memory block with pointers to itself
    for (u64 i = 0; i < size; ++i) {
        memblock[i] = &memblock[i];
    }

    // Shuffle the pointers to create a random access pattern
    for (u64 i = size - 1; i > 0; --i) {
        u64 j = random_index(i + 1);
        std::swap(memblock[i], memblock[j]);
    }

    double total_latency = 0.0;

    // Perform the latency measurement 10 times for better accuracy
    for (int run = 0; run < 10; ++run) {
        void *p = memblock[0];
        auto start = std::chrono::high_resolution_clock::now();

        // Pointer chasing loop
        for (u64 i = 0; i < nbIterations; ++i) {
            p = *(void **)p;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;

        // Accumulate the latency
        total_latency += elapsed.count() / nbIterations;
    }

    // Calculate the average latency over 10 runs
    return total_latency / 10.0;
}

// Constructor
Memory::Memory(std::string name, int nbIterations)
    : Microbench(name, nbIterations) {
    srand(time(nullptr));
}

// Destructor
Memory::~Memory() {}

// Execute the benchmark
void Memory::executeBench() {
    // Define the sizes to test (in B)
    for (u64 size = 512; size <= 10e8 / sizeof(void *); size *= 2) {
        mem_sizes.push_back(size);
    }
    u64 nbIterations = 1000000;

    for (u64 size_B : mem_sizes) {
        // u64 size = size_B / sizeof(void *);
        double latency = measure_latency(size_B, nbIterations);
        mem_times.push_back(latency);
    }
}

// Run the benchmark
void Memory::run() { executeBench(); }

// Get the results in JSON format
json Memory::getJson() {
    json result1, result2, result;
    for (size_t i = 0; i < mem_sizes.size(); ++i) {
        result1["Memory Size (B)"].push_back(mem_sizes[i]);
        result2["Latency (ns)"].push_back(mem_times[i]);
        result["Memory Size (B), Latency (ns)"] = {result1, result2};
    }
    return result;
}
