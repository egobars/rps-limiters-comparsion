#include "pipe.h"

template <typename Item>
Pipe<Item>::PipeReader Pipe<Item>::GetReader() {
    return PipeReader(shared_from_this());
}

template <typename Item>
Pipe<Item>::PipeWriter Pipe<Item>::GetWriter() {
    return PipeWriter(shared_from_this());
}

template <typename Item>
size_t Pipe<Item>::GetSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}
