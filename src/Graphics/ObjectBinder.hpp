#pragma once
#include "glad/glad.h"

template<int Type>
class ObjectBinder
{
public:
	explicit ObjectBinder(const unsigned int bo)
	{
		glBindBuffer(Type, bo);
	}

	~ObjectBinder()
	{
		glBindBuffer(Type, 0);
	}
};
