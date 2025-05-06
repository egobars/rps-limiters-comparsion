#include "../algorithm_base.h"

class AlgorithmMock : public Algorithm {
public:
    bool check_request(Request& /*request*/) override {
        return true;
    }

    void add_request(Request& /*request*/) override {
    }

    std::optional<Response> get_response() override {
        return std::nullopt;
    }

    std::unique_ptr<Algorithm> clone() override {
        return std::make_unique<AlgorithmMock>(*this);
    }
};
