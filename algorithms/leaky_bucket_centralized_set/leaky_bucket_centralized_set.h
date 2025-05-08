#pragma once

#include "../algorithm_base.h"

#include <chrono>
#include <queue>
#include <set>
#include <iostream>

class LeakyBucketCentralizedSet : public Algorithm {
public:
    LeakyBucketCentralizedSet(size_t time_delta, size_t requests_to_process)
        : time_delta_(time_delta)
        , requests_to_process_(requests_to_process)
        , last_check_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& /*request*/) override {
        return false;
    }

    void add_request(Request& request) override {
        PriorityResponse priority_response = construct_response(request);
        if (responses_.size() > 50) {
            auto last_response = responses_.end();
            --last_response;
            if (priority_response < *last_response) {
                responses_to_process_.push(last_response->response);
                responses_.erase(last_response);
                responses_.insert(priority_response);
            } else {
                responses_to_process_.push(priority_response.response);
            }
        } else {
            responses_.insert(priority_response);
        }
    }

    void update() override {
        if (std::chrono::system_clock::now().time_since_epoch().count() / 1000000 - last_check_time_ < time_delta_) {
            return;
        }
        last_check_time_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        int processed_requests = 0;
        while (!responses_.empty()) {
            if (processed_requests == requests_to_process_) {
                break;
            }
            auto response = responses_.begin();
            Response temp_response = response->response;
            temp_response.set_is_allowed(true);
            temp_response.set_is_retry(false);
            responses_to_process_.push(temp_response);
            responses_.erase(response);


            ++processed_requests;
        }
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<LeakyBucketCentralizedSet>(*this);
    }

    std::queue<Response>* get_responses() override {
        return &responses_to_process_;
    }

private:
    struct PriorityResponse {
        int priority;
        uint64_t timestamp;
        Response response;

        PriorityResponse(int priority, uint64_t timestamp, Response response)
            : priority(priority)
            , timestamp(timestamp)
            , response(response)
        {}

        bool operator<(const PriorityResponse& other) const {
            if (priority != other.priority) {
                return priority < other.priority;
            }
            if (timestamp != other.timestamp) {
                return timestamp < other.timestamp;
            }
            return response.id() < other.response.id();
        }
    };

    PriorityResponse construct_response(Request& request) {
        if (request.attempt() == 0) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 700);
            return PriorityResponse(2, std::chrono::system_clock::now().time_since_epoch().count() / 1000000, response);
        } else if (request.attempt() == 1) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 1900);
            return PriorityResponse(1, std::chrono::system_clock::now().time_since_epoch().count() / 1000000, response);
        } else {
            Response response(request.id(), request.user(), request.timestamp(), false, false, request.attempt(), 0);
            return PriorityResponse(0, std::chrono::system_clock::now().time_since_epoch().count() / 1000000, response);
        }
    }

    size_t time_delta_;
    size_t requests_to_process_;
    long last_check_time_;
    std::multiset<PriorityResponse> responses_;
    std::queue<Response> responses_to_process_;
};
