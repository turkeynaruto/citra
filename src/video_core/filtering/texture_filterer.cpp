// Copyright 2016 Citra Emulator Project / PPSSPP Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <vector>
#include "common/vector_math.h"
#include "core/memory.h"
#include "core/settings.h"
#include "video_core/debug_utils/debug_utils.h"
#include "video_core/filtering/null_filter.h"
#include "video_core/filtering/texture_filterer.h"
#include "video_core/filtering/xbrz/xbrz_filter.h"

Filterer::Filterer() {
    int scaling = Settings::values.tex_filter_scaling;

    if (scaling < 1 || scaling > 6) {
        scaling = 1;
    }

    this->scaling = scaling;

    // TODO (Selby): Implement bilinear etc
    switch (static_cast<Filtering::FilteringTypes>(Settings::values.tex_filter)) {
        case Filtering::FilteringTypes::XBRZ:
            filter = new XbrzFilter();
            break;
        case Filtering::FilteringTypes::NONE:
            filter = new NullFilter();
            break;
        default:
            filter = nullptr;
    }
}

Filterer::~Filterer() {
    delete filter;
}

const bool Filterer::isScalingEnabled() {
    return scaling != 1 && filter != nullptr;
}

const int Filterer::getScaling() {
    return scaling;
}

bool Filterer::isEmpty(unsigned int* data, int count, int bytes_per_pixel) {
    int pixelsPerWord = 4 / bytes_per_pixel;
    u32 ref = data[0];
    if (pixelsPerWord > 1 && (ref & 0x0000FFFF) != (ref >> 16)) {
        return false;
    }
    for (int i = 0; i < count / pixelsPerWord; ++i) {
        if (data[i] != ref) return false;
    }
    return true;
}

bool Filterer::canFilter(Pica::Regs::TextureFormat format, int width, int height) {
    return isScalingEnabled() && filter->canFilter(format, width, height, scaling);
}

void Filterer::filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int* fromBuffer,
                              unsigned int* toBuffer, int bytes_per_pixel) {
    if (bytes_per_pixel > 0 && isEmpty(fromBuffer, tex_info.width * tex_info.height, bytes_per_pixel)) {
        // We can cheat here, as isEmpty() checks for a texture with a single color.
        unsigned int data = fromBuffer[0];
        for (int i = 0; i < tex_info.width * tex_info.height * getScaling(); i++) {
            toBuffer[i] = data;
        }
    } else {
        filter->filterTexture(tex_info, fromBuffer, toBuffer, scaling);
    }
}

TextureSize Filterer::getScaledTextureDimensions(Pica::Regs::TextureFormat format, int width, int height) {
    TextureSize dimensions;

    if (canFilter(format, width, height)) { // Check that this is in range
        dimensions.width = width * scaling;
        dimensions.height = height * scaling;

        int currentScaling = scaling;

        while ((width > 2048 || height > 2048) && currentScaling > 1) {
            currentScaling--;
            dimensions.width = width * currentScaling;
            dimensions.height = height * currentScaling;
        }
    } else {
        dimensions.width = width;
        dimensions.height = height;
    }

    return dimensions;
}

int Filterer::getScaledTextureSize(Pica::Regs::TextureFormat format, int width, int height) {
    TextureSize dimensions = getScaledTextureDimensions(format, width, height);
    return dimensions.width * dimensions.height;
}
