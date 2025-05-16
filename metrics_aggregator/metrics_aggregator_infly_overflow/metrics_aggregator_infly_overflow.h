#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorInflyOverflow : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::vector<std::vector<uint64_t>> infly_requests_per_timestamp(journals.size(), std::vector<uint64_t>(duration_seconds * 1000 + 100));
        int j = 0;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                if (log.is_allowed) {
                    for (int i = 0; i < 100; ++i) {
                        infly_requests_per_timestamp[j][log.timestamp - start_time + i]++;
                    }
                }
            }
            ++j;
        }
        std::vector<uint64_t> overflow_metrics(journals.size(), 0);
        for (int j = 0; j < journals.size(); ++j) {
            size_t cur_overflow_metric = 0;
            for (int i = 1000; i < duration_seconds * 1000; ++i) {
                double infly_requests = (double) infly_requests_per_timestamp[j][i];
                if (infly_requests > 110) {
                    overflow_metrics[j] += (infly_requests - 110) * (infly_requests - 110);
                }
            }
        }
        std::sort(overflow_metrics.begin(), overflow_metrics.end());
        size_t overflow_metric = 0;
        for (int i = 2; i < overflow_metrics.size() - 2; ++i) {
            overflow_metric += overflow_metrics[i];
        }
        overflow_metric /= overflow_metrics.size() - 4;
        std::cout << "Overflow metric: " << overflow_metric << "+-" << (double)overflow_metrics[overflow_metrics.size() - 3] - overflow_metric << std::endl;
    }
};
