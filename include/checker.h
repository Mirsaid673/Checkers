#pragma once

#include <glm/glm.hpp>

class Checker
{
public:
	enum Color
	{
		QUEEN_FLAG = 0b10,
		WHITE = 0,
		BLACK = 1,
		WHITE_QUEEN = QUEEN_FLAG | WHITE,
		BLACK_QUEEN = QUEEN_FLAG | BLACK,
		EMPTY = 4,
		UNKNOWN = 8,
	};

	static const glm::vec2 default_pos;
};

const glm::vec2 Checker::default_pos = glm::vec2(-3.5f, -3.5f);