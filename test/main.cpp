#include <cstdio>
#include <iostream>

// --- SDL and its Extensions ---
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_mixer.h>
#include <SDL3/SDL_ttf.h>

// --- Vulkan and Helpers ---
#include <vk_mem_alloc.h> // Vulkan Memory Allocator (VMA)
#include <volk/volk.h>    // Vulkan meta-loader
#include <vulkan/vulkan.h>

// --- Math Library ---
#include <cglm/cglm.h> // cglm for matrix math

// --- Assimp for Model Importing ---
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// --- ImGui for GUI ---
#include <imgui.h>

// --- ENet for Networking ---
#include <enet/enet.h>

int main(int argc, char *argv[]) {
  // === SDL Initialization ===
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
    return 1;
  }
  std::cout << "SDL initialized.\n";

  SDL_Window *window =
      SDL_CreateWindow("Demo Window", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
  }
  std::cout << "SDL window created.\n";

  // Initialize SDL_image for PNG support
  int imgFlags = IMG_INIT_PNG;
  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
    std::cerr << "IMG_Init Error: " << IMG_GetError() << "\n";
  } else {
    std::cout << "SDL_image initialized.\n";
  }

  // Initialize SDL_ttf for text rendering
  if (TTF_Init() == -1) {
    std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
  } else {
    std::cout << "SDL_ttf initialized.\n";
  }

  // Initialize SDL_mixer for audio
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError()
              << "\n";
  } else {
    std::cout << "SDL_mixer initialized.\n";
  }

  // === Vulkan Instance (Minimal Example) ===
  VkInstance instance = VK_NULL_HANDLE;
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Demo App";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Demo Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    std::cerr << "Failed to create Vulkan instance!\n";
  } else {
    std::cout << "Vulkan instance created.\n";
  }

  // === Initialize volk ===
  if (volkInitialize() != VK_SUCCESS) {
    std::cerr << "Failed to initialize volk!\n";
  } else {
    std::cout << "volk initialized.\n";
  }

  // === Vulkan Memory Allocator (VMA) Demo ===
  // In a full application, you would create a VMA allocator using your Vulkan
  // instance, physical device, and logical device.
  std::cout << "Vulkan Memory Allocator (VMA) is included (skipping full setup "
               "in this demo).\n";

  // === cglm: Create and print an identity matrix ===
  mat4 identity;
  glm_mat4_identity(identity);
  std::cout << "cglm: Identity matrix:\n";
  for (int i = 0; i < 4; ++i) {
    std::cout << identity[i][0] << " " << identity[i][1] << " "
              << identity[i][2] << " " << identity[i][3] << "\n";
  }

  // === Assimp: Attempt to load a model (dummy file) ===
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile("dummy.obj", aiProcess_Triangulate);
  if (!scene) {
    std::cout << "Assimp: Unable to load dummy model (expected since dummy.obj "
                 "is not provided).\n";
  } else {
    std::cout << "Assimp: Model loaded with " << scene->mNumMeshes
              << " mesh(es).\n";
  }

  // === ImGui: Minimal GUI Frame ===
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  std::cout << "ImGui context created.\n";
  ImGui::NewFrame();
  ImGui::Begin("Demo ImGui Window");
  ImGui::Text("Hello from ImGui!");
  ImGui::End();
  ImGui::Render();
  ImGui::DestroyContext();
  std::cout << "ImGui frame executed.\n";

  // === ENet: Initialize, create a host, and clean up ===
  if (enet_initialize() != 0) {
    std::cerr << "ENet initialization failed.\n";
  } else {
    std::cout << "ENet initialized.\n";
  }
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = 1234;
  ENetHost *enetServer = enet_host_create(&address, 32, 2, 0, 0);
  if (!enetServer) {
    std::cerr << "ENet server creation failed.\n";
  } else {
    std::cout << "ENet server created.\n";
    enet_host_destroy(enetServer);
  }
  enet_deinitialize();
  std::cout << "ENet deinitialized.\n";

  // === Cleanup SDL and Vulkan ===
  Mix_CloseAudio();
  TTF_Quit();
  IMG_Quit();
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "SDL and subsystems cleaned up.\n";

  if (instance != VK_NULL_HANDLE) {
    vkDestroyInstance(instance, nullptr);
    std::cout << "Vulkan instance destroyed.\n";
  }

  std::cout << "Demo completed.\n";
  return 0;
}
