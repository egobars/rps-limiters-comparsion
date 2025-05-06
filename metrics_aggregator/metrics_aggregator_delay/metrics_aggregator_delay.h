#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorDelay : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        double delay_sum = 0;
        size_t requests_number = 0;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                delay_sum += log.timestamp - log.timestamp_start;
                ++requests_number;
            }
        }
        std::cout << "Delay metric: " << delay_sum / requests_number << std::endl;
    }
};
