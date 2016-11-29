// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/thread_pool.h"
#include "xbrz_filter.h"
#include "xbrz.h"

bool XbrzFilter::canFilter(Pica::Regs::TextureFormat format, int width, int height, int scaling) {
    return scaling >= 2 && scaling <= 6 &&
        width > 4 && height > 4;
}

void XbrzFilter::filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int* fromBuffer, unsigned int* toBuffer, int scaling) {
    xbrz::ScalerCfg cfg;

    if (tex_info.format == Pica::Regs::TextureFormat::RGB8) {
        GlobalThreadPool::Loop(std::bind(&xbrz::scale, scaling, fromBuffer, toBuffer, tex_info.width,
            tex_info.height, xbrz::ColorFormat::RGB, cfg, std::placeholders::_1, std::placeholders::_2), 0, tex_info.height);
    } else {
        GlobalThreadPool::Loop(std::bind(&xbrz::scale, scaling, fromBuffer, toBuffer, tex_info.width,
            tex_info.height, xbrz::ColorFormat::ARGB, cfg, std::placeholders::_1, std::placeholders::_2), 0, tex_info.height);
    }
}
