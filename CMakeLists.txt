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
