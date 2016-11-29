// Copyright 2016 Citra Emulator Project / PPSSPP Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <ctpl_stl.h>

class GlobalThreadPool {
public:
    // will execute slices of "loop" from "lower" to "upper"
    // in parallel on the global thread pool
    static void Loop(const std::function<void(int, int)>& loop, int lower, int upper);

private:
    static std::shared_ptr<ctpl::thread_pool> pool;
    static bool initialized;
    static void Inititialize();
};
