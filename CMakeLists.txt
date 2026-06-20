cmake_minimum_required(VERSION 3.20)
project(RAYTRACING)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)

add_executable(
        RayApp
        Lumis/src/app/main.cpp
        Lumis/src/math/Vector3.cpp
        Lumis/src/math/headersrc/Vector3.h
        Lumis/src/math/Ray.cpp
        Lumis/src/math/headersrc/Ray.h
        Lumis/src/display/Window.cpp
        Lumis/src/display/headersrc/Window.h
        Lumis/src/render/PixelBuffer.cpp
        Lumis/src/render/headersrc/PixelBuffer.h
        Lumis/src/render/Camera.cpp
        Lumis/src/render/headersrc/Camera.h
)

target_include_directories(
        RayApp
        PUBLIC
        Lumis/src/math/headersrc/
        Lumis/src/display/headersrc/
        Lumis/src/render/headersrc/
)

target_link_libraries(
        RayApp
        PRIVATE
        SDL3::SDL3
)