#include "workload_spikes.h"

#include <cmath>
#include <iostream>

std::vector<uint> WorkloadSpikes::generate_requests_per_100ms() {
    std::vector<uint> requests_per_10ms;
    auto third_duration = duration_seconds_ * 100 / 3;
    for (uint i = 0; i < duration_seconds_ * 100; ++i) {
        if (i > third_duration - 3 && i < third_duration + 3 || i > 2 * third_duration - 3 && i < 2 * third_duration + 3) {
            requests_per_10ms.push_back(rps_max_ / 100);
        } else {
            requests_per_10ms.push_back(rps_base_ / 100);
        }
    }
    return requests_per_10ms;
}
