# Copyright 2026 Robert G. Patterson.
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.24)

project(denigma_examples VERSION 0.1.0 LANGUAGES CXX)

set(DENIGMA_EXAMPLES_CXX_STANDARD "20" CACHE STRING "C++ standard used to build Denigma examples; must be at least 20")
if(DENIGMA_EXAMPLES_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "DENIGMA_EXAMPLES_CXX_STANDARD must be at least 20")
endif()

set(CMAKE_CXX_STANDARD ${DENIGMA_EXAMPLES_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Build for both arm64 and x86_64 in Release mode to match Denigma.
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "macOS architectures for Release" FORCE)
    else()
        # Match the current architecture in non-Release builds.
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
            set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "macOS architecture for Debug" FORCE)
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
            set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "macOS architecture for Debug" FORCE)
        else()
            message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
        endif()
    endif()
    message(STATUS "macOS Deployment Target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    message(STATUS "macOS Architectures: ${CMAKE_OSX_ARCHITECTURES}")
endif()

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/DNOMINMAX)
    add_compile_options(/bigobj)
    add_compile_options(/Zc:preprocessor)
    add_compile_options(/W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

option(DENIGMA_EXAMPLES_BUILD_WASM_MNX "Build the MNX WebAssembly example" ON)
option(DENIGMA_EXAMPLES_BUILD_WASM_ENIGMAXML "Build the Enigma XML WebAssembly example" ON)
option(DENIGMA_EXAMPLES_BUILD_TEXT_EXPRESSION_CLASSIFIER "Build the TextExpressionDef classification CLI example" ON)

if(DENIGMA_EXAMPLES_BUILD_WASM_MNX)
    add_subdirectory(examples/wasm-mnx)
endif()

if(DENIGMA_EXAMPLES_BUILD_WASM_ENIGMAXML)
    add_subdirectory(examples/wasm-enigmaxml)
endif()

if(DENIGMA_EXAMPLES_BUILD_TEXT_EXPRESSION_CLASSIFIER)
    add_subdirectory(examples/text-expression-classifier)
endif()
