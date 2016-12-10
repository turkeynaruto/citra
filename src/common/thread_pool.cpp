// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "thread_pool.h"

std::shared_ptr<ctpl::thread_pool> GlobalThreadPool::pool;
bool GlobalThreadPool::initialized = false;

void GlobalThreadPool::Loop(const std::function<void(int, int)>& loop, int lower, int upper) {
    Inititialize();

    int range = upper - lower;
    if (range >= 8 * 2) {
        int chunk = range / 8;
        int s = lower;

        std::future<void> futures[7];

        for (int i = 0; i < 8 - 1; ++i) {
            int end = s + chunk;
            futures[i] = GlobalThreadPool::pool->push([loop, s, end](int id) {
                loop(s, end);
            });
            s += chunk;
        }

        loop(s, upper);

        for (int i = 0; i < 8 - 1; ++i) {
            futures[i].wait();
        }
    } else {
        loop(lower, upper);
    }
}

void GlobalThreadPool::Inititialize() {
    if (!initialized) {
        pool = std::make_shared<ctpl::thread_pool>(8);
        initialized = true;
    }
}
