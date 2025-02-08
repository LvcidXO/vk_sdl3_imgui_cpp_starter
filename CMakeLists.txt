cmake_minimum_required(VERSION 3.26)
project(
  sdvk
  VERSION 0.1.0
  DESCRIPTION "SDL/Vulkan-based C++ project"
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

# Dependency management

FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG b5c3eab)

FetchContent_Declare(
  SDL_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG 4a762bd)

FetchContent_Declare(
  SDL_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG 3d7b6ef)

FetchContent_Declare(
  SDL_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG ebdd9cc)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 6910c9d)

FetchContent_Declare(
  Vulkan-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
  GIT_TAG vulkan-sdk-1.4.304.0)

FetchContent_Declare(
  SPIRV-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
  GIT_TAG vulkan-sdk-1.4.304.0)

FetchContent_Declare(
  SPIRV-Tools
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
  GIT_TAG vulkan-sdk-1.4.304.0)

FetchContent_Declare(
  glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
  GIT_TAG vulkan-sdk-1.4.304.0)

FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  Git_TAG 0b17a76)

FetchContent_Declare(
  VulkanMemoryAllocator
  GIT_REPOSITORY
    https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  Git_TAG c788c52)

FetchContent_Declare(
  cglm
  GIT_REPOSITORY https://github.com/recp/cglm.git
  GIT_TAG 1796cc5)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG c35200e)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG dbb5eea)
FetchContent_Declare(
  enet
  Git_REPOSITORY https://github.com/lsalzman/enet.git
  GIT_TAG 2662c0d)

FetchContent_MakeAvailable(
  SDL
  SDL_image
  SDL_ttf
  SDL_mixer
  googletest
  Vulkan-Headers
  SPIRV-Headers
  SPIRV-Tools
  glslang
  volk
  VulkanMemoryAllocator
  cglm
  assimp
  imgui
  enet)

if(enet_ADDED)
  add_library(enet)
  target_include_directories(enet PUBLIC "${enet_SOURCE_DIR}/include")
  target_sources(
    enet
    PRIVATE ${enet_SOURCE_DIR}/callbacks.c
            ${enet_SOURCE_DIR}/compress.c
            ${enet_SOURCE_DIR}/host.c
            ${enet_SOURCE_DIR}/list.c
            ${enet_SOURCE_DIR}/packet.c
            ${enet_SOURCE_DIR}/peer.c
            ${enet_SOURCE_DIR}/protocol.c)

  # Platform-specific sources
  if(WIN32)
    target_sources(enet PRIVATE ${enet_SOURCE_DIR}/win32.c)
  else()
    target_sources(enet PRIVATE ${enet_SOURCE_DIR}/unix.c)
  endif()

  target_compile_definitions(enet PRIVATE ENET_BUILDING_LIB)
endif()

# Main library target
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE Vulkan::Headers
            SPIRV-Headers::SPIRV-Headers
            volk::volk_headers
            glslang
            GPUOpen::VulkanMemoryAllocator
            SDL3::SDL3
            SDL3_image::SDL3_image
            SDL3_mixer::SDL3_mixer
            SDL3_ttf::SDL3_ttf
            cglm
            assimp::assimp
            imgui
            enet)

# Compiler warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic
                                                   -Wshadow -Werror)
elseif(MSVC)
  target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX)
endif()

# Testing section
if(PROJECT_IS_TOP_LEVEL)
  enable_testing()

  add_executable(${PROJECT_NAME}_tests test/main.cpp test/deps.cpp)

  target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}
                                                      GTest::gtest_main)

  set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER "Tests")

  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_tests)
endif()
