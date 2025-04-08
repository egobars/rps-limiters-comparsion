#pragma once

#include <ctime>
#include <sys/types.h>
#include <memory>

class Algorithm {
public:
    virtual bool check_request(uint, std::time_t) = 0;

    virtual std::unique_ptr<Algorithm> clone() = 0;
};
