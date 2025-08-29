# SDL3 + Vulkan + ImGui C++ Starter Template

A modern, cross-platform C++ application template featuring SDL3 for windowing, Vulkan for high-performance graphics, and Dear ImGui for immediate-mode GUI development.

Forked from a fork of Codotaku's vk_sdl (https://github.com/ilyas-taouaou/vk_sdl) with changes to suit C++ development and a bunch of additional libraries for productivity based applications - editting video, audio, scripts, and other data with a modern set of tools.

Codotaku's original video on vk_sdl:
Vulkan Dev Starter Pack with SDL3, CMake, C23 and more!
https://www.youtube.com/watch?v=WLjuV9_fQfw


## 🚀 Features

- **SDL3**: Modern cross-platform windowing and input handling
- **Vulkan**: High-performance, low-level graphics API
- **Dear ImGui**: Immediate-mode GUI with docking support
- **C++20**: Modern C++ standard with best practices
- **Professional Logging**: Integration with spdlog for structured logging
- **JSON Support**: Configuration and data handling with nlohmann/json
- **Cross-Platform**: Windows, macOS, and Linux support
- **Modern CMake**: Clean, modern build system with dependency management
- **Testing Ready**: GoogleTest integration for unit testing
- **Memory Management**: Vulkan Memory Allocator (VMA) for efficient GPU memory handling

## 📁 Project Structure

```
├── src/
│   └── main.cpp              # Main application entry point
├── vendor/                  # Public headers (add your own)
├── test/
│   ├── main.cpp             # Test runner
│   └── basic_tests.cpp      # Example unit tests
├── assets/                   # Application assets
├── CMakeLists.txt           # Build configuration
├── config.json              # Runtime configuration (auto-generated)
└── README.md               # This file
```

## 🔧 Configuration

The application automatically creates a `config.json` file on first run:

```json
{
    "window": {
        "width": 1280,
        "height": 720,
        "title": "SDL3 + Vulkan + ImGui Application"
    },
    "graphics": {
        "vsync": true
    },
    "ui": {
        "theme": "dark"
    }
}
```

## 🎯 What's Included

### Core Framework
- **Application Class**: Complete lifecycle management
- **Vulkan Setup**: Instance, device, swapchain, render pass creation
- **ImGui Integration**: Docking, viewports, multiple windows
- **Event Handling**: Keyboard, mouse, window events
- **Logging System**: Structured logging with configurable levels

### UI Features
- **Dockable Interface**: Professional multi-window layout
- **Demo Windows**: Examples for logging, settings, Vulkan info
- **Theme Support**: Dark, light, and classic themes
- **Menu System**: Extensible menu bar with common operations

### Development Tools
- **Unit Testing**: GoogleTest framework integration
- **Build Configurations**: Debug/Release configurations
- **Cross-Platform**: Conditional compilation for different platforms
- **IDE Support**: Folder organization for Visual Studio/CLion

## 🚧 Extending the Template

### Adding New Windows
```cpp
void ShowMyCustomWindow() {
    ImGui::Begin("My Custom Window");
    ImGui::Text("Hello from my custom window!");
    if (ImGui::Button("Click Me")) {
        spdlog::info("Button clicked!");
    }
    ImGui::End();
}

// Call from CreateMainUI() or ShowExampleWindows()
```

### Adding Vulkan Rendering
The template provides a basic Vulkan setup. To add custom rendering:

1. Create vertex/index buffers
2. Set up graphics pipelines
3. Record command buffers
4. Implement proper frame synchronization

### Adding New Dependencies
Using FetchContent in CMakeLists.txt:
```cmake
FetchContent_Declare(
  my_library
  GIT_REPOSITORY https://github.com/user/my_library.git
  GIT_TAG v1.0.0)

FetchContent_MakeAvailable(my_library)

# Add to target_link_libraries
target_link_libraries(${PROJECT_NAME}_lib INTERFACE my_library::my_library)
```

## 🧪 Testing

Run tests with:
```bash
cd build
ctest
# or directly:
./bin/Release/vk_sdl3_imgui_cpp_starter_tests
```

Add new tests in `test/` directory following GoogleTest patterns.

## 📝 CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `ENABLE_TESTING` | `ON` | Enable unit testing |
| `CMAKE_BUILD_TYPE` | `Debug` | Build configuration |

Example:
```bash
cmake .. -DENABLE_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
```

## 🔍 Troubleshooting

### Vulkan Issues
- Ensure Vulkan SDK is installed and in PATH
- Check graphics drivers support Vulkan 1.0+
- Verify `VK_SDK_PATH` environment variable

### Build Issues
- Update CMake to 3.26+
- Ensure C++20 compiler support
- Check submodule initialization: `git submodule update --init --recursive`

### Runtime Issues
- Check console output for detailed error messages
- Verify config.json permissions
- Test with Vulkan validation layers enabled

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Push to branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **SDL3**: Cross-platform development library
- **Vulkan**: High-performance graphics API
- **Dear ImGui**: Immediate-mode GUI library
- **spdlog**: Fast logging library
- **nlohmann/json**: JSON parsing library
- **GoogleTest**: Unit testing framework
- **VMA**: Vulkan Memory Allocator

## 🚀 Perfect For

- **Game Engines**: High-performance rendering foundation
- **Creative Tools**: Video editors, image processors, CAD tools
- **Data Visualization**: Scientific computing, analytics dashboards  
- **Prototyping**: Rapid GUI application development
- **Learning**: Modern C++ and graphics programming

---

**Happy coding!** 🎉

For questions or support, please open an issue on GitHub.
