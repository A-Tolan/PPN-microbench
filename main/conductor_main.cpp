#include <PPN-microbench/conductor.hpp>
#include <PPN-microbench/memory.hpp>


int main() {

    Conductor conductor;

    conductor.addBench(new Memory("Memory",1))
        .setOutputFile("../tmp/out.json")
        .run()
        .save()
        .print();

    return 0;
}
