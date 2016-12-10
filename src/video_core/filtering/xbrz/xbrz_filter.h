// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "video_core/filtering/texture_filterer.h"
#include "xbrz.h"

class XbrzFilter : public Filter {
public:
    bool canFilter(Pica::Regs::TextureFormat format, int width, int height, int scaling) override;

    void filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int* fromBuffer,
        unsigned int* toBuffer, int scaling) override;
};
