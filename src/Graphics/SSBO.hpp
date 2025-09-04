#pragma once

#include <vector>

#include "ObjectBinder.hpp"
#include "glad/glad.h"

template<typename T>
class SSBO
{
public:
	SSBO(const std::vector<T>& data, const int index, const int usage = GL_STATIC_DRAW)
	{
		ssbo = {};
		glGenBuffers(1, &ssbo);
		ObjectBinder<GL_SHADER_STORAGE_BUFFER> binder(ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<long long>(data.size() * sizeof(T)), data.data(), usage);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);
	}

	~SSBO()
	{
		glDeleteBuffers(1, &ssbo);
	}

	void UpdateAll(const std::vector<T>& data)
	{
		ObjectBinder<GL_SHADER_STORAGE_BUFFER> binder(ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.size() * sizeof(T), data.data());
	}

	template<size_t U = sizeof(T)>
	std::enable_if_t<U <= 8, void> UpdateSingle(const int offset, T data)
	{
		ObjectBinder<GL_SHADER_STORAGE_BUFFER> binder(ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset * U, U, &data);
	}

	template<size_t U = sizeof(T)>
	std::enable_if_t<(U > 8), void> UpdateSingle(const int offset, const T& data)
	{
		ObjectBinder<GL_SHADER_STORAGE_BUFFER> binder(ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset * U, U, &data);
	}

	void UpdateMultiple(const int start, const int size, const std::vector<T>& data)
	{
		ObjectBinder<GL_SHADER_STORAGE_BUFFER> binder(ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, start * sizeof(T), size * sizeof(T), data.data() + size);
	}

	static void DebugBufferObject(const unsigned int computeShader, const std::string& name)
	{
		const int blockIndex = glGetProgramResourceIndex(computeShader, GL_SHADER_STORAGE_BLOCK, name.c_str());
		int numActiveVars = 0;
		constexpr GLenum glNumActiveVariables[] = {GL_NUM_ACTIVE_VARIABLES};
		glGetProgramResourceiv(computeShader, GL_SHADER_STORAGE_BLOCK, blockIndex, 1,
		                       glNumActiveVariables, 1, nullptr, &numActiveVars);

		constexpr GLenum glActiveVariables[] = {GL_ACTIVE_VARIABLES};
		std::vector<int> vars(numActiveVars);
		glGetProgramResourceiv(computeShader, GL_SHADER_STORAGE_BLOCK, blockIndex, 1,
		                       glActiveVariables, numActiveVars, nullptr, vars.data());

		for (const auto var: vars)
		{
			char nameBuf[128];
			GLsizei length = 0;

			glGetProgramResourceName(computeShader, GL_BUFFER_VARIABLE, var, sizeof(nameBuf), &length, nameBuf);

			int results[4];
			constexpr GLenum props[] =
			{
				GL_OFFSET,
				GL_ARRAY_STRIDE,
				GL_MATRIX_STRIDE,
				GL_TOP_LEVEL_ARRAY_STRIDE
			};

			glGetProgramResourceiv(computeShader,
			                       GL_BUFFER_VARIABLE,
			                       var,
			                       4, props,
			                       4, nullptr,
			                       results);

			std::println("{} offset={} arraystride={} matrixstide={} toplevelarraystride{}", nameBuf, results[0],
			             results[1],
			             results[2], results[3]);
		}
	}

private:
	unsigned int ssbo;
};
