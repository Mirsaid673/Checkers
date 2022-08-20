#pragma once

#include <glm/glm.hpp>

namespace Color // for automatical converting to int
{
    enum
    {
        QUEEN_FLAG = 0b10,
        WHITE = 0,
        BLACK = 1,
        WHITE_QUEEN = QUEEN_FLAG | WHITE,
        BLACK_QUEEN = QUEEN_FLAG | BLACK,
        EMPTY = 4,
        UNKNOWN = 8,
    };
};

struct Desk
{
public:
    static const glm::ivec2 RIGHT_TOP;
    static const glm::ivec2 LEFT_TOP;
    static const glm::ivec2 RIGHT_BOTTOM;
    static const glm::ivec2 LEFT_BOTTOM;

    static const glm::vec2 default_checker_pos;

    static const int desk_size = 8;

private:
    int whoose_move = Color::WHITE;
    static const int next_move[2];
    int map[desk_size][desk_size] = {
        // inversed to better access
        // 0 - white checker
        // 1 - black checker
        // 2 - empty cell
        {0, 4, 0, 4, 0, 4, 0, 4},
        {4, 0, 4, 0, 4, 0, 4, 0},
        {0, 4, 0, 4, 0, 4, 0, 4},
        {4, 4, 4, 4, 4, 4, 4, 4},
        {4, 4, 4, 4, 4, 4, 4, 4},
        {4, 1, 4, 1, 4, 1, 4, 1},
        {1, 4, 1, 4, 1, 4, 1, 4},
        {4, 1, 4, 1, 4, 1, 4, 1},
    };

public:
    int &operator[](const glm::ivec2 &v) { return map[v.y][v.x]; }
    int *operator[](const int i) { return map[i]; }
    bool is_empty(const glm::ivec2 &v) { return map[v.y][v.x] == Color::EMPTY; }
    bool is_checker(const glm::ivec2 &v) { return (map[v.y][v.x] & 0b11) == map[v.y][v.x]; }
    bool is_queen(const glm::ivec2 &v) { return (map[v.y][v.x] & Color::QUEEN_FLAG) == Color::QUEEN_FLAG; }
    bool is_another_color(const glm::ivec2 &v) { return is_checker(v) && get_checker_color(v) == get_next_move(); }
    int get_checker_color(const glm::ivec2 &v) { return map[v.y][v.x] & 1; }
    int get_another_color(const glm::ivec2 &v) { return next_move[get_checker_color(v)]; }

    bool is_empty(int v) { return v == Color::EMPTY; }
    bool is_checker(int v) { return (v & 0b11) == v; }
    bool is_queen(int v) { return (v & Color::QUEEN_FLAG) == Color::QUEEN_FLAG; }
    bool is_another_color(int v) { return is_checker(v) && get_checker_color(v) == get_next_move(); }
    int get_checker_color(int v) { return v & 1; }
    int get_another_color(int v) { return next_move[get_checker_color(v)]; }

    void make_queen(const glm::ivec2 &v) { map[v.y][v.x] = map[v.y][v.x] | Color::QUEEN_FLAG; }

    bool is_inside(const glm::ivec2 &v) { return v.x >= 0 && v.x < desk_size && v.y >= 0 && v.y < desk_size; }

    int get_whoose_move() { return whoose_move; }
    int get_next_move() { return next_move[whoose_move]; }
    void switch_move() { whoose_move = next_move[whoose_move]; }
};

const glm::ivec2 Desk::RIGHT_TOP(1, 1);
const glm::ivec2 Desk::LEFT_TOP(-1, 1);
const glm::ivec2 Desk::RIGHT_BOTTOM(1, -1);
const glm::ivec2 Desk::LEFT_BOTTOM(-1, -1);

const glm::vec2 Desk::default_checker_pos(-3.5f, -3.5f);;

const int Desk::next_move[]{
    Color::BLACK,
    Color::WHITE,
};