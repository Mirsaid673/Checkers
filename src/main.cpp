#include <iostream>

#include "window.h"
#include "mesh2D.h"
#include "shader.h"
#include "camera2D.h"

int main()
{
    Window wnd;
    int width = 800;
    int height = 600;
    wnd.init(width, height, "openGL");
    Shader shader("../resource/shaders/default.vert", "../resource/shaders/default.frag");

    float clear_color[3]{1.0f, 0.5f, 0.75f};

    glm::mat4 model(1.0f);

    Camera2D camera;
    camera.othographic(-2, 2, -2, 2, 0, 10);

    using namespace glm;
    std::vector<Vertex2D> vert{
        Vertex2D{vec2(-1, -1), vec3(0, 0, 0)},
        Vertex2D{vec2(-1,  1), vec3(0, 0, 0)},
        Vertex2D{vec2( 1,  1), vec3(0, 0, 0)},
        Vertex2D{vec2( 1, -1), vec3(0, 0, 0)},
    };

    std::vector<GLuint> ind{
        0, 1, 2,
        0, 2, 3 };

    Mesh2D mesh(vert, ind);

    while (wnd.getKey(GLFW_KEY_ESCAPE) != GLFW_PRESS && wnd.shouldClose() == 0)
    {
        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setCamera(camera);
        shader.setMat4("m", model);

        mesh.draw();

        wnd.swap_buffers();
    }

    mesh.destroy();
    shader.destroy();
    wnd.cleanup();

    return 0;
}