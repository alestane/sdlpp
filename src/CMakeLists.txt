cmake_minimum_required (VERSION 3.8)

add_library(sdlpp STATIC)

target_sources(sdlpp 
	INTERFACE 
		sdl2.h 
	PUBLIC 
		sdl2.cpp
)

target_include_directories(sdlpp PUBLIC "../include/sdlpp/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlpp PROPERTY CXX_STANDARD 20)
endif()
