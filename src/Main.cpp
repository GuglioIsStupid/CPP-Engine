#define GLFW_DLL
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "Engine/Engine.hpp"

int main() {
    Engine::Engine engine(1280, 720, "Engine");

    engine.Run();

    while (engine.IsRunning()) {
        engine.Loop();
    }

    engine.Exit();

    return 0;
}