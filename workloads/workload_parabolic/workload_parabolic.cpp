#include "workload_parabolic.h"

#include <cmath>

std::vector<uint> WorkloadParabolic::generate_requests_per_100ms() {
    std::vector<uint> requests_per_10ms;
    size_t delta = (rps_max_ - rps_min_);
    for (uint i = 0; i < duration_seconds_ * 100; ++i) {
        double progress = (double)i / (duration_seconds_ * 100);
        requests_per_10ms.push_back((rps_min_ + std::sin(3.14 * progress) * delta) / 100);
    }
    return requests_per_10ms;
}
