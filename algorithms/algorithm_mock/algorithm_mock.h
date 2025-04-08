#include "../algorithm_base.h"

class AlgorithmMock : public Algorithm {
public:
    bool check_request(uint, std::time_t) override {
        return true;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<AlgorithmMock>(*this);
    }
};
