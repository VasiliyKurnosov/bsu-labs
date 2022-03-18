#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_


#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <atomic>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size);
    void Send(T value);
    std::pair<T, bool> Recv();
    void Close();

private:
    std::queue<T> buffer_;
    int buffer_size_;
    std::mutex lock_;
    std::condition_variable input_wait_condition_;
    std::condition_variable output_wait_condition_;
    std::atomic_bool is_open_;
};


#endif // BUFFERED_CHANNEL_H_

template<class T>
BufferedChannel<T>::BufferedChannel(int size) : buffer_size_(size), is_open_(true)
{
}

template<class T>
void BufferedChannel<T>::Send(T value)
{
    if (!is_open_)
    {
        throw std::runtime_error("Failed to send: channel is closed");
    }
    std::unique_lock<std::mutex> locker(lock_);
    if (buffer_.size() >= buffer_size_)
    {
        output_wait_condition_.wait(locker, [this]() { return buffer_.size() < buffer_size_ || !is_open_; });
        if (!is_open_)
        {
            throw std::runtime_error("Failed to send: channel is closed");
        }
    }
    buffer_.push(value);
    locker.unlock();
    input_wait_condition_.notify_one();
}

template<class T>
std::pair<T, bool> BufferedChannel<T>::Recv()
{
    std::unique_lock<std::mutex> locker(lock_);
    if (buffer_.empty())
    {
        input_wait_condition_.wait(locker, [this]() { return buffer_.size() > 0 || !is_open_; });
        if (!is_open_ && buffer_.empty())
        {
            return std::pair<T, bool>(T(), false);
        }
    }
    T result = buffer_.front();
    buffer_.pop();
    locker.unlock();
    output_wait_condition_.notify_one();
    return std::pair<T, bool>(result, true);
}

template<class T>
void BufferedChannel<T>::Close()
{
    is_open_ = false;
    input_wait_condition_.notify_all();
    output_wait_condition_.notify_all();
}
