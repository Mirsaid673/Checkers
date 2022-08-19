#include <iostream>

#include "window.h"
#include "mesh2D.h"
#include "shader.h"
#include "camera2D.h"
#include "input.h"
#include "desk.h"

using std::vector, std::pair, std::cout, std::endl, glm::vec2, glm::ivec2;

vec2 position_from_coord(int i, int j) { return vec2(Checker::default_pos + vec2(j, i)); };
ivec2 desk_coord_from_screen_coord(const ivec2 &coord);
ivec2 get_average_pos(const ivec2 &v1, const ivec2 &v2) { return (v1 + v2) / 2; }

vector<pair<ivec2, ivec2>> get_avialable_moves_with_geting(const ivec2 &pos);
vector<ivec2> get_avialable_moves_without_geting(const ivec2 &pos);

bool has_moves_with_getting(const ivec2 &, const ivec2 &dir);
bool has_moves_with_getting(const ivec2 &);

const int move_direction[2]{
    1,  // for whites
    -1, // for blacks
};
const int last_row[2]{
    7, // for whites
    0, // for blacks
};

Desk desk;
Window wnd;
Camera2D camera;
int main()
{
    wnd.init(800, 600, "openGL");
    Input::init(wnd);
    Shader shader("../resource/shaders/default.vert", "../resource/shaders/default.frag");
    const float camera_scale = 4;
    float aspect = camera_scale * (float)wnd.getWidth() / ((float)wnd.getHeight());
    camera.othographic(-aspect, aspect, -camera_scale, camera_scale, 0, 10);

    const float checker_scale = 0.4f;
    vector<Vertex2D> desk_vert{
        Vertex2D{vec2(-1, -1) * 4.0f, vec2(0, 0)},
        Vertex2D{vec2(-1, 1) * 4.0f, vec2(0, 1)},
        Vertex2D{vec2(1, 1) * 4.0f, vec2(1, 1)},
        Vertex2D{vec2(1, -1) * 4.0f, vec2(1, 0)},
    };
    vector<Vertex2D> checker_vert{
        Vertex2D{vec2(-1, -1) * checker_scale, vec2(0, 0)},
        Vertex2D{vec2(-1, 1) * checker_scale, vec2(0, 1)},
        Vertex2D{vec2(1, 1) * checker_scale, vec2(1, 1)},
        Vertex2D{vec2(1, -1) * checker_scale, vec2(1, 0)},
    };
    vector<GLuint> ind{
        0, 1, 2,
        0, 2, 3};

    Mesh2D desk_mesh(desk_vert, ind);
    Mesh2D checker_mesh(checker_vert, ind);

    Texture desk_texture("../resource/textures/desk.jpg", GL_NEAREST);
    Texture white_texture("../resource/textures/White.png");
    Texture black_texture("../resource/textures/Black.png");
    Texture white_quuen_texture("../resource/textures/White queen.png");
    Texture black_queen_texture("../resource/textures/Black queen.png");
    Texture available_to_move_cell_texture("../resource/textures/move.png");

    Model desk_model(desk_mesh, desk_texture, shader);
    Model white_model(checker_mesh, white_texture, shader);
    Model black_model(checker_mesh, black_texture, shader);
    Model white_queen_model(checker_mesh, white_quuen_texture, shader);
    Model black_queen_model(checker_mesh, black_queen_texture, shader);
    Model available_to_move_cell(checker_mesh, available_to_move_cell_texture, shader);

    Model checkers[]{
        white_model, // white one
        black_model, // black one
        white_queen_model, // white queen one
        black_queen_model, // black queen one
    };

    float clear_color[3]{1.0f, 0.5f, 0.75f};

    vector<pair<ivec2, ivec2>> available_moves_with_getting;
    vector<ivec2> available_moves;

    const ivec2 unclicked(8, 8);
    ivec2 checker_clicked(unclicked);
    ivec2 cell_clicked(unclicked);

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
            cout << "clicked\t" << count << endl;
            count++;
            ivec2 cursor_pos = Input::getCursorPos();
            ivec2 n = desk_coord_from_screen_coord((ivec2)cursor_pos);
            if (desk.is_inside(n))
            {
                int cell = desk[n];
                if (desk.is_checker(cell) && !is_continue) // if it is checker
                {
                    if (checker_clicked == n)
                    {
                        checker_clicked = unclicked;
                        available_moves.clear();
                        available_moves_with_getting.clear();
                    }
                    else if (desk.get_checker_color(cell) == desk.get_whoose_move())
                    {
                        checker_clicked = n;
                        available_moves_with_getting = get_avialable_moves_with_geting(checker_clicked);
                        available_moves.clear();
                        if (available_moves_with_getting.size() == 0)
                            available_moves = get_avialable_moves_without_geting(checker_clicked);
                    }
                }
                else if (checker_clicked != unclicked) // if it is already clicked some checker
                    cell_clicked = n;
            }
        }

        if (checker_clicked != unclicked && cell_clicked != unclicked) // checking if move is correct
        {
            if (available_moves_with_getting.size() == 0)
            {
                if (available_moves.size() != 0)
                {
                    for (const auto &move : available_moves)
                    {
                        if (cell_clicked == move)
                        {
                            available_moves.clear();
                            // check if is on the last row to make queen
                            if (cell_clicked.y == last_row[desk.get_checker_color(checker_clicked)])
                            {
                                desk.make_queen(checker_clicked);
                            }
                            std::swap(desk[checker_clicked], desk[cell_clicked]);
                            desk.switch_move();
                            cell_clicked = unclicked;
                            checker_clicked = unclicked;
                            is_continue = false;
                            break;
                        }
                    }
                }
                else
                {
                    checker_clicked = unclicked;
                    cell_clicked = unclicked;
                    is_continue = false;
                }
            }
            else
            {
                for (const auto &move : available_moves_with_getting)
                {
                    if (cell_clicked == move.first)
                    {
                        available_moves.clear();
                        available_moves_with_getting.clear();

                        if (cell_clicked.y == last_row[desk.get_checker_color(checker_clicked)])
                        {
                            desk.make_queen(checker_clicked);
                        }

                        desk[move.second] = Checker::EMPTY;
                        std::swap(desk[checker_clicked], desk[cell_clicked]);
                        checker_clicked = cell_clicked;
                        cell_clicked = unclicked;

                        available_moves_with_getting = get_avialable_moves_with_geting(checker_clicked);
                        if (available_moves_with_getting.size() != 0)
                            is_continue = true;
                        else
                        {
                            checker_clicked = unclicked;
                            desk.switch_move();
                            is_continue = false;
                        }
                        break;
                    }
                }
            }
        }

        //===========drawing==========================
        shader.use();
        shader.setCamera(camera);

        desk_model.draw(vec2(0, 0));
        for (int i = 0; i < Desk::desk_size; i++) // rows
        {
            for (int j = 0; j < Desk::desk_size; j++) // columns
            {
                int curren_cell_type = desk[i][j];
                if (desk.is_checker(curren_cell_type))
                {
                    vec2 position = position_from_coord(i, j);
                    checkers[curren_cell_type & 0b11].draw(position);
                }
            }
        }

        for (const auto &cell : available_moves)
        {
            vec2 position = position_from_coord(cell.y, cell.x);
            available_to_move_cell.draw(position);
        }
        for (const auto &cell : available_moves_with_getting)
        {
            vec2 position = position_from_coord(cell.first.y, cell.first.x);
            available_to_move_cell.draw(position);
        }

        //========frame end========
        Input::update();
        wnd.swapBuffers();
        wnd.updateSize();
        float aspect = camera_scale * (float)wnd.getWidth() / ((float)wnd.getHeight());
        camera.othographic(-aspect, aspect, -camera_scale, camera_scale, 0, 10);
    }

    desk_mesh.destroy();
    checker_mesh.destroy();
    shader.destroy();
    wnd.cleanup();

    return 0;
}

vec2 screen_normalized(vec2 coord)
{
    vec2 window_size(wnd.getWidth(), wnd.getHeight());
    vec2 screen_normalized;
    screen_normalized.x = (2 * coord.x - window_size.x) / window_size.x;
    screen_normalized.y = (-2 * coord.y + window_size.y) / window_size.y;
    return screen_normalized;
}

ivec2 desk_coord_from_screen_coord(const ivec2 &coord)
{
    vec2 normilized = screen_normalized(coord);
    glm::mat2 proj(camera.getProjection());
    vec2 start_pos = (proj / normilized) + vec2(4, 4); // left-bottom world coord;
    return floor(start_pos);
}

const ivec2 directions[]{
    Desk::RIGHT_TOP,
    Desk::LEFT_TOP,
    Desk::RIGHT_BOTTOM,
    Desk::LEFT_BOTTOM,
};

vector<ivec2> get_avialable_moves_without_geting(const ivec2 &pos)
{
    vector<ivec2> ret;

    if (!desk.is_queen(pos))
    {
        const int directions_count = 2;
        ivec2 relative_dir[directions_count]{
            pos + directions[0] * move_direction[desk.get_whoose_move()],
            pos + directions[1] * move_direction[desk.get_whoose_move()],
        };

        for (const auto &dir : relative_dir)
        {
            if (desk.is_inside(dir))
            {
                if (desk.is_empty(dir))
                    ret.push_back(dir);
            }
        }
    }
    else
    {
        for (const auto &dir : directions)
        {
            ivec2 v = pos + dir;
            while (desk.is_inside(v))
            {
                if (!desk.is_empty(v))
                    break;
                ret.push_back(v);
                v += dir;
            }
        }
    }

    return ret;
}

vector<pair<ivec2, ivec2>> get_avialable_moves_with_geting(const ivec2 &pos)
{
    vector<pair<ivec2, ivec2>> ret;

    if (!desk.is_queen(pos))
    {
        const int directions_count = 4;
        ivec2 relative_dir[directions_count]{
            pos + directions[0] * 2,
            pos + directions[1] * 2,
            pos + directions[2] * 2,
            pos + directions[3] * 2,
        };

        for (int i = 0; i < directions_count; i++)
        {
            if (desk.is_inside(relative_dir[i]))
            {
                ivec2 t = relative_dir[i] - directions[i];
                if (desk.is_another_color(t) && desk.is_empty(relative_dir[i]))
                    ret.push_back(pair(relative_dir[i], t));
            }
        }
    }
    else
    {
        for (const auto &dir : directions)
        {
            ivec2 v = pos + dir;
            while (desk.is_inside(v))
            {
                if (desk.is_empty(v))
                {
                    v += dir;
                    continue;
                }
                else if (desk.get_checker_color(v) == desk.get_whoose_move())
                    break;
                else // if is another color
                {
                    ivec2 checker_to_add = v;
                    v += dir;
                    while (desk.is_inside(v))
                    {
                        if (!desk.is_empty(v))
                            break;
                        ret.push_back(pair(v, checker_to_add));
                        v += dir;
                    }
                    break;
                }

                v += dir;
            }
        }
    }

    return ret;
}

bool has_moves_with_getting(const ivec2 &pos, const ivec2 &dir)
{
    ivec2 coord = pos + dir * 2;
    if (desk.is_inside(coord))
    {
        return desk.is_empty(coord) && desk.is_another_color(coord - dir);
    }
    return false;
}

bool has_moves_with_getting(const ivec2 &pos)
{
    return has_moves_with_getting(pos, directions[0]) ||
           has_moves_with_getting(pos, directions[1]) ||
           has_moves_with_getting(pos, directions[2]) ||
           has_moves_with_getting(pos, directions[3]);
}