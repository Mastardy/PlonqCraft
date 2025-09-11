#include "Random.hpp"

std::map<long long, std::mt19937_64> Random::generators{{0, std::mt19937_64(0)}};
long long Random::currentSeed = 0;

void Random::Seed(const long long seed)
{
	currentSeed = seed;
	if (!generators.contains(seed)) generators[seed] = std::mt19937_64(seed);
}

float Random::GetValue(const float min, const float max)
{
	std::uniform_real_distribution distribution(min, max);
	return distribution(generators[currentSeed]);
}

float Random::GetValue(const float min, const int max)
{
	return GetValue(min, static_cast<float>(max));
}

float Random::GetValue(const int min, const float max)
{
	return GetValue(static_cast<float>(min), max);
}


int Random::GetValue(const int min, const int max)
{
	std::uniform_int_distribution distribution(min, max);
	return distribution(generators[currentSeed]);
}
