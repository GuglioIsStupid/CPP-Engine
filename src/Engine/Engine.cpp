#include "Engine.hpp"

#include <iostream>
#include <chrono>

namespace Engine {
    Engine::Engine(int width, int height, const std::string& title) {
        m_title = title;
        m_windowWidth = width;
        m_windowHeight = height;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_sdlWindow = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_windowWidth,
            m_windowHeight,
            SDL_WINDOW_OPENGL
        );

        SDL_GL_SetSwapInterval(-1); // Vsync

        if (!m_sdlWindow) {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
            exit(1);
        }

        // link OpenGL with SDL
        m_glContext = SDL_GL_CreateContext(m_sdlWindow);

        SDL_GL_CreateContext(m_sdlWindow);  

        GL_Init();
    }

    bool Engine::GL_Init() {
        glewExperimental = GL_TRUE; // Allow experimental extensions
        GLenum GlewError = glewInit();
        if (GlewError != GLEW_OK) {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLEW: %s\n", glewGetErrorString(GlewError));
            return false;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);

        // load shaders
        g_uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(g_uiVertexShader, 1, &p_cVertexShaderSource, NULL);
        glCompileShader(g_uiVertexShader);

        GLint iCompilationStatus;
        glGetShaderiv(g_uiVertexShader, GL_COMPILE_STATUS, &iCompilationStatus);
        if (iCompilationStatus == GL_FALSE) {
            GLint iInfoLogLength;
            glGetShaderiv(g_uiVertexShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            GLchar* p_cInfoLog = new GLchar[iInfoLogLength];
            glGetShaderInfoLog(g_uiVertexShader, iInfoLogLength, NULL, p_cInfoLog);
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Vertex shader compilation failed: %s\n", p_cInfoLog);
            delete[] p_cInfoLog;
            return false;
        }

        g_uiFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(g_uiFragmentShader, 1, &p_cFragmentShaderSource, NULL);
        glCompileShader(g_uiFragmentShader);

        glGetShaderiv(g_uiFragmentShader, GL_COMPILE_STATUS, &iCompilationStatus);
        if (iCompilationStatus == GL_FALSE) {
            GLint iInfoLogLength;
            glGetShaderiv(g_uiFragmentShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            GLchar* p_cInfoLog = new GLchar[iInfoLogLength];
            glGetShaderInfoLog(g_uiFragmentShader, iInfoLogLength, NULL, p_cInfoLog);
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Fragment shader compilation failed: %s\n", p_cInfoLog);
            delete[] p_cInfoLog;
            return false;
        }

        g_uiMainProgram = glCreateProgram();
        glAttachShader(g_uiMainProgram, g_uiVertexShader);
        glAttachShader(g_uiMainProgram, g_uiFragmentShader);
        glLinkProgram(g_uiMainProgram);

        GLint iLinkStatus;
        glGetProgramiv(g_uiMainProgram, GL_LINK_STATUS, &iLinkStatus);
        if (iLinkStatus == GL_FALSE) {
            GLint iInfoLogLength;
            glGetProgramiv(g_uiMainProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            GLchar* p_cInfoLog = new GLchar[iInfoLogLength];
            glGetProgramInfoLog(g_uiMainProgram, iInfoLogLength, NULL, p_cInfoLog);
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shader linking failed: %s\n", p_cInfoLog);
            delete[] p_cInfoLog;
            return false;
        }

        glUseProgram(g_uiMainProgram);
    }

    Engine::~Engine() {
    }

    bool Engine::IsRunning() {
        return m_isRunning;
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

        while (SDL_PollEvent(&m_event)) {
            if (m_event.type == SDL_QUIT) {
                m_isRunning = false;
            }
        }

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
        //std::cout << "Frame time: " << m_lastFrameTime << "ms, FPS: " << (1000 / m_lastFrameTime) << std::endl;
    }

    void Engine::Update(double deltaTime) {
    }

    void Engine::Render() {
        //std::cout << "Rendering" << std::endl;
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(g_uiMainProgram);

        GLfloat p_fVertices[] = {
            0.0f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f
        };

        glGenVertexArrays(1, &g_uiVAO);
        glBindVertexArray(g_uiVAO);

        glGenBuffers(1, &g_uiVBO);
        glBindBuffer(GL_ARRAY_BUFFER, g_uiVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(p_fVertices), p_fVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(m_sdlWindow);
    }

    void Engine::Exit() {
        std::cout << "Engine is exiting" << std::endl;

        SDL_GL_DeleteContext(m_glContext);
        

        SDL_GL_DeleteContext(m_sdlWindow);
        SDL_DestroyWindow(m_sdlWindow);
        SDL_Quit();
    }
} // Engine

