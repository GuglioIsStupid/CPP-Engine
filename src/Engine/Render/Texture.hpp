#pragma once
#include <string>

namespace Engine {
namespace Render {
    class Texture {
    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind();
        void Unbind();
    private:
        unsigned int m_textureID;
        int m_width, m_height, m_channels;
    };
} // Render
} // Engine