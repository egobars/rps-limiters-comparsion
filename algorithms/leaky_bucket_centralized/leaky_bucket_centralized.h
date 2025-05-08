#pragma once

#include "../algorithm_base.h"

#include <chrono>
#include <queue>

class LeakyBucketCentralized : public Algorithm {
public:
    LeakyBucketCentralized(size_t time_delta, size_t requests_to_process)
        : time_delta_(time_delta)
        , requests_to_process_(requests_to_process)
        , last_check_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& /*request*/) override {
        return false;
    }

    void add_request(Request& request) override {
        Response response = construct_response(request);
        if (responses_.size() > 50) {
            responses_to_process_.push(response);
        } else {
            responses_.push(response);
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
            auto response = responses_.front();
            response.set_is_allowed(true);
            response.set_is_retry(false);
            responses_to_process_.push(response);
            responses_.pop();
            ++processed_requests;
        }
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<LeakyBucketCentralized>(*this);
    }

    std::queue<Response>* get_responses() override {
        return &responses_to_process_;
    }

private:
    Response construct_response(Request& request) {
        if (request.attempt() == 0) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 700);
            return response;
        } else if (request.attempt() == 1) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 1900);
            return response;
        } else {
            Response response(request.id(), request.user(), request.timestamp(), false, false, request.attempt(), 0);
            return response;
        }
    }

    size_t time_delta_;
    size_t requests_to_process_;
    long last_check_time_;
    std::queue<Response> responses_;
    std::queue<Response> responses_to_process_;
};
