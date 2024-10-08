#pragma once
#include "cdcRender/surfaces/CommonRenderTarget.h"
#include "cdcRender/surfaces/TextureMap.h"

void PPFastBlur(cdc::TextureMap *src, cdc::CommonRenderTarget *dst, uint32_t passMask, bool isHorizonalPass, bool weighted);
void PPAntiAlias(cdc::TextureMap *src, cdc::CommonRenderTarget *dst, uint32_t passMask);
