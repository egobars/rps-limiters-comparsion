#pragma once

#include "../algorithm_base.h"

#include <chrono>

class TokenBucketSimple : public Algorithm {
public:
    TokenBucketSimple(size_t max_capacity, size_t refill_rate)
        : max_capacity_(max_capacity)
        , refill_rate_(refill_rate)
        , current_tokens_(0)
        , last_refill_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& request) override {
        refill_bucket();
        if (current_tokens_ >= 1) {
            --current_tokens_;
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
        return std::make_unique<TokenBucketSimple>(*this);
    }

private:
    void refill_bucket() {
        uint64_t current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        current_tokens_ = std::min(current_tokens_ + refill_rate_ / 1000 * (double)(current_time - last_refill_time_), max_capacity_);
        last_refill_time_ = current_time;
    }

    double max_capacity_;
    double refill_rate_;
    double current_tokens_;
    uint64_t last_refill_time_;
};
