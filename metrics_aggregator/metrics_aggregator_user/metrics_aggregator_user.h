#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorUser : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        double delay_sum = 0;
        size_t requests_number = 0;
        size_t attempt1_sum = 0;
        size_t attempt2_sum = 0;
        size_t rejected_sum = 0;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                delay_sum += log.timestamp - log.timestamp_start;
                ++requests_number;
                if (log.attempt == 1) {
                    ++attempt1_sum;
                } else if (log.attempt == 2) {
                    if (log.is_allowed) {
                        ++attempt2_sum;
                    } else {
                        ++rejected_sum;
                    }
                }
            }
        }
        std::cout << "User metric: " << delay_sum / requests_number * 1000 << ' ' << static_cast<double>(attempt1_sum) / journals.size() << ' ' << static_cast<double>(attempt2_sum) / journals.size() << ' ' << static_cast<double>(rejected_sum) / journals.size() << std::endl;
    }
};
