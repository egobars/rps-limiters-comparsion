#pragma once

#include "../workload_base.h"

class WorkloadSpikes : public Workload {
public:
    WorkloadSpikes(uint duration_seconds, uint rps_base, uint rps_max)
        : duration_seconds_(duration_seconds)
        , rps_base_(rps_base)
        , rps_max_(rps_max)
    {}

    std::vector<uint> generate_requests_per_100ms() override;

private:
    uint duration_seconds_;
    uint rps_base_;
    uint rps_max_;
};
