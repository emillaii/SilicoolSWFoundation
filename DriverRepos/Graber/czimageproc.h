#ifndef CZIMAGEPROC_H
#define CZIMAGEPROC_H
#include <cstdint>

class ImageProc
{
public:
    enum EBayerPattern
    {
        BayerPattern_RG_GB = 0,
        BayerPattern_GR_BG,
        BayerPattern_GB_RG,
        BayerPattern_BG_GR,
    };

public:
    ImageProc();
    ~ImageProc();

    bool PackedRaw10ToRaw16(const uint8_t *src, uint16_t *dest, int width, int height);
    bool PakcedRaw10ToRaw8(const uint8_t *src, uint8_t *dest, int width, int height);
    void RawToRaw8(const uint16_t *src, uint8_t *dst, int w, int h, uint8_t bit);
    bool Raw8ToRGB24(const uint8_t *pRaw8, uint8_t *pRgb24, EBayerPattern pattern, int width, int height);

private:
    bool raw8GRBGSubImageToRgb24(const uint8_t *pRaw8, uint8_t *pRgb24, int width, int height);
};


#endif // CZIMAGEPROC_H
