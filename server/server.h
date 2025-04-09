#include "worker/worker.h"
#include "pipe/pipe.h"
#include "../common/common.h"

#include <thread>

/* http server
class Server;

class Handle : public httpserver::http_resource {
public:
    Handle(Server* parent)
        : parent_(parent) {
    }

    std::shared_ptr<httpserver::http_response> render(const httpserver::http_request& req);

private:
    Server* parent_;
};

class Server {
    friend Handle;
public:
    Server(uint port, Algorithm* algorithm, uint workers_number = 1);

    void start() {
        web_server_->start(false);
    }

private:
    uint get_worker_port(uint id) {
        return workers_ports_[id % workers_ports_.size()];
    }

    std::unique_ptr<httpserver::webserver> web_server_;
    std::unique_ptr<Handle> handle_resource_;
    std::vector<WorkerServer> workers_;
    std::vector<uint> workers_ports_;
};
*/

class Server {
public:
    Server(Pipe<Request>::PipeReader& pipe_reader, Algorithm* algorithm, uint workers_number = 1);

    void start() {
        running_ = true;
        std::thread server_thread([this]() {
            while (running_) {
                Request request = pipe_reader_.read();
                // Обработка запроса
            }
        });
    }

    void stop() {
        running_ = false;
    }

private:
    Pipe<Request>::PipeReader pipe_reader_;
    Algorithm* algorithm_;
    bool running_ = false;
};
