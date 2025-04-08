#include "../../algorithms/algorithm_base.h"

#include <httpserver.hpp>

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
