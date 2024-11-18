#pragma once
#include <string>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>
#ifdef _WIN32
    #include <windows.h>
#endif

#include <gl/GL.h>

#include "Render/Texture.hpp"

// setup Engine namespace
namespace Engine {
    // Engine class
    class Engine {
    public:
        Engine(int width, int height, const std::string& title);
        ~Engine();

        bool IsRunning();
        int GetFPSLimit();
        double GetFrameTime();

        void SetFPSLimit(int fps);

        void Run();
        void Loop();
        void Update(double deltaTime);
        void Render();
        void Exit();
    private:
        int m_fpsLimit = 60;
        int m_lastFrameTime;
        double m_deltaTime;

        std::string m_title;
        int m_windowWidth;
        int m_windowHeight;

        GLFWwindow* m_glfwWindow;
    };
} // Engine