#include "Engine.hpp"

#include <iostream>
#include <chrono>

#include <GLFW/glfw3.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <gl/GL.h>

namespace Engine {
    Engine::Engine(int width, int height, const std::string& title) {
        m_title = title;
        m_windowWidth = width;
        m_windowHeight = height;

        if (!glfwInit())
            return;

        m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (!m_glfwWindow) {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_glfwWindow);

        glEnable(GL_BLEND | GL_DEPTH_TEST | GL_TEXTURE_2D);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }

    Engine::~Engine() {
    }

    bool Engine::IsRunning() {
        return !glfwWindowShouldClose(m_glfwWindow) != 0;
    }

    int Engine::GetFPSLimit() {
        return m_fpsLimit;
    }

    double Engine::GetFrameTime() {
        return m_lastFrameTime;
    }

    void Engine::SetFPSLimit(int fps) {
        m_fpsLimit = fps;
    }

    void Engine::Run() {
        std::cout << "Engine is running" << std::endl;

    }

    void Engine::Loop() {
        auto frameStart = std::chrono::steady_clock::now();

        Update(m_deltaTime);

        Render();

        auto frameEnd = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

        auto sleepTime = (1000 / m_fpsLimit) - frameDuration;
        if (sleepTime > 0) {
            auto sleepStart = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - sleepStart).count() < sleepTime) {
            }
        }

        m_lastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frameStart).count();
        m_deltaTime = m_lastFrameTime / 1000.0;
        std::cout << "Frame time: " << m_lastFrameTime << "ms, FPS: " << (1000 / m_lastFrameTime) << std::endl;
    }

    void Engine::Update(double deltaTime) {
    }

    void Engine::Render() {
        std::cout << "Rendering" << std::endl;
        glfwPollEvents();
        
        // gl triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.5f);
        glEnd();

        glfwSwapBuffers(m_glfwWindow);
    }

    void Engine::Exit() {
        std::cout << "Engine is exiting" << std::endl;
        glfwTerminate();
    }
} // Engine

