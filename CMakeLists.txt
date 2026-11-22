cmake_minimum_required(VERSION 3.16)
project(rpi_embedded_linux_robot VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

option(PVM_BUILD_TESTS "Build unit tests (needs internet for GoogleTest)" OFF)

# Kernel modules are NOT built by CMake: see kernel/*/Makefile.

# Shared headers (protocol.h, ...)
add_library(pvm_common INTERFACE)
target_include_directories(pvm_common INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(tools/chardev_test)
add_subdirectory(tools/gpio_regs)

# Middleware services (secondary track)
add_subdirectory(services/camera_service)
add_subdirectory(services/decision_service)
add_subdirectory(services/motor_bridge)

if(PVM_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
