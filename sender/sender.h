#pragma once

#include "../workloads/workload_base.h"
#include "../pipe/pipe.h"
#include "../common/common.h"

#include <cstdint>

/* Http server sender
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
*/

class Sender {
public:
    Sender(Workload* workload, Pipe<Request>::PipeWriter& pipe_writer)
        : workload_(workload)
        , pipe_writer_(pipe_writer)
    {}

    void start_execution();

private:
    void send_request();

    Workload* workload_;
    Pipe<Request>::PipeWriter pipe_writer_;
    uint current_id_ = 0;
};
