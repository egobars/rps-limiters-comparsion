#include "workload_sinusoid.h"

#include <cmath>
#include <iostream>

std::vector<uint> WorkloadSinusoid::generate_requests_per_10ms() {
    std::vector<uint> requests_per_10ms;
    size_t delta = (rps_max_ - rps_min_);
    for (uint i = 0; i < duration_seconds_ * 100; ++i) {
        double progress = (double)i / (duration_seconds_ * 100);
        requests_per_10ms.push_back((rps_min_ + delta / 2 + std::sin(16 * 3.14159 * progress) * (delta / 2)) / 100);
    }
    return requests_per_10ms;
}
