#pragma once
#include <memory>
#include <vector>

#include "Block.hpp"

class Chunk
{
public:
	Chunk(int chunkWidth, int chunkLength, int chunkHeight);
	void SetBlock(uint8_t x, uint16_t y, uint8_t z, Block block) const;
	Block GetBlock(uint8_t x, uint16_t y, uint8_t z) const;
	const Block* GetBlockPtr() const;
	std::vector<Block>& GetVector() const;

private:
	std::shared_ptr<std::vector<Block>> chunk;

	int CHUNK_WIDTH;
	int CHUNK_LENGTH;
	int CHUNK_HEIGHT;
};
