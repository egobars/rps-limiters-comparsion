#pragma once

#include "../common/common.h"

#include <ctime>
#include <sys/types.h>
#include <memory>
#include <optional>
#include <queue>

class Algorithm {
public:
    virtual bool check_request(Request&) = 0;

    virtual void add_request(Request&) = 0;

    virtual void update() = 0;

    virtual std::queue<Response>* get_responses() = 0;

    virtual std::unique_ptr<Algorithm> clone() = 0;
};
