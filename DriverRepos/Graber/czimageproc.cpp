#include "czimageproc.h"
#include <opencv2/imgproc.hpp>

ImageProc::ImageProc()
{
}


ImageProc::~ImageProc()
{
}


bool ImageProc::PackedRaw10ToRaw16(const uint8_t *src, uint16_t *dest, int width, int height)
{
    if (!src || !dest)
        return false;

    for (int p = 0, k = 0; p < width * height * 5 / 4; p += 5)
    {
        dest[k + 0] = ((uint16_t)src[p + 0] << 2) | (src[p + 4] >> 6 & 0x03);
        dest[k + 1] = ((uint16_t)src[p + 1] << 2) | (src[p + 4] >> 4 & 0x03);
        dest[k + 2] = ((uint16_t)src[p + 2] << 2) | (src[p + 4] >> 2 & 0x03);
        dest[k + 3] = ((uint16_t)src[p + 3] << 2) | (src[p + 4] & 0x03);
        k += 4;
    }
    return true;
}

bool ImageProc::PakcedRaw10ToRaw8(const uint8_t *src, uint8_t *dest, int width, int height)
{
    if (!src || !dest)
        return false;

    for (int p = 0, k = 0; p < width * height * 5 / 4; p += 5)
    {
        dest[k + 0] = (src[p + 0]);
        dest[k + 1] = (src[p + 1]);
        dest[k + 2] = (src[p + 2]);
        dest[k + 3] = (src[p + 3]);
        k += 4;
    }
    return true;
}

void ImageProc::RawToRaw8(const uint16_t *src, uint8_t *dst, int w, int h, uint8_t bit)
{
    assert((nullptr != src) && (nullptr != dst));
    if ((nullptr == src) || (nullptr == dst)) {
        return;
    }

    for (int i = 0; i < w * h; i++)
    {
        dst[i] = (src[i] >> bit) & 0xFF;
    }
}

#include "CmtiSdk.h"
bool ImageProc::Raw8ToRGB24(const uint8_t *pRaw8, uint8_t *pRgb24, EBayerPattern pattern, int width, int height)
{
#if 1
    T_FrameParam frameParam;
    frameParam.Width = width;
    frameParam.Height = height;
    frameParam.ImageFormat = ImgFmt_RAW8;
    frameParam.ImageMode = (E_ImageMode)pattern;
    frameParam.Size = width * height;
    CmtiSdk::ISP_DataToRgb24(pRgb24, pRaw8, frameParam, 0);
#else
    switch (pattern)
    {
    case BayerPattern_RG_GB:
        raw8GRBGSubImageToRgb24(pRaw8 + 1, pRgb24 + 1 * 3, width, height);
        break;
    case BayerPattern_GR_BG:
        raw8GRBGSubImageToRgb24(pRaw8, pRgb24, width, height);
        break;
    case BayerPattern_GB_RG:
        raw8GRBGSubImageToRgb24(pRaw8 + width + 1, pRgb24 + (width + 1) * 3, width, height);
        break;
    case BayerPattern_BG_GR:
        raw8GRBGSubImageToRgb24(pRaw8 + width, pRgb24 + width * 3, width, height);
        break;
    default:
        return false;
        break;
    }
#endif
    return true;
}

bool ImageProc::raw8GRBGSubImageToRgb24(const uint8_t *pRaw8, uint8_t *pRgb24, int width, int height)
{
    int r, c;
    const uint8_t *pUpBG = nullptr, *pGR = nullptr, *pBG = nullptr, *pDownGR = nullptr;
    uint8_t *out = nullptr, *out2 = nullptr;

    for (r = 2; r < height - 4; r += 2) {
        pGR = pRaw8 + r * width + 2; /* 向右偏移2个像素开始插值 */
        pUpBG = pGR - width;
        pBG = pGR + width;
        pDownGR = pGR + 2 * width;
        out = pRgb24 + (r * width + 2) * 3;
        out2 = out + 3 * width;
        for (c = 2; c < width - 2; c += 2) {
            uint32_t B12 = pUpBG[0];
            uint32_t G13 = pUpBG[1];
            uint32_t B14 = pUpBG[2];
            uint32_t R21 = pGR[-1];
            uint32_t G22 = pGR[0];
            uint32_t R23 = pGR[1];
            uint32_t G24 = pGR[2];
            uint32_t G31 = pBG[-1];
            uint32_t B32 = pBG[0];
            uint32_t G33 = pBG[1];
            uint32_t B34 = pBG[2];
            uint32_t R41 = pDownGR[-1];
            uint32_t G42 = pDownGR[0];
            uint32_t R43 = pDownGR[1];

            /*************************gr插值 *****************************/
            uint32_t R = (R21 + R23) / 2;
            uint32_t B = (B12 + B32) / 2;
            uint32_t G = G22;
            //out[0] = R; out[1] = G; out[2] = B;
            out[0] = B; out[1] = G; out[2] = R;

            /*************************r插值 *****************************/
            R = R23;
            G = (G13 + G33 + G22 + G24) / 4;
            B = (B12 + B14 + B32 + B34) / 4;
            //out[3] = R; out[4] = G; out[5] = B;
            out[3] = B; out[4] = G; out[5] = R;

            /*************************b插值 *****************************/
            R = (R21 + R23 + R41 + R43) / 4;
            G = (G22 + G42 + G31 + G33) / 4;
            B = B32;
            //out2[0] = R; out2[1] = G; out2[2] = B;
            out2[0] = B; out2[1] = G; out2[2] = R;

            /*************************bg插值 *****************************/
            R = (R23 + R43) / 2;
            G = G33;
            B = (B32 + B34) / 2;
            //out2[3] = R; out2[4] = G; out2[5] = B;
            out2[3] = B; out2[4] = G; out2[5] = R;

            pGR += 2;
            pUpBG += 2;
            pBG += 2;
            pDownGR += 2;
            out += 2 * 3;
            out2 += 2 * 3;
        }
    }
    return true;
}
