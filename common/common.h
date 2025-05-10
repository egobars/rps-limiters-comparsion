#pragma once

#include <ctime>
#include <sys/types.h>
#include <string>
#include <chrono>

class Request {
public:
    Request(uint id, uint user, uint attempt, uint64_t timestamp)
        : id_(id), user_(user), attempt_(attempt), timestamp_(timestamp) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    uint attempt() const { return attempt_; }
    uint64_t timestamp() const { return timestamp_; }

private:
    uint id_;
    uint user_;
    uint attempt_;
    uint64_t timestamp_;
};

class Response {
public:
    Response(uint id, uint user, uint64_t request_timestamp, bool is_allowed, bool is_retry, uint attempt, uint64_t do_retry_timestamp)
        : id_(id), user_(user), request_timestamp_(request_timestamp), is_allowed_(is_allowed), is_retry_(is_retry), attempt_(attempt), do_retry_timestamp_(do_retry_timestamp), timestamp_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    uint attempt() const { return attempt_; }
    uint64_t do_retry_timestamp() const { return do_retry_timestamp_; }
    uint64_t request_timestamp() const { return request_timestamp_; }
    bool is_allowed() const { return is_allowed_; }
    bool is_retry() const { return is_retry_; } 
    uint64_t timestamp() const { return timestamp_; }

    void set_is_allowed(bool is_allowed) {
        is_allowed_ = is_allowed;
    }

    void set_is_retry(bool is_retry) {
        is_retry_ = is_retry;
    }

    void set_timestamp(uint64_t timestamp) {
        timestamp_ = timestamp;
    }

private:
    uint id_;
    uint user_;
    uint attempt_;
    uint64_t do_retry_timestamp_;
    uint64_t request_timestamp_;
    bool is_allowed_;
    bool is_retry_;
    uint64_t timestamp_;
};

class Retry {
public:
    Retry(uint id, uint user, uint attempt, uint64_t do_retry_timestamp)
        : id_(id), user_(user), attempt_(attempt), do_retry_timestamp_(do_retry_timestamp) {
    }

    uint id() const { return id_; }
    uint user() const { return user_; }
    uint attempt() const { return attempt_; }
    uint64_t do_retry_timestamp() const { return do_retry_timestamp_; }

    bool operator<(const Retry& other) const {
        if (do_retry_timestamp_ != other.do_retry_timestamp_) {
            return do_retry_timestamp_ < other.do_retry_timestamp_;
        }
        return id_ < other.id_;
    }

private:
    uint id_;
    uint user_;
    uint attempt_;
    uint64_t do_retry_timestamp_;
};
