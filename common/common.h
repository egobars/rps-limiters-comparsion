#include <ctime>
#include <sys/types.h>
#include <string>

class Request {
public:
    Request(uint id, uint user, time_t timestamp)
        : id_(id), user_(user), timestamp_(timestamp) {
    }

private:
    uint id_;
    uint user_;
    time_t timestamp_;
};

class Response {
public:
    Response(uint id, uint user, time_t timestamp, bool is_allowed)
        : id_(id), user_(user), timestamp_(timestamp), is_allowed_(is_allowed) {
    }

private:
    uint id_;
    uint user_;
    time_t timestamp_;
    bool is_allowed_;
};
