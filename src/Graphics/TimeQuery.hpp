#pragma once
#include "glad/glad.h"

class TimeQuery
{
public:
	TimeQuery()
	{
		glGenQueries(1, &query);
	}

	void Begin() const
	{
		glBeginQuery(GL_TIME_ELAPSED, query);
	}

	void GetQuery()
	{
		glEndQuery(GL_TIME_ELAPSED);

		int available = 0;
		while (!available)
		{
			glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);
		}
		glGetQueryObjectui64v(query, GL_QUERY_RESULT, &queryTime);
		frameTime = static_cast<long double>(queryTime) / 1e6;
		deltaTime = static_cast<long double>(queryTime) / 1e9;
	}

	long double FrameTime() const { return frameTime; }
	long double DeltaTime() const { return deltaTime; }

	~TimeQuery()
	{
		glDeleteQueries(1, &query);
	}

private:
	unsigned int query{};
	unsigned long long queryTime{};
	long double frameTime{};
	long double deltaTime{};
};
