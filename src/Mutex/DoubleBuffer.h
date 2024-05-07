//
// Created by lukas on 07/05/2024.
//

#ifndef LORACHAT_DOUBLEBUFFER_H
#define LORACHAT_DOUBLEBUFFER_H

#include <vector>
#include <cstdint>

using namespace std;

template<typename T>
class DoubleBuffer {
    vector<T> _buffers[2];
    uint8_t index = 0;

    vector<T>& getFront(){
        return _buffers[index%2];
    }

    vector<T> &getBack() {
        return _buffers[(index + 1) % 2];
    }

    void swap(){
        index++;
    }
};


#endif //LORACHAT_DOUBLEBUFFER_H
