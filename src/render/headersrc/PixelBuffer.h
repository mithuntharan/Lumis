//
// Created by mithun on 6/7/26.
//

#ifndef RAYTRACING_PIXELBUFFER_H
#define RAYTRACING_PIXELBUFFER_H
#include <vector>
#include <cstdint>
#include  "Vector3.h"

namespace Lumis{
    class PixelBuffer{
    private:
        int pb_width; int pb_height;
        std::vector<uint32_t> pb_data;

    public:
        PixelBuffer(int width = 800, int height = 600);

        void setPixel(int x, int y, uint32_t color);
        void setPixel(int x, int y, const Vector3& color, float alpha = 1.0f);
        void set(uint32_t color);
        uint32_t* getData();
    };
} // Lumis

#endif //RAYTRACING_PIXELBUFFER_H
