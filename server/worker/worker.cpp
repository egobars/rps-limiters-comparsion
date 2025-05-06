#include "worker.h"

#include <iostream>

/* http server worker handle
std::shared_ptr<httpserver::http_response> WorkerHandle::render(const httpserver::http_request& req) {
    std::string id_arg(req.get_arg("id"));
    uint id = atoi(id_arg.c_str());
    bool verdict = parent_->algorithm_->check_request(id, std::time(nullptr));
    return std::shared_ptr<httpserver::http_response>(
        verdict ?
        new httpserver::string_response("allowed\n") :
        new httpserver::string_response("rejected\n")
    );
}

WorkerServer::WorkerServer(uint port, Algorithm* algorithm) {
    worker_handle_resource_ = std::make_unique<WorkerHandle>(this);

    web_server_ = std::make_unique<httpserver::webserver>(httpserver::create_webserver(port));
    web_server_->register_resource("/limiter", worker_handle_resource_.get());

    algorithm_ = std::move(algorithm->clone());
}
*/

Worker::Worker(Pipe<Request>::PipeReader pipe_reader, Pipe<Response>::PipeWriter pipe_writer, Algorithm* algorithm, uint id)
    : pipe_reader_(pipe_reader)
    , pipe_writer_(pipe_writer)
    , algorithm_(algorithm)
    , id_(id)
    , rd_()
    , gen_(rd_())
    , dist_(0, 0) {
}
