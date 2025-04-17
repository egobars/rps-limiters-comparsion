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
        , algorithms(algorithms)
        , id(id)
        , rd()
        , gen(rd())
        , dist(0, nodes_number - 1) {
        accepted_requests_by_nodes.resize(nodes_number);
        sum_of_accepted_requests = 0;
    }

    bool check_request(Request& request) override {
        refill_bucket();
        if (sum_of_accepted_requests <= max_capacity - 1) {
            ++sum_of_accepted_requests;
            ++accepted_requests_by_nodes[id];
            return true;
        }
        return false;
    }

    std::unique_ptr<Algorithm> clone() override {
        return nullptr;
    }

    double max_capacity;
    double refill_rate;
    long last_refill_time;

    std::vector<double> accepted_requests_by_nodes;
    double sum_of_accepted_requests;

    std::vector<TokenBucketDecentralized*>* algorithms;
    int id;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<size_t> dist;

private:
    void refill_bucket() {
        dist = std::uniform_int_distribution<size_t>(0, algorithms->size() - 1);

        for (size_t i = 0; i < algorithms->size(); ++i) {
            if (i == id) {
                continue;
            }
            size_t id_to_update = i;
            auto accepted_requests_to_update = algorithms->at(id_to_update)->accepted_requests_by_nodes[id_to_update];
            sum_of_accepted_requests += accepted_requests_to_update - accepted_requests_by_nodes[id_to_update];
            accepted_requests_by_nodes[id_to_update] = accepted_requests_to_update;
        }

        long current_time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
        if (current_time - last_refill_time > 100) {
            max_capacity = std::min(max_capacity + refill_rate / 10 * (double)(current_time - last_refill_time) / 100, sum_of_accepted_requests + refill_rate / 10 * (double)(current_time - last_refill_time) / 100);
            last_refill_time = current_time;
        }
    }
};
