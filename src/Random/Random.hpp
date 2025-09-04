#pragma once
#include <map>
#include <random>

class Random
{
public:
	static void Seed(long long seed);
	static float GetValue(float min, float max);
	static float GetValue(int min, float max);
	static float GetValue(float min, int max);
	static int GetValue(int min, int max);

private:
	static long long currentSeed;
	static std::map<long long, std::mt19937_64> generators;
};
