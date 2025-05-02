#pragma once

#include "../logs_journal/logs_journal.h"

class MetricsAggregatorBase {
public:
    virtual void aggregate(const size_t duration_seconds, const std::vector<std::shared_ptr<LogsJournal>>& journals) = 0;
};
