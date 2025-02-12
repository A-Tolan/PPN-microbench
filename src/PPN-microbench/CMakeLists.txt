add_library(PPN-microbench STATIC)

target_sources(PPN-microbench PRIVATE microbench.cpp context.cpp conductor.cpp ops.cpp cpu_frequency.cpp memory.cpp)

target_sources(
    PPN-microbench
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES 
            ${PROJECT_SOURCE_DIR}/include/PPN-microbench/microbench.hpp
            ${PROJECT_SOURCE_DIR}/include/PPN-microbench/context.hpp
            ${PROJECT_SOURCE_DIR}/include/PPN-microbench/conductor.hpp
            ${PROJECT_SOURCE_DIR}/include/PPN-microbench/ops.hpp
            ${PROJECT_SOURCE_DIR}/include/PPN-microbench/memory.hpp
)

set_target_properties(PPN-microbench PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
target_link_libraries(PPN-microbench PRIVATE nlohmann_json::nlohmann_json OpenMP::OpenMP_CXX)
