#pragma once

#include "../algorithm_base.h"

#include <chrono>
#include <vector>
#include <memory>
#include <random>

class TokenBucketDecentralized : public Algorithm {
public:
    TokenBucketDecentralized(size_t max_capacity, size_t refill_rate, int id, std::vector<TokenBucketDecentralized*>* algorithms, size_t nodes_number)
        : max_capacity(max_capacity)
        , refill_rate(refill_rate)
        , last_refill_time(std::chrono::system_clock::now().time_since_epoch().count() / 1000000)
        , current_tokens_(0)
        , current_limit_(max_capacity)
        , algorithms(algorithms)
        , id(id)
        , rd()
        , gen(rd())
        , dist(0, nodes_number - 1) {
        delta_by_nodes.resize(nodes_number);
        limit_by_nodes.resize(nodes_number);
        last_sync_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    }

    bool check_request(Request& request) override {
        refill_bucket();
        if (current_tokens_ <= current_limit_ - 1) {
            ++current_tokens_;
            ++delta_by_nodes[id];
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
        return nullptr;
    }

    double max_capacity;
    double refill_rate;
    long last_refill_time;
    double current_tokens_;
    double current_limit_;

    std::vector<double> delta_by_nodes;
    std::vector<double> limit_by_nodes;

    std::vector<TokenBucketDecentralized*>* algorithms;
    int id;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<size_t> dist;

    int64_t last_sync_time;

private:
    void refill_bucket() {
        dist = std::uniform_int_distribution<size_t>(0, algorithms->size() - 1);

        // if (std::chrono::system_clock::now().time_since_epoch().count() / 1000000 - last_sync_time > 10) {
            double sum_tokens = 0;
            // double sum_limits = 0;
            for (size_t i = 0; i < 5; ++i) {
                /*size_t id_to_update = dist(gen); 
                if (id_to_update == id) {
                    continue;
                }
                auto delta = algorithms->at(id_to_update)->delta_by_nodes[id];
                algorithms->at(id_to_update)->delta_by_nodes[id] = 0;
                current_tokens_ += delta;*/
                sum_tokens += algorithms->at(i)->delta_by_nodes[i];
                // sum_limits += algorithms->at(i)->limit_by_nodes[id];
            }
            current_tokens_ = sum_tokens;
            last_sync_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        // }

        long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time - last_refill_time > 10) {
            current_limit_ = std::min(current_limit_ + refill_rate / 1000 * (double)(current_time - last_refill_time), current_tokens_ + max_capacity);
            last_refill_time = current_time;
        }
    }
};
