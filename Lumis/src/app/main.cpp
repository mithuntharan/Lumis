#include "Vector3.h"
#include "Window.h"
#include "Camera.h"
#include "PixelBuffer.h"
#include "Ray.h"

int main(){
    Lumis::Window window(800, 600, "LUMIS - Ray Tracer");

    Lumis::PixelBuffer pixels(800, 600);
    Lumis::Camera camera({0, 0, 1}, {0, 0, -1}, 108, static_cast<float>(800 / 600));

    for (int i = 0; i < 800; i++){
        for (int j = 0; j < 600; j++){
            Lumis::Ray ray = camera.generateRay(i, j, 800, 600);
            Lumis::Vector3 color = (ray.direction + Lumis::Vector3(1, 1, 1)) * 0.5;
            pixels.setPixel(i, j, color);
        }
    }

    // Main Loop
    while (!window.shouldClose()) {
        window.update(pixels.getData());
        window.pollEvents();
    }
}
