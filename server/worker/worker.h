#include "../../algorithms/algorithm_base.h"
#include "../../pipe/pipe.h"
#include "../../common/common.h"

#include <thread>
#include <optional>
#include <ctime>
#include <iostream>
#include <random>

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
    Worker(Pipe<Request>::PipeReader pipe_reader, Pipe<Response>::PipeWriter pipe_writer, Algorithm* algorithm, uint id);

    void start() {
        running_ = true;
        worker_thread_ = std::make_unique<std::thread>([this]() {
            int i = 0;
            while (running_) {
                /*auto response = algorithm_->get_response();
                if (response) {
                    pipe_writer_.write(response.value());
                }*/
                auto request = pipe_reader_.read();
                if (!request) {
                    continue;
                }

                bool result = algorithm_->check_request(request.value());
                auto val = request.value();
                //if (result) {
                    Response response(val.id(), val.user(), val.timestamp(), result, false, val.attempt(), 0);
                    pipe_writer_.write(response);
                /*} else {
                    if (val.attempt() == 0) {
                        Response response(val.id(), val.user(), val.timestamp(), result, true, val.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 500 + dist_(gen_));
                        pipe_writer_.write(response);
                    } else if (val.attempt() == 1) {
                        Response response(val.id(), val.user(), val.timestamp(), result, true, val.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 1500 + dist_(gen_));
                        pipe_writer_.write(response);
                    } else {
                        Response response(val.id(), val.user(), val.timestamp(), result, false, val.attempt(), 0);
                        pipe_writer_.write(response);
                    }
                }*/
                // algorithm_->add_request(request.value());
            }
        });
    }

    void stop() {
        running_ = false;
        worker_thread_->join();
    }

private:
    Algorithm* algorithm_;
    uint id_;
    Pipe<Request>::PipeReader pipe_reader_;
    Pipe<Response>::PipeWriter pipe_writer_;
    bool running_ = false;
    std::unique_ptr<std::thread> worker_thread_;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<uint> dist_;
};
