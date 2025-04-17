#include "server.h"

/* http server
std::shared_ptr<httpserver::http_response> Handle::render(const httpserver::http_request& req) {
    std::string id_arg(req.get_arg("id"));
    uint id = atoi(id_arg.c_str());
    std::stringstream ss;
    ss << "http://localhost:" << parent_->get_worker_port(id) << "/limiter?id=" << id_arg;
    std::string url = ss.str();
    http::Request request{url};
    const auto response = request.send("GET");
    return std::shared_ptr<httpserver::http_response>(
        new httpserver::string_response(std::string{response.body.begin(), response.body.end()})
    );
}

Server::Server(uint port, Algorithm* algorithm, uint workers_number) {
    web_server_ = std::make_unique<httpserver::webserver>(httpserver::create_webserver(port));
    for (uint i = 0; i < workers_number; ++i) {
        uint worker_port = port + i + 1;
        workers_.emplace_back(worker_port, algorithm);
        workers_.back().start();
        workers_ports_.push_back(worker_port);
    }
    handle_resource_ = std::make_unique<Handle>(this);
    web_server_->register_resource("/", handle_resource_.get());
}
*/

Server::Server(Pipe<Request>::PipeReader& pipe_reader, std::vector<Algorithm*>* algorithms, std::shared_ptr<LogsJournal> logs_journal, uint workers_number) 
    : pipe_reader_(pipe_reader)
    , logs_journal_(logs_journal)
    , algorithms_(algorithms) {
    gen = std::mt19937(rd());
    dist = std::uniform_int_distribution<uint>(0, workers_number - 1);
    for (uint i = 0; i < workers_number; ++i) {
        auto pipe_to_worker = std::make_shared<Pipe<Request>>();
        auto pipe_from_worker = std::make_shared<Pipe<Response>>();
        workers_readers_.push_back(pipe_from_worker->GetReader());
        workers_writers_.push_back(pipe_to_worker->GetWriter());
        workers_.push_back(std::make_unique<Worker>(
            pipe_to_worker->GetReader(), 
            pipe_from_worker->GetWriter(), 
            algorithms_->at(i),
            i
        ));
    }
}
