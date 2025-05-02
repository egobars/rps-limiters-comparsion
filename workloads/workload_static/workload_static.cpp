#include "workload_static.h"

std::vector<uint> WorkloadStatic::generate_requests_per_100ms() {
    std::vector<uint> workload;
    for (uint i = 0; i < duration_seconds * 100; i++) {
        workload.push_back(rps / 100);
    }
    return workload;
}
