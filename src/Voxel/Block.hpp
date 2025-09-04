#pragma once
enum class BlockType : int
{
	Air,
	Stone,
	Grass
};

struct Block
{
	BlockType type;
};
