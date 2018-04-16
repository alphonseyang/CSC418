cmake_minimum_required(VERSION 3.9)
project(raytracer)

set(CMAKE_CXX_STANDARD 11)

add_executable(raytracer
    bmp_io.cpp
    bmp_io.h
    light_source.cpp
    light_source.h
    main.cpp
    raytracer.cpp
    raytracer.h
    scene_object.cpp
    scene_object.h
    util.cpp
    util.h texture.h)
