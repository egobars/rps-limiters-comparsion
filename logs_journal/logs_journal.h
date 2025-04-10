#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <variant>

class LogLine {
public:
    LogLine(uint64_t timestamp, bool is_allowed, uint user_id)
        : timestamp(timestamp), is_allowed(is_allowed), user_id(user_id)
    {}

    uint64_t timestamp;
    bool is_allowed;
    uint user_id;
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
