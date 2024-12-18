#include <PPN-microbench/cpu_frequency.hpp>

CPUFrequency::CPUFrequency(int nbMeasures) : Microbench("CPU Frequency", 777777){
    this->nbMeasures = nbMeasures;
    Context context = Context::getInstance();
    nbCores = context.getCpus();
    measures = std::make_unique<double[]>(nbMeasures * ((nbCores * (nbCores + 1)) / 2));
}

CPUFrequency::~CPUFrequency() {}

void CPUFrequency::executeAdds() {
    int cpt = 0;
    for (int i = 0; i < getNbIterations(); i++) {
        // 32 adds
        cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
        cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
        cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
        cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
    }
}

json CPUFrequency::getJson() {
    json cpuSpeedJson = json::object();
    cpuSpeedJson["name"] = getName();
    for (int id = 1; id <= nbTestingCores; id++) {
        for (int i = 0; i < nbMeasures * id; i++) {
            cpuSpeedJson["results"]["Cores" + std::to_string(id)][i/nbMeasures] += measures[id * nbCores + i];
        }
    }
    return cpuSpeedJson;
}

void CPUFrequency::run() {
    Context context = Context::getInstance();
    std::vector<size_t> threadMapping = context.getThreadMapping();

    cpu_set_t cpusets[nbCores];

    for (size_t i = 0; i < nbCores; i++) {
        CPU_ZERO(&cpusets[i]);
        CPU_SET(threadMapping[i], &cpusets[i]);
    }

    std::thread threads[nbCores];

    // To stop earlier if it's needed (but protection if maxCores is bigger than the cores count)
    nbTestingCores = threadMapping.size();

    for (int coresToExecute = 1; coresToExecute <= nbTestingCores; coresToExecute++) { // Main for, equivalent to a graph
        for (int coresExecuted = 1; coresExecuted <= coresToExecute; coresExecuted++) { // For every core count, equivalent to a point in a graph
            for (int sample = -5; sample < nbMeasures; sample++) { // 5 Warmup runs and samples to average tests (in python, later)
                // Execute on 1 Core, then 2 Cores, 3 Cores, etc...
                auto start = std::chrono::steady_clock::now();

                for (int id = 0; id < coresExecuted; id++) {
                    // To call the threads (only 1;  1 and 2;  1, 2 and 3;  etc...)
                    threads[id] = std::thread([this] {
                            this->executeAdds();
                    });
                    pthread_setaffinity_np(threads[id].native_handle(),
                                        sizeof(cpu_set_t), &cpusets[id]);
                }
            
                for (int id = 0; id < coresExecuted; id++) {
                    // Waiting for the threads
                    threads[id].join();
                }

                u64 duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count();
                if (sample >= 0) {
                    measures[(coresToExecute * (coresToExecute - 1) / 2) * nbMeasures + (coresExecuted - 1) * nbMeasures + sample] = ((32.f * getNbIterations()) / duration);
                }
            }
        }
    }
}
