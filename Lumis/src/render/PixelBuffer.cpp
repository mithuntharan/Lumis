#include "headersrc/PixelBuffer.h"

#include <algorithm>
#include  <iostream>
#include  <cmath>
#include  "Vector3.h"
#include <SDL3/SDL_log.h>

namespace Lumis{
    PixelBuffer::PixelBuffer(const int width, const int height):
        pb_width(width), pb_height(height),
        pb_data((width * height), 0xFF000000) {}

    void PixelBuffer::set(const uint32_t color){
        for (uint32_t& pixel: pb_data){
            pixel = color;
        }
    }

    uint32_t* PixelBuffer::getData(){
        return &(pb_data[0]);
    }

    void PixelBuffer::setPixel(const int x,const  int y, const uint32_t color){
        if (y > pb_height - 1 || x > pb_width - 1 || x < 0 || y < 0) return;
        pb_data[y * pb_width + x] = color;
    }

    void PixelBuffer::setPixel(const int x, const int y, const Vector3& color, const float alpha){
        if (y > pb_height - 1 || x > pb_width - 1 || x < 0 || y < 0) return;

        const uint8_t red = std::clamp(std::sqrtf(color[0]), 0.0f, 1.0f) * 255;
        const uint8_t green = std::clamp(std::sqrtf(color[1]), 0.0f, 1.0f) * 255;
        const uint8_t blue = std::clamp(std::sqrtf(color[2]), 0.0f, 1.0f) * 255;

        //
        const uint32_t extractedColor =
            static_cast<uint32_t>(alpha * 255) << 24 |
            static_cast<uint32_t>(red) << 16 |
            static_cast<uint32_t>(green) << 8  |
            static_cast<uint32_t>(blue);

        pb_data[y * pb_width + x] = extractedColor;
    }
} // Lumis