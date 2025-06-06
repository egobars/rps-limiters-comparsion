#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorInfly : public MetricsAggregatorBase {
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
        std::ofstream output_file("../artifacts/rps_infly.txt");
        for (int i = 0; i < infly_requests_per_timestamp.size(); ++i) {
            output_file << (double)(i) / 1000;
            if (i < infly_requests_per_timestamp.size() - 1) {
                output_file << " ";
            }
        }
        output_file << std::endl;
        size_t i = 0;
        for (auto count : infly_requests_per_timestamp) {
            output_file << (double) count / journals.size();
            if (i != infly_requests_per_timestamp.size() - 1) {
                output_file << " ";
            }
            ++i;
        }
        output_file.close();
    }
};
