#include "../algorithm_base.h"

class AlgorithmMock : public Algorithm {
public:
    bool check_request(Request& /*request*/) override {
        return true;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<AlgorithmMock>(*this);
    }
};
