#include "workload_sinusoid.h"

#include <cmath>
#include <iostream>

std::vector<uint> WorkloadSinusoid::generate_requests_per_100ms() {
    std::vector<uint> requests_per_10ms;
    size_t delta = (rps_max_ - rps_min_);
    for (uint i = 0; i < duration_seconds_ * 100; ++i) {
        double progress = (double)i / (duration_seconds_ * 100);
        requests_per_10ms.push_back(std::round((rps_min_ + delta / 2 + std::round(std::sin(10 * 3.14159 * progress) * (delta / 2))) / 100));
    }
    return requests_per_10ms;
}
