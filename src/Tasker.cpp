#include <Arduino.h>

#include "Tasker.h"

#include <mutex>

std::vector<std::function<void(void)>> ops;
std::mutex mutex;

Tasker::Tasker(int core) {
    this->core = core;
}

[[noreturn]] static void executeLoop(void *uarg) {
    auto *fnc_ptr = static_cast<std::function<void(void)> *>(uarg);
    auto fnc = std::move(*fnc_ptr);
    delete fnc_ptr;

    for (;;) {
        fnc();
    }
}

void execute(void *uarg) {
    auto *fnc_ptr = static_cast<std::function<void(void)> *>(uarg);
    auto fnc = std::move(*fnc_ptr);
    delete fnc_ptr;

    fnc();
}

void Tasker::loop(std::function<void(void)> op) const {
    auto *fnc = new std::function<void(void)>{[op] {
        op();
        taskYIELD();
    }};
    xTaskCreatePinnedToCore(executeLoop, "", 10000, static_cast<void *>(fnc), 1, NULL, core);
}

void Tasker::loopEvery(int millis, std::function<void(void)> op) const {
    auto *fnc = new std::function<void(void)>{[op, millis] {
        op();
        vTaskDelay(millis / portTICK_PERIOD_MS);
    }};
    xTaskCreatePinnedToCore(executeLoop, "", 10000, static_cast<void *>(fnc), 1, NULL, core);
}

void Tasker::once(std::function<void(void)> op) const {
    auto *fnc = new std::function<void(void)>{[op] {
        op();
        vTaskDelete(NULL);
    }};
    xTaskCreatePinnedToCore(execute, "", 10000, static_cast<void *>(fnc), 1, NULL, core);
}


Tasker DefaultTasker(0);
Tasker NetworkTasker(1);

