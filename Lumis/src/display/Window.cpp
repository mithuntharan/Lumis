#include "headersrc/Window.h"
#include  <SDL3/SDL.h>

namespace Lumis{
    Window::Window(const int width, const int height, const std::string& title){
        SDL_Init(SDL_INIT_VIDEO);
        w_window = SDL_CreateWindow(title.c_str(), width, height, 0); if (!w_window) SDL_Log("Window Failed: %s", SDL_GetError());
        w_renderer = SDL_CreateRenderer(w_window, nullptr); if (!w_renderer) SDL_Log("Renderer failed: %s", SDL_GetError());
        w_texture = SDL_CreateTexture(w_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height); if (!w_texture) SDL_Log("Texture failed: %s", SDL_GetError());
        SDL_SetRenderDrawColor(w_renderer, 255, 255, 255, 0);
        w_width = width; w_height = height;
        w_shouldClose = false;
    }

    Window::~Window(){
        SDL_DestroyTexture(w_texture);
        SDL_DestroyRenderer(w_renderer);
        SDL_DestroyWindow(w_window);
        SDL_Quit();
    }

    void Window::update(const uint32_t* pixels) const{
        SDL_UpdateTexture(w_texture, nullptr, pixels, w_width * sizeof(uint32_t));
        SDL_RenderClear(w_renderer);
        SDL_RenderTexture(w_renderer, w_texture, nullptr, nullptr);
        SDL_RenderPresent(w_renderer);
    }

    void Window::pollEvents(){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                w_shouldClose = true;
            }
        }
    }

    bool Window::shouldClose() const{
        return w_shouldClose;
    }

    int Window::getHeight() const{
        return w_height;
    }

    int Window::getWidth() const{
        return w_width;
    }
}
