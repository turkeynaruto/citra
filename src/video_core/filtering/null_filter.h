// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "texture_filterer.h"

class NullFilter : public Filter {
    bool canFilter(Pica::Regs::TextureFormat format, int width, int height, int scaling) override;
    void filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int * fromBuffer, unsigned int * toBuffer, int scaling) override;
};
