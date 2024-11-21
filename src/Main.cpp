#include <iostream>
#include "Engine/Engine.hpp"

#ifdef _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int)
#else
int main(int argc, char **argv)
#endif
{
    // call GetInstance to get the instance of the engine
    Engine::Engine* engine = Engine::Engine::GetInstance(1280, 720, "Engine");

    engine->Run();

    while (engine->IsRunning()) {
        engine->Loop();
    }

    engine->Exit();

    return 0;
}