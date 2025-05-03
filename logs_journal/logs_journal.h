#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <variant>

class LogLine {
public:
    LogLine(uint64_t timestamp, bool is_allowed, uint user_id, uint request_id, uint attempt)
        : timestamp(timestamp), is_allowed(is_allowed), user_id(user_id), request_id(request_id), attempt(attempt)
    {}

    uint64_t timestamp;
    bool is_allowed;
    uint user_id;
    uint request_id;
    uint attempt;
};

class LogsJournal {
public:
    void add_log(LogLine log) {
        logs_.push_back(std::move(log));
    }
    
    const std::vector<LogLine>& get_logs() const {
        return logs_;
    }

private:
    std::vector<LogLine> logs_;
};
