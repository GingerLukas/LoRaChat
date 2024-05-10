//
// Created by lukas on 07/05/2024.
//

#ifndef LORACHAT_TOUCHPOINT_H
#define LORACHAT_TOUCHPOINT_H


#include "stdint.h"

struct TouchPoint {
    uint16_t x, y;
    uint64_t time;

    TouchPoint() {
        x = -1;
        y = -1;
        time = -1;
    }

    TouchPoint(const TP_Point &point) :
            x(point.x), y(point.y),
            time(millis()) {
    }

    uint64_t age() {
        return millis() - time;
    }

    bool operator==(const TouchPoint &other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const TouchPoint &other) {
        return !(*this == other);
    }
};

#endif //LORACHAT_TOUCHPOINT_H
