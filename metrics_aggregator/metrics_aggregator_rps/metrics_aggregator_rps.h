#pragma once

#include "../metrics_aggregator.h"
#include <iostream>

class MetricsAggregatorRPS : public MetricsAggregatorBase {
public:
    void aggregate(const LogsJournal& journal) override {
        auto logs = journal.get_logs();
        uint64_t start_time = logs[0].timestamp / 1000;
        std::unordered_map<time_t, uint64_t> rps;
        for (auto log : logs) {
            if (log.timestamp / 1000 >= start_time + 10) {
                break;
            }
            if (log.is_allowed) {
                rps[log.timestamp / 1000]++;
            }
        }
        std::vector<std::pair<time_t, uint64_t>> rps_vector(rps.begin(), rps.end());
        std::sort(rps_vector.begin(), rps_vector.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        size_t i = 0;
        std::cout << "[";
        for (auto [timestamp, _] : rps_vector) {
            std::cout << timestamp - start_time;
            if (i != rps_vector.size() - 1) {
                std::cout << ", ";
            }
            ++i;
        }
        std::cout << "]" << std::endl;
        i = 0;
        std::cout << "[";
        for (auto [_, count] : rps_vector) {
            std::cout << count;
            if (i != rps_vector.size() - 1) {
                std::cout << ", ";
            }
            ++i;
        }
        std::cout << "]" << std::endl;
    }
};
