#include <Arduino.h>

#include "Looper.h"

void Looper::tick() {
    for (auto &op : ops)
        op();
}

void Looper::add(const std::function<void(void)> &op) {
    ops.push_back(op);
}

Looper::Looper() = default;

Looper DefaultLooper;

