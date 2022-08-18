#include <iostream>

#include "window.h"
#include "mesh2D.h"
#include "shader.h"
#include "camera2D.h"
#include "checker.h"
#include "input.h"
#include "desk.h"

glm::vec2 position_from_coord(int i, int j);
glm::ivec2 desk_coord_from_screen_coord(const glm::ivec2 &coord);
std::vector<glm::ivec2> get_avialable_moves_with_eating(const glm::ivec2 &pos);
std::vector<glm::ivec2> get_avialable_moves(const glm::ivec2 &pos);
bool has_moves_with_getting(const glm::ivec2 &, const glm::ivec2 &dir);
bool has_moves_with_getting(const glm::ivec2 &);
bool is_inside_the_desk(const glm::ivec2 &v);
int desk_cell(glm::ivec2 v);
int get_color(int checker) { return checker & 1; }
bool is_checker(int color) { return (color & 1) == color; }
bool is_cell_empty(glm::ivec2 pos) { return desk_cell(pos)  == Checker::EMPTY; }

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
int move_direction[2]{
    1,
    -1,
};

Window wnd;
Camera2D camera;
int main()
{
    wnd.init(800, 600, "openGL");
    Input::init(wnd);
    Shader shader("../resource/shaders/default.vert", "../resource/shaders/default.frag");
    const float camera_scale = 6;
    float aspect = camera_scale * ((float)wnd.getHeight() / (float)wnd.getWidth());
    camera.othographic(-camera_scale, camera_scale, -aspect, aspect, 0, 10);

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
    Texture available_to_move_cell_texture("../resource/textures/move.png");

    Model desk_model(desk_mesh, desk_texture, shader);
    Model white_model(checker_mesh, white_texture, shader);
    Model black_model(checker_mesh, black_texture, shader);
    Model available_to_move_cell(checker_mesh, available_to_move_cell_texture, shader);

    Checker checkers[2]{
        Checker(white_model, Checker::Color::WHITE), // white one
        Checker(black_model, Checker::Color::BLACK), // black one
    };

    float clear_color[3]{1.0f, 0.5f, 0.75f};

    std::vector<glm::ivec2> available_moves;

    const glm::ivec2 unclicked(8, 8);
    glm::ivec2 checker_clicked(unclicked);
    glm::ivec2 cell_clicked(unclicked);

    bool is_continue = false;
    while (Input::getKeyUp(GLFW_KEY_ESCAPE) && wnd.shouldClose() == 0)
    {
        glViewport(0, 0, wnd.getWidth(), wnd.getHeight());
        glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //========frame start========
        if (Input::getMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
        {
            static int count = 0;
            std::cout << "clicked\t" << count << std::endl;
            count++;
            glm::ivec2 cursor_pos = Input::getCursorPos();
            glm::ivec2 n = desk_coord_from_screen_coord((glm::ivec2)cursor_pos);
            if (is_inside_the_desk(n))
            {
                int cell = desk_cell(n);
                if (is_checker(cell) && !is_continue) // if it is checker
                {
                    if (checkers[cell].getColor() == whoose_move)
                    {
                        checker_clicked = n;
                        available_moves = get_avialable_moves(checker_clicked);
                    }
                }
                else if (checker_clicked != unclicked) // if it is alreaddy clicked some checker
                    cell_clicked = n;
            }
        }

        if (checker_clicked != unclicked && cell_clicked != unclicked) // checking if move is correct
        {
            std::vector<glm::ivec2> moves = get_avialable_moves(checker_clicked);
            if (moves.size() == 0)
            {
                checker_clicked = unclicked;
                cell_clicked = unclicked;
                is_continue = false;
            }
            else
            {
                for (const glm::ivec2 &move : moves)
                {
                    if (cell_clicked == move)
                    {
                        available_moves.clear();
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
                        break;
                    }
                }
            }
        }

        // ===========drawing===========
        shader.use();
        shader.setCamera(camera);

        desk_model.draw(glm::vec2(0, 0));
        for (int i = 0; i < desk_size; i++) // rows
        {
            for (int j = 0; j < desk_size; j++) // columns
            {
                int curren_cell_type = desk[i][j];
                if (is_checker(curren_cell_type))
                {
                    Checker &current = checkers[curren_cell_type % 2];
                    current.position = position_from_coord(i, j);
                    current.draw();
                }
            }
        }

        for(const auto& cell : available_moves)
        {
            glm::vec2 position = position_from_coord(cell.y, cell.x);
            available_to_move_cell.draw(position);
        }

        //========frame end========
        Input::update();
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
std::vector<glm::ivec2> get_avialable_moves_with_eating(const glm::ivec2 &pos)
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
            if (get_color(desk_cell(relative_dir[i] - directions[i])) == another_color && is_cell_empty(relative_dir[i]))
                ret.push_back(relative_dir[i]);
        }
    }

    return ret;
}

std::vector<glm::ivec2> get_avialable_moves(const glm::ivec2 &pos)
{
    std::vector<glm::ivec2> ret = get_avialable_moves_with_eating(pos);

    const int directions_count = 2;
    glm::ivec2 relative_dir[directions_count]{
        pos + directions[0] * move_direction[whoose_move],
        pos + directions[1] * move_direction[whoose_move],
    };
 
    for (int i = 0; i < directions_count; i++)
    {
        if (is_inside_the_desk(relative_dir[i]))
        {
            if (is_cell_empty(relative_dir[i]))
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
        return is_cell_empty(coord) && get_color(desk_cell(coord - dir)) == next_move[whoose_move];
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