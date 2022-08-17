#include <iostream>

#include "window.h"
#include "mesh2D.h"
#include "shader.h"
#include "camera2D.h"
#include "checker.h"

glm::vec2 position_from_coord(int i, int j);
glm::ivec2 desk_coord_from_screen_coord(const glm::ivec2 &coord);
std::vector<glm::ivec2> get_avialable_moves_with_eating(glm::ivec2 pos);
bool has_moves_with_getting(const glm::ivec2 &, const glm::ivec2 &dir);
bool has_moves_with_getting(const glm::ivec2 &);
int desk_cell(glm::ivec2 v);
int get_color(int checker) { return checker & 1;}

const int desk_size = 8;
int desk[desk_size][desk_size] =
    {
        // inversed to better access
        // 0 - white checker
        // 1 - black checker
        // 2 - empty cell
        {0, 2, 0, 2, 0, 2, 0, 2},
        {2, 0, 2, 0, 2, 0, 2, 0},
        {0, 2, 0, 2, 0, 2, 0, 2},
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 1, 2, 1, 2, 1, 2, 1},
        {1, 2, 1, 2, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 1, 2, 1},
};

Checker::Color whoose_move = Checker::Color::WHITE;
Checker::Color next_move[2]{
    Checker::Color::BLACK,
    Checker::Color::WHITE,
};

Window wnd;
Camera2D camera;
int main()
{
    wnd.init(800, 600, "openGL");
    Shader shader("../resource/shaders/default.vert", "../resource/shaders/default.frag");
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

    Checker checkers[2]{
        Checker(white_model, Checker::Color::WHITE), // white one
        Checker(black_model, Checker::Color::BLACK), // black one
    };

    glm::ivec2 unclicked(8, 8);
    glm::ivec2 checker_clicked(unclicked);
    glm::ivec2 cell_clicked(unclicked);

    int last_button_state = GLFW_RELEASE;
    int current_button_state = GLFW_RELEASE;

    int move_direction[2]{
        1,
        -1,
    };

    bool is_continue = false;
    ;
    while (wnd.getKey(GLFW_KEY_ESCAPE) != GLFW_PRESS && wnd.shouldClose() == 0)
    {
        glViewport(0, 0, wnd.getWidth(), wnd.getHeight());
        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //========frame start========

        last_button_state = current_button_state;
        current_button_state = wnd.getMouseButton(GLFW_MOUSE_BUTTON_LEFT);

        if (last_button_state == GLFW_PRESS && current_button_state == GLFW_RELEASE) // if clicked
        {
            static int count = 0;
            std::cout << "clicked\t" << count << std::endl;
            count++;
            glm::dvec2 cursor_pos;
            wnd.getCursorPos(&cursor_pos.x, &cursor_pos.y);
            glm::ivec2 n = desk_coord_from_screen_coord((glm::ivec2)cursor_pos);
            if (n.x < desk_size && n.x >= 0 && n.y < desk_size && n.y >= 0)
            {
                int cell = desk_cell(n);
                if (cell != Checker::Color::EMPTY && !is_continue) // if it is checker
                {
                    if (checkers[cell].getColor() == whoose_move)
                        checker_clicked = n;
                }
                else if (checker_clicked != unclicked) // if it is alreaddy clicked some checker
                    cell_clicked = n;
            }
        }

        if (checker_clicked != unclicked && cell_clicked != unclicked) // checking if move is correct
        {
            if (abs(cell_clicked.y - checker_clicked.y) == 2 &&
                abs(cell_clicked.x - checker_clicked.x) == 2 &&
                get_color(desk[(cell_clicked.y + checker_clicked.y) / 2][(cell_clicked.x + checker_clicked.x) / 2]) == next_move[whoose_move])
            {
                desk[(cell_clicked.y + checker_clicked.y) / 2][(cell_clicked.x + checker_clicked.x) / 2] = Checker::Color::EMPTY;
                std::swap(desk[checker_clicked.y][checker_clicked.x], desk[cell_clicked.y][cell_clicked.x]);
                checker_clicked = cell_clicked;
                cell_clicked = unclicked;
                if (has_moves_with_getting(checker_clicked))
                {
                    is_continue = true;
                }
                else
                {
                    checker_clicked = unclicked;
                    whoose_move = next_move[whoose_move];
                    is_continue = false;
                }
            }
            else if (cell_clicked.y - checker_clicked.y == move_direction[whoose_move] && abs(cell_clicked.x - checker_clicked.x) == 1)
            {
                std::swap(desk[checker_clicked.y][checker_clicked.x], desk[cell_clicked.y][cell_clicked.x]);
                checker_clicked = unclicked;
                cell_clicked = unclicked;
                whoose_move = next_move[whoose_move];
            }
            else
            {
                cell_clicked = unclicked;
            }
        }

        shader.use();
        shader.setCamera(camera);

        desk_model.draw(glm::vec2(0, 0));
        for (int i = 0; i < desk_size; i++) // rows
        {
            for (int j = 0; j < desk_size; j++) // columns
            {
                int curren_cell_type = desk[i][j];
                if (curren_cell_type != Checker::Color::EMPTY)
                {
                    Checker &current = checkers[curren_cell_type % 2];
                    current.position = position_from_coord(i, j);
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

glm::vec2 position_from_coord(int i, int j)
{
    return glm::vec2(Checker::default_pos + glm::vec2(j, i));
}

glm::vec2 screen_normalized(glm::vec2 coord)
{
    glm::vec2 window_size(wnd.getWidth(), wnd.getHeight());
    glm::vec2 screen_normalized;
    screen_normalized.x = (2 * coord.x - window_size.x) / window_size.x;
    screen_normalized.y = (-2 * coord.y + window_size.y) / window_size.y;
    return screen_normalized;
}

glm::ivec2 desk_coord_from_screen_coord(const glm::ivec2 &coord)
{
    glm::vec2 normilized = screen_normalized(coord);
    glm::mat2 proj(camera.getProjection());
    glm::vec2 start_pos = (proj / normilized) + glm::vec2(4, 4); // left-bottom world coord;
    return glm::floor(start_pos);
}

bool is_inside_the_desk(const glm::ivec2 &v)
{
    return v.x >= 0 && v.x < desk_size && v.y >= 0 && v.y < desk_size;
}

const glm::ivec2 directions[]{
    {1, 1},
    {-1, 1},
    {1, -1},
    {-1, -1},
};
std::vector<glm::ivec2> get_avialable_moves_with_eating(glm::ivec2 pos)
{
    const int directions_count = 4;
    glm::ivec2 relative_dir[directions_count]{
        pos + directions[0] * 2,
        pos + directions[1] * 2,
        pos + directions[2] * 2,
        pos + directions[3] * 2,
    };

    int another_color = next_move[whoose_move];
    std::vector<glm::ivec2> ret;

    for (int i = 0; i < directions_count; i++)
    {
        if (is_inside_the_desk(relative_dir[i]))
        {
            if (get_color(desk_cell(relative_dir[i])) == another_color && desk_cell(relative_dir[i] - directions[i]) == Checker::Color::EMPTY)
                ret.push_back(relative_dir[i]);
        }
    }

    return ret;
}
bool has_moves_with_getting(const glm::ivec2 &pos, const glm::ivec2 &dir)
{
    glm::ivec2 coord = pos + dir * 2;
    if (is_inside_the_desk(coord))
    {
        return desk_cell(coord) == Checker::Color::EMPTY && get_color(desk_cell(coord - dir)) == next_move[whoose_move];
    }
    return false;
}

bool has_moves_with_getting(const glm::ivec2 &pos)
{
    return has_moves_with_getting(pos, directions[0]) ||
           has_moves_with_getting(pos, directions[1]) ||
           has_moves_with_getting(pos, directions[2]) ||
           has_moves_with_getting(pos, directions[3]);
}

int desk_cell(glm::ivec2 v)
{
    return desk[v.y][v.x];
}