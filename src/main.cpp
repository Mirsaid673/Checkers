#include <iostream>

#include "window.h"
#include "mesh2D.h"
#include "shader.h"
#include "camera2D.h"
#include "checker.h"

glm::vec2 get_position_from_coord(int i, int j);

const int desk_size = 8;
int desk[desk_size][desk_size] =
    {
        {0, 2, 0, 2, 0, 2, 0, 2},
        {2, 0, 2, 0, 2, 0, 2, 0},
        {0, 2, 0, 2, 0, 2, 0, 2},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0},
};

int main()
{
    Window wnd;
    wnd.init(800, 600, "openGL");
    Shader shader("../resource/shaders/default.vert", "../resource/shaders/default.frag");
    Camera2D camera;
    float camera_scale = 8;

    float aspect = camera_scale * ((float)wnd.getHeight() / (float)wnd.getWidth());
    camera.othographic(-camera_scale, camera_scale, -aspect, aspect, 0, 10);
    float clear_color[3]{1.0f, 0.5f, 0.75f};

    const float checker_scale = 0.4f;
    std::vector<Vertex2D> desk_vert{
        Vertex2D{glm::vec2(-1, -1) * 4.0f, glm::vec2(0, 0)},
        Vertex2D{glm::vec2(-1, 1) * 4.0f, glm::vec2(0, 1)},
        Vertex2D{glm::vec2(1, 1) * 4.0f, glm::vec2(1, 1)},
        Vertex2D{glm::vec2(1, -1) * 4.0f, glm::vec2(1, 0)},
    };
    std::vector<Vertex2D> checker_vert{
        Vertex2D{glm::vec2(-1, -1) * checker_scale, glm::vec2(0, 0)},
        Vertex2D{glm::vec2(-1, 1) * checker_scale, glm::vec2(0, 1)},
        Vertex2D{glm::vec2(1, 1) * checker_scale, glm::vec2(1, 1)},
        Vertex2D{glm::vec2(1, -1) * checker_scale, glm::vec2(1, 0)},
    };
    std::vector<GLuint> ind{
        0, 1, 2,
        0, 2, 3};

    Mesh2D desk_mesh(desk_vert, ind);
    Mesh2D checker_mesh(checker_vert, ind);

    Texture desk_texture("../resource/textures/desk.jpg");
    Texture white_texture("../resource/textures/White.png");
    Texture black_texture("../resource/textures/Black.png");

    Model desk_model(desk_mesh, desk_texture, shader);
    Model white_model(checker_mesh, white_texture, shader);
    Model black_model(checker_mesh, black_texture, shader);

    Checker chekcers[2]{
        Checker(white_model, Checker::Color::WHITE), // white one
        Checker(black_model, Checker::Color::BLACK)  // black one
    };

    while (wnd.getKey(GLFW_KEY_ESCAPE) != GLFW_PRESS && wnd.shouldClose() == 0)
    {
        glViewport(0, 0, wnd.getWidth(), wnd.getHeight());
        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //========frame start========

        glm::vec<2, double> cursor_pos;
        wnd.getCursorPos(&cursor_pos.x, &cursor_pos.y);
        if(wnd.getMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            std::cout << cursor_pos.x << '\t' << cursor_pos.y << std::endl;

        shader.use();
        shader.setCamera(camera);

        desk_model.draw(glm::vec2(0, 0));
        for (int i = 0; i < desk_size; i++) // rows
        {
            for (int j = 0; j < desk_size; j++) // columns
            {
                int curren_cell_type = desk[i][j];
                if (curren_cell_type != 0)
                {
                    Checker &current = chekcers[desk[i][j] % 2];
                    current.position = get_position_from_coord(i, j);
                    current.draw();
                }
            }
        }

        //========frame end========
        wnd.swapBuffers();
        wnd.updateSize();
        float aspect = camera_scale * ((float)wnd.getHeight() / (float)wnd.getWidth());
        camera.othographic(-camera_scale, camera_scale, -aspect, aspect, 0, 10);
    }

    desk_mesh.destroy();
    checker_mesh.destroy();
    shader.destroy();
    wnd.cleanup();

    return 0;
}

glm::vec2 get_position_from_coord(int i, int j)
{
    return glm::vec2(Checker::default_pos + glm::vec2(j, i));
}