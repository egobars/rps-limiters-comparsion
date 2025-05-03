#pragma once

#include <ctime>
#include <sys/types.h>
#include <string>

class Request {
public:
    Request(uint id, uint user, uint attempt, time_t timestamp)
        : id_(id), user_(user), attempt_(attempt), timestamp_(timestamp) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    uint attempt() const { return attempt_; }
    time_t timestamp() const { return timestamp_; }

private:
    uint id_;
    uint user_;
    uint attempt_;
    time_t timestamp_;
};

class Response {
public:
    Response(uint id, uint user, time_t request_timestamp, time_t response_timestamp, bool is_allowed, bool is_retry, uint attempt, uint64_t do_retry_timestamp)
        : id_(id), user_(user), request_timestamp_(request_timestamp), response_timestamp_(response_timestamp), is_allowed_(is_allowed), is_retry_(is_retry), attempt_(attempt), do_retry_timestamp_(do_retry_timestamp) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    uint attempt() const { return attempt_; }
    uint64_t do_retry_timestamp() const { return do_retry_timestamp_; }
    time_t request_timestamp() const { return request_timestamp_; }
    time_t response_timestamp() const { return response_timestamp_; }
    bool is_allowed() const { return is_allowed_; }
    bool is_retry() const { return is_retry_; } 

private:
    uint id_;
    uint user_;
    uint attempt_;
    uint64_t do_retry_timestamp_;
    time_t request_timestamp_;
    time_t response_timestamp_;
    bool is_allowed_;
    bool is_retry_;
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
