//
// Created by lukas on 11/05/2024.
//

#ifndef LORACHAT_THREADSAFEQUEUE_H
#define LORACHAT_THREADSAFEQUEUE_H

#include <queue>
#include "ThreadSafe.h"

template<typename T>
struct ThreadSafeQueue {
    T get() {
        T tmp = T();
        _queue.lock();
        if (!_queue.value.empty()) {
            tmp = _queue.value.front();
            _queue.value.pop();
        }
        _queue.unlock();
        return tmp;
    }

    void put(const T &value) {
        _queue.lock();
        _queue.value.push(value);
        _queue.unlock();
    }

    size_t size() {
        _queue.lock();
        size_t size = _queue.value.size();
        _queue.unlock();

        return size;
    }

private:
    ThreadSafe<std::queue<T>> _queue;
};

#endif //LORACHAT_THREADSAFEQUEUE_H
