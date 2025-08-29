cmake_minimum_required(VERSION 3.26)
project(
  vk_sdl3_imgui_cpp_starter
  VERSION 0.1.0
  DESCRIPTION "SDL3/Vulkan-based C++ starter template with ImGui"
  LANGUAGES C CXX)

# Set C++ standard and policies
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Improve build output organization
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable folder organization for IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Package management with CPM
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
  file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH
      SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494)
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Core Dependencies
CPMAddPackage("gh:libsdl-org/SDL#b5c3eab")
CPMAddPackage("gh:libsdl-org/SDL_image#4a762bd")

# Vulkan Dependencies
CPMAddPackage("gh:KhronosGroup/Vulkan-Headers#vulkan-sdk-1.4.304.0")
CPMAddPackage("gh:KhronosGroup/SPIRV-Headers#vulkan-sdk-1.4.304.0")
CPMAddPackage("gh:KhronosGroup/SPIRV-Tools#vulkan-sdk-1.4.304.0")
CPMAddPackage("gh:KhronosGroup/glslang#vulkan-sdk-1.4.304.0")
CPMAddPackage("gh:zeux/volk#0b17a76")
CPMAddPackage("gh:GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator#c788c52")

# UI Framework - Native C++ ImGui (not cimgui)
CPMAddPackage("gh:ocornut/imgui#docking")  # Use docking branch for multi-window support

# Math Library
CPMAddPackage("gh:recp/cglm#1796cc5")

# JSON Support (useful for many applications)
CPMAddPackage("gh:nlohmann/json#v3.11.3")

# Logging (professional applications should have logging)
CPMAddPackage("gh:gabime/spdlog#v1.12.0")

# Testing Framework (optional, can be disabled)
CPMAddPackage("gh:google/googletest#v1.14.0")

# Configure ImGui backends for SDL3 + Vulkan
if(imgui_ADDED)
  target_sources(imgui PRIVATE
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
  )
  target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}/backends
  )
  target_link_libraries(imgui PUBLIC SDL3::SDL3 Vulkan::Headers)
endif()

# Main library target (for reusable components)
add_library(${PROJECT_NAME}_lib INTERFACE)
target_link_libraries(
  ${PROJECT_NAME}_lib
  INTERFACE 
    Vulkan::Headers
    SPIRV-Headers::SPIRV-Headers
    volk::volk_headers
    glslang
    GPUOpen::VulkanMemoryAllocator
    SDL3::SDL3
    SDL3_image::SDL3_image
    imgui
    cglm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

# Compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(${PROJECT_NAME}_lib INTERFACE 
    -Wall -Wextra -Wpedantic -Wshadow)
    # Note: -Werror removed for development flexibility
elseif(MSVC)
  target_compile_options(${PROJECT_NAME}_lib INTERFACE /W4)
  # Note: /WX removed for development flexibility
endif()

# Main executable
add_executable(${PROJECT_NAME} 
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

target_include_directories(${PROJECT_NAME} PRIVATE
  src
  vendor
)

# Testing section (can be disabled with -DENABLE_TESTING=OFF)
option(ENABLE_TESTING "Enable unit testing" ON)

if(ENABLE_TESTING AND PROJECT_IS_TOP_LEVEL)
  enable_testing()
  
  add_executable(${PROJECT_NAME}_tests 
    test/main.cpp 
    test/basic_tests.cpp)
  
  target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    ${PROJECT_NAME}_lib
    GTest::gtest_main)
  
  set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER "Tests")
  
  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_tests)
endif()

# Platform-specific configurations
if(WIN32)
  target_compile_definitions(${PROJECT_NAME}_lib INTERFACE 
    WIN32_LEAN_AND_MEAN NOMINMAX)
elseif(APPLE)
  target_compile_definitions(${PROJECT_NAME}_lib INTERFACE 
    GL_SILENCE_DEPRECATION)
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}_lib INTERFACE pthread dl)
endif()

# Status messages
message(STATUS "SDL3 + Vulkan + ImGui C++ Starter Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Testing: ${ENABLE_TESTING}")
message(STATUS "  Build Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "")
message(STATUS "Key Libraries:")
message(STATUS "  SDL3: Window management and input")
message(STATUS "  Vulkan: Modern graphics API")
message(STATUS "  ImGui: Immediate mode GUI (docking branch)")
message(STATUS "  VMA: Vulkan memory allocator")
message(STATUS "  spdlog: Fast logging library")
message(STATUS "  nlohmann/json: JSON parsing")
