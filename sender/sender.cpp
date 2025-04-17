#include "sender.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include "HTTPRequest.hpp"


/* http server sender
void Sender::start_execution() {
    std::vector<uint> rps = workload->generate_rps();

    std::cout << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << std::endl;
    time_t start_from = time(NULL) + 1;
    std::this_thread::sleep_until(
        std::chrono::system_clock::from_time_t(start_from)
    );
    std::cout << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << std::endl;

    for (uint current_rps : rps) {
        while (current_rps > 0) {
            send_request();
            auto current_time_ms = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
            auto remaining_time = 1000 - (current_time_ms % 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(remaining_time / current_rps));
            --current_rps;
            std::cout << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << std::endl;
        }
    }
}

void Sender::send_request() {
    std::stringstream ss;
    ss << "http://localhost:" << port;
    std::string url = ss.str();
    http::Request request{url};
    const auto response = request.send("GET");
    std::cout << response.status.code << ' ' << std::string(response.body.begin(), response.body.end()) << std::endl;
}
*/

void Sender::start_execution() {
    std::vector<uint> requests_per_10ms = workload_->generate_requests_per_10ms();

    time_t start_from = time(NULL) + 1;
    std::this_thread::sleep_until(
        std::chrono::system_clock::from_time_t(start_from)
    );

    for (uint current_requests_per_10ms : requests_per_10ms) {
        auto end_next_period_ms = std::chrono::system_clock::now().time_since_epoch().count() / 1000000 + 10;
        while (current_requests_per_10ms > 0) {
            send_request();
            auto current_time_ms = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
            auto remaining_time = std::max(static_cast<long>(0), end_next_period_ms - current_time_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(remaining_time / current_requests_per_10ms));
            --current_requests_per_10ms;
        }
    }
}

void Sender::send_request() {
    Request request = Request(current_id_, 0, time(NULL));
    pipe_writer_.write(request);
    ++current_id_;
}
