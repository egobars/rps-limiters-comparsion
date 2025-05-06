#pragma once

#include "../algorithm_base.h"

#include <chrono>
#include <queue>

class TokenBucketCentralizedQueue : public Algorithm {
public:
    TokenBucketCentralizedQueue(size_t max_capacity, size_t refill_rate)
        : max_capacity_(max_capacity)
        , refill_rate_(refill_rate)
        , current_bucket_size_(max_capacity)
        , last_refill_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& /*request*/) override {
        return false;
    }

    void add_request(Request& request) override {
        if (request.attempt() == 0) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 500);
            responses_.push(response);
        } else if (request.attempt() == 1) {
            Response response(request.id(), request.user(), request.timestamp(), false, true, request.attempt(), std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 1500);
            responses_.push(response);
        } else {
            Response response(request.id(), request.user(), request.timestamp(), false, false, request.attempt(), 0);
            responses_.push(response);
        }
    }

    std::optional<Response> get_response() override {
        refill_bucket();
        if (responses_.empty()) {
            return std::nullopt;
        }
        auto response = responses_.front();

        bool is_allowed = current_bucket_size_ <= max_capacity_ - 1;
        if (is_allowed) {
            ++current_bucket_size_;
            response.set_is_allowed(true);
            response.set_is_retry(false);
            responses_.pop();
            return response;
        } else {
            if (responses_.size() > 200) {
                responses_.pop();
                return response;
            }
        }
        return std::nullopt;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<TokenBucketCentralizedQueue>(*this);
    }

private:
    void refill_bucket() {
        long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time - last_refill_time_ > 10) {
            max_capacity_ = std::min(max_capacity_ + refill_rate_ / 100 * (double)(current_time - last_refill_time_) / 10, current_bucket_size_ + refill_rate_ / 100 * (double)(current_time - last_refill_time_) / 10);
            last_refill_time_ = current_time;
        }
    }

    double max_capacity_;
    double refill_rate_;
    double current_bucket_size_;
    long last_refill_time_;
    std::queue<Response> responses_;
};
