#pragma once

#include <glm/glm.hpp>

#include "model.h"

class Checker
{
public:
	enum Color
	{
		WHITE = 0,
		BLACK = 1,
		EMPTY = 2,
		UNKNOWN = 3,
		QUEEN_FLAG = 0xA0,
		WHITE_QUEEN = QUEEN_FLAG + WHITE,
		BLACK_QUEEN = QUEEN_FLAG + BLACK,
	};

private:
	Model &model;

	Color color;
	bool is_queen = false;

public:
	static const glm::vec2 default_pos;
	glm::vec2 position;

	Checker(Model &m, Color c) : color(c), model(m), position(default_pos) {}
	void setColor(Color c) { color = c; }
	void draw() { model.draw(position); };

	Color getColor() const { return color; }
};

const glm::vec2 Checker::default_pos = glm::vec2(-3.5f, -3.5f);