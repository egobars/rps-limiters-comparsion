#include "server/server.h"
#include "algorithms/algorithm_mock/algorithm_mock.h"
#include "workloads/workload_static/workload_static.h"
#include "sender/sender.h"
#include "pipe/pipe.h"
#include "metrics_aggregator/metrics_aggregator_rps/metrics_aggregator_rps.h"

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
    std::shared_ptr<Algorithm> algorithm = std::make_shared<AlgorithmMock>();
    std::shared_ptr<LogsJournal> logs_journal = std::make_shared<LogsJournal>();
    Server server(request_reader, algorithm, logs_journal);
    server.start();
    
    WorkloadStatic workload(10, 1000);
    Sender sender(&workload, request_writer);
    sender.start_execution();

    server.wait();
    server.stop();

    auto metrics_aggregator = std::make_shared<MetricsAggregatorRPS>();
    metrics_aggregator->aggregate(*logs_journal);

    return 0;
}
