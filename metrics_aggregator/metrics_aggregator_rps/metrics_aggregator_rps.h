#pragma once

#include "../metrics_aggregator.h"

#include <iostream>
#include <fstream>

class MetricsAggregatorRPS : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::vector<std::unordered_map<time_t, uint64_t>> requests_per_ms_by_attempts(3);
        for (size_t i = 0; i < journals.size(); ++i) {
            auto logs = journals[i]->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                requests_per_ms_by_attempts[log.attempt][log.timestamp - start_time]++;
            }
        }
        std::vector<std::vector<std::pair<time_t, uint64_t>>> rps_vector(3, std::vector<std::pair<time_t, uint64_t>>(duration_seconds * 1000));
        for (size_t attempt = 0; attempt < 3; ++attempt) {
            size_t current_sum = 0;
            for (size_t i = 0; i < rps_vector[attempt].size(); ++i) {
                current_sum += requests_per_ms_by_attempts[attempt][i];
                if (i >= 100) {
                    current_sum -= requests_per_ms_by_attempts[attempt][i - 100];
                }
                rps_vector[attempt][i] = {i, current_sum};
            }
        }

        std::ofstream output_file("../artifacts/rps.txt");
        size_t i = 0;
        for (auto [timestamp, _] : rps_vector[0]) {
            output_file << (double)(timestamp) / 1000;
            if (i != rps_vector[0].size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file << std::endl;
        for (size_t attempt = 0; attempt < 3; ++attempt) {
            i = 0;
            for (auto [_, count] : rps_vector[attempt]) {
                output_file << (double) count / journals.size();
                if (i != rps_vector[attempt].size() - 1) {
                    output_file << " ";
                }
                ++i;
            }
            output_file << std::endl;
        }
        output_file.close();
    }
};
