#pragma once

class TimeQuery
{
public:
	void Begin()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	void GetQuery()
	{
		const auto end = std::chrono::high_resolution_clock::now();
		const auto queryTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		start = end;

		frameTime = static_cast<long double>(queryTime.count()) / 1e3;
		deltaTime = static_cast<long double>(queryTime.count()) / 1e6;
	}

	long double FrameTime() const { return frameTime; }
	long double DeltaTime() const { return deltaTime; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	long double frameTime{};
	long double deltaTime{};
};
