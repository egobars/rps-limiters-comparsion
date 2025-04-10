#pragma once

#include <ctime>
#include <sys/types.h>
#include <string>

class Request {
public:
    Request(uint id, uint user, time_t timestamp)
        : id_(id), user_(user), timestamp_(timestamp) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    time_t timestamp() const { return timestamp_; }

private:
    uint id_;
    uint user_;
    time_t timestamp_;
};

class Response {
public:
    Response(uint id, uint user, time_t request_timestamp, time_t response_timestamp, bool is_allowed)
        : id_(id), user_(user), request_timestamp_(request_timestamp), response_timestamp_(response_timestamp), is_allowed_(is_allowed) {
    }
    
    uint id() const { return id_; }
    uint user() const { return user_; }
    time_t request_timestamp() const { return request_timestamp_; }
    time_t response_timestamp() const { return response_timestamp_; }
    bool is_allowed() const { return is_allowed_; }

private:
    uint id_;
    uint user_;
    time_t request_timestamp_;
    time_t response_timestamp_;
    bool is_allowed_;
};
