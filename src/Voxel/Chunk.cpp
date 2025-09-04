#include "Chunk.hpp"

#include <memory>

Chunk::Chunk(const int chunkWidth, const int chunkLength, const int chunkHeight)
{
	CHUNK_WIDTH = chunkWidth;
	CHUNK_LENGTH = chunkLength;
	CHUNK_HEIGHT = chunkHeight;

	chunk = std::make_shared<std::vector<Block>>(chunkWidth * chunkLength * chunkHeight);
}

void Chunk::SetBlock(const uint8_t x, const uint16_t y, const uint8_t z, const Block block) const
{
	chunk->data()[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_LENGTH] = block;
}

Block Chunk::GetBlock(const uint8_t x, const uint16_t y, const uint8_t z) const
{
	return chunk->data()[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_LENGTH];
}

std::vector<Block>& Chunk::GetVector() const
{
	return *chunk;
}

const Block* Chunk::GetBlockPtr() const
{
	return chunk->data();
}
