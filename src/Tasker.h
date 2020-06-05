#ifndef BOMBA_TASKER_H
#define BOMBA_TASKER_H

#include <functional>
#include <vector>

class Tasker {
public:
    explicit Tasker(int core);

    void once(const std::function<void(void)> &op) const;
    void loop(const std::function<void(void)> &op) const;
    void loopEvery(int millis, const std::function<void(void)> &op) const;

private:
    int core;
};

extern Tasker DefaultTasker;
extern Tasker NetworkTasker;

#endif //BOMBA_TASKER_H
