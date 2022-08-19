#pragma once

#include "checker.h"

struct Desk
{
public:
    static const glm::ivec2 RIGHT_TOP;
    static const glm::ivec2 LEFT_TOP;
    static const glm::ivec2 RIGHT_BOTTOM;
    static const glm::ivec2 LEFT_BOTTOM;

    static const int desk_size = 8;

private:
    Checker::Color whoose_move = Checker::Color::WHITE;
    static const Checker::Color next_move[2];
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
    bool is_empty(const glm::ivec2 &v) { return map[v.y][v.x] == Checker::EMPTY; }
    bool is_checker(const glm::ivec2 &v) { return (map[v.y][v.x] & 1) == map[v.y][v.x]; }
    bool is_queen(const glm::ivec2 &v) { return (map[v.y][v.x] & Checker::QUEEN_FLAG) == Checker::QUEEN_FLAG; }
    bool is_another_color(const glm::ivec2 &v) { return is_checker(v) && get_checker_color(v) == get_next_move(); }
    int get_checker_color(const glm::ivec2 &v) { return map[v.y][v.x] & 1; }

    bool is_empty(int v) { return v == Checker::EMPTY; }
    bool is_checker(int v) { return (v & 0b11) == v; }
    bool is_queen(int v) { return (v & Checker::QUEEN_FLAG) == Checker::QUEEN_FLAG; }
    bool is_another_color(int v) { return is_checker(v) && get_checker_color(v) == get_next_move(); }
    int get_checker_color(int v) { return v & 1; }

    void make_queen(const glm::ivec2 &v) { map[v.y][v.x] = map[v.y][v.x] | Checker::QUEEN_FLAG; }

    bool is_inside(const glm::ivec2 &v) { return v.x >= 0 && v.x < desk_size && v.y >= 0 && v.y < desk_size; }

    int get_whoose_move() { return whoose_move; }
    int get_next_move() { return next_move[whoose_move]; }
    void switch_move() { whoose_move = next_move[whoose_move]; }
};

const glm::ivec2 Desk::RIGHT_TOP(1, 1);
const glm::ivec2 Desk::LEFT_TOP(-1, 1);
const glm::ivec2 Desk::RIGHT_BOTTOM(1, -1);
const glm::ivec2 Desk::LEFT_BOTTOM(-1, -1);

const Checker::Color Desk::next_move[]{
    Checker::Color::BLACK,
    Checker::Color::WHITE,
};