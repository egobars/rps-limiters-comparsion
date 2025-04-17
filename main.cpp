#include "server/server.h"
#include "algorithms/algorithm_mock/algorithm_mock.h"
#include "workloads/workload_static/workload_static.h"
#include "workloads/workload_parabolic/workload_parabolic.h"
#include "sender/sender.h"
#include "pipe/pipe.h"
#include "metrics_aggregator/metrics_aggregator_rps/metrics_aggregator_rps.h"
#include "algorithms/token_bucket_centralized/token_bucket_centralized.h"
#include "algorithms/token_bucket_decentralized/token_bucket_decentralized.h"
#include <iostream>
#include <memory>

int main() {
    // Создаем общие каналы передачи данных
    auto request_pipe = std::make_shared<Pipe<Request>>();
    auto response_pipe = std::make_shared<Pipe<Response>>();
    
    auto request_writer = request_pipe->GetWriter();
    auto request_reader = request_pipe->GetReader();
    
    auto response_writer = response_pipe->GetWriter();
    auto response_reader = response_pipe->GetReader();
    
    // Инициализируем компоненты
    /*std::vector<TokenBucketDecentralized*> algorithms;
    for (size_t i = 0; i < 10; ++i) {
        algorithms.push_back(new TokenBucketDecentralized(800, 800, i, &algorithms, 10));
    }
    std::vector<Algorithm*> algorithm_pointers(algorithms.begin(), algorithms.end());*/
    // std::unique_ptr<Algorithm> algorithm = std::make_unique<TokenBucketCentralized>(100, 100);

    std::vector<TokenBucketCentralized*> algorithms;
    algorithms.push_back(new TokenBucketCentralized(800, 800));
    std::vector<Algorithm*> algorithm_pointers(algorithms.begin(), algorithms.end());

    std::shared_ptr<LogsJournal> logs_journal = std::make_shared<LogsJournal>();
    Server server(request_reader, &algorithm_pointers, logs_journal, 1);
    server.start();

    WorkloadStatic workload(10, 1200);
    Sender sender(&workload, request_writer);
    sender.start_execution();

    server.wait();
    server.stop();

    auto metrics_aggregator = std::make_shared<MetricsAggregatorRPS>();
    metrics_aggregator->aggregate(*logs_journal);

    return 0;
}
