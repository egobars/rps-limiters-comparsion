#pragma once

#include <sys/types.h>
#include <vector>

class Workload {
public:
    virtual std::vector<uint> generate_rps() = 0;
};
