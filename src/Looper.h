#ifndef BOMBA_LOOPER_H
#define BOMBA_LOOPER_H

#include <functional>
#include <vector>

class Looper {
public:
    Looper();

    void tick();

    void add(const std::function<void(void)> &op);

private:
    std::vector<std::function<void(void)>> ops;
};

extern Looper DefaultLooper;

#endif //BOMBA_LOOPER_H
