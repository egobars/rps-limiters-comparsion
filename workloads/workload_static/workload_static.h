#pragma once

#include "../workload_base.h"

class WorkloadStatic : public Workload {
public:
    WorkloadStatic(uint duration_seconds, uint rps)
        : duration_seconds(duration_seconds)
        , rps(rps)
    {}

    std::vector<uint> generate_requests_per_10ms() override;

private:
    uint duration_seconds;
    uint rps;
};
