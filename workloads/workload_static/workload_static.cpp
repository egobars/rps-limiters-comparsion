#include "workload_static.h"

std::vector<uint> WorkloadStatic::generate_rps() {
    std::vector<uint> workload;
    for (uint i = 0; i < duration_seconds; i++) {
        workload.push_back(rps);
    }
    return workload;
}
