#pragma once

#include <sys/types.h>
#include <vector>

class Workload {
public:
    virtual std::vector<uint> generate_requests_per_10ms() = 0;
};
