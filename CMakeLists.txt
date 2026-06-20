cmake_minimum_required(VERSION 3.20)
project(RAYTRACING)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)

add_executable(
        RayApp
        src/app/main.cpp
        src/math/Vector3.cpp
        src/math/headersrc/Vector3.h
        src/math/Ray.cpp
        src/math/headersrc/Ray.h
        src/display/Window.cpp
        src/display/headersrc/Window.h
        src/render/PixelBuffer.cpp
        src/render/headersrc/PixelBuffer.h
        src/render/Camera.cpp
        src/render/headersrc/Camera.h
)

target_include_directories(
        RayApp
        PUBLIC
        src/math/headersrc/
        src/display/headersrc/
        src/render/headersrc/
)

target_link_libraries(
        RayApp
        PRIVATE
        SDL3::SDL3
)