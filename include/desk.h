#pragma once

#include "checker.h"

struct Desk
{
public:
    static const glm::ivec2 RIGHT_TOP;
    static const glm::ivec2 LEFT_TOP;
    static const glm::ivec2 RIGHT_BOTTOM;
    static const glm::ivec2 LEFT_BOTTOM;

private:
    static const int desk_size = 8;

    int map[desk_size][desk_size] = {
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

public:
    int &operator[](const glm::ivec2 &v) { return map[v.y][v.x]; }
    bool is_empty(const glm::ivec2 &v) { return map[v.y][v.x] == Checker::EMPTY; }
    bool is_checker(const glm::ivec2 &v) { return (map[v.y][v.x] & 1) == map[v.y][v.x]; }
    int get_checker_color(const glm::ivec2 &v) { return map[v.y][v.x] & 1; }
    bool is_inside(const glm::ivec2 &v) { return v.x >= 0 && v.x < desk_size && v.y >= 0 && v.y < desk_size; }
};

const glm::ivec2 Desk::RIGHT_TOP(1, 1);
const glm::ivec2 Desk::LEFT_TOP(-1, 1);
const glm::ivec2 Desk::RIGHT_BOTTOM(1, -1);
const glm::ivec2 Desk::LEFT_BOTTOM(-1, -1);