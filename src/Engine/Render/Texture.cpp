#include "Texture.hpp"

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <gl/GL.h>

#include "stb_image.h"

namespace Engine {
namespace Render {
    Texture::Texture(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

        if (!data) {
            std::cout << "Failed to load texture " << path << std::endl;
            std::cout << "Error: " << stbi_failure_reason() << std::endl;
            return;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_textureID);
    }

    void Texture::Bind() {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture::Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // Render
} // Engine