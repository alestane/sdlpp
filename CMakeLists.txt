cmake_minimum_required (VERSION 3.8)

project(sdlpp LANGUAGES CXX)

add_subdirectory(src)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlpp PROPERTY CXX_STANDARD 20)
endif()
