cmake_minimum_required(VERSION 3.15)

# ==============================================================================
# V4-hal Integration for V4 RTOS
# ==============================================================================
#
# This CMake file integrates V4-hal (Hardware Abstraction Layer) into V4 RTOS. It supports
# two modes: 1. Local source: Use V4-hal from a local directory (faster development) 2.
# Fetch from Git: Download V4-hal from GitHub (reproducible builds)
#
# Usage: - Local source: cmake -DV4HAL_LOCAL_PATH=/path/to/V4-hal .. - Fetch from Git:
# cmake .. (default behavior)
#

# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------

set(V4HAL_LOCAL_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/../V4-hal"
    CACHE PATH "Path to local V4-hal source directory")

set(V4HAL_GIT_REPOSITORY
    "https://github.com/V4-project/V4-hal.git"
    CACHE STRING "V4-hal Git repository URL")

set(V4HAL_GIT_TAG
    "v0.2.0"
    CACHE STRING "V4-hal Git tag or commit hash")

# ------------------------------------------------------------------------------
# Dependency Resolution
# ------------------------------------------------------------------------------

# Check if local V4-hal exists and is valid
if(EXISTS "${V4HAL_LOCAL_PATH}/CMakeLists.txt")
  message(STATUS "V4-hal: Using local source from ${V4HAL_LOCAL_PATH}")
  add_subdirectory(${V4HAL_LOCAL_PATH} ${CMAKE_BINARY_DIR}/v4hal-build)
else()
  # Fetch V4-hal from Git repository
  message(STATUS "V4-hal: Fetching from ${V4HAL_GIT_REPOSITORY} (${V4HAL_GIT_TAG})")

  include(FetchContent)
  fetchcontent_declare(
    v4hal
    GIT_REPOSITORY ${V4HAL_GIT_REPOSITORY}
    GIT_TAG ${V4HAL_GIT_TAG}
    GIT_SHALLOW TRUE)

  # Make V4-hal available
  fetchcontent_makeavailable(v4hal)

  message(STATUS "V4-hal: Successfully fetched from repository")
endif()

# ------------------------------------------------------------------------------
# Target Aliases
# ------------------------------------------------------------------------------

# Create consistent alias for V4-hal library This allows V4 RTOS components to link
# against 'v4_hal' regardless of source
if(TARGET v4-hal-lib)
  add_library(v4_hal ALIAS v4-hal-lib)
  message(STATUS "V4-hal: Created alias 'v4_hal' -> 'v4-hal-lib'")
else()
  message(FATAL_ERROR "V4-hal: Target 'v4-hal-lib' not found after dependency resolution")
endif()

# ------------------------------------------------------------------------------
# Platform Configuration
# ------------------------------------------------------------------------------

# V4 RTOS inherits HAL_PLATFORM from parent scope or sets default
if(NOT DEFINED HAL_PLATFORM)
  set(HAL_PLATFORM
      "posix"
      CACHE STRING "HAL target platform (posix, esp32, ch32v203)")
  message(STATUS "V4-hal: Platform not specified, defaulting to 'posix'")
endif()

message(STATUS "V4-hal: Target platform = ${HAL_PLATFORM}")

# ------------------------------------------------------------------------------
# Build Information
# ------------------------------------------------------------------------------

message(STATUS "V4-hal integration complete")
message(STATUS "  - Library target: v4-hal-lib")
message(STATUS "  - Alias target: v4_hal")
message(STATUS "  - Platform: ${HAL_PLATFORM}")
