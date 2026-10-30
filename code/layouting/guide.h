#pragma once
#include <cstddef>
#include "viewport.h"
struct GuideCreateInfo
{
    bool relativeSizing;
    float height, width;
    float offsetX, offsetY;	
};

typedef GuideId uint32_t;

/// Create a new guide
GuideId CreateGuide(const GuideCreateInfo info);
/// Destroy guide
void DestroyGuide(GuideId id);

/// Apply a viewport to guide, resizing it if necessary, and adjusting offsets
void GuideApplyViewport(GuideId guide, ViewportId viewport);

