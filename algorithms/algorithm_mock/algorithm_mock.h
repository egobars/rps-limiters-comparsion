#include "../algorithm_base.h"

class AlgorithmMock : public Algorithm {
public:
    bool check_request(Request& /*request*/) override {
        return true;
    }

    void add_request(Request& /*request*/) override {
    }

    void update() override {
    }

    std::queue<Response>* get_responses() override {
        return nullptr;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<AlgorithmMock>(*this);
    }
};
