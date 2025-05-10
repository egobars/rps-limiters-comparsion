#pragma once

#include "../algorithm_base.h"

#include <chrono>
#include <queue>
#include <set>
#include <iostream>

class SlidingWindowDecentralized : public Algorithm {
public:
    SlidingWindowDecentralized(size_t infly_window_size, int id, std::vector<SlidingWindowDecentralized*>* algorithms, size_t nodes_number)
        : infly_window_size_(infly_window_size)
        , id_(id)
        , algorithms_(algorithms)
    {
        last_sync_timestamp_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    }

    bool check_request(Request& /*request*/) override {
        return false;
    }

    void add_request(Request& request) override {
        PriorityResponse priority_response = construct_response(request);
        if (responses_.size() > 10) {
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
        if (std::chrono::system_clock::now().time_since_epoch().count() / 1000000 > last_sync_timestamp_ + 10) {
            summary_length_ = 0;
            for (size_t i = 0; i < algorithms_->size(); ++i) {
                summary_length_ += algorithms_->at(i)->infly_end_timestamps_.size();
            }
            last_sync_timestamp_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        }
        if (summary_length_ >= infly_window_size_ && std::chrono::system_clock::now().time_since_epoch().count() / 1000000 < *infly_end_timestamps_.begin()) {
            return;
        }
        while (!infly_end_timestamps_.empty() && std::chrono::system_clock::now().time_since_epoch().count() / 1000000 >= *infly_end_timestamps_.begin()) {
            infly_end_timestamps_.erase(infly_end_timestamps_.begin());
        }
        while (!responses_.empty()) {
            if (summary_length_ >= infly_window_size_) {
                break;
            }
            auto response = responses_.begin();
            Response temp_response = response->response;
            temp_response.set_is_allowed(true);
            temp_response.set_is_retry(false);
            temp_response.set_timestamp(std::chrono::system_clock::now().time_since_epoch().count() / 1000000);
            responses_to_process_.push(temp_response);
            infly_end_timestamps_.insert(std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 100);
            ++summary_length_;
            responses_.erase(response);
        }
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<SlidingWindowDecentralized>(*this);
    }

    std::queue<Response>* get_responses() override {
        return &responses_to_process_;
    }

    int id_;
    std::vector<SlidingWindowDecentralized*>* algorithms_;
    size_t summary_length_ = 0;
    std::multiset<uint64_t> infly_end_timestamps_;
    uint64_t last_sync_timestamp_ = 0;

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

    size_t infly_window_size_;
    std::multiset<PriorityResponse> responses_;
    std::queue<Response> responses_to_process_;
};
