# Copyright 2026 Robert G. Patterson.
# SPDX-License-Identifier: MIT

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "examples/wasm-enigmaxml must be configured with Emscripten. Use emcmake cmake ...")
endif()

include(FetchContent)

set(DENIGMA_GIT_REPOSITORY "https://github.com/rpatters1/denigma.git" CACHE STRING "Denigma git repository used by FetchContent" FORCE)
set(DENIGMA_GIT_TAG "main" CACHE STRING "Denigma git tag, branch, or commit used by FetchContent")
set(DENIGMA_SOURCE_DIR "" CACHE PATH "Optional local Denigma source checkout; when set, overrides FetchContent download")

if(DENIGMA_SOURCE_DIR)
    get_filename_component(DENIGMA_SOURCE_DIR "${DENIGMA_SOURCE_DIR}" ABSOLUTE)
    if(NOT EXISTS "${DENIGMA_SOURCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "DENIGMA_SOURCE_DIR does not point to a Denigma checkout: ${DENIGMA_SOURCE_DIR}")
    endif()
    set(FETCHCONTENT_SOURCE_DIR_DENIGMA "${DENIGMA_SOURCE_DIR}" CACHE PATH "Local Denigma source override" FORCE)
endif()

set(denigma_BUILD_TESTING OFF CACHE BOOL "Do not build Denigma tests from examples" FORCE)
set(denigma_BUILD_DOCS OFF CACHE BOOL "Do not build Denigma docs from examples" FORCE)

FetchContent_Declare(
    denigma
    GIT_REPOSITORY ${DENIGMA_GIT_REPOSITORY}
    GIT_TAG        ${DENIGMA_GIT_TAG}
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(denigma)

add_executable(denigma_wasm_enigmaxml
    src/denigma_wasm_enigmaxml.cpp
)

set_target_properties(denigma_wasm_enigmaxml PROPERTIES
    CXX_STANDARD ${DENIGMA_EXAMPLES_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_link_libraries(denigma_wasm_enigmaxml PRIVATE denigma::enigmaxml)

set_target_properties(denigma_wasm_enigmaxml PROPERTIES
    OUTPUT_NAME denigma_wasm_enigmaxml
    SUFFIX ".js"
)

target_link_options(denigma_wasm_enigmaxml PRIVATE
    "-sMODULARIZE=1"
    "-sEXPORT_ES6=1"
    "-sENVIRONMENT=web,node"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sEXPORTED_FUNCTIONS=['_denigma_malloc','_denigma_free','_denigma_musx_to_enigmaxml']"
    "-sEXPORTED_RUNTIME_METHODS=['HEAPU8','UTF8ToString','getValue','setValue']"
)
