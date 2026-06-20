
#ifndef RAYTRACING_WINDOW_H
#define RAYTRACING_WINDOW_H
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace Lumis{
    class Window{
    private:
        SDL_Window* w_window = nullptr;
        SDL_Renderer* w_renderer = nullptr;
        SDL_Texture* w_texture = nullptr;
        int w_width = 0, w_height = 0;
        bool w_shouldClose;

    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        Window(Window&&) = delete;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        // Methods
        void update(const uint32_t* pixels) const;
        void pollEvents();
        bool shouldClose() const;

        // Getters
        int getWidth() const;
        int getHeight() const;

    };
}


#endif //RAYTRACING_WINDOW_H
