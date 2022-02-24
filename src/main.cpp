#include <cstdlib>

#include <window.hpp>

int32_t main(int32_t argc, char** argv)
{
    (void)argc;
    (void)argv;

    auto &window = visualizer::Window::GetInstance();

    if (!window.InitWindow("3D Programming Course", 1280, 720))
    {
        return EXIT_FAILURE;
    }

    window.Run();

    return EXIT_SUCCESS;
}
