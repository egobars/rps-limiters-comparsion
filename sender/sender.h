#pragma once

#include "../workloads/workload_base.h"
#include "../pipe/pipe.h"
#include "../common/common.h"

#include <cstdint>
#include <thread>
#include <atomic>
#include <set>

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
    Sender(Workload* workload, Pipe<Request>::PipeWriter& pipe_writer_requests, Pipe<Retry>::PipeReader& pipe_reader_retries)
        : workload_(workload)
        , pipe_writer_requests_(pipe_writer_requests)
        , pipe_reader_retries_(pipe_reader_retries)
        , is_running_(false)
    {}

    void start_execution();

private:
    void send_request();
    void check_retries();

    Workload* workload_;
    Pipe<Request>::PipeWriter pipe_writer_requests_;
    Pipe<Retry>::PipeReader pipe_reader_retries_;
    std::atomic<uint> current_id_ = 0;
    uint current_user_ = 0;
    std::thread retries_thread_;
    std::atomic<bool> is_running_;
    std::set<Retry> retries_;
};
