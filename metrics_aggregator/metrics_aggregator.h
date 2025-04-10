#pragma once

#include "../logs_journal/logs_journal.h"

class MetricsAggregatorBase {
public:
    virtual void aggregate(const LogsJournal& journal) = 0;
};
