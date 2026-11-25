cmake_minimum_required(VERSION 3.20)
project(RAYTRACING)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL")
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
        src/render/Material.cpp
        src/render/headersrc/Material.h
        src/scene/Scene.cpp
        src/scene/headersrc/Scene.h
        src/scene/Sphere.cpp
        src/scene/headersrc/Sphere.h
        src/scene/headersrc/Hittable.h
        src/scene/headersrc/HitRecord.h
)

target_include_directories(
        RayApp
        PUBLIC
        src/math/headersrc/
        src/display/headersrc/
        src/render/headersrc/
        src/scene/headersrc/
        dependencies/SDL/include/
        dependencies/SDL/include/SDL3
)

target_link_libraries(
        RayApp
        PRIVATE
        SDL3::SDL3
)

add_custom_command(TARGET RayApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:RayApp>
)