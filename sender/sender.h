#pragma once

#include "../workloads/workload_base.h"
#include <cstdint>

class Sender {
public:
    Sender(Workload* workload, uint32_t port)
        : workload(workload)
        , port(port)
    {}

    void start_execution();

private:
    void send_request();

    Workload* workload;
    uint32_t port;
};
