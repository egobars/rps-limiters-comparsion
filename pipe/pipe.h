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

        bool is_empty() {
            std::lock_guard<std::mutex> lock(pipe_->mutex_);
            return pipe_->queue_.empty();
        }

        Item read() {
            std::lock_guard<std::mutex> lock(pipe_->mutex_);
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

    Pipe();

    PipeReader GetReader();

    PipeWriter GetWriter();

    size_t GetSize();

private:
    std::mutex mutex_;
    std::queue<Item> queue_;
};
