#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorDelay : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::vector<double> delays;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            double delay_sum = 0;
            size_t requests_number = 0;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                delay_sum += log.timestamp - log.timestamp_start;
                ++requests_number;
            }
            delays.push_back(delay_sum / requests_number);
        }
        std::sort(delays.begin(), delays.end());
        double delay = 0;
        for (int i = 2; i < delays.size() - 2; ++i) {
            delay += delays[i];
        }
        delay /= delays.size() - 4;
        std::cout << "Delay metric: " << delay << "+-" << (double)delays[delays.size() - 3] - delay << std::endl;
    }
};
