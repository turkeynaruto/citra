// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "null_filter.h"

bool NullFilter::canFilter(Pica::Regs::TextureFormat format, int width, int height, int scaling) {
    return false;
}

void NullFilter::filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int * fromBuffer, unsigned int * toBuffer, int scaling) {
    LOG_CRITICAL(Render_OpenGL, "Attempting to filter using null filter!");
}
