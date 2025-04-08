#include "common/common.h"

#include <mutex>
#include <memory>

template <typename Item>
class Pipe {
public:
    class PipeReader {
    public:
        PipeReader(std::shared_ptr<Pipe<Item>> pipe)
            : pipe_(pipe) {
        }

        Item read() {
            std::lock_guard<std::mutex> lock(pipe_->mutex_);
            Item item = pipe_->queue_.front();
            pipe_->queue_.pop();
            return item;
        }

    private:
        std::shared_ptr<Pipe<Item>> pipe_;
    };

    class PipeWriter {

    };

    Pipe();

private:
    std::mutex mutex_;
    std::queue<Item> queue_;
};
