#pragma once

#include <mutex>
#include <memory>
#include <optional>
#include <queue>

template <typename Item>
class Pipe : public std::enable_shared_from_this<Pipe<Item>> {
public:
    class PipeReader {
    public:
        PipeReader(std::shared_ptr<Pipe<Item>> pipe)
            : pipe_(pipe) {
        }

        std::optional<Item> read() {
            std::lock_guard<std::mutex> lock(pipe_->mutex_);
            if (pipe_->queue_.empty()) {
                return std::nullopt;
            }
            Item item = pipe_->queue_.front();
            pipe_->queue_.pop();
            return item;
        }

        Pipe<Item>* GetPipe() {
            return pipe_.get();
        }

    private:
        std::shared_ptr<Pipe<Item>> pipe_;
    };

    class PipeWriter {
    public:
        PipeWriter(std::shared_ptr<Pipe<Item>> pipe)
            : pipe_(pipe) {
        }

        void write(const Item& item) {
            std::lock_guard<std::mutex> lock(pipe_->mutex_);
            pipe_->queue_.push(item);
        }

        Pipe<Item>* GetPipe() {
            return pipe_.get();
        }

    private:
        std::shared_ptr<Pipe<Item>> pipe_;
    };

    Pipe() = default;

    PipeReader GetReader() {
        return PipeReader(this->shared_from_this());
    }

    PipeWriter GetWriter() {
        return PipeWriter(this->shared_from_this());
    }

    size_t GetSize() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

protected:
    std::mutex mutex_;
    std::queue<Item> queue_;
};
