#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorInflyOverflow : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::vector<uint64_t> infly_requests_per_timestamp(duration_seconds * 1000 + 100);
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                if (log.is_allowed) {
                    for (int i = 0; i < 100; ++i) {
                        infly_requests_per_timestamp[log.timestamp - start_time + i]++;
                    }
                }
            }
        }
        size_t overflow_metric = 0;
        for (int i = 1000; i < duration_seconds * 1000; ++i) {
            double infly_requests = (double) infly_requests_per_timestamp[i] / journals.size();
            if (infly_requests > 100) {
                overflow_metric += (infly_requests - 100) * (infly_requests - 100);
            }
        }
        std::cout << "Overflow metric: " << overflow_metric << std::endl;
    }
};
