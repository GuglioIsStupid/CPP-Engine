#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#ifdef _WIN32
    #include <windows.h>
    #include <windef.h>
    #include <GL/glew.h>
#else
    #include <GL/glew.h>
#endif

#include <gl/GL.h>

// setup Engine namespace
namespace Engine {
    // Engine class
    class Engine {
    public:
        Engine(int width, int height, const std::string& title);
        ~Engine();

        bool GL_Init();

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

        bool m_isRunning = true;

        SDL_Window* m_sdlWindow;
        SDL_Event m_event;
        SDL_GLContext m_glContext;

        GLuint g_uiVAO;
        GLuint g_uiVBO;
        GLuint g_uiMainProgram;

        const GLchar* p_cVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

        const GLchar* p_cFragmentShaderSource = R"(
#version 330 core
out vec3 FragColor;
void main() {
    FragColor = vec3(1.0, 1.0, 1.0);
}
)";

        GLuint g_uiVertexShader;
        GLuint g_uiFragmentShader;
    };
} // Engine