#pragma once

#include "../metrics_aggregator.h"

#include <iostream>
#include <fstream>

class MetricsAggregatorRPSAccepted : public MetricsAggregatorBase {
public:
    void aggregate(const LogsJournal& journal) override {
        auto logs = journal.get_logs();
        uint64_t start_time = logs[0].timestamp / 10;
        std::unordered_map<time_t, uint64_t> requests_per_10ms;
        for (auto log : logs) {
            if (log.timestamp / 10 >= start_time + 10 * 100) {
                break;
            }
            if (log.is_allowed) {
                requests_per_10ms[log.timestamp / 10]++;
            }
        }
        std::vector<std::pair<time_t, uint64_t>> rps_vector(10 * 100);
        size_t current_sum = 0;
        for (size_t i = 0; i < rps_vector.size(); ++i) {
            rps_vector[i] = {start_time + i, requests_per_10ms[start_time + i]};
        }
        std::sort(rps_vector.begin(), rps_vector.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        std::ofstream output_file("../artifacts/rps_accepted.txt");
        size_t i = 0;
        for (auto [timestamp, _] : rps_vector) {
            output_file << (double)(timestamp - start_time) / 100;
            if (i != rps_vector.size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file << std::endl;
        i = 0;
        for (auto [_, count] : rps_vector) {
            output_file << count;
            if (i != rps_vector.size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file.close();
    }
};
