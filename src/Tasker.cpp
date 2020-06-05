#include <Arduino.h>

#include "Tasker.h"

#include <map>
#include <mutex>

std::vector<std::function<void(void)>> ops;
std::mutex mutex;

Tasker::Tasker(int core) {
    this->core = core;
}

[[noreturn]] void executeLoop(void *notUsed) {
    auto op = ops.back();
    ops.pop_back();
    mutex.unlock();

    for (;;) {
        op();
    }
}

void execute(void *notUsed) {
    auto op = ops.back();
    mutex.unlock();

    op();
}

void Tasker::loop(const std::function<void(void)> &op) const {
    mutex.lock();
    ops.emplace_back([op] {
        op();
        taskYIELD();
    });
    xTaskCreatePinnedToCore(executeLoop, "", 10000, NULL, 1, NULL, core);
}

void Tasker::loopEvery(int millis, const std::function<void(void)> &op) const {
    mutex.lock();
    ops.emplace_back([op, millis] {
        op();
        vTaskDelay(millis / portTICK_PERIOD_MS);
    });
    xTaskCreatePinnedToCore(executeLoop, "", 10000, NULL, 1, NULL, core);
}

void Tasker::once(const std::function<void(void)> &op) const {
    mutex.lock();
    ops.emplace_back([op] {
        op();
        vTaskDelete(NULL);
    });
    xTaskCreatePinnedToCore(execute, "", 10000, NULL, 1, NULL, core);
}


Tasker DefaultTasker(0);
Tasker NetworkTasker(1);

