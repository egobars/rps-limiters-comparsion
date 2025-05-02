#pragma once

#include "../metrics_aggregator.h"

#include <iostream>
#include <fstream>

class MetricsAggregatorRPS : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::unordered_map<time_t, uint64_t> requests_per_10ms;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                requests_per_10ms[log.timestamp - start_time]++;
            }
        }
        std::vector<std::pair<time_t, uint64_t>> rps_vector(duration_seconds * 1000);
        size_t current_sum = 0;
        for (size_t i = 0; i < rps_vector.size(); ++i) {
            current_sum += requests_per_10ms[i];
            if (i >= 100) {
                current_sum -= requests_per_10ms[i - 100];
            }
            rps_vector[i] = {i, current_sum};
        }
        std::sort(rps_vector.begin(), rps_vector.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        std::ofstream output_file("../artifacts/rps.txt");
        size_t i = 0;
        for (auto [timestamp, _] : rps_vector) {
            output_file << (double)(timestamp) / 1000;
            if (i != rps_vector.size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file << std::endl;
        i = 0;
        for (auto [_, count] : rps_vector) {
            output_file << (double) count / journals.size();
            if (i != rps_vector.size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file.close();
    }
};
