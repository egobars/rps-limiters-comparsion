#pragma once

#include "../metrics_aggregator.h"
#include <iostream>

class MetricsAggregatorInfly : public MetricsAggregatorBase {
public:
    void aggregate(const LogsJournal& journal) override {
        auto logs = journal.get_logs();
        uint64_t start_time = logs[0].timestamp / 10;
        std::vector<uint64_t> infly_requests_per_timestamp(10 * 100 + 10);
        for (auto log : logs) {
            if (log.timestamp / 10 >= start_time + 10 * 100) {
                break;
            }
            if (log.is_allowed) {
                for (int i = 0; i < 10; ++i) {
                    infly_requests_per_timestamp[log.timestamp / 10 - start_time + i]++;
                }
            }
        }
        std::cout << "[";
        for (int i = 0; i < infly_requests_per_timestamp.size(); ++i) {
            std::cout << (double)(i) / 100;
            if (i < infly_requests_per_timestamp.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
        size_t i = 0;
        std::cout << "[";
        for (auto count : infly_requests_per_timestamp) {
            std::cout << count;
            if (i != infly_requests_per_timestamp.size() - 1) {
                std::cout << ", ";
            }
            ++i;
        }
        std::cout << "]" << std::endl;
    }
};
