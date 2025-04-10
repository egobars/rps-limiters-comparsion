#include "../../algorithms/algorithm_base.h"
#include "../../pipe/pipe.h"
#include "../../common/common.h"

#include <thread>
#include <optional>
#include <ctime>
#include <iostream>

/* http server worker
class WorkerServer;

class WorkerHandle : public httpserver::http_resource {
public:
    WorkerHandle(WorkerServer* parent)
        : parent_(parent) {
    }

    std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&);

private:
    WorkerServer* parent_;
};

class WorkerServer {
    friend WorkerHandle;
public:
    WorkerServer(uint port, Algorithm* algorithm);

    void start() {
        web_server_->start(false);
    }

private:
    std::unique_ptr<httpserver::webserver> web_server_;
    std::unique_ptr<WorkerHandle> worker_handle_resource_;
    std::unique_ptr<Algorithm> algorithm_;
};
*/

class Worker {
public:
    Worker(Pipe<Request>::PipeReader pipe_reader, Pipe<Response>::PipeWriter pipe_writer, std::shared_ptr<Algorithm> algorithm);

    void start() {
        running_ = true;
        worker_thread_ = std::make_unique<std::thread>([this]() {
            while (running_) {
                auto request = pipe_reader_.read();
                if (!request) {
                    continue;
                }

                bool result = algorithm_->check_request(request.value());
                auto val = request.value();
                Response response(val.id(), val.user(), val.timestamp(), std::time(nullptr), result);
                pipe_writer_.write(response);
            }
        });
    }

    void stop() {
        running_ = false;
        worker_thread_->join();
    }

private:
    std::shared_ptr<Algorithm> algorithm_;
    Pipe<Request>::PipeReader pipe_reader_;
    Pipe<Response>::PipeWriter pipe_writer_;
    bool running_ = false;
    std::unique_ptr<std::thread> worker_thread_;
};
