#pragma once

#include "../algorithm_base.h"

#include <chrono>

class FixedWindow : public Algorithm {
public:
    FixedWindow(size_t tokens_to_add, size_t refill_time)
        : tokens_to_add_(tokens_to_add)
        , refill_time_(refill_time)
        , current_tokens_(0)
        , last_refill_time_(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
    {}

    bool check_request(Request& request) override {
        refill_bucket();
        if (current_tokens_ > 0) {
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
        return std::make_unique<FixedWindow>(*this);
    }

private:
    void refill_bucket() {
        long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time - last_refill_time_ >= refill_time_) {
            current_tokens_ = tokens_to_add_;
            last_refill_time_ = current_time;
        }
    }

    size_t tokens_to_add_;
    size_t refill_time_;
    size_t current_tokens_;
    long last_refill_time_;
};
