#pragma once

#include "../workload_base.h"

class WorkloadSinusoid : public Workload {
public:
    WorkloadSinusoid(uint duration_seconds, uint rps_min, uint rps_max)
        : duration_seconds_(duration_seconds)
        , rps_min_(rps_min)
        , rps_max_(rps_max)
    {}

    std::vector<uint> generate_requests_per_100ms() override;

private:
    uint duration_seconds_;
    uint rps_min_;
    uint rps_max_;
};
