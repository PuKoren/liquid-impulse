project(LiquidImpulse)
cmake_minimum_required(VERSION 2.8)

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

find_Package ( SDL REQUIRED )
find_Package ( SDL_image REQUIRED )
find_Package ( SDL_ttf REQUIRED )
find_Package ( SDL_mixer REQUIRED )

if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

include_directories(
${SDL_INCLUDE_DIR}
${SDLIMAGE_INCLUDE_DIR}
${SDLTTF_INCLUDE_DIR}
${SDLMIXER_INCLUDE_DIR}
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/include/MinIni
)

file(GLOB SRCS src/*.cpp)
file(GLOB MININI src/MinIni/*.c)
file(GLOB HDRS include/*.h)

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLTTF_LIBRARY}
   ${SDLMIXER_LIBRARY}
   SDLmain
)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${MININI})

