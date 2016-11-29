// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "video_core/debug_utils/debug_utils.h"

namespace Filtering {
enum class FilteringTypes {
    NONE = 0,
    XBRZ = 1,
};
};

struct TextureSize {
    int width;
    int height;
};

/**
 * The implementation of a specific filter.
 */
class Filter {
public:
    virtual bool canFilter(Pica::Regs::TextureFormat format, int width, int height, int scaling) {
        LOG_CRITICAL(Render_OpenGL, "Calling non-derived filter!");
        return false;
    }

    virtual void filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int* fromBuffer,
        unsigned int* toBuffer, int scaling) {

    }
};

class Filterer {
public:
    Filterer();
    ~Filterer();

    // Checks if scaling is enabled
    const bool isScalingEnabled();

    // Returns the size of a particular scale
    const int getScaling();

    // Filters a texture using the specified texture
    void filterTexture(Pica::DebugUtils::TextureInfo tex_info, unsigned int* fromBuffer,
        unsigned int* toBuffer, int bytes_per_pixel);

    // Returns the scaled texture size (width * height) of this texture
    int getScaledTextureSize(Pica::Regs::TextureFormat format, int width, int height);

    // Returns the scaled texture dimensions of this texture
    TextureSize getScaledTextureDimensions(Pica::Regs::TextureFormat format, int width, int height);

    // Checks if a shader is willing to filter a particular image
    bool canFilter(Pica::Regs::TextureFormat format, int width, int height);

private:
    bool isEmpty(unsigned int* data, int count, int format);

    int scaling;
    Filtering::FilteringTypes type;

    Filter* filter;
};
