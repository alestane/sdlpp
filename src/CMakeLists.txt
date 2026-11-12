cmake_minimum_required (VERSION 3.8)

set(SDL_HPP ../include/sdlpp/)

set(SDLPP_HEADERS
	"_def"
	"${SDL_HPP}sdl.h")

set(SDLPP_SOURCES
	"sdl.cpp")

add_library(sdlpp STATIC ${SDLPP_HEADERS} ${SDLPP_SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlpp PROPERTY CXX_STANDARD 20)
endif()
