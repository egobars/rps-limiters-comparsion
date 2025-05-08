#pragma once

#include "../algorithm_base.h"

#include <chrono>

class TokenBucketCentralized : public Algorithm {
public:
    TokenBucketCentralized(size_t max_capacity, size_t refill_rate)
        : max_capacity_(0)
        , refill_rate_(refill_rate)
        , current_bucket_size_(0)
        , max_delta_(max_capacity)
        , last_refill_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& request) override {
        refill_bucket();
        if (current_bucket_size_ <= max_capacity_ - 1) {
            ++current_bucket_size_;
            return true;
        }
        return false;
    }

    void add_request(Request& /*request*/) override {
    }

    void update() override {
    }

    std::queue<Response>* get_responses() override {
        return nullptr;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<TokenBucketCentralized>(*this);
    }

private:
    void refill_bucket() {
        long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time - last_refill_time_ > 10) {
            max_capacity_ = std::min(max_capacity_ + refill_rate_ / 100 * (double)(current_time - last_refill_time_) / 10, current_bucket_size_ + max_delta_);
            last_refill_time_ = current_time;
        }
    }

    double max_capacity_;
    double refill_rate_;
    double current_bucket_size_;
    double max_delta_;
    long last_refill_time_;
};
