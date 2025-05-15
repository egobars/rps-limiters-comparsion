#include "server/server.h"
#include "algorithms/algorithm_mock/algorithm_mock.h"
#include "workloads/workload_static/workload_static.h"
#include "workloads/workload_parabolic/workload_parabolic.h"
#include "workloads/workload_sinusoid/workload_sinusoid.h"
#include "workloads/workload_spikes/workload_spikes.h"
#include "sender/sender.h"
#include "pipe/pipe.h"
#include "metrics_aggregator/metrics_aggregator_rps/metrics_aggregator_rps.h"
#include "metrics_aggregator/metrics_aggregator_infly/metrics_aggregator_infly.h"
#include "metrics_aggregator/metrics_aggregator_infly_overflow/metrics_aggregator_infly_overflow.h"
#include "metrics_aggregator/metrics_aggregator_delay/metrics_aggregator_delay.h"
#include "metrics_aggregator/metrics_aggregator_user/metrics_aggregator_user.h"
#include "algorithms/fixed_window/fixed_window.h"
#include "algorithms/token_bucket_decentralized/token_bucket_decentralized.h"
#include "algorithms/token_bucket_simple/token_bucket_simple.h"
#include "algorithms/leaky_bucket_centralized/leaky_bucket_centralized.h"
#include "algorithms/leaky_bucket_centralized_set/leaky_bucket_centralized_set.h"
#include "algorithms/sliding_window_centralized/sliding_window_centralized.h"
#include "algorithms/sliding_window_decentralized/sliding_window_decentralized.h"
#include <iostream>
#include <memory>

int main() {
    std::vector<std::shared_ptr<LogsJournal>> logs_journals;
    for (int i = 0; i < 10; ++i) {
        auto request_pipe = std::make_shared<Pipe<Request>>();
        auto retry_pipe = std::make_shared<Pipe<Retry>>();
        
        auto request_writer = request_pipe->GetWriter();
        auto request_reader = request_pipe->GetReader();
        auto retry_writer = retry_pipe->GetWriter();
        auto retry_reader = retry_pipe->GetReader();

        std::vector<LeakyBucketCentralizedSet*> algorithms;
        for (size_t i = 0; i < 4; ++i) {
            algorithms.push_back(new LeakyBucketCentralizedSet(10, 2));
        }
        algorithms.push_back(new LeakyBucketCentralizedSet(10, 3));
        std::vector<Algorithm*> algorithm_pointers(algorithms.begin(), algorithms.end());
        // std::unique_ptr<Algorithm> algorithm = std::make_unique<TokenBucketCentralized>(100, 100);

        /*std::vector<LeakyBucketCentralizedSet*> algorithms;
        algorithms.push_back(new LeakyBucketCentralizedSet(10, 10));
        std::vector<Algorithm*> algorithm_pointers(algorithms.begin(), algorithms.end());*/

        std::shared_ptr<LogsJournal> logs_journal = std::make_shared<LogsJournal>();
        Server server(request_reader, retry_writer, &algorithm_pointers, logs_journal, 5);
        server.start();

        WorkloadSinusoid workload(10, 400, 1600);
        Sender sender(&workload, request_writer, retry_reader);
        sender.start_execution();

        server.wait();
        server.stop();

        logs_journals.push_back(logs_journal);
    }

    auto metrics_aggregator = std::make_shared<MetricsAggregatorRPS>();
    metrics_aggregator->aggregate(10, logs_journals);

    auto metrics_aggregator_infly = std::make_shared<MetricsAggregatorInfly>();
    metrics_aggregator_infly->aggregate(10, logs_journals);

    auto metrics_aggregator_infly_overflow = std::make_shared<MetricsAggregatorInflyOverflow>();
    metrics_aggregator_infly_overflow->aggregate(10, logs_journals);

    auto metrics_aggregator_delay = std::make_shared<MetricsAggregatorDelay>();
    metrics_aggregator_delay->aggregate(10, logs_journals);

    auto metrics_aggregator_user = std::make_shared<MetricsAggregatorUser>();
    metrics_aggregator_user->aggregate(10, logs_journals);

    return 0;
}
