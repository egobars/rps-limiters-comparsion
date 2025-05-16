#pragma once

#include "../metrics_aggregator.h"
#include <iostream>
#include <fstream>

class MetricsAggregatorUser : public MetricsAggregatorBase {
public:
    void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) override {
        std::vector<double> attempt1_sum(journals.size(), 0);
        std::vector<double> attempt2_sum(journals.size(), 0);
        std::vector<double> rejected_sum(journals.size(), 0);
        std::vector<double> avg_time_for_user_sum(journals.size(), 0);
        int j = 0;
        for (auto journal : journals) {
            auto logs = journal->get_logs();
            std::unordered_map<size_t, size_t> time_for_user;
            std::unordered_map<size_t, bool> is_closed_for_user;
            uint64_t start_time = logs[0].timestamp;
            for (auto log : logs) {
                if (log.timestamp >= start_time + duration_seconds * 1000) {
                    break;
                }
                if (log.attempt == 0) {
                    time_for_user[log.user_id] = log.timestamp;
                } else if (log.attempt == 1) {
                    ++attempt1_sum[j];
                } else if (log.attempt == 2) {
                    if (log.is_allowed) {
                        ++attempt2_sum[j];
                    } else {
                        ++rejected_sum[j];
                    }
                }
                if (log.is_allowed) {
                    time_for_user[log.user_id] = log.timestamp - time_for_user[log.user_id];
                    is_closed_for_user[log.user_id] = true;
                }
            }
            double time_for_user_sum = 0;
            size_t users_number = 0;
            for (auto user : time_for_user) {
                if (is_closed_for_user[user.first]) {
                    time_for_user_sum += user.second;
                    ++users_number;
                }
            }
            avg_time_for_user_sum[j] = time_for_user_sum / users_number;
            ++j;
        }
        std::sort(avg_time_for_user_sum.begin(), avg_time_for_user_sum.end());
        std::sort(attempt1_sum.begin(), attempt1_sum.end());
        std::sort(attempt2_sum.begin(), attempt2_sum.end());
        std::sort(rejected_sum.begin(), rejected_sum.end());
        double avg_time_for_user = 0;
        for (int i = 2; i < journals.size() - 2; ++i) {
            avg_time_for_user += avg_time_for_user_sum[i];
        }
        avg_time_for_user /= journals.size() - 4;
        double attempt1 = 0;
        for (int i = 2; i < journals.size() - 2; ++i) {
            attempt1 += attempt1_sum[i];
        }
        attempt1 /= journals.size() - 4;
        double attempt2 = 0;
        for (int i = 2; i < journals.size() - 2; ++i) {
            attempt2 += attempt2_sum[i];
        }
        attempt2 /= journals.size() - 4;
        double rejected = 0;
        for (int i = 2; i < journals.size() - 2; ++i) {
            rejected += rejected_sum[i];
        }
        rejected /= journals.size() - 4;
        std::cout << "Avg time for user: " << avg_time_for_user << "+-" << (double)avg_time_for_user_sum[journals.size() - 3] - avg_time_for_user << std::endl;
        std::cout << "Attempt 1: " << attempt1 << "+-" << (double)attempt1_sum[journals.size() - 3] - attempt1 << std::endl;
        std::cout << "Attempt 2: " << attempt2 << "+-" << (double)attempt2_sum[journals.size() - 3] - attempt2 << std::endl;
        std::cout << "Rejected: " << rejected << "+-" << (double)rejected_sum[journals.size() - 3] - rejected << std::endl;
    }
};
