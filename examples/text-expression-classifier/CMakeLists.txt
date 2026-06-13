# Copyright 2026 Robert G. Patterson.
# SPDX-License-Identifier: MIT

include(FetchContent)

set(DENIGMA_GIT_REPOSITORY "https://github.com/rpatters1/denigma.git" CACHE STRING "Denigma git repository used by FetchContent" FORCE)
set(DENIGMA_GIT_TAG "main" CACHE STRING "Denigma git tag, branch, or commit used by FetchContent")

set(denigma_BUILD_TESTING OFF CACHE BOOL "Do not build Denigma tests from examples" FORCE)
set(denigma_BUILD_DOCS OFF CACHE BOOL "Do not build Denigma docs from examples" FORCE)

# Disable building tests for tinyxml2
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "Do not build tests for tinyxml2")

# Add tinyxml2
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(tinyxml2)

FetchContent_Declare(
    denigma
    GIT_REPOSITORY ${DENIGMA_GIT_REPOSITORY}
    GIT_TAG        ${DENIGMA_GIT_TAG}
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(denigma)

add_executable(text_expression_classifier
    src/main.cpp
)

set_target_properties(text_expression_classifier PROPERTIES
    CXX_STANDARD ${DENIGMA_EXAMPLES_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(text_expression_classifier PRIVATE
    denigma::classify
)

if(TARGET tinyxml2)
    target_link_libraries(text_expression_classifier PRIVATE tinyxml2)
elseif(TARGET tinyxml2::tinyxml2)
    target_link_libraries(text_expression_classifier PRIVATE tinyxml2::tinyxml2)
else()
    message(FATAL_ERROR "Unable to find a tinyxml2 target after FetchContent_MakeAvailable(tinyxml2).")
endif()
