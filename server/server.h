#include "worker/worker.h"
#include "../pipe/pipe.h"
#include "../common/common.h"
#include "../logs_journal/logs_journal.h"


#include <thread>
#include <vector>
#include <memory>
#include <iostream>

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
    Server(Pipe<Request>::PipeReader& pipe_reader, std::shared_ptr<Algorithm> algorithm, std::shared_ptr<LogsJournal> logs_journal, uint workers_number = 1);

    void start() {
        for (auto& worker : workers_) {
            worker->start();
        }
        server_thread_ = std::make_unique<std::thread>([this]() {
            while (!no_more_requests_ || pipe_reader_.GetPipe()->GetSize() > 0 || requests_received_ != requests_processed_) {
                auto request_from_client = pipe_reader_.read();
                if (request_from_client) {
                    ++requests_received_;
                    auto request_to_worker = workers_writers_[request_from_client.value().id() % workers_writers_.size()];
                    request_to_worker.write(request_from_client.value());
                }

                for (auto& worker_reader : workers_readers_) {
                    auto response = worker_reader.read();
                    if (response) {
                        ++requests_processed_;
                        auto val = response.value();
                        LogLine log_line(std::chrono::system_clock::now().time_since_epoch().count() / 1000000, val.is_allowed(), val.id());
                        logs_journal_->add_log(std::move(log_line));
                    }
                }
            }
        });
    }

    void wait() {
        no_more_requests_ = true;
        if (server_thread_ && server_thread_->joinable()) {
            server_thread_->join();
        }
    }

    void stop() {
        wait();

        for (auto& worker : workers_) {
            worker->stop();
        }
    }

private:
    Pipe<Request>::PipeReader& pipe_reader_;
    std::shared_ptr<Algorithm> algorithm_;
    std::shared_ptr<LogsJournal> logs_journal_;
    bool no_more_requests_ = false;
    std::unique_ptr<std::thread> server_thread_;
    std::vector<std::unique_ptr<Worker>> workers_;
    std::vector<Pipe<Request>::PipeWriter> workers_writers_;
    std::vector<Pipe<Response>::PipeReader> workers_readers_;
    size_t requests_received_ = 0;
    size_t requests_processed_ = 0;
};
