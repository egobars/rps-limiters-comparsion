#include "sender.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
#include <string>


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
    // Устанавливаем флаг запуска
    is_running_ = true;
    
    // Запускаем поток для проверки повторных запросов
    retries_thread_ = std::thread(&Sender::check_retries, this);
    
    std::vector<uint> requests_per_100ms = workload_->generate_requests_per_100ms();

    time_t start_from = time(NULL) + 1;
    std::this_thread::sleep_until(
        std::chrono::system_clock::from_time_t(start_from)
    );

    for (uint current_requests_per_100ms : requests_per_100ms) {
        auto end_next_period_mcs = std::chrono::system_clock::now().time_since_epoch().count() / 10000000 * 10000 + 10000;
        while (current_requests_per_100ms > 0) {
            send_request();
            auto current_time_mcs = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
            auto remaining_time = std::max(static_cast<long>(0), end_next_period_mcs - current_time_mcs);
            auto next_time_mcs = current_time_mcs + remaining_time / current_requests_per_100ms;
            while (std::chrono::system_clock::now().time_since_epoch().count() / 1000 < next_time_mcs) {
                continue;
            }
            --current_requests_per_100ms;
        }
    }
    
    // Останавливаем поток проверки повторов
    is_running_ = false;
    
    // Ожидаем завершения потока
    if (retries_thread_.joinable()) {
        retries_thread_.join();
    }
}

void Sender::send_request() {
    Request request = Request(current_id_, current_user_, 0, std::chrono::system_clock::now().time_since_epoch().count() / 1000000);
    pipe_writer_requests_.write(request);
    ++current_id_;
    ++current_user_;
}

void Sender::check_retries() {
    while (is_running_) {
        auto retry = pipe_reader_retries_.read();
        if (retry.has_value()) {
            auto retry_value = retry.value();
            retries_.insert(retry_value);
        }
        if (retries_.size() > 0 && retries_.begin()->do_retry_timestamp() <= std::chrono::system_clock::now().time_since_epoch().count() / 1000000) {
            auto retry = *retries_.begin();
            retries_.erase(retries_.begin());
            Request request = Request(current_id_, retry.user(), retry.attempt() + 1, std::chrono::system_clock::now().time_since_epoch().count() / 1000000);
            pipe_writer_requests_.write(request);
            ++current_id_;
        }
    }
}
